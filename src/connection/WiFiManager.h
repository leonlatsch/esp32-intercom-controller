#ifndef WiFiManager_H
#define WiFiManager_H

#include "PrefsWrapper.h"
#include <WiFi.h>

extern const char *PREFS_KEY_SSID;
extern const char *PREFS_KEY_PASSWORD;

class WiFiManager;

class WiFiManager {
    private:
        PrefsWrapper prefs;
        void setup_wifi_sta();
        void setup_wifi_ap();
        bool wifi_creds_configured();
    public:
        WiFiManager(PrefsWrapper _prefs) {
            prefs = _prefs;
        }
        wifi_mode_t setup_wifi();
};

#endif