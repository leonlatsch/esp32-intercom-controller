#include "config.h"
#include "connection/WiFiManager.h"
#include "board.h"

t_config get_config(PrefsWrapper prefs) {
    t_config config;

    config.ssid = prefs.getString(PREFS_KEY_SSID);
    config.pass = prefs.getString(PREFS_KEY_PASSWORD);
    config.low_trigger_relay = prefs.getBool(PREFS_KEY_LOW_TRIGGER_RELAY);

    return config;
}

bool save_config(PrefsWrapper prefs, t_config config) {
    bool success = true;

    success += prefs.putString(PREFS_KEY_SSID, config.ssid);
    success += prefs.putString(PREFS_KEY_PASSWORD, config.pass);
    success += prefs.putBool(PREFS_KEY_LOW_TRIGGER_RELAY, config.low_trigger_relay);

    return success;
}
