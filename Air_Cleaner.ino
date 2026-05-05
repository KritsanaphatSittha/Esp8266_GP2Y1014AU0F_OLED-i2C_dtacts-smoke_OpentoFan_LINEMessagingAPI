#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>

const char *ssid = "Krit";
const char *password = "0916543675";
const int fanPin = D6;

// --- ส่วนของ OTA Update ---
const float currentVersion = 1.0; // เปลี่ยนเลขนี้ทุกครั้งที่อัปเดตโค้ดใหม่

// *** สำคัญ: เปลี่ยน USER_NAME และ REPO_NAME เป็นของคุณ ***
const String fwUrl = "https://raw.githubusercontent.com/USER_NAME/REPO_NAME/"
                     "main/Air_Cleaner.bin";
const String versionUrl = "https://raw.githubusercontent.com/USER_NAME/"
                          "REPO_NAME/main/Air_Cleaner_Version.txt";

unsigned long lastUpdateCheck = 0;
const unsigned long updateInterval = 3600000; // เช็คทุก 1 ชั่วโมง (3,600,000 ms)

ESP8266WebServer server(80);

void checkForUpdates() {
  WiFiClientSecure client;
  client.setInsecure(); // สำหรับ GitHub HTTPS

  HTTPClient http;
  Serial.println("Checking version from: " + versionUrl);

  if (http.begin(client, versionUrl)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String newVersion = http.getString();
      newVersion.trim();
      if (newVersion.toFloat() > currentVersion) {
        Serial.printf("New version available: %s. Updating...\n",
                      newVersion.c_str());
        t_httpUpdate_return ret = ESPhttpUpdate.update(client, fwUrl);

        if (ret == HTTP_UPDATE_FAILED) {
          Serial.printf("Update Failed (%d): %s\n",
                        ESPhttpUpdate.getLastError(),
                        ESPhttpUpdate.getLastErrorString().c_str());
        }
      } else {
        Serial.println("Current version is up to date.");
      }
    } else {
      Serial.printf("Failed to check version, HTTP error: %s\n",
                    http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Unable to connect to GitHub");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // ครั้งที่ 1: เช็คอัปเดตทันทีที่เปิดเครื่อง
  checkForUpdates();
  lastUpdateCheck = millis(); // เริ่มนับเวลาถอยหลัง 1 ชม. หลังจากเช็คครั้งแรกเสร็จ

  server.on("/trigger", HTTP_POST, []() {
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
    }
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // ครั้งต่อๆ ไป: เช็คอัปเดตทุกๆ 1 ชั่วโมง
  if (millis() - lastUpdateCheck > updateInterval) {
    lastUpdateCheck = millis();
    checkForUpdates();
  }
}
