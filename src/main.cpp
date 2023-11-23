#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "uuid/uuid.h"

const char *PREFS_NAMESPACE = "icc";
const char *PREFS_KEY_SSID = "SSID";
const char *PREFS_KEY_PASSWORD = "PASS";
const char *PREFS_KEY_DEVICE_SECRET = "DSEC";
Preferences prefs;

const int LED_BLUE = 2;
const char* EMPTY_STRING = "";

String DEVICE_SECRET = "aca9d58d-a839-49ab-8977-6b5d62257998";
const char* SECTER_HEADER_NAME = "secret";
const char* EXPECTED_HEADERS[] = { "secret" };

const int PORT = 80;
WebServer server(PORT);

/// BOARD INTERACTION

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

String getOrCreateDeviceSecret() {
    prefs.begin(PREFS_NAMESPACE, false);
    String existingSecret = prefs.getString(PREFS_KEY_DEVICE_SECRET);

    if (existingSecret != EMPTY_STRING) {
        prefs.end();
        return existingSecret;
    }

    uuid_t uuid;
    char uuidStr[UUID_STR_LEN];

    uuid_generate(uuid);
    uuid_unparse(uuid, uuidStr);

    String newDeviceSecret(uuidStr);

    prefs.putString(PREFS_KEY_DEVICE_SECRET, newDeviceSecret);
    prefs.end();
    return newDeviceSecret;
}

/// HTTP STA

bool requestIsAuthorized() {
    String sentSecret = server.header(SECTER_HEADER_NAME);
    return DEVICE_SECRET == sentSecret;
}

void handleOpenDoor() {
    if (requestIsAuthorized()) {
        server.send(200);
        openDoor();
    } else {
        server.send(403);
    }
}

void handleWifiConfig() {
    if (requestIsAuthorized()) {
        prefs.begin(PREFS_NAMESPACE);
        String ssid = prefs.getString(PREFS_KEY_SSID);
        String pass = prefs.getString(PREFS_KEY_PASSWORD);
        prefs.end();

        server.send(200, "text/text", ssid + " | " + pass);
    } else {
        server.send(403);
    }
}

void handleReset() {
    if (requestIsAuthorized()) {
        prefs.begin(PREFS_NAMESPACE, false);
        prefs.clear();
        prefs.end();

        server.send(200, "text/text", "Reset. Restarting in 5 seconds...");
        delay(5000);
        ESP.restart();
    } else {
        server.send(403);
    }
}

/// HTTP AP

void handleSetup() {
    prefs.begin(PREFS_NAMESPACE);
    prefs.putString(PREFS_KEY_SSID, "E-CORB"); // TODO: get from req
    prefs.putString(PREFS_KEY_PASSWORD, "gna730gj0q368539fh638"); // TODO: get from req
    prefs.end();

    server.send(200, "text/text", "Setup complete. Restarting in 5 seconds | Device Secret: " + DEVICE_SECRET);
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

/// WIFI SETUP

bool wifiConfigured() {
    prefs.begin(PREFS_NAMESPACE, false);
    String ssid = prefs.getString(PREFS_KEY_SSID, EMPTY_STRING);
    String pass = prefs.getString(PREFS_KEY_PASSWORD, EMPTY_STRING);
    prefs.end();

    return ssid != EMPTY_STRING && pass != EMPTY_STRING;
}

void setup_wifi_sta() {
    Serial.println("Starting in STA Mode");

    prefs.begin(PREFS_NAMESPACE, false);
    String ssid = prefs.getString(PREFS_KEY_SSID, EMPTY_STRING);
    String pass = prefs.getString(PREFS_KEY_PASSWORD, EMPTY_STRING);
    prefs.end();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    if (WiFi.isConnected()) {
        Serial.print(ssid);
        Serial.print(" | ");
        Serial.println(WiFi.localIP());

        blink(2);
    }
}

void setup_wifi_ap() {
    Serial.println("Starting in AP Mode");
    const char* ap_ssid = "ESP32 Intercom Controller";

    WiFi.softAP(ap_ssid, NULL, 1, 0, 1);
    Serial.print(ap_ssid);
    Serial.print(" | ");
    Serial.println(WiFi.softAPIP());
    digitalWrite(LED_BLUE, HIGH);
}

/// GENERAL SETUP

void setupBoard() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println(EMPTY_STRING);
    pinMode(LED_BLUE, OUTPUT);
    delay(100);
}

/// LIFECYCLE METHODS

void setup() {
    setupBoard();

    Serial.println(getOrCreateDeviceSecret());

    if (wifiConfigured()) {
        setup_wifi_sta();
        setup_sta_routing();
    } else {
        setup_wifi_ap();
        setup_ap_routing();
    }
}

void loop(){
    server.handleClient();
}
