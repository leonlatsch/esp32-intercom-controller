#ifndef CONFIG_H
#define CONFIG_H

#include <WString.h>
#include "PrefsWrapper.h"

struct t_config {
    String ssid;
    String pass;
    bool low_trigger_relay;
    String mqtt_broker;
    int mqtt_port;
    String mqtt_user;
    String mqtt_pass;
};

t_config get_config(PrefsWrapper prefs);
bool save_config(PrefsWrapper prefs, t_config config);

#endif