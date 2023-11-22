#include <WiFi.h>
#include <WebServer.h>

const char *SSID = "E-CORB";
const char *PASSWORD = "gna730gj0q368539fh638";

const int LED_BUILTIN = 2;

String DEVICE_SECRET = "aca9d58d-a839-49ab-8977-6b5d62257998";
const char* SECTER_HEADER_NAME = "secret";
const char* EXPECTED_HEADERS[] = { "secret" };

WebServer server(80);

void openDoor() {
    Serial.println("Opening Door\n");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(3000);
    digitalWrite(LED_BUILTIN, LOW);
}

void handleOpenDoor() {
    String sentSecret = server.header(SECTER_HEADER_NAME);

    if (DEVICE_SECRET == sentSecret) {
        server.send(200, "text/text", "Opening door\n");
        openDoor();
    } else {
        server.send(403, "text/test", "You shall not pass");
    }
}

void setup_routing() {
    server.on("/opendoor", handleOpenDoor);

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
        Serial.print("Connected to ");
        Serial.print(SSID);
        Serial.print(" IP: ");
        Serial.println(WiFi.localIP());

        digitalWrite(LED_BUILTIN, HIGH);
        delay(5000);
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("");
    pinMode(LED_BUILTIN, OUTPUT);
    delay(1000);

    setup_wifi();
    setup_routing();
}

void loop(){
    server.handleClient();
}
