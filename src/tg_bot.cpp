#include "tg_bot.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "pump_drv.h"
#include "wifi_drv.h"
#include "log.h"

static const char TAG[] = "TG_BOT";

static WiFiClientSecure secured_client;
static UniversalTelegramBot bot(NCFG_TG_BOT_TOKEN, secured_client);
static unsigned long last_tg_check = 0;

// State machine for settings
enum {
    TG_IDLE,
    TG_WAIT_ON_TIME,
    TG_WAIT_OFF_TIME
} tg_state = TG_IDLE;

static unsigned long temp_on_time = 0;

// Function to send main menu (buttons below)
static void send_main_menu(String chat_id, String text) {
    // Reply Keyboard structure (buttons instead of keyboard)
    String kbd = "[";
    kbd += "[\"🟢 AUTO\", \"ℹ️ STATUS\"],";
    kbd += "[\"💧 TURN ON\", \"🛑 TURN OFF\"],";
    kbd += "[\"⚙️ Configure timers\"]";
    kbd += "]";
    
    // resize_keyboard: true makes buttons compact
    bot.sendMessageWithReplyKeyboard(chat_id, text, "", kbd, true);
}

static void handle_new_messages(int numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;

        if (chat_id != NCFG_TG_CHAT_ID) continue;

        LOG_I(TAG, "Message: %s", text.c_str());

        // 1. Processing numeric input (settings mode)
        if (tg_state == TG_WAIT_ON_TIME) {
            int mins = text.toInt();
            if (mins > 0) {
                temp_on_time = mins * 60 * 1000UL;
                tg_state = TG_WAIT_OFF_TIME;
                bot.sendMessage(chat_id, "Ok. Now enter PAUSE time (in minutes):", "");
            } else {
                bot.sendMessage(chat_id, "Please enter a number greater than 0.", "");
            }
            continue;
        } 
        
        if (tg_state == TG_WAIT_OFF_TIME) {
            int mins = text.toInt();
            if (mins > 0) {
                unsigned long temp_off_time = mins * 60 * 1000UL;
                pump_set_intervals(temp_on_time, temp_off_time); // Saving to EEPROM
                tg_state = TG_IDLE;
                send_main_menu(chat_id, "💾 Settings saved!\n\n" + pump_get_mode_string());
            } else {
                bot.sendMessage(chat_id, "Please enter a number greater than 0.", "");
            }
            continue;
        }

        // 2. Processing button commands
        if (text == "🟢 AUTO") {
            pump_set_mode(PUMP_MODE_AUTO);
            send_main_menu(chat_id, "Mode: AUTOMATIC");
        } 
        else if (text == "💧 TURN ON") {
            pump_set_mode(PUMP_MODE_MANUAL_ON);
            send_main_menu(chat_id, "Mode: MANUAL (On)");
        } 
        else if (text == "🛑 TURN OFF") {
            pump_set_mode(PUMP_MODE_MANUAL_OFF);
            send_main_menu(chat_id, "Mode: MANUAL (Off)");
        } 
        else if (text == "ℹ️ STATUS") {
            send_main_menu(chat_id, pump_get_mode_string());
        } 
        else if (text == "⚙️ Configure timers") {
            tg_state = TG_WAIT_ON_TIME;
            bot.sendMessage(chat_id, "Enter PUMP RUNNING time (minutes):", "");
        } 
        else {
            // For all other cases or /start command
            send_main_menu(chat_id, "Tower control:");
        }
    }
}

bool tg_bot_init(void) {
    secured_client.setInsecure(); 
    LOG_I(TAG, "Bot initialized.");
    return true;
}

void tg_bot_poll(void) {
    if (!wifi_is_connected()) return;

    if (millis() - last_tg_check > NCFG_TG_POLL_TIME_MS) {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) {
            handle_new_messages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        last_tg_check = millis();
    }
}