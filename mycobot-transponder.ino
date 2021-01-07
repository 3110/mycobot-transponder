#include <M5Stack.h>

const byte FRAME_HEADER = 0xfe;
const byte FRAME_FOOTER = 0xfa;

const byte CMD_SET_LED = 0x6a;
const byte CMD_SET_LED_LEN = 5;

const char* TITLE = "Transponder";

const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

void setup() {
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL2_BAUD_RATE);

  setLED(255, 255, 255);
  setTitle(TITLE);
}

void loop() {
  setSend(false);
  while (Serial.available() > 0) {
    setSend(true);
    Serial2.write(Serial.read());
  }

  setRecv(false);
  while (Serial2.available() > 0) {
    setRecv(true);
    Serial.write(Serial2.read());
  }
}

void setLED(const byte r, const byte g, const byte b) {
  Serial2.write(FRAME_HEADER);
  Serial2.write(FRAME_HEADER);
  Serial2.write(CMD_SET_LED_LEN);
  Serial2.write(CMD_SET_LED);
  Serial2.write(r);
  Serial2.write(g);
  Serial2.write(b);
  Serial2.write(FRAME_FOOTER);
  Serial2.flush();
}

void setTitle(const char* title) {
  M5.Lcd.fillRect(0, 0, M5.Lcd.width(), 28, TFT_BLUE);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString(title, (int)(M5.Lcd.width() / 2), 2, 4);
}

void setSend(const bool isOn) {
  M5.Lcd.fillCircle(280, 14, 10, isOn ? TFT_GREEN : TFT_BLACK);
}

void setRecv(const bool isOn) {
  M5.Lcd.fillCircle(305, 14, 10, isOn ? TFT_RED : TFT_BLACK);
}
