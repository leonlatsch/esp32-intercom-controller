#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Preferences.h>

const char *PREFS_KEY_SSID = "SSID";
const char *PREFS_KEY_PASSWORD = "PASS";

class WiFiManager;

class WiFiManager {
    public:
    WiFiManager(Preferences _prefs);
    void setup_wifi_sta();
    void setup_wifi_ap();
    bool wifi_creds_configures();
};

#endif