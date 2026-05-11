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

### 2. การตั้งค่า Arduino IDE สำหรับ ESP8266
เพื่อให้ Arduino IDE รู้จักบอร์ด ESP8266 และทำงานได้ถูกต้อง ให้ทำตามขั้นตอนดังนี้:

1.  **เพิ่ม URL ของบอร์ด ESP8266:**
    *   ไปที่เมนู **Arduino IDE** -> **Settings...** (หรือ **Preferences**)
    *   ในช่อง **Additional boards manager URLs** ให้วาง URL นี้:
        `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
    *   กด **OK**
2.  **ติดตั้งบอร์ด ESP8266:**
    *   ไปที่เมนู **Tools** -> **Board** -> **Boards Manager...**
    *   พิมพ์ค้นหา `esp8266` และกด **Install** ตัวเลือกโดย *ESP8266 Community*
3.  **เลือกบอร์ดและพอร์ต:**
    *   **Board:** เลือก `NodeMCU 1.0 (ESP-12E Module)`
    *   **Port:** เลือกพอร์ตที่เชื่อมต่อกับบอร์ด (หากไม่พบ ให้ติดตั้ง Driver CH340 หรือ CP2102)
    *   **Upload Speed:** แนะนำที่ `115200`

⚠️ **ข้อควรระวังเรื่อง Baud Rate:**
*   ในโปรแกรม **Sensor**: ตั้งค่าไว้ที่ `9600`
*   ในโปรแกรม **Air_Cleaner**: ตั้งค่าไว้ที่ `115200`
*   *กรุณาปรับ Serial Monitor ให้ตรงกับค่าข้างต้นเพื่อดูข้อมูล*

### 3. การติดตั้งไลบรารี (Library Manager)
ไปที่เมนู **Tools** -> **Manage Libraries...** ค้นหาและติดตั้งไลบรารีดังนี้:

**System Libraries:**
- `ArduinoJson`
- `ArduinoHttpClient`
- `PubSubClient`
- `Adafruit SSD1306`
- `Adafruit GFX Library`
- `Adafruit BusIO`

**Sensor & Display Libraries:**
- `Adafruit Unified Sensor`
- `DHT sensor library`
- `LiquidCrystal_I2C`
*หมายเหตุ: เมื่อติดตั้ง Adafruit SSD1306 หากระบบถามหา dependencies ให้เลือก "Install All"*

---

## 📺 แหล่งข้อมูลเรียนรู้เพิ่มเติม

*   **🎬 วิดีโอสอนการใช้งาน:** [YouTube Playlist](https://youtube.com/playlist?list=PLDEk-pFe7o2oBiwkpyxHmaJMOvWCJq-qa&si=C_K_D3OSdKXf1M0z)
    *   *(สอนการต่อสายไฟ, การตั้งค่าแจ้งเตือน Line และอื่นๆ)*
*   **🔌 ผังการต่อวงจร:** [Tinkercad Circuit](https://www.tinkercad.com/things/kGfVL0NhL9O/editel?returnTo=%2Fdashboard&sharecode=AYGHNVcWbfXhSwfA0dNsWSto6_8WgoMVQUDHucn6s80)
*   **💬 Line Messaging API:** [Line Developers Console](https://developers.line.biz/console/channel/2007364452/basics)
    *   *(Account: gard_234@hotmail.com)*

---

> **หมายเหตุ:** โปรดตรวจสอบการต่อสายไฟและขั้วไฟฟ้าให้ถูกต้องก่อนการจ่ายไฟทุกครั้ง เพื่อป้องกันความเสียหายต่ออุปกรณ์