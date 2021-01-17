#include <M5Stack.h>
#include <Preferences.h>
#include "MyCobot.h"

const char *PREFS_NAMESPACE = "transponder";
const char *PREFS_IS_DUMPED = "is_dumped";

const char *VERSION = "v0.0.2";
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
const uint8_t COMMAND_DATUM = TL_DATUM;
const uint32_t COMMAND_COLOR = TFT_WHITE;
const uint32_t COMMAND_BG_COLOR = TFT_BLACK;
const char *COMMAND_LABEL = "Send:";
const int32_t COMMAND_LABEL_X_POS = 0;
const int32_t COMMAND_LABEL_Y_POS = 0;
const int32_t COMMAND_COUNTER_X_POS = 80;
const int32_t COMMAND_COUNTER_Y_POS = 0;
const uint8_t COMMAND_COUNTER_FONT_SIZE = COMMAND_FONT_SIZE;
const uint8_t COMMAND_COUNTER_DATUM = TL_DATUM;
const uint32_t COMMAND_COUNTER_COLOR = COMMAND_COLOR;
const uint32_t COMMAND_COUNTER_BG_COLOR = COMMAND_BG_COLOR;

enum ButtonName
{
  ButtonA,
  ButtonB,
  ButtonC
};

const int16_t BUTTON_NAME_WIDTH = 16 * 5;
const int16_t BUTTON_NAME_HEIGHT = 20;
const int32_t BUTTON_A_NAME_X_POS = 65 - BUTTON_NAME_WIDTH / 2;
const int32_t BUTTON_B_NAME_X_POS = 160 - BUTTON_NAME_WIDTH / 2;
const int32_t BUTTON_C_NAME_X_POS = 255 - BUTTON_NAME_WIDTH / 2;
const int32_t BUTTON_NAME_Y_POS = 210;
const uint8_t BUTTON_NAME_FONT_SIZE = 4;
const uint8_t BUTTON_NAME_DATUM = MC_DATUM;
const uint32_t BUTTON_NAME_COLOR = TFT_WHITE;
const uint32_t BUTTON_NAME_BG_COLOR = TFT_BLACK;

const enum ButtonName DUMP_BUTTON_NAME = ButtonName::ButtonA;
const char *DUMP_BUTTON_ON = "ON";
const char *DUMP_BUTTON_OFF = "OFF";

const char *FREE_MOVE_BUTTON_LABEL_NAME = "Free";
const enum ButtonName FREE_MOVE_BUTTON_NAME = ButtonName::ButtonC;

const byte ATOM_LED_R = 0xff;
const byte ATOM_LED_G = 0xff;
const byte ATOM_LED_B = 0xff;

const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

extern void setup(void);
extern void loop(void);
extern void setLED(const byte, const byte, const byte);
extern void setFreeMove(void);
extern void setTitle(const char *, const char *);
extern void clearCommandName(void);
extern void setCommandName(const int, const uint16_t);
extern void setSend(const bool);
extern void setRecv(const bool);
extern bool getDumped(void);
extern bool toggleDumped(const bool);
extern void setButtonName(enum ButtonName, const char *);
extern void setDumpButton(const enum ButtonName, const bool dumped);

TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);
MyCobot::FrameState frame_state;
Preferences prefs;
bool is_dumped;
uint16_t command_counter;

void setup(void)
{
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL2_BAUD_RATE);

  setLED(ATOM_LED_R, ATOM_LED_G, ATOM_LED_B);
  setTitle(TITLE, VERSION);

  frame_state = MyCobot::STATE_NONE;
  command_counter = 0;

  is_dumped = getDumped();
  setDumpButton(DUMP_BUTTON_NAME, is_dumped);

  setButtonName(FREE_MOVE_BUTTON_NAME, FREE_MOVE_BUTTON_LABEL_NAME);
}

void loop(void)
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    is_dumped = toggleDumped(is_dumped);
    setDumpButton(DUMP_BUTTON_NAME, is_dumped);
    if (!is_dumped)
    {
      clearCommandName();
    }
  }

  if (M5.BtnC.wasPressed())
  {
    if (frame_state == MyCobot::STATE_NONE)
    {
      setFreeMove();
    }
  }

  setSend(Serial.available() > 0);
  while (Serial.available() > 0)
  {
    const int b = Serial.read();
    Serial2.write(b);
    frame_state = MyCobot::checkFrameState(frame_state, b);
    if (frame_state == MyCobot::STATE_CMD)
    {
      ++command_counter;
      if (is_dumped)
      {
        setCommandName(b, command_counter);
      }
    }
  }

  setRecv(Serial2.available() > 0);
  while (Serial2.available() > 0)
  {
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

void setFreeMove(void)
{
  Serial2.write(MyCobot::FRAME_HEADER);
  Serial2.write(MyCobot::FRAME_HEADER);
  Serial2.write(MyCobot::CMD_SET_FREE_MOVE_LEN);
  Serial2.write(MyCobot::SET_FREE_MOVE);
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

void clearCommandName(void)
{
  sprite.setColorDepth(8);
  sprite.createSprite(COMMAND_WIDTH, COMMAND_HEIGHT);
  sprite.fillSprite(COMMAND_BG_COLOR);
  sprite.pushSprite(COMMAND_X_POS, COMMAND_Y_POS);
  sprite.deleteSprite();
}

void setCommandName(const int cmd, const uint16_t counter)
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
  sprite.drawNumber(counter,
                    COMMAND_COUNTER_X_POS, COMMAND_COUNTER_Y_POS,
                    COMMAND_COUNTER_FONT_SIZE);
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

bool getDumped(void)
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

void setButtonName(enum ButtonName btn, const char *name)
{
  int32_t x = 0;
  switch (btn)
  {
  case ButtonName::ButtonA:
    x = BUTTON_A_NAME_X_POS;
    break;
  case ButtonName::ButtonB:
    x = BUTTON_B_NAME_X_POS;
    break;
  case ButtonName::ButtonC:
    x = BUTTON_C_NAME_X_POS;
    break;
  default:
    return;
  }
  sprite.setColorDepth(8);
  sprite.createSprite(BUTTON_NAME_WIDTH, BUTTON_NAME_HEIGHT);
  sprite.fillSprite(BUTTON_NAME_BG_COLOR);
  sprite.setTextColor(BUTTON_NAME_COLOR);
  sprite.setTextDatum(BUTTON_NAME_DATUM);
  sprite.drawString(name,
                    BUTTON_NAME_WIDTH / 2, BUTTON_NAME_HEIGHT / 2,
                    BUTTON_NAME_FONT_SIZE);
  sprite.pushSprite(x, BUTTON_NAME_Y_POS);
  sprite.deleteSprite();
}

void setDumpButton(const enum ButtonName btn, const bool dumped)
{
  setButtonName(btn, is_dumped ? DUMP_BUTTON_ON : DUMP_BUTTON_OFF);
}
