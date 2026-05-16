#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

#include <inttypes.h>
#include <stdint.h>
#include <stddef.h>
#include "hydro_port.h"

#include "config.h"

// Global pump states
typedef enum {
  PUMP_MODE_AUTO,       // Runs on timer 15/45
  PUMP_MODE_MANUAL_ON,  // Forced on via Telegram
  PUMP_MODE_MANUAL_OFF  // Forced off via Telegram
} pump_mode_t;

#endif // _MAIN_H_