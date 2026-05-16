#include "wifi_drv.h"
#include <ESP8266WiFi.h>
#include "log.h"

static const char TAG[] = "WIFI";

bool wifi_init(void) {
    LOG_I(TAG, "Connecting to %s", NCFG_WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(NCFG_OTA_HOSTNAME);
    WiFi.begin(NCFG_WIFI_SSID, NCFG_WIFI_PASS);

    // Wait for connection (this is normal at startup)
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    LOG_I(TAG, "Connected! IP: %s", WiFi.localIP().toString().c_str());
    return true;
}

void wifi_poll(void) {
    // If Wi-Fi connection is lost during operation - reconnect without blocking main loop
    if (WiFi.status() != WL_CONNECTED) {
        LOG_W(TAG, "Connection lost. Reconnecting...");
        WiFi.reconnect();
    }
}

bool wifi_is_connected(void) {
    return WiFi.status() == WL_CONNECTED;
}