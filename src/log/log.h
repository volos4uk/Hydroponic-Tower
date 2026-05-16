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

#ifndef _LOG_H_
#define _LOG_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "main.h"

#if defined(NCFG_USE_LOG) && ((NCFG_LOG_TYPE == NCFG_LOG_TYPE_DIRECT) || \
                              (NCFG_LOG_TYPE == NCFG_LOG_TYPE_BUFF))

#include "log_impl.h"
#include "log_com.h"
#include "vocabulary.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

// API(module enabled) ---
#define log_init()    log_init_impl()
#define log_deinit()  log_deinit_impl()

#define log_print(type, \
                  tag_p, \
                  fn_p, \
                  line, \
                  mess_fmt_p, ...)  log_print_impl(type, \
                                                   tag_p, \
                                                   fn_p, \
                                                   line, \
                                                   mess_fmt_p, \
                                                   ##__VA_ARGS__)

#define log_print_local(type, \
                        tag_p, \
                        fn_p, \
                        line, \
                        mess_fmt_p, ...)  log_print_local_impl(type, \
                                                               tag_p, \
                                                               fn_p, \
                                                               line, \
                                                               mess_fmt_p, \
                                                               ##__VA_ARGS__)
// logs messages with header and line end
# define LOG_T(tag, message, ...)                    log_print(LOG_TYPE_T,         tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOG_D(tag, message, ...)                    log_print(LOG_TYPE_D,         tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOG_I(tag, message, ...)                    log_print(LOG_TYPE_I,         tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOG_W(tag, message, ...)                    log_print(LOG_TYPE_W,         tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOG_E(tag, message, ...)                    log_print(LOG_TYPE_E,         tag, LOG_FN, __LINE__, message, ##__VA_ARGS__)
# define LOG_F(tag, message, ...)                    log_print(LOG_TYPE_F,         tag, LOG_FN, __LINE__, message, ##__VA_ARGS__)

// logs messages where you can define existing of the header or line end
# define LOGC_T(tag, flags, message, ...)            log_print((LOG_TYPE_T|flags), tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGC_D(tag, flags, message, ...)            log_print((LOG_TYPE_D|flags), tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGC_I(tag, flags, message, ...)            log_print((LOG_TYPE_I|flags), tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGC_W(tag, flags, message, ...)            log_print((LOG_TYPE_W|flags), tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGC_E(tag, flags, message, ...)            log_print((LOG_TYPE_E|flags), tag, LOG_FN, __LINE__, message, ##__VA_ARGS__)
# define LOGC_F(tag, flags, message, ...)            log_print((LOG_TYPE_F|flags), tag, LOG_FN, __LINE__, message, ##__VA_ARGS__)

// Local log messages(only to Serial, no limit in size, always with header and end)
# define LOGL_T(tag, message, ...)                   log_print_local(LOG_TYPE_T,   tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGL_D(tag, message, ...)                   log_print_local(LOG_TYPE_D,   tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGL_I(tag, message, ...)                   log_print_local(LOG_TYPE_I,   tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGL_W(tag, message, ...)                   log_print_local(LOG_TYPE_W,   tag, NULL,   0,        message, ##__VA_ARGS__)
# define LOGL_E(tag, message, ...)                   log_print_local(LOG_TYPE_E,   tag, LOG_FN, __LINE__, message, ##__VA_ARGS__)
# define LOGL_F(tag, message, ...)                   log_print_local(LOG_TYPE_F,   tag, LOG_FN, __LINE__, message, ##__VA_ARGS__)

// Undef not used log levels
# if NCFG_LOG_LEVEL == NT_LOG_LEVEL_OFF

#   undef  LOG_T
#   define LOG_T(tag, message, ...)                  ((void)0)
#   undef  LOG_D
#   define LOG_D(tag, message, ...)                  ((void)0)
#   undef  LOG_I
#   define LOG_I(tag, message, ...)                  ((void)0)
#   undef  LOG_W
#   define LOG_W(tag, message, ...)                  ((void)0)
#   undef  LOG_E
#   define LOG_E(tag, message, ...)                  ((void)0)
#   undef  LOG_F
#   define LOG_F(tag, message, ...)                  ((void)0)

#   undef  LOGC_T
#   define LOGC_T(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_D
#   define LOGC_D(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_I
#   define LOGC_I(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_W
#   define LOGC_W(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_E
#   define LOGC_E(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_F
#   define LOGC_F(tag, flags, message, ...)          ((void)0)

#   undef  LOGL_T
#   define LOGL_T(tag, message, ...)                 ((void)0)
#   undef  LOGL_D
#   define LOGL_D(tag, message, ...)                 ((void)0)
#   undef  LOGL_I
#   define LOGL_I(tag, message, ...)                 ((void)0)
#   undef  LOGL_W
#   define LOGL_W(tag, message, ...)                 ((void)0)
#   undef  LOGL_E
#   define LOGL_E(tag, message, ...)                 ((void)0)
#   undef  LOGL_F
#   define LOGL_F(tag, message, ...)                 ((void)0)

# elif NCFG_LOG_LEVEL == NT_LOG_LEVEL_FATAL

#   undef  LOG_T
#   define LOG_T(tag, message, ...)                  ((void)0)
#   undef  LOG_D
#   define LOG_D(tag, message, ...)                  ((void)0)
#   undef  LOG_I
#   define LOG_I(tag, message, ...)                  ((void)0)
#   undef  LOG_W
#   define LOG_W(tag, message, ...)                  ((void)0)
#   undef  LOG_E
#   define LOG_E(tag, message, ...)                  ((void)0)

#   undef  LOGC_T
#   define LOGC_T(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_D
#   define LOGC_D(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_I
#   define LOGC_I(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_W
#   define LOGC_W(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_E
#   define LOGC_E(tag, flags, message, ...)          ((void)0)

#   undef  LOGL_T
#   define LOGL_T(tag, message, ...)                 ((void)0)
#   undef  LOGL_D
#   define LOGL_D(tag, message, ...)                 ((void)0)
#   undef  LOGL_I
#   define LOGL_I(tag, message, ...)                 ((void)0)
#   undef  LOGL_W
#   define LOGL_W(tag, message, ...)                 ((void)0)
#   undef  LOGL_E
#   define LOGL_E(tag, message, ...)                 ((void)0)

# elif NCFG_LOG_LEVEL == NT_LOG_LEVEL_ERROR

#   undef  LOG_T
#   define LOG_T(tag, message, ...)                  ((void)0)
#   undef  LOG_D
#   define LOG_D(tag, message, ...)                  ((void)0)
#   undef  LOG_I
#   define LOG_I(tag, message, ...)                  ((void)0)
#   undef  LOG_W
#   define LOG_W(tag, message, ...)                  ((void)0)

#   undef  LOGC_T
#   define LOGC_T(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_D
#   define LOGC_D(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_I
#   define LOGC_I(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_W
#   define LOGC_W(tag, flags, message, ...)          ((void)0)

#   undef  LOGL_T
#   define LOGL_T(tag, message, ...)                 ((void)0)
#   undef  LOGL_D
#   define LOGL_D(tag, message, ...)                 ((void)0)
#   undef  LOGL_I
#   define LOGL_I(tag, message, ...)                 ((void)0)
#   undef  LOGL_W
#   define LOGL_W(tag, message, ...)                 ((void)0)

# elif NCFG_LOG_LEVEL == NT_LOG_LEVEL_WARN

#   undef  LOG_T
#   define LOG_T(tag, message, ...)                  ((void)0)
#   undef  LOG_D
#   define LOG_D(tag, message, ...)                  ((void)0)
#   undef  LOG_I
#   define LOG_I(tag, message, ...)                  ((void)0)

#   undef  LOGC_T
#   define LOGC_T(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_D
#   define LOGC_D(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_I
#   define LOGC_I(tag, flags, message, ...)          ((void)0)

#   undef  LOGL_T
#   define LOGL_T(tag, message, ...)                 ((void)0)
#   undef  LOGL_D
#   define LOGL_D(tag, message, ...)                 ((void)0)
#   undef  LOGL_I
#   define LOGL_I(tag, message, ...)                 ((void)0)

# elif NCFG_LOG_LEVEL == NT_LOG_LEVEL_INFO

#   undef  LOG_T
#   define LOG_T(tag, message, ...)                  ((void)0)
#   undef  LOG_D
#   define LOG_D(tag, message, ...)                  ((void)0)

#   undef  LOGC_T
#   define LOGC_T(tag, flags, message, ...)          ((void)0)
#   undef  LOGC_D
#   define LOGC_D(tag, flags, message, ...)          ((void)0)

#   undef  LOGL_T
#   define LOGL_T(tag, message, ...)                 ((void)0)
#   undef  LOGL_D
#   define LOGL_D(tag, message, ...)                 ((void)0)

# elif NCFG_LOG_LEVEL == NT_LOG_LEVEL_DEBUG

#   undef  LOG_T
#   define LOG_T(tag, message, ...)                  ((void)0)

#   undef  LOGC_T
#   define LOGC_T(tag, flags, message, ...)          ((void)0)

#   undef  LOGL_T
#   define LOGL_T(tag, message, ...)                 ((void)0)

# elif NCFG_LOG_LEVEL == NT_LOG_LEVEL_TRACE

// All logs are tracked

# endif // NCFG_LOG_LEVEL

#else

// API(module disabled) ---
#define log_init()                                          (true)
#define log_deinit()                                        ((void)0)

#define log_print(type, \
                  tag_p, \
                  fn_p, \
                  line, \
                  mess_fmt_p, ...)                          ((void)0)

