#include <WString.h>
#include <json_parser.h>

#include "JsonHelper.h"

const char *SSID_JSON_KEY = "ssid";
const char *PASS_JSON_KEY = "pass";

void readCredentialsFromRequest(String rawBody, String &ssidOut, String &passOut) {
    jparse_ctx_t jctx;
    json_parse_start(&jctx, rawBody.c_str(), rawBody.length());

    char ssid[64];
    char pass[64];

    if (json_obj_get_string(&jctx, SSID_JSON_KEY, ssid, sizeof(ssid)) == OS_SUCCESS) {
        ssidOut = String(ssid);
    }
    if (json_obj_get_string(&jctx, PASS_JSON_KEY, pass, sizeof(pass)) == OS_SUCCESS) {
        passOut = String(pass);
    }
}