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
#include "utils.h"

#if NCFG_COMMON_LOG_LEVEL < NCFG_LOG_LEVEL
#  undef NCFG_LOG_LEVEL
#  define NCFG_LOG_LEVEL NCFG_COMMON_LOG_LEVEL
#endif // NCFG_COMMON_LOG_LEVEL < NCFG_LOG_LEVEL
#include "log.h"

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
static const char TAG[] = "UTILS";

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
// Timeout ---
uint32_t utils_calc_timeout(uint32_t ts_prev) {
  uint32_t ts_now  = ht_now();
  uint32_t timeout;
  if (ts_now >= ts_prev) {
    timeout = ts_now - ts_prev;
  } else {
    timeout = (0xffffffff - ts_prev) + ts_now;
  }
  return timeout;
}

// Capacity calculations ---
float utils_bytes_to_kb(uint64_t bytes) {
  return (((float)bytes)/((float)UTILS_KB));
}

float utils_bytes_to_mb(uint64_t bytes) {
  return (((float)bytes)/((float)UTILS_MB));
}

float utils_bytes_to_gb(uint64_t bytes) {
  return (((float)bytes)/((float)UTILS_GB));
}

void utils_capacity_calculate(uint64_t total_bytes, float* capacity_p, char* units_p, size_t units_size) {
  if (capacity_p != NULL) {
    *capacity_p = 0.0;
  }
  if (units_p != NULL) {
    memset(units_p, 0, units_size);
  }
  if ((capacity_p == NULL) || (units_p == NULL) || (units_size == 0)) {
    LOG_E(TAG, vocm_wrong_args);
    return;
  }
  if (total_bytes < UTILS_KB) {
    // bytes
    *capacity_p = (float)total_bytes;
    memcpy(units_p, vocm_bytes, UTILS_MIN(units_size-1, strlen(vocm_bytes)));
  } else if (total_bytes < UTILS_MB) {
    // kb
    *capacity_p = ((float)total_bytes)/((float)UTILS_KB);
    memcpy(units_p, vocm_kbytes, UTILS_MIN(units_size-1, strlen(vocm_kbytes)));
  } else if (total_bytes < UTILS_GB) {
    // mb
    *capacity_p = ((float)total_bytes)/((float)UTILS_MB);
    memcpy(units_p, vocm_mbytes, UTILS_MIN(units_size-1, strlen(vocm_mbytes)));
  } else {
    // gb
    *capacity_p = ((float)total_bytes)/((float)UTILS_GB);
    memcpy(units_p, vocm_gbytes, UTILS_MIN(units_size-1, strlen(vocm_gbytes)));
  }

  if ((*capacity_p) <= 1.0) {
    units_p[strlen(units_p)-1] = '\0'; // remove 's' ending
  }
}

// Utility function to convert a float to a string
void utils_float_to_str(float value, int decimals, char* buffer, size_t buffer_size) {
  if (buffer == NULL || buffer_size == 0) {
    return;
  }
  dtostrf(value, 0, decimals, buffer);
  buffer[buffer_size - 1] = '\0'; // Ensure null-termination
}