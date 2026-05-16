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
#include <Arduino.h>
#include "hydro_port.h"

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
typedef struct {
  uint32_t heap_min_free_u8;
} node_port_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
const char* sp_sys_rst_rsn_to_str(uint8_t code);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const char TAG[] = "H-PORT";

/****************************************************************************
 * Private Functions
 ****************************************************************************/
const char* ht_sys_rst_rsn_to_str(uint8_t code) {
  switch (code) {
    case 0: return "Power-on reset";
    case 1: return "External reset";
    case 2: return "Brown-out reset";
    case 3: return "Watchdog reset";
    default: return "Undefined";
  }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
// Log system information
void ht_log_sys_info(void) {
  LOG_I(TAG, "System Information:");
  LOG_I(TAG, "Board: Arduino Mega 2560");
  LOG_I(TAG, "CPU Frequency: 16 MHz");
  LOG_I(TAG, "Flash Memory: 256 KB");
  LOG_I(TAG, "SRAM: 8 KB");
  LOG_I(TAG, "EEPROM: 4 KB");
}

// Log memory information
void ht_log_memory_info(void) {
  extern int __heap_start, *__brkval;
  int v;
  int free_memory = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);

  LOG_I(TAG, "Free Memory: %d bytes", free_memory);
}

// Dynamic memory allocation
void* ht_calloc(size_t* size_p, uint8_t mem_type, bool log) {
  if ((size_p == NULL) || ((*size_p) == 0)) {
    if (log) {
      LOG_E(TAG, "Error: Invalid arguments for memory allocation");
    }
    return NULL;
  }

  void* mem_p = calloc(*size_p, 1);
  if (mem_p == NULL) {
    if (log) {
      LOG_E(TAG, "Error: Memory allocation failed for size %zu", *size_p);
    }
    return NULL;
  }

  return mem_p;
}

bool ht_free_alt(void* mem_p) {
  if (mem_p != NULL) {
    free(mem_p);
    return true;
  }
  return false;
}

// System reboot
void ht_sys_reboot(uint32_t delay_ms) {
  LOG_I(TAG, "Rebooting...");

  // delay before reboot
  ht_delay(delay_ms);

  // Simulate a reboot by jumping to the reset vector
  ESP.restart();
}
