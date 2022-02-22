#ifndef _MY_COBOT_H
#define _MY_COBOT_H

#include <M5Stack.h>

enum DataFrame { FRAME_HEADER = 0xfe, FRAME_FOOTER = 0xfa };

enum DataFrameState {
    STATE_NONE,
    STATE_HEADER_START,
    STATE_HEADER_END,
    STATE_CMD_LEN,
    STATE_CMD,
    STATE_DATA,
    STATE_FOOTER,
    STATE_ILLEGAL
};

enum CommandCode {
    VERSION = 0x00,

    POWER_ON = 0x10,
    POWER_OFF = 0x11,
    IS_POWERED_ON = 0x12,
    RELEASE_ALL_SERVOS = 0x13,
    IS_CONTROLLER_CONNECTED = 0x14,
    READ_NEXT_ERROR = 0x15,
    SET_FREE_MODE = 0x1A,
    IS_FREE_MODE = 0x1B,

    GET_ANGLES = 0x20,
    SEND_ANGLE = 0x21,
    SEND_ANGLES = 0x22,
    GET_COORDS = 0x23,
    SEND_COORD = 0x24,
    SEND_COORDS = 0x25,
    PAUSE = 0x26,
    IS_PAUSED = 0x27,
    RESUME = 0x28,
    STOP = 0x29,
    IS_IN_POSITION = 0x2a,
    IS_MOVING = 0x2b,

    JOG_ANGLE = 0x30,
    JOG_COORD = 0x32,
    JOG_STOP = 0x34,
    SET_ENCODER = 0x3a,
    GET_ENCODER = 0x3b,
    SET_ENCODERS = 0x3c,
    GET_ENCODERS = 0x3d,

    GET_SPEED = 0x40,
    SET_SPEED = 0x41,
    GET_FEED_OVERRIDE = 0x42,
    GET_ACCELERATION = 0x44,
    GET_JOINT_MIN_ANGLE = 0x4a,
    GET_JOINT_MAX = 0x4b,

    IS_SERVO_ENABLED = 0x50,
    IS_ALL_SERVO_ENABLED = 0x51,
    SET_SERVO_DATA = 0x52,
    GET_SERVO_DATA = 0x53,
    SET_SERVO_CALIBRATION = 0x54,
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
    SET_COLOR = 0x6a,

    SET_BASIC_OUTPUT = 0xa0,
    GET_BASIC_INPUT = 0xa1,

    SET_GPIO_MODE = 0xaa,
    SET_GPIO_UP = 0xab,
    SET_GPIO_OUTPUT = 0xac,
    GET_GPIO_IN = 0xad,

    SET_SSID_PWD = 0xb0,
    GET_SSID_PWD = 0xb1,
    SET_SERVER_PORT = 0xb2,

    GET_TOF_DISTANCE = 0xc0,
};

class MyCobotTransponder {
public:
    virtual ~MyCobotTransponder() = 0;
    virtual bool is_received(void) = 0;
    virtual void send(uint8_t b) = 0;
    virtual uint8_t recv(void) = 0;
    virtual void flush(void) = 0;
    virtual void flush_received(void) = 0;
};

class MyCobotParser {
public:
    MyCobotParser(MyCobotTransponder *transponder);
    virtual ~MyCobotParser(void);

    virtual DataFrameState parse(int b);
    virtual bool isFrameState(DataFrameState state) const;
    virtual DataFrameState getFrameState(void) const;
    virtual const char *const getCommandName(int b);
    virtual const char *const getFrameStateName(DataFrameState state);
    virtual bool findFrameHeader(void);
    virtual bool parseAnglesReply(float *angles, size_t n_angles);

    virtual uint16_t getCommandCounter(void) const;
    virtual uint8_t getParsePosition(void) const;

    virtual void send(int b);
    virtual uint8_t recv(void);
    virtual bool available(void);
    virtual void flush(void);
    virtual void flush_received(void);

private:
    MyCobotTransponder *transponder;
    DataFrameState frame_state;
    uint16_t command_counter;
    uint8_t parse_position;
};

class MyCobot {
public:
    static const size_t N_JOINTS = 6;

    MyCobot(MyCobotTransponder *transponder);
    virtual ~MyCobot(void);

    virtual const char *const getCommandName(int cmd);
    virtual void setLED(byte r, byte g, byte b);
    virtual void releaseAllServos(void);
    virtual bool getAngles(float *angles, size_t n_angles);
    virtual bool isFrameState(DataFrameState state) const;
    virtual bool isInFrame(void) const;
    virtual DataFrameState getFrameState(void) const;
    virtual uint16_t getCommandCounter(void) const;
    virtual uint8_t getParsePosition(void) const;
    virtual DataFrameState parse(int b);
    virtual MyCobotParser &getParser(void);

private:
    static const byte CMD_SET_COLOR_LEN = 5;
    static const byte CMD_RELEASE_ALL_SERVOS_LEN = 2;
    static const byte CMD_GET_ANGLES_LEN = 2;

    MyCobotParser parser;
};

#endif  // _MY_COBOT_H
