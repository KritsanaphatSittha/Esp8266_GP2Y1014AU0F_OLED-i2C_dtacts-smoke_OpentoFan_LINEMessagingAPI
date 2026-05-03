#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Krit";
const char *password = "0916543675";
const int fanPin = D6;

ESP8266WebServer server(80);

void setup()
{
    Serial.begin(115200);

    pinMode(fanPin, OUTPUT);
    digitalWrite(fanPin, LOW);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/trigger", HTTP_POST, []()
              {
    String payload = server.arg("plain");
    if (payload == "ON") {
      digitalWrite(fanPin, HIGH);
      Serial.println("Fan turned on");
      server.send(200, "text/plain", "Fan turned on");
    } else if (payload == "OFF") {
      digitalWrite(fanPin, LOW);
      Serial.println("Fan turned off");
      server.send(200, "text/plain", "Fan turned off");
    } else {
      server.send(400, "text/plain", "Invalid command");
    } });

    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();
}
