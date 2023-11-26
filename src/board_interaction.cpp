#include <esp32-hal-gpio.h>

#include "board_interaction.h"

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

void openDoor() {
    digitalWrite(LED_BLUE, HIGH);
    delay(4000);
    digitalWrite(LED_BLUE, LOW);
}
