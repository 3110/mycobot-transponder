#include "MyCobot.h"

using namespace MyCobot;

const size_t MyCobot::N_JOINTS = 6;

const byte MyCobot::FRAME_HEADER = 0xfe;
const byte MyCobot::FRAME_FOOTER = 0xfa;

const byte MyCobot::CMD_SET_LED_LEN = 5;
const byte MyCobot::CMD_SET_FREE_MOVE_LEN = 2;
const byte MyCobot::CMD_GET_ANGLES_LEN = 2;

FrameState MyCobot::checkFrameState(const FrameState state, const int b)
{
  switch (state)
  {
  case STATE_NONE:
    return b == FRAME_HEADER ? STATE_HEADER_START : STATE_NONE;
  case STATE_HEADER_START:
    return b == FRAME_HEADER ? STATE_HEADER_END : STATE_ILLEGAL;
  case STATE_HEADER_END:
    return STATE_CMD_LEN;
  case STATE_CMD_LEN:
    return STATE_CMD;
  case STATE_CMD:
    return b == FRAME_FOOTER ? STATE_FOOTER : STATE_DATA;
  case STATE_DATA:
    return b == FRAME_FOOTER ? STATE_FOOTER : STATE_DATA;
  case STATE_FOOTER:
    return b == FRAME_HEADER ? STATE_HEADER_START : STATE_NONE;
  case STATE_ILLEGAL:
    return b == FRAME_HEADER ? STATE_HEADER_START : STATE_NONE;
  default:
    return STATE_ILLEGAL;
  }
}

const char *MyCobot::getFrameStateName(const FrameState s)
{
  switch (s)
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

const char *MyCobot::getCommandName(const int cmd)
{
  switch (cmd)
  {
  case MyCobot::GET_ROBOT_VERSION:
    return "Get Robot Version";
  case MyCobot::GET_SYSTEM_VERSION:
    return "Get System Version";
  case MyCobot::GET_ROBOT_ID:
    return "Get Robot ID";
  case MyCobot::SET_ROBOT_ID:
    return "Set Robot ID";
  case MyCobot::POWER_ON:
    return "Power On";
  case MyCobot::POWER_OFF:
    return "Power Off";
  case MyCobot::IS_POWERED_ON:
    return "Is Powered On?";
  case MyCobot::SET_FREE_MOVE:
    return "Set Free Move";
  case MyCobot::IS_CONTROLLER_CONNECTED:
    return "Is Controller Connected?";
  case MyCobot::READ_NEXT_ERROR:
    return "Read Next Error";
  case MyCobot::GET_ANGLES:
    return "Get Angles";
  case MyCobot::WRITE_ANGLE:
    return "Write Angle";
  case MyCobot::SET_ANGLES:
    return "Set Angles";
  case MyCobot::GET_COORDS:
    return "Get Coords";
  case MyCobot::WRITE_COORD:
    return "Write Coord";
  case MyCobot::WRITE_COORDS:
    return "Write Coords";
  case MyCobot::PROGRAM_PAUSE:
    return "Program Pause";
  case MyCobot::IS_PROGRAM_PAUSED:
    return "Is Program Paused?";
  case MyCobot::PROGRAM_RESUME:
    return "Program Resume";
  case MyCobot::TASK_STOP:
    return "Task Stop";
  case MyCobot::IS_MOVING:
    return "Is Moving?";
  case MyCobot::JOG_ANGLE:
    return "Jog Angle";
  case MyCobot::JOG_ABSOLUTE:
    return "Jog Absolute";
  case MyCobot::JOG_COORD:
    return "Jog Coord";
  case MyCobot::SEND_JOG_INCREMENT:
    return "Send Jog Increment";
  case MyCobot::SET_ENCODER:
    return "Set Encoder";
  case MyCobot::GET_ENCODER:
    return "Get Encoder";
  case MyCobot::SET_ENCODERS:
    return "Set Encoders";
  case MyCobot::GET_SPEED:
    return "Get Speed";
  case MyCobot::SET_SPEED:
    return "Set Speed";
  case MyCobot::GET_FEED_OVERRIDE:
    return "Get Feed Override";
  case MyCobot::SEND_OVERRIDE:
    return "Send Override";
  case MyCobot::GET_ACCELERATION:
    return "Get Acceleration";
  case MyCobot::GET_JOINT_MIN:
    return "Get Joint Min";
  case MyCobot::GET_JOINT_MAX:
    return "Get Joint Max";
  case MyCobot::SET_JOINT_MIN:
    return "Set Joint Min";
  case MyCobot::SET_JOINT_MAX:
    return "Set Joint Max";
  case MyCobot::IS_SERVO_ENABLED:
    return "Is Servo Enabled?";
  case MyCobot::IS_ALL_SERVO_ENABLED:
    return "Is All Servo Enabled?";
  case MyCobot::SET_SERVO_DATA:
    return "Set Servo Data";
  case MyCobot::GET_SERVO_DATA:
    return "Get Servo Data";
  case MyCobot::SET_SERVO_CALIBRATION:
    return "Set Servo Calibration";
  case MyCobot::VOID_JOINT_BRAKE:
    return "Void Joint Brake";
  case MyCobot::SET_PIN_MODE:
    return "Set Pin Mode";
  case MyCobot::SET_PIN_DATA:
    return "Set Pin Data";
  case MyCobot::GET_PIN_DATA:
    return "Get Pin Data";
  case MyCobot::SET_GRIPPER_STATE:
    return "Set Gripper State";
  case MyCobot::SET_LED:
    return "Set LED";
  default:
    return "Unknown";
  };
}
