#include <M5Stack.h>
#include <Preferences.h>
#include "MyCobot.h"

const char *PREFS_NAMESPACE = "transponder";
const char *PREFS_IS_DUMPED = "is_dumped";

const char *VERSION = "v0.0.1";
const uint8_t VERSION_DATUM = TL_DATUM;
const int32_t VERSION_X_POS = 5;
const int32_t VERSION_Y_POS = 10;
const uint8_t VERSION_FONT_SIZE = 2;
const uint32_t VERSION_COLOR = TFT_WHITE;

const int32_t TITLE_BG_WIDTH = 320;
const int32_t TITLE_BG_HEIGHT = 28;
const uint32_t TITLE_BG_COLOR = TFT_BLUE;

const char *TITLE = "Transponder";
const uint8_t TITLE_DATUM = MC_DATUM;
const int32_t TITLE_X_POS = TITLE_BG_WIDTH / 2;
const int32_t TITLE_Y_POS = TITLE_BG_HEIGHT / 2 + 1;
const uint8_t TITLE_FONT_SIZE = 4;
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

const int32_t COMMAND_X_POS = 0;
const int32_t COMMAND_Y_POS = 50;
const int16_t COMMAND_WIDTH = 320;
const int16_t COMMAND_HEIGHT = 26 * 2;
const int32_t COMMAND_NAME_X_POS = 16;
const int32_t COMMAND_NAME_Y_POS = 26;
const uint8_t COMMAND_FONT_SIZE = 4;
const uint8_t COMMAND_DATUM TL_DATUM;
const uint32_t COMMAND_COLOR = TFT_WHITE;
const uint32_t COMMAND_BG_COLOR = TFT_BLACK;
const char *COMMAND_LABEL = "Send:";
const int32_t COMMAND_LABEL_X_POS = 0;
const int32_t COMMAND_LABEL_Y_POS = 0;

const char *DUMP_BUTTON_ON = "ON";
const char *DUMP_BUTTON_OFF = "OFF";
const uint8_t DUMP_BUTTON_FONT_SIZE = 4;
const uint8_t DUMP_BUTTON_DATUM = MC_DATUM;
const int32_t DUMP_BUTTON_X_POS = 40;
const int32_t DUMP_BUTTON_Y_POS = 210;
const int16_t DUMP_BUTTON_WIDTH = 48;
const int16_t DUMP_BUTTON_HEIGHT = 20;
const uint32_t DUMP_BUTTON_COLOR = TFT_WHITE;
const uint32_t DUMP_BUTTON_BG_COLOR = TFT_BLACK;

const byte ATOM_LED_R = 0xff;
const byte ATOM_LED_G = 0xff;
const byte ATOM_LED_B = 0xff;

const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);
MyCobot::FrameState frame_state;
Preferences prefs;
bool is_dumped;

void setup()
{
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL2_BAUD_RATE);

  setLED(ATOM_LED_R, ATOM_LED_G, ATOM_LED_B);
  setTitle(TITLE, VERSION);

  frame_state = MyCobot::STATE_NONE;
  is_dumped = getDumped();
  setDumpButton(DUMP_BUTTON_X_POS, DUMP_BUTTON_Y_POS, is_dumped);
}

void loop()
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    is_dumped = toggleDumped(is_dumped);
    setDumpButton(DUMP_BUTTON_X_POS, DUMP_BUTTON_Y_POS, is_dumped);
  }

  setSend(false);
  while (Serial.available() > 0)
  {
    const int b = Serial.read();
    setSend(true);
    Serial2.write(b);
    frame_state = MyCobot::checkFrameState(frame_state, b);
    if (is_dumped && frame_state == MyCobot::STATE_CMD)
    {
      setCommandName(b);
    }
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
  Serial2.write(MyCobot::FRAME_HEADER);
  Serial2.write(MyCobot::FRAME_HEADER);
  Serial2.write(MyCobot::CMD_SET_LED_LEN);
  Serial2.write(MyCobot::SET_LED);
  Serial2.write(r);
  Serial2.write(g);
  Serial2.write(b);
  Serial2.write(MyCobot::FRAME_FOOTER);
  Serial2.flush();
}

void setTitle(const char *title, const char *version)
{
  sprite.setColorDepth(8);
  sprite.createSprite(TITLE_BG_WIDTH, TITLE_BG_HEIGHT);
  sprite.fillSprite(TITLE_BG_COLOR);
  sprite.setTextColor(VERSION_COLOR);
  sprite.setTextDatum(VERSION_DATUM);
  sprite.drawString(version, VERSION_X_POS, VERSION_Y_POS, VERSION_FONT_SIZE);
  sprite.setTextColor(TITLE_COLOR);
  sprite.setTextDatum(TITLE_DATUM);
  sprite.drawString(title, TITLE_X_POS, TITLE_Y_POS, TITLE_FONT_SIZE);
  sprite.pushSprite(0, 0);
  sprite.deleteSprite();
}

void setCommandName(const int cmd)
{
  sprite.setColorDepth(8);
  sprite.createSprite(COMMAND_WIDTH, COMMAND_HEIGHT);
  sprite.fillSprite(COMMAND_BG_COLOR);
  sprite.setTextColor(COMMAND_COLOR);
  sprite.setTextDatum(COMMAND_DATUM);
  sprite.drawString(COMMAND_LABEL,
                    COMMAND_LABEL_X_POS, COMMAND_LABEL_Y_POS,
                    COMMAND_FONT_SIZE);
  sprite.drawString(MyCobot::getCommandName(cmd),
                    COMMAND_NAME_X_POS, COMMAND_NAME_Y_POS,
                    COMMAND_FONT_SIZE);
  sprite.pushSprite(COMMAND_X_POS, COMMAND_Y_POS);
  sprite.deleteSprite();
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

const char *getDumpButtonName(const bool dumped)
{
  return dumped ? DUMP_BUTTON_ON : DUMP_BUTTON_OFF;
}

bool getDumped()
{
  prefs.begin(PREFS_NAMESPACE, true);
  bool dumped = prefs.getBool(PREFS_IS_DUMPED, false);
  prefs.end();
  return dumped;
}

bool toggleDumped(const bool prev)
{
  prefs.begin(PREFS_NAMESPACE, false);
  prefs.putBool(PREFS_IS_DUMPED, !prev);
  prefs.end();
  return !prev;
}

void setDumpButton(const int32_t x, const int32_t y, const bool dumped)
{
  sprite.setColorDepth(8);
  sprite.createSprite(DUMP_BUTTON_WIDTH, DUMP_BUTTON_HEIGHT);
  sprite.fillSprite(DUMP_BUTTON_BG_COLOR);
  sprite.setTextColor(DUMP_BUTTON_COLOR);
  sprite.setTextDatum(DUMP_BUTTON_DATUM);
  sprite.drawString(dumped ? DUMP_BUTTON_ON : DUMP_BUTTON_OFF,
                    DUMP_BUTTON_WIDTH / 2, DUMP_BUTTON_HEIGHT / 2,
                    DUMP_BUTTON_FONT_SIZE);
  sprite.pushSprite(x, y);
  sprite.deleteSprite();
}
