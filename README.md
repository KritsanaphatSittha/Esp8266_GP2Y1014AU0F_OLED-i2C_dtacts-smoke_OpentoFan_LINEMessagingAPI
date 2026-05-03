# 🌬️ ESP8266 Dust Monitoring & Air Purifier System
### โปรเจ็คเครื่องตรวจวัดฝุ่น PM 2.5 และแจ้งเตือนผ่าน LINE 
**โรงเรียนเทศบาลบ้านย่านยาว (ครูจอยเคมี)**

เมื่อระบบตรวจพบค่าฝุ่น PM 2.5 เกินมาตรฐาน ระบบจะสั่งให้เครื่องกรองฝุ่นทำงานโดยอัตโนมัติ และส่งข้อความแจ้งเตือนไปยัง Line Application ทันที

---

## 🛠 อุปกรณ์ที่ใช้ (Hardware)

| ลำดับ | รายการอุปกรณ์ | รายละเอียด |
| :--- | :--- | :--- |
| 1 | **NodeMCU V3 (ESP8266)** | โมดูล WiFi หลักในการควบคุม |
| 2 | **NodeMCU Base Ver 1.0** | บอร์ดขยายขาสำหรับการเชื่อมต่อ |
| 3 | **OLED Display (I2C 0.91")** | จอแสดงผลค่าฝุ่น |
| 4 | **GP2Y1014AU0F** | เซ็นเซอร์วัดฝุ่น PM2.5 และควัน |
| 5 | **HLK-PM01** | โมดูลแปลงไฟ AC เป็น DC |
| 6 | **Relay 5V (1-2 Channel)** | สวิตช์อิเล็กทรอนิกส์ควบคุมพัดลม |
| 7 | **Fan** | พัดลมสำหรับกรองฝุ่น |
| 8 | **Adapter 9V 1A** | แหล่งจ่ายไฟสำหรับบอร์ด |
| 9 | **Breadboard & Jumpers** | บอร์ดทดลองและสายเชื่อมต่อ (M-M, F-F, M-F) |
| 10 | **Filter** | แผ่นกรองฝุ่น |

---

## 💻 ซอฟต์แวร์และการติดตั้ง (Software)

### 1. โปรแกรมหลัก
*   ติดตั้ง **Arduino IDE**: [ดาวน์โหลดที่นี่](https://www.arduino.cc/en/software/)

### 2. การตั้งค่า Board
*   ไปที่เมนู Boards Manager แล้วค้นหา: `NodeMCU 1.0 (ESP-12E Module)`
*   เลือกพอร์ต (Port) ให้ตรงกับที่เชื่อมต่อกับคอมพิวเตอร์

### 3. ไลบรารีที่จำเป็น (Library Manager)
กรุณาติดตั้งไลบรารีต่อไปนี้ผ่าน Library Manager ใน Arduino IDE:

**System Libraries:**
- `ArduinoJson`
- `ArduinoHttpClient`
- `PubSubClient`

**Sensor & Display Libraries:**
- `Adafruit Unified Sensor`
- `DHT sensor library`
- `LiquidCrystal_I2C`

---

## 📺 แหล่งข้อมูลเรียนรู้เพิ่มเติม

*   **🎬 วิดีโอสอนการใช้งาน:** YouTube Playlist
    *   *(สอนการต่อสายไฟ, การตั้งค่าแจ้งเตือน Line และอื่นๆ)*
*   **🔌 ผังการต่อวงจร:** Tinkercad Circuit
*   **💬 Line Messaging API:** Line Developers Console
    *   *(Account เชื่อมต่อกับ: gard_234@hotmail.com)*

---

> **หมายเหตุ:** โปรดตรวจสอบการต่อสายไฟและขั้วไฟฟ้าให้ถูกต้องก่อนการจ่ายไฟทุกครั้ง เพื่อป้องกันความเสียหายต่ออุปกรณ์