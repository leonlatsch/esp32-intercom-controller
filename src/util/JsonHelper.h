#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <WString.h>
#include "board.h"

void readCredentialsFromRequest(String rawBody, String &ssidOut, String &passOut);
String create_device_information_json(t_device_info device_info);

#endif