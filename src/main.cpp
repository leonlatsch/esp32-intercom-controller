#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

const char *PREFS_NAMESPACE = "icc";
const char *PREFS_KEY_SSID = "SSID";
const char *PREFS_KEY_PASSWORD = "PASS";
Preferences prefs;

const char *SSID = "E-CORB";
const char *PASSWORD = "gna730gj0q368539fh638";

const int LED_BLUE = 2;
const char* EMPTY_STRING = "";

String DEVICE_SECRET = "aca9d58d-a839-49ab-8977-6b5d62257998";
const char* SECTER_HEADER_NAME = "secret";
const char* EXPECTED_HEADERS[] = { "secret" };

const int PORT = 80;
WebServer server(PORT);

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

void handleOpenDoor() {
    String sentSecret = server.header(SECTER_HEADER_NAME);

    if (DEVICE_SECRET == sentSecret) {
        server.send(200);
        openDoor();
    } else {
        server.send(403);
    }
}

void handleSetup() {
    prefs.begin(PREFS_NAMESPACE);
    prefs.putString(PREFS_KEY_SSID, SSID);
    prefs.putString(PREFS_KEY_PASSWORD, PASSWORD);
    prefs.end();

    server.send(200, "text/text", "Setup complete. Restarting in 5 seconds");
    delay(5000);
    ESP.restart();
}

void handleWifiConfig() {
    prefs.begin(PREFS_NAMESPACE);
    String ssid = prefs.getString(PREFS_KEY_SSID);
    String pass = prefs.getString(PREFS_KEY_PASSWORD);
    prefs.end();

    server.send(200, "text/text", ssid + " | " + pass);
}

void handleReset() {
    prefs.begin(PREFS_NAMESPACE, false);
    prefs.clear();
    prefs.end();

    server.send(200, "text/text", "Reset. Restarting in 5 seconds...");
    delay(5000);
    ESP.restart();
}

void setup_routing() {
    server.on("/setup", handleSetup);
    server.on("/wificonfig", handleWifiConfig);
    server.on("/opendoor", handleOpenDoor);
    server.on("/reset", handleReset);

    server.collectHeaders(EXPECTED_HEADERS, sizeof(EXPECTED_HEADERS));
    server.begin();
}

bool wifiConfigured() {
    prefs.begin(PREFS_NAMESPACE, false);
    String ssid = prefs.getString(PREFS_KEY_SSID, EMPTY_STRING);
    String pass = prefs.getString(PREFS_KEY_PASSWORD, EMPTY_STRING);
    prefs.end();

    return ssid != EMPTY_STRING && pass != EMPTY_STRING;
}

void setup_wifi_sta() {
    Serial.println("Starting in STA Mode");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    if (WiFi.isConnected()) {
        Serial.print(SSID);
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
    blink(4);
}

void setupBoard() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println(EMPTY_STRING);
    pinMode(LED_BLUE, OUTPUT);
    delay(100);
}

void setup() {
    setupBoard();
    if (wifiConfigured()) {
        setup_wifi_sta();
    } else {
        setup_wifi_ap();
    }
    setup_routing();
}

void loop(){
    server.handleClient();
}
