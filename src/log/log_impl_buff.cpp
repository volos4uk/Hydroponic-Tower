/****************************************************************************
 *  Copyright (C) 2025. All rights reserved.
 *  Author:  Voloshchuk Oleksandr

 *  Confidentially.
 *  No disclosing or distribution to common access without prior written consent.

 *  This software is provided by the copyright holder "AS IS"
 *  and any warranties related to this software are DISCLAIMED.
 *  The copyright owner is NOT LIABLE for any damages caused
 *  by the use of this software.

 *  Business inquiries for customization and projects can be sent to:
 *  
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "log_impl.h"

#if defined(NCFG_USE_LOG) && (NCFG_LOG_TYPE == NCFG_LOG_TYPE_BUFF)

//#include "os_app.h"
#include "log_to_serial.h"
#include "utils.h"
//#include "watchdog.h"

// Included here to have access to LOGL_x API(an exception to the rules)
#include "log.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LOG_TS_STR_MAX_SIZE          10  // do not use brackets() here
#define LOG_MESS_MAX_SIZE            (NCFG_LOG_MESS_BODY_MAX_SIZE + 2 + 1 + 1)  // +2 - max log line end, +1 - line terminator, +1 - spare
#define LOG_NORMAL_HEADER_END_INDEX  (1 + NCFG_LOG_TAG_STR_MAX_SIZE + 1 + 1 + LOG_TS_STR_MAX_SIZE + 1 + 2 +2) // see header message format: [TAG]-TS-C0-I

#define LOG_BUFF_CYCLE_OVERFLOW_DELAY_MS  (5) // delay in case of the cycle buffer overflow

/****************************************************************************
 * Private Types
 ****************************************************************************/
typedef struct {
  size_t   overflow_err_cntr;
  size_t   count;
  size_t   head;
  size_t   tail;
  uint8_t* buff_p;
} cycle_buff_t;

typedef struct {
  union {
    uint8_t all;
    struct {
      uint8_t is_serial_init:   1;                                // serial interface init
      uint8_t is_init:          1;                                // module global init
    } f;                                                          // flags
  } d;                                                            // discrete

  char                fmt_msg[LOG_MESS_MAX_SIZE];                 // formatted message
  os_app_mh_t         fmt_msg_mux;                                // mutex for cycle buffer
  uint32_t            fmt_msg_lock_ts;                            // ts when formatted message mux was locked

  cycle_buff_t        cycle_buff;                                 // cycle buffer for logs
  os_app_mh_t         cycle_buff_mux;                             // mutex for cycle buffer

  char                ready_msg[LOG_MESS_MAX_SIZE];               // ready message
  uint16_t            ready_msg_cntr;                             // ready message counter

} log_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static bool log_put_char(cycle_buff_t* cycle_buff_p, uint8_t data, size_t max_capacity);
static bool log_get_char(cycle_buff_t* cycle_buff_p, uint8_t* data_p, size_t max_capacity);

static bool log_message_get(void);

static void log_task(void* params);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const char TAG[] = "LOG";
static const char header_fmt[] = "[%-" UTILS_STRINGIFY_EXPAND(NCFG_LOG_TAG_STR_MAX_SIZE) "s]-%0" UTILS_STRINGIFY_EXPAND(LOG_TS_STR_MAX_SIZE) "lu-C%d-%c-%s-L%lu: ";
static log_t ctx;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static bool log_put_char(cycle_buff_t* cycle_buff_p, uint8_t data, size_t max_capacity) {
  if (cycle_buff_p == NULL) {
    return false;
  }

  if (cycle_buff_p->count < max_capacity) {
    cycle_buff_p->buff_p[cycle_buff_p->tail] = data;
    cycle_buff_p->count++;
    cycle_buff_p->tail++;
    if (cycle_buff_p->tail >= max_capacity) {
      cycle_buff_p->tail = 0;
    }
    return true;
  }

  cycle_buff_p->overflow_err_cntr++;
  return false;
}

static bool log_get_char(cycle_buff_t* cycle_buff_p, uint8_t* data_p, size_t max_capacity) {
  if ((cycle_buff_p == NULL) || (data_p == NULL)) {
    return false;
  }

  if (cycle_buff_p->count > 0) {
    *data_p = cycle_buff_p->buff_p[cycle_buff_p->head];
    cycle_buff_p->count--;
    cycle_buff_p->head++;
    if (cycle_buff_p->head >= max_capacity) {
      cycle_buff_p->head = 0;
    }
    return true;
  }
  return false;
}

