#include <esp32-hal-gpio.h>

#include "board.h"

void blink(int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(LED_BLUE, HIGH);
        delay(200);
        digitalWrite(LED_BLUE, LOW);
        delay(200);
    }
}

void led_on() {
    digitalWrite(LED_BLUE, HIGH);
}

void led_off() {
    digitalWrite(LED_BLUE, LOW);
}

t_device_info get_device_information() {
    t_device_info d_info;

    d_info.name = "Name";
    d_info.manufacturer = "Techguyz";
    d_info.serial_number = "TODO";

    return d_info;
}

void openDoor() {
    digitalWrite(LED_BLUE, HIGH);
    delay(4000);
    digitalWrite(LED_BLUE, LOW);
}
