#ifndef _PUMP_DRV_H_
#define _PUMP_DRV_H_

#include "main.h"
#include <Arduino.h> // Required for working with String class

bool pump_init(void);
void pump_poll(void);

// New functions for interaction with bot
void pump_set_mode(pump_mode_t new_mode);
String pump_get_mode_string(void);

void pump_set_intervals(unsigned long on_ms, unsigned long off_ms);

#endif // _PUMP_DRV_H_