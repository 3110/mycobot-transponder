#include <M5Stack.h>
#include <Preferences.h>
#include "m5stack/UITask.h"
#include "MyCobot.h"

#ifdef ENABLE_ESP_NOW
#include "m5stack/EspNowReceiver.h"
#endif

const char *const VERSION = "v0.0.5";
const char *const TITLE = "Transponder";

const char *const PREFS_NAMESPACE = "transponder";
const char *const PREFS_IS_DUMPED = "is_dumped";

const byte ATOM_LED_R = 0xff;
const byte ATOM_LED_G = 0xff;
const byte ATOM_LED_B = 0xff;

const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

extern void setup(void);
extern void loop(void);

extern void sendCommandByte(const int);
extern void setLED(const byte, const byte, const byte);
extern void setFreeMove(void);
extern const bool findFrameHeader(void);
extern const bool parseGetAnglesReply(float *, const size_t);
extern bool getAngles(float *, const size_t);
extern bool getDumped(void);
extern bool toggleDumped(bool prev);

Preferences prefs;
bool is_dumped;
UITask uiTask;
MyCobot::FrameState frame_state;
uint16_t command_counter;
uint8_t parse_position;

#ifdef ENABLE_ESP_NOW
EspNowReceiver receiver(MYCOBOT_CHANNEL);
#endif

void setup(void)
{
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL2_BAUD_RATE);

  uiTask.begin(1);
  uiTask.drawTitle(TITLE, VERSION);

  setLED(ATOM_LED_R, ATOM_LED_G, ATOM_LED_B);

  frame_state = MyCobot::STATE_NONE;
  command_counter = 0;
  parse_position = 0;

  is_dumped = getDumped();
  uiTask.drawButtonA(is_dumped);
  uiTask.drawButtonB();
  uiTask.drawButtonC();

#ifdef ENABLE_ESP_NOW
  uiTask.drawEspNowStatus(receiver.begin());
#endif
}

void loop(void)
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    is_dumped = toggleDumped(is_dumped);
    uiTask.drawButtonA(is_dumped);
    if (!is_dumped)
    {
      uiTask.clearCommandName();
    }
  }

  if (M5.BtnB.wasPressed())
  {
    if (frame_state == MyCobot::STATE_NONE)
    {
      float angles[MyCobot::N_JOINTS] = {
          0.0,
      };
      getAngles(angles, MyCobot::N_JOINTS);
      uiTask.drawJointAngles(angles, MyCobot::N_JOINTS);
    }
  }

  if (M5.BtnC.wasPressed())
  {
    if (frame_state == MyCobot::STATE_NONE)
    {
      setFreeMove();
    }
  }

  uiTask.drawSendStatus(Serial.available() > 0);
  int b;
  while (Serial.available() > 0)
  {
    b = Serial.read();
    Serial2.write(b);
    sendCommandByte(b);
  }

  uiTask.drawRecvStatus(Serial2.available() > 0);
  while (Serial2.available() > 0)
  {
    Serial.write(Serial2.read());
  }
}

void sendCommandByte(const int b)
{
  frame_state = MyCobot::checkFrameState(frame_state, b);
  if (frame_state == MyCobot::STATE_CMD)
  {
    ++command_counter;
  }
  if (frame_state == MyCobot::STATE_HEADER_START ||
      frame_state == MyCobot::STATE_NONE ||
      frame_state == MyCobot::STATE_ILLEGAL)
  {
    parse_position = 0;
  }
  else
  {
    ++parse_position;
  }
  if (is_dumped)
  {
    if (frame_state == MyCobot::STATE_CMD)
    {
      uiTask.drawCommandName(b, command_counter);
    }
    if (frame_state == MyCobot::STATE_HEADER_START)
    {
      uiTask.clearDataFrame();
    }
    if (frame_state != MyCobot::STATE_NONE &&
        frame_state != MyCobot::STATE_ILLEGAL)
    {
      uiTask.drawDataFrame(frame_state, parse_position, b);
    }
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

const bool findFrameHeader(void)
{
  int prev = -1;
  int cur = -1;
  while (Serial2.available() > 0)
  {
    cur = Serial2.read();
    if (prev == MyCobot::FRAME_HEADER && cur == MyCobot::FRAME_HEADER)
    {
      return true;
    }
    prev = cur;
  }
  return false;
}

const bool parseGetAnglesReply(float *angles, const size_t n_angles)
{
  const int data_len = Serial2.read();
  if (data_len == -1 || Serial2.available() != data_len)
  {
    return false;
  }
  const int cmd = Serial2.read();
  if (cmd != MyCobot::Command::GET_ANGLES)
  {
    return false;
  }
  int high = 0;
  int low = 0;
  for (int i = 0; i < n_angles; ++i)
  {
    high = Serial2.read();
    low = Serial2.read();
    angles[i] = (short)((high << 8 | low) & 0xFFFF) / 100.0;
  }
  return Serial2.read() == MyCobot::FRAME_FOOTER;
}

bool getAngles(float *angles, const size_t n_angles)
{
  Serial2.write(MyCobot::FRAME_HEADER);
  Serial2.write(MyCobot::FRAME_HEADER);
  Serial2.write(MyCobot::CMD_GET_ANGLES_LEN);
  Serial2.write(MyCobot::GET_ANGLES);
  Serial2.write(MyCobot::FRAME_FOOTER);
  Serial2.flush();
  delay(500);
  return findFrameHeader() && parseGetAnglesReply(angles, n_angles);
}

bool getDumped(void)
{
  prefs.begin(PREFS_NAMESPACE, true);
  bool dumped = prefs.getBool(PREFS_IS_DUMPED, false);
  prefs.end();
  return dumped;
}

bool toggleDumped(bool prev)
{
  prefs.begin(PREFS_NAMESPACE, false);
  prefs.putBool(PREFS_IS_DUMPED, !prev);
  prefs.end();
  return !prev;
}
