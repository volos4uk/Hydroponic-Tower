#include <Arduino.h>
#include "config.h"
#include "main.h"

#include "wifi_drv.h"
#include "ota_drv.h"
#include "tg_bot.h"
#include "pump_drv.h"

#include "utils.h"

#if NCFG_COMMON_LOG_LEVEL < NCFG_LOG_LEVEL
#  undef NCFG_LOG_LEVEL
#  define NCFG_LOG_LEVEL NCFG_COMMON_LOG_LEVEL
#endif // NCFG_COMMON_LOG_LEVEL < NCFG_LOG_LEVEL
#include "log.h"

static const char TAG[] = "MAIN";

void setup() {

  log_init();
  wifi_init();
  ota_init();
  tg_bot_init();
  pump_init();

}

void loop() {
  
  wifi_poll();
  ota_poll();
  tg_bot_poll();
  pump_poll();
}