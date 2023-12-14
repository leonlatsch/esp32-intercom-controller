#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <WString.h>
#include "board.h"
#include "config.h"

t_config read_config_from_request(String rawBody);
String create_config_json(t_config config);
String create_device_information_json(t_device_info device_info);

#endif