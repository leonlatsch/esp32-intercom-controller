#include <Preferences.h>
#include <WiFi.h>
#include <board.h>

#include "WiFiManager.h"

const char *PREFS_KEY_SSID = "SSID";
const char *PREFS_KEY_PASSWORD = "PASS";

const char *AP_SSID = "ESP32 Intercom Controller";

const int CONNECTION_TRIES = 60;

void print_connection(String ssid, IPAddress ip) {
    Serial.print(ssid);
    Serial.print(" | ");
    Serial.println(ip);
}

void WiFiManager::setup_wifi_sta() {
    Serial.println("Starting WiFi in STA mode");

    String ssid = prefs.getString(PREFS_KEY_SSID);
    String pass = prefs.getString(PREFS_KEY_PASSWORD);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, pass);

    long tries = 0;

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        tries++;
        led_on();
        delay(500);
        if (tries > CONNECTION_TRIES) {
            Serial.println("Connecting to " + ssid + " failed for 60 seconds");
            led_off();
            break;
        }
        led_off();
        delay(500);
    }

    if (WiFi.isConnected()) {
        print_connection(ssid, WiFi.localIP());
        blink(5);
    }
}

void WiFiManager::setup_wifi_ap() {
     Serial.println("Starting WiFi in AP mode");

    // SSID, no password, channel 1, not hidden, 1 connection max
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, NULL, 1, 0, 1);
    print_connection(AP_SSID, WiFi.softAPIP());
    digitalWrite(LED_BLUE, HIGH);
}

bool WiFiManager::wifi_creds_configured() {
    String ssid = prefs.getString(PREFS_KEY_SSID);
    String pass = prefs.getString(PREFS_KEY_PASSWORD);

    return !ssid.isEmpty() && !pass.isEmpty();
}

wifi_mode_t WiFiManager::setup_wifi() {
    if (wifi_creds_configured()) {
        setup_wifi_sta();
        if (WiFi.isConnected()) {
            return WIFI_STA;
        } else {
            setup_wifi_ap();
            return WIFI_AP;
        }
    } else {
        setup_wifi_ap();
        return WIFI_AP;
    }
}