#define log_print_local(type, \
                        tag_p, \
                        fn_p, \
                        line, \
                        mess_fmt_p, ...)                    ((void)0)

// logs messages with header and line end
#define LOG_T(tag, message, ...)                            ((void)0)
#define LOG_D(tag, message, ...)                            ((void)0)
#define LOG_I(tag, message, ...)                            ((void)0)
#define LOG_W(tag, message, ...)                            ((void)0)
#define LOG_E(tag, message, ...)                            ((void)0)
#define LOG_F(tag, message, ...)                            ((void)0)

// logs messages where you can define existing of the header or line end
#define LOGC_T(tag, flags, message, ...)                    ((void)0)
#define LOGC_D(tag, flags, message, ...)                    ((void)0)
#define LOGC_I(tag, flags, message, ...)                    ((void)0)
#define LOGC_W(tag, flags, message, ...)                    ((void)0)
#define LOGC_E(tag, flags, message, ...)                    ((void)0)
#define LOGC_F(tag, flags, message, ...)                    ((void)0)

// Local log messages(only to Serial, no limit in size, always with header and end)
#define LOGL_T(tag, message, ...)                           ((void)0)
#define LOGL_D(tag, message, ...)                           ((void)0)
#define LOGL_I(tag, message, ...)                           ((void)0)
#define LOGL_W(tag, message, ...)                           ((void)0)
#define LOGL_E(tag, message, ...)                           ((void)0)
#define LOGL_F(tag, message, ...)                           ((void)0)

#endif // NCFG_USE_LOG && ((NCFG_LOG_TYPE == NCFG_LOG_TYPE_DIRECT) || (NCFG_LOG_TYPE == NCFG_LOG_TYPE_BUFF))

#endif // _LOG_H_