static bool log_message_get(void) {
  bool is_msg_ready = false;
  char character    = 0;

  // check if we have some data in the buff
  if (ctx.cycle_buff.count == 0) {
    return false; // no any data in the buff
  }

  // lock cycle buff mux
  if (!os_app_mutex_lock(ctx.cycle_buff_mux, OS_APP_MAX_DELAY, false)) {
    return false; // can't lock cycle buff mux
  }

  // read all available data
  while (1) {
    // get one character
    if (!log_get_char(&ctx.cycle_buff, (uint8_t*)&character, NCFG_LOG_BUFF_MAX_SIZE)) {
      goto END; // no any data in the buff or some error
    }

    // store character in the ready message buffer
    ctx.ready_msg[ctx.ready_msg_cntr] = character;
    ctx.ready_msg_cntr++;

    // check for message end
    if (NCFG_LOG_LINE_END == NT_LINE_END_CRLF) {
      // '\r''\n'
      if (ctx.ready_msg_cntr > 2) {
        if ((ctx.ready_msg[ctx.ready_msg_cntr-2] == '\r') &&
            (ctx.ready_msg[ctx.ready_msg_cntr-1] == '\n'))  {
          is_msg_ready = true;
          goto END;
        }
      }
    } else {
      // '\n'
      if (character == '\n') {
        is_msg_ready = true;
        goto END;
      }
    }

    // check for message overflow cases
    if (ctx.ready_msg_cntr >= (LOG_MESS_MAX_SIZE-1)) {
      // it will process all data in ready_msg and continue collect the end of the message
      ctx.ready_msg_cntr = LOG_MESS_MAX_SIZE-1;
      is_msg_ready        = true;
      goto END;
    }
  }

END:
  // unlock cycle buff mux
  os_app_mutex_unlock(ctx.cycle_buff_mux, false);

  // terminate message
  if (is_msg_ready) {
    ctx.ready_msg[ctx.ready_msg_cntr] = '\0';
    ctx.ready_msg_cntr = 0;
  }

  return is_msg_ready;
}

