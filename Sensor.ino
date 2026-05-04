#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h> // เพิ่มไลบรารีสำหรับเชื่อมต่อ HTTPS ของ LINE Messaging API
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

// ================= ตั้งค่า LINE Messaging API =================
const String lineToken = "NokZLVFOvnfN5njB/LUfnJtWP/GyNmQ/Y/H8Z0qj6cXNA3elz39e1UNhT+mzjcFAw14SJ07YJ3tDyaOXCu+fJB6IesCQjiHkwq0grACJlCrjYVmo1I0f/HrhE5HJUFUQQ9puQtiLy6xPUnvVIv8F5gdB04t89/1O/w1cDnyilFU=";
const String userId = "Ude2477bb62de0d8e7e3c39b0b080274b";

WiFiClient client;

unsigned long previousMillis = 0;
const long interval = 10000; // 10 seconds

// เพิ่มตัวแปรสำหรับจำสถานะฝุ่น ป้องกันการส่งข้อความซ้ำรัวๆ
bool isDustHigh = false;

// ================= ฟังก์ชันส่งข้อความเข้า LINE =================
void sendLineMessage(String message)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClientSecure secureClient;
        secureClient.setInsecure(); // ข้ามการตรวจสอบ SSL

        HTTPClient httpLine;
        httpLine.begin(secureClient, "https://api.line.me/v2/bot/message/push");
        httpLine.addHeader("Content-Type", "application/json");
        httpLine.addHeader("Authorization", "Bearer " + lineToken);

        String payload = "{\"to\": \"" + userId + "\", \"messages\": [{\"type\": \"text\", \"text\": \"" + message + "\"}]}";

        int httpCode = httpLine.POST(payload);

        if (httpCode > 0)
        {
            Serial.println("LINE Push HTTP Response code: " + String(httpCode));
        }
        else
        {
            Serial.println("Error sending LINE Push: " + String(httpCode));
        }
        httpLine.end();
    }
}

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
            // ---------------- ส่วนควบคุมพัดลม (Local Server) ----------------
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

                // ---------------- ส่วนแจ้งเตือน LINE ----------------
                if (dustDensity > 37.5)
                {
                    // แจ้งเตือนเมื่อฝุ่นสูง (ส่งแค่ครั้งแรกที่เกินค่า)
                    if (!isDustHigh)
                    {
                        String message = "ตอนนี้ฝุ่น PM 2.5 ได้มากกว่า 37.5 µg/m³ แล้ว อยู่ที่  " + String(dustDensity) + " µg/m³" + "ระบบจะทำการเปิดเครื่องกรองฝุ่น 🟢";
                        sendLineMessage(message);
                        isDustHigh = true; // อัปเดตสถานะว่าตอนนี้ฝุ่นสูงแล้ว
                    }
                }
                else
                {
                    // แจ้งเตือนเมื่อฝุ่นต่ำลง (ส่งแค่ครั้งแรกที่กลับมาต่ำกว่าเกณฑ์)
                    if (isDustHigh)
                    {
                        String message = "ตอนนี้ฝุ่น PM 2.5 ได้ต่ำกว่า 37.5 µg/m³ แล้ว ระบบจะทำการปิดเครื่องกรองฝุ่น 🛑";
                        sendLineMessage(message);
                        isDustHigh = false; // รีเซ็ตสถานะกลับเป็นปกติ
                    }
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