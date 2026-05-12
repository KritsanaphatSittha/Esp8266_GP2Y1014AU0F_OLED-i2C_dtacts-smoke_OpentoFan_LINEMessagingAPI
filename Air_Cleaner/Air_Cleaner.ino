#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>

const char *ssid = "Krit";
const char *password = "0916543675";
const int fanPin = D1;

// กำหนด Static IP เพื่อให้ตัว Sensor ค้นหาเจอที่เดิมเสมอ
IPAddress local_IP(192, 168, 0, 109);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 0, 1);

// --- ส่วนของ OTA Update ---
const float currentVersion =
    1.3; // เปลี่ยนเลขนี้ให้ตรงกับ Air_Cleaner_Version.txt บน GitHub

const String fwUrl = "https://raw.githubusercontent.com/KritsanaphatSittha/"
                     "Esp8266_GP2Y1014AU0F_OLED-i2C_dtacts-smoke_OpentoFan_"
                     "LINEMessagingAPI/main/Air_Cleaner.bin";
const String versionUrl =
    "https://raw.githubusercontent.com/KritsanaphatSittha/"
    "Esp8266_GP2Y1014AU0F_OLED-i2C_dtacts-smoke_OpentoFan_LINEMessagingAPI/"
    "main/Air_Cleaner_Version.txt";

unsigned long lastUpdateCheck = 0;
const unsigned long updateInterval = 3600000; // เช็คทุก 1 ชั่วโมง (3,600,000 ms)

ESP8266WebServer server(80);

void checkForUpdates() {
  WiFiClientSecure client;
  client.setInsecure(); // สำหรับ GitHub HTTPS

  HTTPClient http;
  Serial.println("Checking version from: " + versionUrl);

  bool updateNeeded = false;
  if (http.begin(client, versionUrl)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String newVersion = http.getString();
      newVersion.trim();
      if (newVersion.toFloat() > currentVersion) {
        Serial.println("New version available: " + newVersion);
        updateNeeded = true;
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

  if (updateNeeded) {
    Serial.println("Updating...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, fwUrl);
    if (ret == HTTP_UPDATE_FAILED) {
      Serial.printf("Update Failed (%d): %s\n", ESPhttpUpdate.getLastError(),
                    ESPhttpUpdate.getLastErrorString().c_str());
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);

  // ตั้งค่า Static IP ก่อนเริ่มเชื่อมต่อ WiFi
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("STA Failed to configure Static IP");
  }

  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true); // สั่งให้ ESP8266 เชื่อมต่อ WiFi อัตโนมัติเมื่อหลุด
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
