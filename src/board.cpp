#include <esp32-hal-gpio.h>

#include "board.h"
#include <WiFi.h>

const char* DEVICE_NAME = "ESP32 Intercom Controller V1 Prototype";
const char* DEVICE_MANUFACTURER = "Techguyz";
const char* PREFS_KEY_LOW_TRIGGER_RELAY = "pkltr";

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

    d_info.name = DEVICE_NAME;
    d_info.manufacturer = DEVICE_MANUFACTURER;
    d_info.serial_number = WiFi.macAddress();

    return d_info;
}

void openDoor(bool low_trigger_relay) {
    int on_action;
    int off_action;

    if (low_trigger_relay) {
        on_action = LOW;
        off_action = HIGH;
    } else {
        on_action = HIGH;
        off_action = LOW;
    }

    digitalWrite(DOOR_OPENER_PIN, on_action);
    delay(4000);
    digitalWrite(DOOR_OPENER_PIN, off_action);
}
