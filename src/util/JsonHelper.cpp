#include <WString.h>
#include <json_parser.h>
#include <json_generator.h>

#include "JsonHelper.h"

const char *SSID_JSON_KEY = "ssid";
const char *PASS_JSON_KEY = "pass";
const char *LOW_TRIGGER_RELAY_KEY = "low_trigger_relay";
const char *MQTT_BROKER_KEY = "mqtt_broker";
const char *MQTT_PORT_KEY = "mqtt_port";
const char *MQTT_USER_KEY = "mqtt_user";
const char *MQTT_PASS_KEY = "mqtt_pass";

t_config read_config_from_request(String rawBody) {
    jparse_ctx_t jctx;
    json_parse_start(&jctx, rawBody.c_str(), rawBody.length());

    t_config config;

    char ssid[64];
    char pass[64];
    bool low_trigger_relay;
    char mqtt_broker[64];
    int mqtt_port;
    char mqtt_user[64];
    char mqtt_pass[64];

    if (json_obj_get_string(&jctx, SSID_JSON_KEY, ssid, sizeof(ssid)) == OS_SUCCESS) {
        config.ssid = String(ssid);
    }
    if (json_obj_get_string(&jctx, PASS_JSON_KEY, pass, sizeof(pass)) == OS_SUCCESS) {
        config.pass = String(pass);
    }
    if (json_obj_get_bool(&jctx, LOW_TRIGGER_RELAY_KEY, &low_trigger_relay) == OS_SUCCESS) {
        config.low_trigger_relay = low_trigger_relay;
    }
    if (json_obj_get_string(&jctx, MQTT_BROKER_KEY, mqtt_broker, sizeof(mqtt_broker)) == OS_SUCCESS) {
        config.mqtt_broker = String(mqtt_broker);
    }
    if (json_obj_get_int(&jctx, MQTT_PORT_KEY, &mqtt_port) == OS_SUCCESS) {
        config.mqtt_port = mqtt_port;
    }
    if (json_obj_get_string(&jctx, MQTT_USER_KEY, mqtt_user, sizeof(mqtt_user)) == OS_SUCCESS) {
        config.mqtt_user = String(mqtt_user);
    }
    if (json_obj_get_string(&jctx, MQTT_PASS_KEY, mqtt_pass, sizeof(mqtt_pass)) == OS_SUCCESS) {
        config.mqtt_pass = String(mqtt_pass);
    }

    return config;
}

String create_config_json(t_config config) {
    char jsonString[512];

    json_gen_str_t jstr;
    json_gen_str_start(&jstr, jsonString, sizeof(jsonString), nullptr, nullptr);

    json_gen_start_object(&jstr);
    json_gen_obj_set_string(&jstr, SSID_JSON_KEY, config.ssid.c_str());
    json_gen_obj_set_string(&jstr, PASS_JSON_KEY, config.pass.c_str());
    json_gen_obj_set_bool(&jstr, LOW_TRIGGER_RELAY_KEY, config.low_trigger_relay);
    json_gen_obj_set_string(&jstr, MQTT_BROKER_KEY, config.mqtt_broker.c_str());
    json_gen_obj_set_int(&jstr, MQTT_PORT_KEY, config.mqtt_port);
    json_gen_obj_set_string(&jstr, MQTT_USER_KEY, config.mqtt_user.c_str());
    json_gen_obj_set_string(&jstr, MQTT_PASS_KEY, config.mqtt_pass.c_str());
    json_gen_end_object(&jstr);

    int totalLen = json_gen_str_end(&jstr);

    return String(jsonString);
}

String create_device_information_json(t_device_info device_info) {
    char jsonString[256];

    json_gen_str_t jstr;
    json_gen_str_start(&jstr, jsonString, sizeof(jsonString), nullptr, nullptr);

    json_gen_start_object(&jstr);
    json_gen_obj_set_string(&jstr, "name", device_info.name.c_str());
    json_gen_obj_set_string(&jstr, "serial", device_info.serial_number.c_str());
    json_gen_obj_set_string(&jstr, "manufacturer", device_info.manufacturer.c_str());
    json_gen_end_object(&jstr);

    int totalLen = json_gen_str_end(&jstr);

    return String(jsonString);
}
