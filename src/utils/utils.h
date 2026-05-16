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

#ifndef _UTILS_H_
#define _UTILS_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "main.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define UTILS_STRINGIFY(n)             #n
#define UTILS_STRINGIFY_EXPAND(n)      UTILS_STRINGIFY(n)

#define UTILS_MIN(a,b)                 ((a)>=(b)?(b):(a))
#define UTILS_ABS(val)                 (((val)<0)?(0-(val)):(val))

#define UTILS_SAVE_STR_P(p)            ((p)==NULL?"NULL":(p))

#define UTILS_ARR_LEN(x)               (sizeof(x) / sizeof(x[0]))

#define UTILS_KB                       1024UL
#define UTILS_MB                       (UTILS_KB*1024UL)
#define UTILS_GB                       (UTILS_MB*1024UL)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
// Timeout ---
uint32_t utils_calc_timeout(uint32_t ts_prev);

// Capacity calculations ---
float utils_bytes_to_kb(uint64_t bytes);
float utils_bytes_to_mb(uint64_t bytes);
float utils_bytes_to_gb(uint64_t bytes);
void utils_capacity_calculate(uint64_t total_bytes, float* capacity_p, char* units_p, size_t units_size);

// Utility function to convert a float to a string
void utils_float_to_str(float value, int decimals, char* buffer, size_t buffer_size);

#endif // _UTILS_H_
