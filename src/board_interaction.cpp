#include <esp32-hal-gpio.h>

#include "board_interaction.h"

const int LED_BLUE = 2;

void blink(int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(LED_BLUE, HIGH);
        delay(100);
        digitalWrite(LED_BLUE, LOW);
        delay(100);
    }
}

void openDoor() {
    digitalWrite(LED_BLUE, HIGH);
    delay(4000);
    digitalWrite(LED_BLUE, LOW);
}
