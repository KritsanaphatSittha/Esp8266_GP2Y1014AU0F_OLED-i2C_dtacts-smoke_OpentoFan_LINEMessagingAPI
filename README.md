# Esp8266_GP2Y1014AU0F_OLED-i2C_dtacts-smoke_OpentoFan_LINEMessagingAPI

##โปรเจ็คของโรงเรียนเทศบาลบ้านย่านยาว (ครูจอยเคมี) เมื่อตรวจพบฝุ่น PM 2.5 สั่งให้เครื่องกรองฝุ่นเปิด และทำการแจ้งเตือนใน Line

##อุปกรณ์
1.Nodemcu V3 Lua WIFI Module ESP8266 /n
2.Nodemcubase Ver1.0 /n
3.OLED i2C 0.91 
4.GP2Y1014AU0F เซ็นเซอร์วัดฝุ่น PM2.5 วัดควัน
5.HLK-PM01 โมดูลแปลงไฟ
6.Relay 5V 1-2 ช่อง
7.สาย USB ต่อ ESP8266 เข้าคอม 
8.Adapter 9V1A สำหรับ Arduino
9.Fan
10.สายจัมป์ เมีย-เมีย ผู้-ผู้ ผู้-เมีย
11.บอร์ดทดลอง Breadboard
12.แผ่นกรองฝุ่น

##โปรแกรมและคู่มือการติดตั้ง
1.https://www.arduino.cc/en/software/   (ทำการติดตั้งโปรแกรมลงเครื่อง)
2.ทำการ Install Library Manager ลงในโปรแกรม Arduino IDE 
    - ArduinoJson
    - ArduinoHttpClient
    - PubSubClient

    ## Library Manager ของ Sensor
    - Adafruit Unified Sensor
    - DHT sensor library
    - LiquidCrystal_I2C

    #หมายเหตุ อย่าลืมต่อสายบอร์ดเข้ากับคอม แล้วเลือกพอร์ตให้ถูกต้อง จากนั้นเลือกในส่วนของ BOARDS ให้พิมพ์ค้นหาเป็น "NodeMCU 1.0 (ESP-12E Module)"

##Link PlayLists YouTube (สอนการต่อสายไฟและการทำแจ้งเตือน Line ฯลฯ)
https://youtube.com/playlist?list=PLDEk-pFe7o2oBiwkpyxHmaJMOvWCJq-qa&si=C_K_D3OSdKXf1M0z

##Link ตัวอย่างการต่อสายไฟ 
https://www.tinkercad.com/things/kGfVL0NhL9O/editel?returnTo=%2Fdashboard&sharecode=AYGHNVcWbfXhSwfA0dNsWSto6_8WgoMVQUDHucn6s80

##Link Line LINE Messaging API (ตอนนี้ Line ที่ใช้ทำโปรเจ็ค เชื่อมอยู่กับ Mail gard_234@hotmail.com)
https://developers.line.biz/console/channel/2007364452/basics