#include "ota_drv.h"
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include "wifi_drv.h"
#include "log.h"

static const char TAG[] = "OTA";

bool ota_init(void) {
    if (MDNS.begin(NCFG_OTA_HOSTNAME)) {
        LOG_I(TAG, "mDNS started: %s.local", NCFG_OTA_HOSTNAME);
    }
    
    ArduinoOTA.setHostname(NCFG_OTA_HOSTNAME);
    ArduinoOTA.onStart([]() { LOG_I(TAG, "Start OTA..."); });
    ArduinoOTA.onEnd([]()   { LOG_I(TAG, "End OTA"); });
    ArduinoOTA.onError([](ota_error_t error) { LOG_E(TAG, "Error[%u]", error); });

    ArduinoOTA.begin();
    return true;
}

void ota_poll(void) {
    MDNS.update();
    ArduinoOTA.handle();
}