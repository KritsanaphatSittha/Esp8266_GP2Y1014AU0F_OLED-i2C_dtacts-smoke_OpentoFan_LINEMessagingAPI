#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <TridentTD_LineNotify.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int measurePin = A0;
int ledPower = D5;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

const char *ssid = "Krit";
const char *password = "0916543675";
const char *serverIp = "192.168.0.109";
const int serverPort = 80;
const char *lineToken = "zligGTMmvvVbqT5vKtjcBibrQNXvWDQJlfJR5KCgDx2";

WiFiClient client;

unsigned long previousMillis = 0;
const long interval = 10000; // 10 seconds

void setup()
{
    Serial.begin(9600);
    pinMode(ledPower, OUTPUT);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to WiFi");

    // Set Line Token
    LINE.setToken(lineToken);
}

void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        display.clearDisplay();
        digitalWrite(ledPower, LOW);
        delayMicroseconds(samplingTime);

        voMeasured = analogRead(measurePin);

        delayMicroseconds(deltaTime);
        digitalWrite(ledPower, HIGH);
        delayMicroseconds(sleepTime);

        calcVoltage = voMeasured * (3.3 / 1024);
        dustDensity = 0.17 * calcVoltage - 0.1;

        Serial.print("Raw Signal Value (0-1023): ");
        Serial.print(voMeasured);

        Serial.print(" - Voltage: ");
        Serial.print(calcVoltage);

        if (dustDensity <= 0.00)
        {
            dustDensity = 0.00;
        }

        dustDensity = dustDensity * 1000;

        Serial.print(" - Dust Density: ");
        Serial.print(dustDensity);
        Serial.println(" µg/m³");

        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(F("PM 2.5"));

        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println("");

        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.print(dustDensity);
        display.println(" ug");

        display.display();

        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;
            String url = String("http://") + serverIp + ":" + serverPort + "/trigger";
            http.begin(client, url);
            String payload = (dustDensity > 37.5) ? "ON" : "OFF";
            int httpCode = http.POST(payload);

            if (httpCode > 0)
            {
                String response = http.getString();
                Serial.println("HTTP Response code: " + String(httpCode));
                Serial.println("Response: " + response);

                // Send Line notification if dust density exceeds 35 µg/m³
                if (dustDensity > 37.5)
                {
                    String message = "Dust level is high: " + String(dustDensity) + " µg/m³";
                    LINE.notify(message);
                }
            }
            else
            {
                Serial.println("Error on HTTP request");
            }

            http.end();
        }

        delay(2000); // Ensure a small delay between cycles
    }
}
