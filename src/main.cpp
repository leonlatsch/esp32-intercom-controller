#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "uuid/uuid.h"
#include "JsonHelper.h"
#include "board_interaction.h"
#include "WiFiManager.h"

const char *PREFS_NAMESPACE = "icc";
const char *PREFS_KEY_DEVICE_SECRET = "DSEC";
Preferences prefs;

const char* SECTER_HEADER_NAME = "secret";
const char* EXPECTED_HEADERS[] = { "secret" };

const int PORT = 80;
WebServer server(PORT);

WiFiManager wifiManager(prefs);

String DEVICE_SECRET;

String getOrCreateDeviceSecret() {
    String existingSecret = prefs.getString(PREFS_KEY_DEVICE_SECRET);

    if (!existingSecret.isEmpty()) {
        return existingSecret;
    }

    uuid_t uuid;
    char uuidStr[UUID_STR_LEN];

    uuid_generate(uuid);
    uuid_unparse(uuid, uuidStr);

    String newDeviceSecret(uuidStr);

    prefs.putString(PREFS_KEY_DEVICE_SECRET, newDeviceSecret);
    return newDeviceSecret;
}

void reboot() {
    prefs.end();
    server.close();
    ESP.restart();
}

/// HTTP STA

void securedEndpoint(std::function<void(void)> handler) {
    String sentSecret = server.header(SECTER_HEADER_NAME);
    if (DEVICE_SECRET == sentSecret) {
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
    reboot();
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

    server.send(200, "text/text", "Setup complete. Restarting in 5 seconds | Device Secret: " + DEVICE_SECRET);
    delay(5000);
    digitalWrite(LED_BLUE, LOW);
    reboot();
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

    // Device Secret
    DEVICE_SECRET = getOrCreateDeviceSecret();
    Serial.println(DEVICE_SECRET);

    delay(100);
}

void setupPersistence() {
    prefs.begin(PREFS_NAMESPACE, false);
}

/// LIFECYCLE METHODS

void setup() {
    setupBoard();
    setupPersistence();

    if (wifiManager.wifi_creds_configures()) {
        wifiManager.setup_wifi_sta();
        setup_sta_routing();
    } else {
        wifiManager.setup_wifi_ap();
        setup_ap_routing();
    }
}

void loop(){
    server.handleClient();
}
