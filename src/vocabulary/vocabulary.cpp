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
#include "vocabulary.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

// logs messages
#if defined(NCFG_USE_LOG)

const char vocm_init[]              = "Init ...";
const char vocm_init_done[]         = "Init done.";
const char vocm_wrong_args[]        = "Wrong args!";
const char vocm_printf_err[]        = "Can't print buff! Err: %d!";

const char vocm_out_of_range_mem[]  = "Out of range! Addr(%lu)+size(%lu)=%lu >= %lu!";
const char vocm_out_of_range_idx[]  = "Out of range! Index: %u. Max value: %u!";
const char vocm_alloc_fail[]        = "Can't allocate %u bytes!";

#endif // NCFG_USE_LOG

const char vocm_bytes[]             = "bytes";
const char vocm_kbytes[]            = "KBytes";
const char vocm_mbytes[]            = "MBytes";
const char vocm_gbytes[]            = "GBytes";

const char vocm_json_state[]        = "{\"state\":%u}";

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/