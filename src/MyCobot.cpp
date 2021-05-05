#include "MyCobot.h"

MyCobotParser::MyCobotParser(MyCobotTransponder *transponder)
    : transponder(transponder),
      frame_state(STATE_NONE),
      command_counter(0),
      parse_position(0)

{
}

MyCobotParser::~MyCobotParser(void)
{
  delete transponder;
  transponder = NULL;
}

DataFrameState MyCobotParser::getFrameState(void) const
{
  return frame_state;
}

DataFrameState MyCobotParser::parse(int b)
{
  switch (frame_state)
  {
  case STATE_NONE:
    frame_state = b == FRAME_HEADER ? STATE_HEADER_START : STATE_NONE;
    parse_position = 0;
    break;
  case STATE_HEADER_START:
    frame_state = b == FRAME_HEADER ? STATE_HEADER_END : STATE_ILLEGAL;
    ++parse_position;
    break;
  case STATE_HEADER_END:
    frame_state = STATE_CMD_LEN;
    ++parse_position;
    break;
  case STATE_CMD_LEN:
    frame_state = STATE_CMD;
    ++command_counter;
    ++parse_position;
    break;
  case STATE_CMD:
    frame_state = b == FRAME_FOOTER ? STATE_FOOTER : STATE_DATA;
    ++parse_position;
    break;
  case STATE_DATA:
    frame_state = b == FRAME_FOOTER ? STATE_FOOTER : STATE_DATA;
    ++parse_position;
    break;
  case STATE_FOOTER:
    frame_state = b == FRAME_HEADER ? STATE_HEADER_START : STATE_NONE;
    parse_position = 0;
    break;
  case STATE_ILLEGAL:
    frame_state = b == FRAME_HEADER ? STATE_HEADER_START : STATE_NONE;
    parse_position = 0;
    break;
  default:
    frame_state = STATE_ILLEGAL;
    parse_position = 0;
    break;
  }
  return frame_state;
}

bool MyCobotParser::isFrameState(DataFrameState state) const
{
  return frame_state == state;
}

const char *const MyCobotParser::getCommandName(int cmd)
{
  switch (cmd)
  {
  case GET_ROBOT_VERSION:
    return "Get Robot Version";
  case GET_SYSTEM_VERSION:
    return "Get System Version";
  case GET_ROBOT_ID:
    return "Get Robot ID";
  case SET_ROBOT_ID:
    return "Set Robot ID";
  case POWER_ON:
    return "Power On";
  case POWER_OFF:
    return "Power Off";
  case IS_POWERED_ON:
    return "Is Powered On?";
  case SET_FREE_MOVE:
    return "Set Free Move";
  case IS_CONTROLLER_CONNECTED:
    return "Is Controller Connected?";
  case READ_NEXT_ERROR:
    return "Read Next Error";
  case GET_ANGLES:
    return "Get Angles";
  case WRITE_ANGLE:
    return "Write Angle";
  case SET_ANGLES:
    return "Set Angles";
  case GET_COORDS:
    return "Get Coords";
  case WRITE_COORD:
    return "Write Coord";
  case WRITE_COORDS:
    return "Write Coords";
  case PROGRAM_PAUSE:
    return "Program Pause";
  case IS_PROGRAM_PAUSED:
    return "Is Program Paused?";
  case PROGRAM_RESUME:
    return "Program Resume";
  case TASK_STOP:
    return "Task Stop";
  case IS_MOVING:
    return "Is Moving?";
  case JOG_ANGLE:
    return "Jog Angle";
  case JOG_ABSOLUTE:
    return "Jog Absolute";
  case JOG_COORD:
    return "Jog Coord";
  case SEND_JOG_INCREMENT:
    return "Send Jog Increment";
  case JOG_STOP:
    return "Jog Stop";
  case SET_ENCODER:
    return "Set Encoder";
  case GET_ENCODER:
    return "Get Encoder";
  case SET_ENCODERS:
    return "Set Encoders";
  case GET_SPEED:
    return "Get Speed";
  case SET_SPEED:
    return "Set Speed";
  case GET_FEED_OVERRIDE:
    return "Get Feed Override";
  case SEND_OVERRIDE:
    return "Send Override";
  case GET_ACCELERATION:
    return "Get Acceleration";
  case GET_JOINT_MIN:
    return "Get Joint Min";
  case GET_JOINT_MAX:
    return "Get Joint Max";
  case SET_JOINT_MIN:
    return "Set Joint Min";
  case SET_JOINT_MAX:
    return "Set Joint Max";
  case IS_SERVO_ENABLED:
    return "Is Servo Enabled?";
  case IS_ALL_SERVO_ENABLED:
    return "Is All Servo Enabled?";
  case SET_SERVO_DATA:
    return "Set Servo Data";
  case GET_SERVO_DATA:
    return "Get Servo Data";
  case SET_SERVO_CALIBRATION:
    return "Set Servo Calibration";
  case VOID_JOINT_BRAKE:
    return "Void Joint Brake";
  case RELEASE_SERVO:
    return "Release Servo";
  case FOCUS_SERVO:
    return "Focus Servo";
  case SET_PIN_MODE:
    return "Set Pin Mode";
  case SET_DIGITAL_OUTPUT:
    return "Set Digital Output";
  case GET_DIGITAL_INPUT:
    return "Get Digital Input";
  case SET_PWM_MODE:
    return "Set PWM Mode";
  case SET_PWM_OUTPUT:
    return "set PWM Output";
  case GET_GRIPPER_VALUE:
    return "Get Gripper Value";
  case SET_GRIPPER_STATE:
    return "Set Gripper State";
  case SET_GRIPPER_VALUE:
    return "Set Gripper Value";
  case SET_GRIPPER_INI:
    return "Set Gripper Ini";
  case IS_GRIPPER_MOVING:
    return "Is Gripper Moving?";
  case SET_LED:
    return "Set LED";
  case SET_BASIC_OUTPUT:
    return "Set Basic Output";
  default:
    return "Unknown";
  };
}

