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

#if defined(NCFG_USE_LOG) && (NCFG_LOG_TYPE == NCFG_LOG_TYPE_DIRECT)

//#include "os_app.h"
#include "log_to_serial.h"
#include "utils.h"

// Included here to have access to LOGL_x API(an exception to the rules)
#include "log.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LOG_TS_STR_MAX_SIZE          10  // do not use brackets() here
#define LOG_MESS_MAX_SIZE            (NCFG_LOG_MESS_BODY_MAX_SIZE + 2 + 1 + 1)  // +2 - max log line end, +1 - line terminator, +1 - spare
#define LOG_NORMAL_HEADER_END_INDEX  (1 + NCFG_LOG_TAG_STR_MAX_SIZE + 1 + 1 + LOG_TS_STR_MAX_SIZE + 1 + 2 +2) // see header message format: [TAG]-TS-C0-I

/****************************************************************************
 * Private Types
 ****************************************************************************/
typedef struct {
  union {
    uint8_t all;
    struct {
      uint8_t is_serial_init:   1;                // serial interface init
      uint8_t is_init:          1;                // module global init
    } f;                                          // flags
  } d;                                            // discrete

  char msg[LOG_MESS_MAX_SIZE];                    // message buffer

} log_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

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

/****************************************************************************
 * Public Functions
 ****************************************************************************/
bool log_init_impl(void) {
  // init 'Serial' output direction
  log_to_serial_init();
  ctx.d.f.is_serial_init = 1;
  ctx.d.f.is_init = 1;

  LOGL_T(TAG, "sizeof(log_t) = %u", sizeof(log_t));
  LOGL_I(TAG, vocm_init_done);
  return ctx.d.f.is_init;
}

void log_print_impl(const uint8_t  type,
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

  //bool state      = false;
  //bool msg_unlock = false;
  char* p         = ctx.msg;
  int remain      = NCFG_LOG_MESS_BODY_MAX_SIZE;
  int ret;

  // print message header ---
  if ((type & LOG_NH_F) == 0) {
    // 'no header' flag is not set

    // print message header
    ret = snprintf(p, remain, header_fmt, tag_p, ht_now(), 0, type_loc, (fn_p==NULL)?"NULL":fn_p, (unsigned long)line);
    if (ret <= 0) {
      LOGL_E(TAG, "snprintf() error: %d!", ret);
      goto END; // some errors
    }
    if ((type_loc != LOG_TYPE_E) && (type_loc != LOG_TYPE_F)) {
      // normal log message header - need to replace not needed characters
      p[LOG_NORMAL_HEADER_END_INDEX]   = ':';
      p[LOG_NORMAL_HEADER_END_INDEX+1] = ' ';
      p[LOG_NORMAL_HEADER_END_INDEX+2] = '\0';
      ret = strlen(ctx.msg);
    }
    p      += ret;
    remain -= ret;
    if (remain <= 0) {
      LOGL_E(TAG, "No space for header! ret: %d!", remain);
      goto END; // not enough space even for a header
    }
  }

  // print message body ---
  va_list params;
  va_start(params, mess_fmt_p);
  ret = vsnprintf(p, remain, mess_fmt_p, params);
  va_end(params);
  if (ret <= 0) {
    LOGL_E(TAG, "vsnprintf() error: %d!", ret);
    goto END; // some errors
  }
  remain -= ret;
  if (remain < 0) {
    // the message buffer is overflowed - terminate buffer(we can print a part of message)
    ctx.msg[NCFG_LOG_MESS_BODY_MAX_SIZE-4] = '.';
    ctx.msg[NCFG_LOG_MESS_BODY_MAX_SIZE-3] = '.';
    ctx.msg[NCFG_LOG_MESS_BODY_MAX_SIZE-2] = '.';
    ctx.msg[NCFG_LOG_MESS_BODY_MAX_SIZE-1] = '\0';
  }

  // print message end ---
  if ((type & LOG_NE_F) == 0) {
    // 'no end' flag is not set - send line end
    ret = strlen(ctx.msg);
    if (NCFG_LOG_LINE_END == NT_LINE_END_CRLF) {
      ctx.msg[ret] = 0x0D;  // CR
      ret++;
    }
    ctx.msg[ret] = 0x0A;    // LF

    ret++;
    ctx.msg[ret] = '\0';

    //  message mux can be unlocked as message is ready
    //msg_unlock = true;
  }

  // send to outputs ---
  log_to_serial_send(ctx.msg, strlen(ctx.msg));  // send to 'Serial'

  //state = true;

END:

  // Added here to fix compilation error if !NCFG_USE_OS_APP
  return;
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
  mess_p = (char*)ht_calloc(&tot_size, 0, false);
  if (mess_p == NULL) {
    goto END; // can't allocate buff for message
  }
  p = mess_p;

  // print message header ---
  ret = snprintf(p, remain, header_fmt, tag_p, ht_now(), 0, type_loc, (fn_p==NULL)?"NULL":fn_p, (unsigned long)line);
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
  ht_free(mess_p); // free message
}
#endif // NCFG_LOG_TO_SERIAL

#endif // (NCFG_USE_LOG) && (NCFG_LOG_TYPE == NCFG_LOG_TYPE_DIRECT)