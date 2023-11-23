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
    prefs.begin("icc");
    prefs.putString("SSID", SSID);
    prefs.putString("PASS", PASSWORD);
    prefs.end();
    server.send(200);
}

void handleWifiConfig() {
    prefs.begin("icc");
    String ssid = prefs.getString("SSID");
    String pass = prefs.getString("PASS");
    prefs.end();

    server.send(200, "text/text", ssid + " | " + pass);
}

void handleReset() {
    prefs.begin(PREFS_NAMESPACE, false);
    prefs.clear();
    prefs.end();
    server.send(200);
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
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    if (WiFi.isConnected()) {
        Serial.print(SSID);
        Serial.print(" | ");
        Serial.println(WiFi.localIP());

        digitalWrite(LED_BLUE, HIGH);
        delay(1000);
        digitalWrite(LED_BLUE, LOW);
    }
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
    setup_wifi_sta();
    setup_routing();

    if (wifiConfigured()) {
        Serial.println("WiFi configured, starting STA");
    } else {
        Serial.println("No WiFi configured, starting AP");
    }
}

void loop(){
    server.handleClient();
}
