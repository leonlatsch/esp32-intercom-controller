#include <Preferences.h>
#include <WiFi.h>
#include <board_interaction.h>

#include "WiFiManager.h"

const char *AP_SSID = "ESP32 Intercom Controller";
const char *PREFS_KEY_SSID = "SSID";
const char *PREFS_KEY_PASSWORD = "PASS";

class WiFiManager {
    Preferences prefs;

    public:
    WiFiManager(Preferences &_prefs) {
        prefs = _prefs;
    }

    void setup_wifi_sta() {
        Serial.println("Starting WiFi in STA mode");

        String ssid = prefs.getString(PREFS_KEY_SSID);
        String pass = prefs.getString(PREFS_KEY_PASSWORD);

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);

        // Wait for connection
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
        }

        if (WiFi.isConnected()) {
            print_connection(ssid, WiFi.localIP());
            blink(2);
        }
    }

    void setup_wifi_ap() {
        Serial.println("Starting WiFi in AP mode");

        // SSID, no password, channel 1, not hidden, 1 connection max
        WiFi.softAP(AP_SSID, NULL, 1, 0, 1);
        print_connection(AP_SSID, WiFi.softAPIP());
        digitalWrite(LED_BLUE, HIGH);
    }

    bool wifi_creds_configured() {
        String ssid = prefs.getString(PREFS_KEY_SSID);
        String pass = prefs.getString(PREFS_KEY_PASSWORD);

        return !ssid.isEmpty() && !pass.isEmpty();
    }
    
    protected:
    void print_connection(String ssid, IPAddress ip) {
        Serial.print(ssid);
        Serial.print(" | ");
        Serial.println(ip);
    }
};