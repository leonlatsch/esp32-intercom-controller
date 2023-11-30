#ifndef BOARD_H
#define BOARD_H

#include <WString.h>

const int LED_BLUE = 2;

struct t_device_info {
    String name;
    String manufacturer;
    String serial_number;
};

void blink(int times);
void led_on();
void led_off();

t_device_info get_device_information();

void openDoor();

#endif