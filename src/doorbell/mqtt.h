#ifndef MQTT_H
#define MQTT_H

#include "config.h"

extern const char *PREFS_KEY_MQTT_BROKER;
extern const char *PREFS_KEY_MQTT_PORT;
extern const char *PREFS_KEY_MQTT_USER;
extern const char *PREFS_KEY_MQTT_PASS;

class MQTTConnection;

class MQTTConnection {
private:
    long previous_millis;
public:
    void setup(t_config config);
    void send_doorbell_event();
    void handle_doorbell_sensor();
};

#endif