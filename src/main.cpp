#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "util/uuid/uuid.h"
#include "util/JsonHelper.h"
#include "board.h"
#include "connection/WiFiManager.h"
#include "PrefsWrapper.h"
#include "security/DeviceSecretStore.h"
#include "config.h"

const char *CONTENT_TYPE_TEXT = "text/text";
const char *CONTENT_TYPE_JSON = "application/json";

const char *SECTER_HEADER_NAME = "secret";
const char *EXPECTED_HEADERS[] = {"secret"};

t_config config;

const int PORT = 80;
WebServer server(PORT);

PrefsWrapper prefs = PrefsWrapper();
WiFiManager wifiManager(prefs);
DeviceSecretStore deviceSecretStore(prefs);

/// HTTP STA

void securedEndpoint(std::function<void(void)> handler) {
    String sentSecret = server.header(SECTER_HEADER_NAME);
    if (deviceSecretStore.getDeviceSecret() == sentSecret) {
        handler();
    } else {
        server.send(403);
    }
}

void handleHealthCheck() {
    server.send(200, CONTENT_TYPE_TEXT, "ESP32 Intercom Controller");
}

void handleDeviceInformation() {
    t_device_info device_info = get_device_information();
    String device_info_json = create_device_information_json(device_info);

    server.send(200, CONTENT_TYPE_JSON, device_info_json);
}

void handleOpenDoor() {
    securedEndpoint([]() {
        server.send(200);
        openDoor(config.low_trigger_relay); 
    });
}

void handleConfig() {
    securedEndpoint([]() {
        t_config config = get_config(prefs);
        String config_json = create_config_json(config);

        server.send(200, CONTENT_TYPE_JSON, config_json); 
    });
}

void handleReset() {
    prefs.clear();

    server.send(200, CONTENT_TYPE_TEXT, "Reset. Restarting in 5 seconds...");
    delay(5000);
    ESP.restart();
}

/// HTTP AP

void handleSetup() {
    String rawBody = server.arg(0);

    t_config config = read_config_from_request(rawBody);
    if (config.ssid == NULL || config.pass == NULL) {
        server.send(400);
        return;
    }
    save_config(prefs, config);

    String deviceSecret = deviceSecretStore.getDeviceSecret();

    server.send(200, CONTENT_TYPE_TEXT, "Setup complete. Restarting in 5 seconds | Device Secret: " + deviceSecret);
    delay(5000);
    digitalWrite(LED_BLUE, LOW);
    ESP.restart();
}

/// ROUTING SETUP

void setup_sta_routing() {
    server.on("/", HTTP_GET, handleHealthCheck);
    server.on("/device", HTTP_GET, handleDeviceInformation);
    server.on("/config", HTTP_GET, handleConfig);
    server.on("/opendoor", HTTP_POST, handleOpenDoor);
    server.on("/reset", HTTP_GET, handleReset);

    server.on("/setup", HTTP_POST, handleSetup);

    server.collectHeaders(EXPECTED_HEADERS, sizeof(EXPECTED_HEADERS));
    server.begin();
}

void setup_ap_routing() {
    server.on("/setup", HTTP_POST, handleSetup);
    server.begin();
}

/// GENERAL SETUP

void setupBoard() {
    // Serial
    Serial.begin(9600);
    while (!Serial);
    Serial.println();

    // Pins
    pinMode(LED_BLUE, OUTPUT);
    pinMode(DOOR_OPENER_PIN, OUTPUT);

    config = get_config(prefs);

    if (config.low_trigger_relay) {
        Serial.println("Setting up for low trigger relay");
        digitalWrite(DOOR_OPENER_PIN, HIGH);
    }

    Serial.println(deviceSecretStore.getDeviceSecret());

    delay(100);
}

/// LIFECYCLE METHODS

void setup() {
    setupBoard();

    wifi_mode_t mode = wifiManager.setup_wifi();
    if (mode == WIFI_STA) {
        setup_sta_routing();
    } else if (mode == WIFI_AP) {
        setup_ap_routing();
    }
}

void loop() {
    server.handleClient();
}