const char *const MyCobotParser::getFrameStateName(DataFrameState state)
{
  switch (state)
  {
  case STATE_NONE:
    return "NONE";
  case STATE_HEADER_START:
    return "HEADER_START";
  case STATE_HEADER_END:
    return "HEADER_END";
  case STATE_CMD_LEN:
    return "CMD_LEN";
  case STATE_CMD:
    return "CMD";
  case STATE_DATA:
    return "DATA";
  case STATE_FOOTER:
    return "FOOTER";
  default:
    return "ILLEGAL";
  };
}

bool MyCobotParser::findFrameHeader(void)
{
  int prev = -1;
  int cur = -1;
  while (available())
  {
    cur = recv();
    if (prev == FRAME_HEADER && cur == FRAME_HEADER)
    {
      return true;
    }
    prev = cur;
  }
  return false;
}

bool MyCobotParser::parseAnglesReply(float *angles, size_t n_angles)
{
  const int data_len = recv();
  if (data_len == -1)
  {
    return false;
  }
  const int cmd = recv();
  if (cmd != GET_ANGLES)
  {
    return false;
  }
  int high = 0;
  int low = 0;
  for (int i = 0; i < n_angles; ++i)
  {
    high = recv();
    low = recv();
    angles[i] = (short)((high << 8 | low) & 0xFFFF) / 100.0;
  }
  return recv() == FRAME_FOOTER;
}

uint16_t MyCobotParser::getCommandCounter() const
{
  return command_counter;
}

uint8_t MyCobotParser::getParsePosition(void) const
{
  return parse_position;
}

void MyCobotParser::send(int b)
{
  transponder->send(b);
}

uint8_t MyCobotParser::recv(void)
{
  return transponder->recv();
}

bool MyCobotParser::available(void)
{
  return transponder->is_received();
}

void MyCobotParser::flush(void)
{
  transponder->flush();
}

void MyCobotParser::flush_received(void)
{
  transponder->flush_received();
}

MyCobotTransponder::~MyCobotTransponder(void)
{
}

MyCobot::MyCobot(MyCobotTransponder *transponder) : parser(transponder)
{
}

MyCobot::~MyCobot(void)
{
}

MyCobotParser &MyCobot::getParser(void)
{
  return parser;
}

const char *const MyCobot::getCommandName(int cmd)
{
  return parser.getCommandName(cmd);
}

void MyCobot::setLED(byte r, byte g, byte b)
{
  parser.send(FRAME_HEADER);
  parser.send(FRAME_HEADER);
  parser.send(CMD_SET_LED_LEN);
  parser.send(SET_LED);
  parser.send(r);
  parser.send(g);
  parser.send(b);
  parser.send(FRAME_FOOTER);
  parser.flush();
}

void MyCobot::setFreeMove(void)
{
  parser.send(FRAME_HEADER);
  parser.send(FRAME_HEADER);
  parser.send(CMD_SET_FREE_MOVE_LEN);
  parser.send(SET_FREE_MOVE);
  parser.send(FRAME_FOOTER);
  parser.flush();
  delay(100);
  parser.flush_received();
}

bool MyCobot::getAngles(float *angles, const size_t n_angles)
{
  parser.send(FRAME_HEADER);
  parser.send(FRAME_HEADER);
  parser.send(CMD_GET_ANGLES_LEN);
  parser.send(GET_ANGLES);
  parser.send(FRAME_FOOTER);
  parser.flush();
  delay(10);
  return parser.findFrameHeader() && parser.parseAnglesReply(angles, n_angles);
}

bool MyCobot::isFrameState(DataFrameState state) const
{
  return parser.isFrameState(state);
}

bool MyCobot::isInFrame(void) const
{
  return !isFrameState(STATE_NONE) && !isFrameState(STATE_ILLEGAL);
}

DataFrameState MyCobot::getFrameState(void) const
{

  return parser.getFrameState();
}

uint16_t MyCobot::getCommandCounter(void) const
{

  return parser.getCommandCounter();
}

uint8_t MyCobot::getParsePosition(void) const
{
  return parser.getParsePosition();
}

DataFrameState MyCobot::parse(int b)
{
  return parser.parse(b);
}
