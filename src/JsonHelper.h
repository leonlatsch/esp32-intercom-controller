#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <WString.h>

void readCredentialsFromRequest(String rawBody, String &ssidOut, String &passOut);

#endif