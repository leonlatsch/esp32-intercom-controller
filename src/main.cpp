#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "util/uuid/uuid.h"
#include "util/JsonHelper.h"
#include "board.h"
#include "connection/WiFiManager.h"
#include "PrefsWrapper.h"
#include "security/DeviceSecretStore.h"

const char *CONTENT_TYPE_TEXT = "text/text";
const char *CONTENT_TYPE_JSON = "application/json";

const char *SECTER_HEADER_NAME = "secret";
const char *EXPECTED_HEADERS[] = {"secret"};

const int PORT = 80;
WebServer server(PORT);

PrefsWrapper prefs = PrefsWrapper();
WiFiManager wifiManager(prefs);
DeviceSecretStore deviceSecretStore(prefs);

bool low_level_relay;

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
        openDoor(low_level_relay); 
    });
}

void handleConfig() {
    securedEndpoint([]() {
        String ssid = prefs.getString(PREFS_KEY_SSID);
        String pass = prefs.getString(PREFS_KEY_PASSWORD);
        bool low_trigger_relay = prefs.getString(PREFS_KEY_LOW_TRIGGER_RELAY);

        server.send(200, CONTENT_TYPE_TEXT, "SSID: " + ssid + "\nPASS: " + pass + "\nLOW_TRIGGER_RELAY: " + low_level_relay); 
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

    String ssid;
    String password;
    bool low_level_relay;
    read_config_from_request(rawBody, ssid, password, low_level_relay);

    if (ssid == NULL || password == NULL) {
        server.send(400);
        return;
    }

    prefs.putString(PREFS_KEY_SSID, ssid);
    prefs.putString(PREFS_KEY_PASSWORD, password);
    prefs.putBool(PREFS_KEY_LOW_TRIGGER_RELAY, low_level_relay);
    String deviceSecret = deviceSecretStore.getDeviceSecret();

    server.send(200, CONTENT_TYPE_TEXT, "Setup complete. Restarting in 5 seconds | Device Secret: " + deviceSecret);
    delay(5000);
    digitalWrite(LED_BLUE, LOW);
    ESP.restart();
}

/// ROUTING SETUP

void setup_sta_routing() {
    server.on("/", handleHealthCheck);
    server.on("/device", handleDeviceInformation);
    server.on("/config", handleConfig);
    server.on("/opendoor", handleOpenDoor);
    server.on("/reset", handleReset);

    server.collectHeaders(EXPECTED_HEADERS, sizeof(EXPECTED_HEADERS));
    server.begin();
}

void setup_ap_routing() {
    server.on("/setup", handleSetup);
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

    low_level_relay = prefs.getBool(PREFS_KEY_LOW_TRIGGER_RELAY);
    if (low_level_relay) {
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
