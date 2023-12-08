#include <WString.h>
#include <json_parser.h>
#include <json_generator.h>

#include "JsonHelper.h"

const char *SSID_JSON_KEY = "ssid";
const char *PASS_JSON_KEY = "pass";
const char *LOW_TRIGGER_RELAY_KEY = "low_trigger_relay";

void read_config_from_request(String rawBody, String &ssidOut, String &passOut, bool &low_trigger_relay_out) {
    jparse_ctx_t jctx;
    json_parse_start(&jctx, rawBody.c_str(), rawBody.length());

    char ssid[64];
    char pass[64];
    bool low_trigger_relay;

    if (json_obj_get_string(&jctx, SSID_JSON_KEY, ssid, sizeof(ssid)) == OS_SUCCESS) {
        ssidOut = String(ssid);
    }
    if (json_obj_get_string(&jctx, PASS_JSON_KEY, pass, sizeof(pass)) == OS_SUCCESS) {
        passOut = String(pass);
    }
    if (json_obj_get_bool(&jctx, LOW_TRIGGER_RELAY_KEY, &low_trigger_relay) == OS_SUCCESS) {
        low_trigger_relay_out = low_trigger_relay;
    }
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
