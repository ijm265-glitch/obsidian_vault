#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// ESP32-S3 I2C 핀 지정
#define I2C_SDA 1
#define I2C_SCL 2

// 맨 앞 U8G2를 대문자로 수정했습니다.
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int count = 0;

void setup(void) {
  Serial.begin(115200);

  // ESP32-S3 I2C 핀 초기화
  Wire.begin(I2C_SDA, I2C_SCL);

  // OLED 디스플레이 초기화
  u8g2.begin();
  
  Serial.println("U8g2 OLED 초기화 완료!");
}

void loop(void) {
  u8g2.clearBuffer();          

  u8g2.setFont(u8g2_font_ncenB08_tr); 
  u8g2.drawStr(0, 10, "ESP32-S3 + U8g2");

  u8g2.drawHLine(0, 14, 128);3.3
  3.0

  u8g2.setFont(u8g2_font_profont15_tf);
  u8g2.setCursor(0, 29);
  u8g2.print("COUNT: ");
  u8g2.print(count++);

  u8g2.sendBuffer();           

  delay(500);
}