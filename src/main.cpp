#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "E-CORB";
const char *password = "gna730gj0q368539fh638";

const int LED_BUILTIN = 2;

WebServer server(80);

void openDoor() {
  server.send(200, "text/text", "Opening door\n");
  Serial.println("Opening Door\n");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup_routing() {
  server.on("/opendoor", openDoor);

  server.begin();
}

void setup_wifi() {
  Serial.begin(9600);
  while (!Serial);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  if (WiFi.isConnected()) {
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);

  setup_wifi();
  setup_routing();
}

void loop(){
  server.handleClient();
}
