#include <M5Stack.h>

const byte FRAME_HEADER = 0xfe;
const byte FRAME_FOOTER = 0xfa;

const byte CMD_SET_LED = 0x6a;
const byte CMD_SET_LED_LEN = 5;

const char* TITLE = "Transponder";
const uint32_t TITLE_COLOR = TFT_BLUE;
const int32_t TITLE_X_POS = 0;
const int32_t TITLE_Y_POS = 0;
const int32_t TITLE_WIDTH = 320;
const int32_t TITLE_HEIGHT = 28;
const int8_t TITLE_STR_FONT_SIZE = 4;
const int32_t TITLE_STR_X_POS = 160;
const int32_t TITLE_STR_Y_POS = 2;
const uint32_t TITLE_STR_COLOR = TFT_WHITE;

const int32_t SEND_X_POS = 280;
const int32_t SEND_Y_POS = 14;
const int32_t SEND_RADIUS = 10;
const uint32_t SEND_ON_COLOR = TFT_GREEN;
const uint32_t SEND_OFF_COLOR = TFT_BLACK;

const int32_t RECV_X_POS = 305;
const int32_t RECV_Y_POS = SEND_Y_POS;
const int32_t RECV_RADIUS = SEND_RADIUS;
const uint32_t RECV_ON_COLOR = TFT_RED;
const uint32_t RECV_OFF_COLOR = SEND_OFF_COLOR;

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
  M5.Lcd.fillRect(TITLE_X_POS, TITLE_Y_POS, TITLE_WIDTH, TITLE_HEIGHT, TITLE_COLOR);
  M5.Lcd.setTextColor(TITLE_STR_COLOR);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString(title, TITLE_STR_X_POS, TITLE_STR_Y_POS, TITLE_STR_FONT_SIZE);
}

void setSend(const bool isOn) {
  M5.Lcd.fillCircle(SEND_X_POS, SEND_Y_POS, SEND_RADIUS,
                    isOn ? SEND_ON_COLOR : SEND_OFF_COLOR);
}

void setRecv(const bool isOn) {
  M5.Lcd.fillCircle(RECV_X_POS, RECV_Y_POS, RECV_RADIUS,
                    isOn ? RECV_ON_COLOR : RECV_OFF_COLOR);
}
