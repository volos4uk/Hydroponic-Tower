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

#ifndef _LOG_IMPL_H_
#define _LOG_IMPL_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "main.h"

#if defined(NCFG_USE_LOG)

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
bool log_init_impl(void);
#define log_deinit_impl() ((void)0)

void log_print_impl(uint8_t        type,
                    const char*    tag_p,
                    const char*    fn_p,
                    const uint32_t line,
                    const char*    mess_fmt_p, ...);

# if defined(NCFG_LOG_TO_SERIAL)
void log_print_local_impl(const uint8_t  type,
                          const char*    tag_p,
                          const char*    fn_p,
                          const uint32_t line,
                          const char*    mess_fmt_p, ...);
# else
#define log_print_local_impl(type, \
                             tag_p, \
                             fn_p, \
                             line, \
                             mess_fmt_p, ...) ((void)0)
# endif // NCFG_LOG_TO_SERIAL

#endif // NCFG_USE_LOG

#endif // _LOG_IMPL_H_
