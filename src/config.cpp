#include "config.h"
#include "connection/WiFiManager.h"
#include "board.h"
#include "doorbell/mqtt.h"

t_config get_config(PrefsWrapper prefs) {
    t_config config;

    config.ssid = prefs.getString(PREFS_KEY_SSID);
    config.pass = prefs.getString(PREFS_KEY_PASSWORD);
    config.low_trigger_relay = prefs.getBool(PREFS_KEY_LOW_TRIGGER_RELAY);
    config.mqtt_broker = prefs.getString(PREFS_KEY_MQTT_BROKER);
    config.mqtt_port = prefs.getInt(PREFS_KEY_MQTT_PORT);
    config.mqtt_user = prefs.getString(PREFS_KEY_MQTT_USER);
    config.mqtt_pass = prefs.getString(PREFS_KEY_MQTT_PASS);

    return config;
}

bool save_config(PrefsWrapper prefs, t_config config) {
    bool success = true;

    success += prefs.putString(PREFS_KEY_SSID, config.ssid);
    success += prefs.putString(PREFS_KEY_PASSWORD, config.pass);
    success += prefs.putBool(PREFS_KEY_LOW_TRIGGER_RELAY, config.low_trigger_relay);
    success += prefs.putString(PREFS_KEY_MQTT_BROKER, config.mqtt_broker);
    success += prefs.putInt(PREFS_KEY_MQTT_PORT, config.mqtt_port);
    success += prefs.putString(PREFS_KEY_MQTT_USER, config.mqtt_user);
    success += prefs.putString(PREFS_KEY_MQTT_PASS, config.mqtt_pass);

    return success;
}
