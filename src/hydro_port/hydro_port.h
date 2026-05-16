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

#ifndef _HYDRO_PORT_H_
#define _HYDRO_PORT_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/

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
// Information ---
void ht_log_sys_info(void);
void ht_log_memory_info(void);

// TS and delay ---
#define ht_now()            millis()
#define ht_delay(x)         delay(x)

// Dynamic memory ---
void* ht_calloc(size_t* size_p, uint8_t mem_type, bool log);
bool ht_free_alt(void* mem_p);
#define ht_free(mem_p)  if (ht_free_alt((mem_p))) (mem_p) = NULL

// System reboot ---
void ht_sys_reboot(uint32_t delay_ms);

#endif // _HYDRO_PORT_H_