static void log_task(void* params) {
  // subscribe task to TWDT
  watchdog_subscribe();

  while(1) {
    // get message
    if (log_message_get()) {
      // message is ready - send to outputs
      log_to_serial_send(ctx.ready_msg, strlen(ctx.ready_msg));    // send to 'Serial'
    }

    watchdog_reset();
    sp_delay(NCFG_LOG_TASK_DELAY_MS);
  }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
bool log_init_impl(void) {
  size_t size = NCFG_LOG_BUFF_MAX_SIZE;

  // init 'Serial' output direction
  log_to_serial_init();
  ctx.d.f.is_serial_init = 1;

  // create mutex for formatted message
  if (!os_app_mutex_create(&ctx.fmt_msg_mux, false)) {
    LOGL_F(TAG, "Can't create mutex for formatted message!");
    goto END;
  }

  // create cycle buffer mutex
  if (!os_app_mutex_create(&ctx.cycle_buff_mux, false)) {
    LOGL_F(TAG, "Can't create mutex for cycle buffer!");
    goto END;
  }

  // create cycle buff
  ctx.cycle_buff.buff_p = (uint8_t*)np_calloc(&size, 0, false);
  if (ctx.cycle_buff.buff_p == NULL) {
    LOGL_F(TAG, "Can't create cycle buffer!");
    goto END;
  }

  // create task
  if (!os_app_task_create(log_task,
                          "log-task",
                          NCFG_LOG_TASK_STACK_SIZE,
                          NULL,
                          NCFG_LOG_TASK_PRIORITY,
                          NULL,
                          NCFG_LOG_TASK_CORE_ID,
                          false)) {
    LOGL_F(TAG, "Can't create log task!");
    goto END;
  }

  ctx.d.f.is_init = 1;

END:
  LOGL_T(TAG, "sizeof(log_t) = %u", sizeof(log_t));
  LOGL_I(TAG, vocm_init_done);
  sp_delay(10); // TODO: to fix init flow(by some reason it may stuck)
  return ctx.d.f.is_init;
}

void log_print_impl(uint8_t        type,
                    const char*    tag_p,
                    const char*    fn_p,
                    const uint32_t line,
                    const char*    mess_fmt_p, ...) {
  // NOTE: only LOGL_x API can be used here. Any other logs from this or nested functions will cause deadlock
  if (!ctx.d.f.is_init) {
    return; // not inited yet
  }

  uint8_t type_loc = type & (~(LOG_NH_F | LOG_NE_F));
  if ( (tag_p == NULL) || (mess_fmt_p == NULL) ||
       (((type_loc == LOG_TYPE_E) || (type_loc == LOG_TYPE_F)) && (fn_p == NULL)) ) {
    LOGL_E(TAG, vocm_wrong_args); // wrong args
    return;
  }

  bool state      = false;
  bool msg_unlock = false;
  bool cycle_lock = false;
  char* p         = ctx.fmt_msg;
  int remain      = NCFG_LOG_MESS_BODY_MAX_SIZE;
  int ret;

  // print message header ---
  if ((type & LOG_NH_F) == 0) {
    // 'no header' flag is not set

    // start of the mew message - lock formatted message mux
    if (!os_app_mutex_lock(ctx.fmt_msg_mux, OS_APP_MAX_DELAY, false)) {
      LOGL_E(TAG, "Can't lock formatted message mutex!");
      return;
    }
    ctx.fmt_msg_lock_ts = np_now();

    // print message header
    ret = snprintf(p, remain, header_fmt, tag_p, np_now(), os_app_get_core_id(), type_loc, (fn_p==NULL)?"NULL":fn_p, line);
    if (ret <= 0) {
      LOGL_E(TAG, "snprintf() error: %d!", ret);
      goto END;
    }
    if ((type_loc != LOG_TYPE_E) && (type_loc != LOG_TYPE_F)) {
      // normal log message header - need to replace not needed characters
      p[LOG_NORMAL_HEADER_END_INDEX]   = ':';
      p[LOG_NORMAL_HEADER_END_INDEX+1] = ' ';
      p[LOG_NORMAL_HEADER_END_INDEX+2] = '\0';
      ret = strlen(p);
    }
    p      += ret;
    remain -= ret;
    if (remain <= 0) {
      LOGL_E(TAG, "No space for header! ret: %d!", remain);
      goto END;
    }
  }

  // print message body ---
  va_list params;
  va_start(params, mess_fmt_p);
  ret = vsnprintf(p, remain, mess_fmt_p, params);
  va_end(params);
  if (ret <= 0) {
    LOGL_E(TAG, "vsnprintf() error: %d!", ret);
    goto END;
  }
  remain -= ret;
  if (remain < 0) {
    // the message buffer is overflowed - terminate buffer(we can print a part of message)
    ctx.fmt_msg[NCFG_LOG_MESS_BODY_MAX_SIZE-4] = '.';
    ctx.fmt_msg[NCFG_LOG_MESS_BODY_MAX_SIZE-3] = '.';
    ctx.fmt_msg[NCFG_LOG_MESS_BODY_MAX_SIZE-2] = '.';
    ctx.fmt_msg[NCFG_LOG_MESS_BODY_MAX_SIZE-1] = '\0';
  }

  // print message end ---
  if ((type & LOG_NE_F) == 0) {
    // 'no end' flag is not set - send line end
    ret = strlen(ctx.fmt_msg);
    if (NCFG_LOG_LINE_END == NT_LINE_END_CRLF) {
      ctx.fmt_msg[ret] = 0x0D;  // CR
      ret++;
    }
    ctx.fmt_msg[ret] = 0x0A;    // LF

    ret++;
    ctx.fmt_msg[ret] = '\0';

    // formatted message mux can be unlocked as message is ready
    msg_unlock = true;
  }

  // put message to cycle buffer
  p = ctx.fmt_msg;
  while (*p) {
    // lock cycle buffer mux
    if (!cycle_lock) {
      if (!os_app_mutex_lock(ctx.cycle_buff_mux, OS_APP_MAX_DELAY, false)) {
        LOGL_E(TAG, "Can't lock cycle buffer mutex!");
        goto END;
      }
      cycle_lock = true;
    }

    // put character to the cycle buffer
    if (log_put_char(&ctx.cycle_buff, (uint8_t)(*p), NCFG_LOG_BUFF_MAX_SIZE)) {
      p++; // increase pointer not next character
    } else {
      // seems the cycle buffer overflowed - unlock mux and wait for some free space
      os_app_mutex_unlock(ctx.cycle_buff_mux, false);
      cycle_lock = false;
      sp_delay(LOG_BUFF_CYCLE_OVERFLOW_DELAY_MS);
    }
  }

  state = true;

END:
  // unlock cycle buffer mux
  if (cycle_lock) {
    os_app_mutex_unlock(ctx.cycle_buff_mux, false);
    cycle_lock = false;
  }

  // unlock formatted message mux
  if ((!state || msg_unlock) && (ctx.fmt_msg_lock_ts > 0)) {
    // in case of any error or if message end
    ctx.fmt_msg_lock_ts = 0;
    os_app_mutex_unlock(ctx.fmt_msg_mux, false);
  }
}

#if defined(NCFG_LOG_TO_SERIAL)
void log_print_local_impl(const uint8_t  type,
                          const char*    tag_p,
                          const char*    fn_p,
                          const uint32_t line,
                          const char*    mess_fmt_p, ...) {
  // NOTE: no any LOGX_x API can be used here.
  uint8_t type_loc = type & (~(LOG_NH_F | LOG_NE_F));
  if (!ctx.d.f.is_serial_init || (tag_p == NULL) || (mess_fmt_p == NULL) ||
      (((type_loc == LOG_TYPE_E) || (type_loc == LOG_TYPE_F)) && (fn_p == NULL)) ) {
    // wrong args or module is not inited yet
    return;
  }

  bool va_end      = false;
  char* mess_p     = NULL;
  char* p          = NULL;
  char tmp[10];
  size_t mess_size;
  size_t tot_size;
  int remain;
  int ret;

  // calculate message size ---
  va_list params;
  va_start(params, mess_fmt_p);
  ret = vsnprintf(tmp, sizeof(tmp), mess_fmt_p, params);
  if (ret <= 0) {
    goto END; // some errors
  }
  mess_size = remain = LOG_NORMAL_HEADER_END_INDEX + ((fn_p==NULL)?0:strlen(fn_p)) + 20 + ret; // +20 - spare
  tot_size  = mess_size + 2 + 1 + 1; //  +2 - max log line end, +1 - line terminator, +1 - spare

  // allocate buff for message ---
  mess_p = (char*)np_calloc(&tot_size, 0, false);
  if (mess_p == NULL) {
    goto END; // can't allocate buff for message
  }
  p = mess_p;

  // print message header ---
  ret = snprintf(p, remain, header_fmt, tag_p, np_now(), os_app_get_core_id(), type_loc, (fn_p==NULL)?"NULL":fn_p, line);
  if (ret <= 0) {
    goto END; // some errors
  }
  if ((type_loc != LOG_TYPE_E) && (type_loc != LOG_TYPE_F)) {
    // normal log message header - need to replace not needed characters
    p[LOG_NORMAL_HEADER_END_INDEX]   = ':';
    p[LOG_NORMAL_HEADER_END_INDEX+1] = ' ';
    p[LOG_NORMAL_HEADER_END_INDEX+2] = '\0';
    ret = strlen(p);
  }
  p      += ret;
  remain -= ret;
  if (remain <= 0) {
    goto END; // not enough space even for a header
  }

  // print message body ---
  ret = vsnprintf(p, remain, mess_fmt_p, params);
  va_end(params);
  va_end = true;
  if (ret <= 0) {
    goto END; // some errors
  }
  remain -= ret;
  if (remain <= 0) {
    // the message buffer is overflowed - terminate buffer(we can print a part of message)
    mess_p[mess_size-4] = '.';
    mess_p[mess_size-3] = '.';
    mess_p[mess_size-2] = '.';
    mess_p[mess_size-1] = '\0';
  }

  // print message end ---
  ret = strlen(mess_p);
  if (NCFG_LOG_LINE_END == NT_LINE_END_CRLF) {
    mess_p[ret] = 0x0D;  // CR
    ret++;
  }
  mess_p[ret] = 0x0A;    // LF

  ret++;
  mess_p[ret] = '\0';

  // send to 'Serial' ---
  log_to_serial_send(mess_p, strlen(mess_p));

END:
  if (!va_end) {
    va_end(params);
  }
  np_free(mess_p); // free message
}
#endif // NCFG_LOG_TO_SERIAL

#endif // (NCFG_USE_LOG) && (NCFG_LOG_TYPE == NCFG_LOG_TYPE_BUFF)