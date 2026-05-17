#ifndef _CONFIG_H_
#define _CONFIG_H_

#define NCFG_USE_LOG                 1
#define NCFG_LOG_TO_SERIAL           1
#define NCFG_LOG_TYPE_DIRECT         1
#define NCFG_LOG_TYPE_BUFF           2
#define NCFG_LOG_TYPE                NCFG_LOG_TYPE_DIRECT

#define NT_LOG_LEVEL_TRACE           5
#define NCFG_LOG_LEVEL               NT_LOG_LEVEL_TRACE

#define NCFG_LOG_SERIAL_PORT         Serial
#define NCFG_LOG_SERIAL_BAUDRATE     115200
#define NCFG_LOG_TAG_STR_MAX_SIZE    10
#define NCFG_LOG_MESS_BODY_MAX_SIZE  128
#define NT_LINE_END_CRLF             1
#define NCFG_LOG_LINE_END            NT_LINE_END_CRLF

#define os_app_get_core_id()         0

// --- Wi-Fi and OTA Settings ---
#define NCFG_WIFI_SSID       "TP-Link_IoT"
#define NCFG_WIFI_PASS       "18972559"
#define NCFG_OTA_HOSTNAME    "Hydro-Tower"

// --- Hardware Settings (Hardware) ---
// D1 on Wemos D1 Mini corresponds to GPIO5
#define NCFG_PUMP_PIN        5 
#define NCFG_PUMP_ACTIVE_LEVEL   HIGH

// --- Watering timings (in milliseconds) ---
// Warning: using UL (Unsigned Long) to avoid overflow
#define NCFG_DEFAULT_PUMP_ON_MS      (1 * 60 * 1000UL) // 15 minutes of work
#define NCFG_DEFAULT_PUMP_OFF_MS     (1 * 60 * 1000UL) // 45 minutes of rest

#define NCFG_TG_BOT_TOKEN    "YOUR_BOT_TOKEN_HERE" 
// Find your Chat ID (for example, from @userinfobot bot) so that only you can control it
#define NCFG_TG_CHAT_ID      "YOUR_CHAT_ID_HERE"    
#define NCFG_TG_POLL_TIME_MS 1000 // How often to check for new messages (1 sec)

#endif // _CONFIG_H_