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

#ifndef _LOG_COM_H_
#define _LOG_COM_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

// char-code of 'T', 'D', 'I' and so on
#define LOG_TYPE_T      (0x54)  // Trace
#define LOG_TYPE_D      (0x44)  // Debug
#define LOG_TYPE_I      (0x49)  // Info
#define LOG_TYPE_W      (0x57)  // Warning
#define LOG_TYPE_E      (0x45)  // Error
#define LOG_TYPE_F      (0x46)  // Fail

// flags(7 and 5 it is free bits according to all possible LOG_TYPE)
#define LOG_HE_F        (0x00)                          // header + line end
#define LOG_NH_F        (1<<7)                          // no header but line end
#define LOG_NE_F        (1<<5)                          // header but no line end
#define LOG_NH_NE_F     (LOG_NH_F | LOG_NE_F)           // no header no line end

// Filename without path
#define LOG_FN          (__builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#endif // _LOG_COM_H_
