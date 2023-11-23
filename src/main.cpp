#include <WiFi.h>
#include <WebServer.h>

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

const char* RESPONSE_CONTENT_TYPE = "*/*";
void handleOpenDoor() {
    String sentSecret = server.header(SECTER_HEADER_NAME);

    if (DEVICE_SECRET == sentSecret) {
        server.send(200, RESPONSE_CONTENT_TYPE, EMPTY_STRING);
        openDoor();
    } else {
        server.send(403, RESPONSE_CONTENT_TYPE, EMPTY_STRING);
    }
}

const char* OPEN_DOOR_ROUTE = "/opendoor";
void setup_routing() {
    server.on(OPEN_DOOR_ROUTE, handleOpenDoor);

    server.collectHeaders(EXPECTED_HEADERS, sizeof(EXPECTED_HEADERS));
    server.begin();
}

void setup_wifi() {
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

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println(EMPTY_STRING);
    pinMode(LED_BLUE, OUTPUT);
    delay(100);

    setup_wifi();
    setup_routing();
}

void loop(){
    server.handleClient();
}
