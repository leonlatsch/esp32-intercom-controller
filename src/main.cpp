#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "util/uuid/uuid.h"
#include "util/JsonHelper.h"
#include "board_interaction.h"
#include "connection/WiFiManager.h"
#include "PrefsWrapper.h"
#include "security/DeviceSecretStore.h"

const char* SECTER_HEADER_NAME = "secret";
const char* EXPECTED_HEADERS[] = { "secret" };

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

void handleOpenDoor() {
    securedEndpoint([]() {
        server.send(200);
        openDoor();
    });
}

void handleWifiConfig() {
    securedEndpoint([]() {
        String ssid = prefs.getString(PREFS_KEY_SSID);
        String pass = prefs.getString(PREFS_KEY_PASSWORD);

        server.send(200, "text/text", ssid + " | " + pass);
    });
}

void handleReset() {
    prefs.clear();

    server.send(200, "text/text", "Reset. Restarting in 5 seconds...");
    delay(5000);
    ESP.restart();
}

/// HTTP AP

void handleSetup() {
    String rawBody = server.arg(0);

    String ssid;
    String password;
    readCredentialsFromRequest(rawBody, ssid, password);

    if (ssid == NULL || password == NULL) {
        server.send(400);
        return;
    }

    prefs.putString(PREFS_KEY_SSID, ssid);
    prefs.putString(PREFS_KEY_PASSWORD, password);
    String deviceSecret = deviceSecretStore.getDeviceSecret();

    server.send(200, "text/text", "Setup complete. Restarting in 5 seconds | Device Secret: " + deviceSecret);
    delay(5000);
    digitalWrite(LED_BLUE, LOW);
    ESP.restart();
}

/// ROUTING SETUP

void setup_sta_routing() {
    server.on("/wificonfig", handleWifiConfig);
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

void loop(){
    server.handleClient();
}
