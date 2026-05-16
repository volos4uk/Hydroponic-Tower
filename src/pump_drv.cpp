#include "pump_drv.h"
#include "log.h"
#include <EEPROM.h>

static const char TAG[] = "PUMP";

// Current working timings
static unsigned long pump_on_ms = NCFG_DEFAULT_PUMP_ON_MS;
static unsigned long pump_off_ms = NCFG_DEFAULT_PUMP_OFF_MS;

static unsigned long previousMillis = 0;
static pump_mode_t currentMode = PUMP_MODE_AUTO;
static bool isPumpRunning = false;

static void set_relay(bool state) {
    if (state != isPumpRunning) {
        isPumpRunning = state;
        uint8_t pin_signal = state ? NCFG_PUMP_ACTIVE_LEVEL : !NCFG_PUMP_ACTIVE_LEVEL;
        digitalWrite(NCFG_PUMP_PIN, pin_signal); 
        LOG_I(TAG, "Relay -> %s", state ? "ON" : "OFF");
    }
}

bool pump_init(void) {
    pinMode(NCFG_PUMP_PIN, OUTPUT);
    set_relay(false); 
    
    // Initialize memory (16 bytes is enough for two variables)
    EEPROM.begin(16);
    uint32_t saved_on, saved_off;
    EEPROM.get(0, saved_on);
    EEPROM.get(4, saved_off);
    
    // If memory is not empty (clean flash returns 0xFFFFFFFF)
    if (saved_on != 0xFFFFFFFF && saved_on > 0 && saved_off != 0xFFFFFFFF && saved_off > 0) {
        pump_on_ms = saved_on;
        pump_off_ms = saved_off;
        LOG_I(TAG, "Loaded EEPROM: ON=%lu, OFF=%lu", pump_on_ms, pump_off_ms);
    }
    
    currentMode = PUMP_MODE_AUTO; 
    previousMillis = millis();
    return true;
}

void pump_set_intervals(unsigned long on_ms, unsigned long off_ms) {
    pump_on_ms = on_ms;
    pump_off_ms = off_ms;
    
    // Save to flash memory
    EEPROM.put(0, (uint32_t)pump_on_ms);
    EEPROM.put(4, (uint32_t)pump_off_ms);
    EEPROM.commit();
    
    LOG_I(TAG, "Saved new timings");
    
    // Restart cycle if we are in AUTO mode
    if (currentMode == PUMP_MODE_AUTO) {
        previousMillis = millis();
        set_relay(false);
    }
}

void pump_set_mode(pump_mode_t new_mode) {
    if (currentMode == new_mode) return; 
    currentMode = new_mode;
    
    switch (currentMode) {
        case PUMP_MODE_MANUAL_ON:  set_relay(true); break;
        case PUMP_MODE_MANUAL_OFF: set_relay(false); break;
        case PUMP_MODE_AUTO:
            previousMillis = millis(); 
            set_relay(false); 
            break;
    }
}

String pump_get_mode_string() {
    String info = "";
    if (currentMode == PUMP_MODE_MANUAL_ON) info = "MANUAL (Forced ON)";
    else if (currentMode == PUMP_MODE_MANUAL_OFF) info = "MANUAL (Forced OFF)";
    else info = isPumpRunning ? "AUTO (Currently pumping)" : "AUTO (Currently resting)";
    
    info += "\n⏱ Settings: " + String(pump_on_ms / 60000) + " min / " + String(pump_off_ms / 60000) + " min";
    return info;
}

void pump_poll(void) {
    if (currentMode != PUMP_MODE_AUTO) return; 

    unsigned long currentMillis = millis();
    if (isPumpRunning) {
        if (currentMillis - previousMillis >= pump_on_ms) { // Using dynamic variable
            set_relay(false);
            previousMillis = currentMillis;
        }
    } else {
        if (currentMillis - previousMillis >= pump_off_ms) { // Using dynamic variable
            set_relay(true);
            previousMillis = currentMillis;
        }
    }
}