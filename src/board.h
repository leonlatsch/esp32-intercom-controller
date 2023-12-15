#ifndef BOARD_H
#define BOARD_H

#include <WString.h>

const int LED_BLUE = 2;
const int DOOR_OPENER_PIN = 5;
const int DOORBELL_PIN = 15;

extern const char* DEVICE_NAME;
extern const char* DEVICE_MANUFACTURER;
extern const char* PREFS_KEY_LOW_TRIGGER_RELAY;

struct t_device_info {
    String name;
    String manufacturer;
    String serial_number;
};

void blink(int times);
void led_on();
void led_off();

t_device_info get_device_information();

void openDoor(bool low_trigger_relay);

#endif