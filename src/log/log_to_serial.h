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

#ifndef _LOG_TO_SERIAL_H_
#define _LOG_TO_SERIAL_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "main.h"

#if defined(NCFG_USE_LOG) && defined(NCFG_LOG_TO_SERIAL)

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
#define log_to_serial_init()            NCFG_LOG_SERIAL_PORT.begin(NCFG_LOG_SERIAL_BAUDRATE);\
                                        NCFG_LOG_SERIAL_PORT.println()

#define log_to_serial_send(buff, size)  NCFG_LOG_SERIAL_PORT.write(buff, size)

#else

#define log_to_serial_init()            ((void)0)
#define log_to_serial_send(buff, size)  ((void)0)

#endif // NCFG_USE_LOG && NCFG_LOG_TO_SERIAL

#endif // _LOG_TO_SERIAL_H_
