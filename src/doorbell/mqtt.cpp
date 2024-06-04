#include "mqtt.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "board.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

const char *PREFS_KEY_MQTT_BROKER = "MQTTBROKER";
const char *PREFS_KEY_MQTT_PORT = "MQTTPORT";
const char *PREFS_KEY_MQTT_USER = "MQTTUSER";
const char *PREFS_KEY_MQTT_PASS = "MQTTPASS";

const char *topic = "home/esp32-icc/doorbell";

void MQTTConnection::setup(t_config config) {
    if (config.mqtt_broker.isEmpty() || config.mqtt_port == 0 || config.mqtt_user.isEmpty() || config.mqtt_pass.isEmpty()) {
        Serial.println("MQTT Config insufficent. Skipping setup");
        return;
    }

    client.setServer(config.mqtt_broker.c_str(), config.mqtt_port);
    String client_id = "esp32-icc-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), config.mqtt_user.c_str(), config.mqtt_pass.c_str())) {
        Serial.println("MQTT Connected to " + config.mqtt_broker);
    } else {
        Serial.print("MQTT connection failed with status: ");
        Serial.println(client.state());
    }
}

void MQTTConnection::send_doorbell_event() {
    if (client.connected()) {
        Serial.println("Sending ring event");
        client.publish(topic, "ring");
    }
}

void MQTTConnection::handle_doorbell_sensor() {
    if (digitalRead(DOORBELL_PIN) == HIGH) {
        long currentMillis = millis();
        if (currentMillis - previous_millis >= 5000L) {
            send_doorbell_event();
            previous_millis = currentMillis;
        }
    }
}
