#ifndef _MY_COBOT_H
#define _MY_COBOT_H

#include <Arduino.h>

namespace MyCobot
{
  extern const size_t N_JOINTS;

  extern const byte FRAME_HEADER;
  extern const byte FRAME_FOOTER;

  extern const byte CMD_SET_LED_LEN;
  extern const byte CMD_SET_FREE_MOVE_LEN;
  extern const byte CMD_GET_ANGLES_LEN;

  enum Command
  {
    UNKNOWN = 0x00,
    GET_ROBOT_VERSION = 0x01,
    GET_SYSTEM_VERSION = 0x02,
    GET_ROBOT_ID = 0x03,
    SET_ROBOT_ID = 0x04,
    POWER_ON = 0x10,
    POWER_OFF = 0x11,
    IS_POWERED_ON = 0x12,
    SET_FREE_MOVE = 0x13,
    IS_CONTROLLER_CONNECTED = 0x14,
    READ_NEXT_ERROR = 0x15,
    GET_ANGLES = 0x20,
    WRITE_ANGLE = 0x21,
    SET_ANGLES = 0x22,
    GET_COORDS = 0x23,
    WRITE_COORD = 0x24,
    WRITE_COORDS = 0x25,
    PROGRAM_PAUSE = 0x26,
    IS_PROGRAM_PAUSED = 0x27,
    PROGRAM_RESUME = 0x28,
    TASK_STOP = 0x29,
    IS_MOVING = 0x2b,
    JOG_ANGLE = 0x30,
    JOG_ABSOLUTE = 0x31,
    JOG_COORD = 0x32,
    SEND_JOG_INCREMENT = 0x33,
    SET_ENCODER = 0x3a,
    GET_ENCODER = 0x3b,
    SET_ENCODERS = 0x3c,
    GET_SPEED = 0x40,
    SET_SPEED = 0x41,
    GET_FEED_OVERRIDE = 0x42,
    SEND_OVERRIDE = 0x43,
    GET_ACCELERATION = 0x44,
    GET_JOINT_MIN = 0x4a,
    GET_JOINT_MAX = 0x4b,
    SET_JOINT_MIN = 0x4c,
    SET_JOINT_MAX = 0x4d,
    IS_SERVO_ENABLED = 0x50,
    IS_ALL_SERVO_ENABLED = 0x51,
    SET_SERVO_DATA = 0x52,
    GET_SERVO_DATA = 0x53,
    SET_SERVO_CALIBRATION = 0x54,
    VOID_JOINT_BRAKE = 0x55,
    RELEASE_SERVO = 0x56,
    FOCUS_SERVO = 0x57,
    SET_PIN_MODE = 0x60,
    SET_DIGITAL_OUTPUT = 0x61,
    GET_DIGITAL_INPUT = 0x62,
    SET_PWM_MODE = 0x63,
    SET_PWM_OUTPUT = 0x64,
    GET_GRIPPER_VALUE = 0x65,
    SET_GRIPPER_STATE = 0x66,
    SET_GRIPPER_VALUE = 0x67,
    SET_GRIPPER_INI = 0x68,
    IS_GRIPPER_MOVING = 0x69,
    SET_LED = 0x6a,
  };

  enum FrameState
  {
    STATE_NONE,
    STATE_HEADER_START,
    STATE_HEADER_END,
    STATE_CMD_LEN,
    STATE_CMD,
    STATE_DATA,
    STATE_FOOTER,
    STATE_ILLEGAL
  };

  extern FrameState checkFrameState(const FrameState, const int);
  extern const char *getCommandName(const int);
  extern const char *getFrameStateName(const FrameState);
} // namespace MyCobot

#endif // _MY_COBOT_H