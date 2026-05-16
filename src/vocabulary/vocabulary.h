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

#ifndef _VOCABULARY_H_
#define _VOCABULARY_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "main.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

// logs messages
#if defined(NCFG_USE_LOG)

extern const char vocm_init[];
extern const char vocm_init_done[] ;
extern const char vocm_wrong_args[];
extern const char vocm_printf_err[];

extern const char vocm_out_of_range_mem[];
extern const char vocm_out_of_range_idx[];
extern const char vocm_alloc_fail[];

#endif // MOD_CFG_USE_LOG

extern const char vocm_bytes[];
extern const char vocm_kbytes[];
extern const char vocm_mbytes[];
extern const char vocm_gbytes[];

extern const char vocm_json_state[];

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#endif // _VOCABULARY_H_