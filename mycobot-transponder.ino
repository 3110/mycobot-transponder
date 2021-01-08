#include <M5Stack.h>

const char *VERSION = "v0.0.1";
const int VERSION_X_POS = 5;
const int VERSION_Y_POS = 25;
const uint8_t VERSION_DATUM = BL_DATUM;
const int VERSION_FONT_SIZE = 2;
const uint32_t VERSION_COLOR = TFT_WHITE;

const int32_t TITLE_BG_X_POS = 0;
const int32_t TITLE_BG_Y_POS = 0;
const int32_t TITLE_BG_WIDTH = 320;
const int32_t TITLE_BG_HEIGHT = 28;
const uint32_t TITLE_BG_COLOR = TFT_BLUE;

const char *TITLE = "Transponder";
const int TITLE_X_POS = TITLE_BG_WIDTH / 2;
const int TITLE_Y_POS = TITLE_BG_HEIGHT / 2 + 1;
const int TITLE_FONT_SIZE = 4;
const uint8_t TITLE_DATUM = MC_DATUM;
const uint32_t TITLE_COLOR = TFT_WHITE;

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

const byte ATOM_LED_R = 0xff;
const byte ATOM_LED_G = 0xff;
const byte ATOM_LED_B = 0xff;

const byte FRAME_HEADER = 0xfe;
const byte FRAME_FOOTER = 0xfa;

const byte CMD_SET_LED = 0x6a;
const byte CMD_SET_LED_LEN = 5;

const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

void setup()
{
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL2_BAUD_RATE);

  setLED(ATOM_LED_R, ATOM_LED_G, ATOM_LED_B);
  setTitle(TITLE);
  setVersion(VERSION);
}

void loop()
{
  setSend(false);
  while (Serial.available() > 0)
  {
    setSend(true);
    Serial2.write(Serial.read());
  }

  setRecv(false);
  while (Serial2.available() > 0)
  {
    setRecv(true);
    Serial.write(Serial2.read());
  }
}

void setLED(const byte r, const byte g, const byte b)
{
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

void setText(const char *text, const int x, const int y,
             const int font_size, const uint16_t text_color,
             const uint8_t text_datum)
{
  M5.Lcd.setTextColor(text_color);
  M5.Lcd.setTextDatum(text_datum);
  M5.Lcd.drawString(text, x, y, font_size);
}

void setTitle(const char *title)
{
  M5.Lcd.fillRect(TITLE_BG_X_POS, TITLE_BG_Y_POS, TITLE_BG_WIDTH,
                  TITLE_BG_HEIGHT, TITLE_BG_COLOR);
  setText(title, TITLE_X_POS, TITLE_Y_POS,
          TITLE_FONT_SIZE, TITLE_COLOR, TITLE_DATUM);
}

void setVersion(const char *version)
{
  setText(version, VERSION_X_POS, VERSION_Y_POS,
          VERSION_FONT_SIZE, VERSION_COLOR, VERSION_DATUM);
}

void setSend(const bool isOn)
{
  M5.Lcd.fillCircle(SEND_X_POS, SEND_Y_POS, SEND_RADIUS,
                    isOn ? SEND_ON_COLOR : SEND_OFF_COLOR);
}

void setRecv(const bool isOn)
{
  M5.Lcd.fillCircle(RECV_X_POS, RECV_Y_POS, RECV_RADIUS,
                    isOn ? RECV_ON_COLOR : RECV_OFF_COLOR);
}
