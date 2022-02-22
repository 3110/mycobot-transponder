#include "MyCobot.h"

MyCobotParser::MyCobotParser(MyCobotTransponder *transponder)
    : transponder(transponder),
      frame_state(STATE_NONE),
      command_counter(0),
      parse_position(0)

{
}

MyCobotParser::~MyCobotParser(void) {
    delete transponder;
    transponder = NULL;
}

DataFrameState MyCobotParser::getFrameState(void) const {
    return frame_state;
}

DataFrameState MyCobotParser::parse(int b) {
    switch (frame_state) {
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

bool MyCobotParser::isFrameState(DataFrameState state) const {
    return frame_state == state;
}

const char *const MyCobotParser::getCommandName(int cmd) {
    switch (cmd) {
        case VERSION:
            return "Get Version";
        case POWER_ON:
            return "Power On";
        case POWER_OFF:
            return "Power Off";
        case IS_POWERED_ON:
            return "Is Powered On?";
        case RELEASE_ALL_SERVOS:
            return "Release All Servos";
        case IS_CONTROLLER_CONNECTED:
            return "Is Controller Connected?";
        case READ_NEXT_ERROR:
            return "Read Next Error";
        case SET_FREE_MODE:
            return "Set Free Mode";
        case IS_FREE_MODE:
            return "Is Free Mode?";
        case GET_ANGLES:
            return "Get Angles";
        case SEND_ANGLE:
            return "Send Angle";
        case SEND_ANGLES:
            return "Send Angles";
        case GET_COORDS:
            return "Get Coords";
        case SEND_COORD:
            return "Send Coord";
        case SEND_COORDS:
            return "Send Coords";
        case PAUSE:
            return "Pause";
        case IS_PAUSED:
            return "Is Paused?";
        case RESUME:
            return "Resume";
        case STOP:
            return "Stop";
        case IS_IN_POSITION:
            return "Is in Position?";
        case IS_MOVING:
            return "Is Moving?";
        case JOG_ANGLE:
            return "Jog Angle";
        case JOG_COORD:
            return "Jog Coord";
        case JOG_STOP:
            return "Jog Stop";
        case SET_ENCODER:
            return "Set Encoder";
        case GET_ENCODER:
            return "Get Encoder";
        case SET_ENCODERS:
            return "Set Encoders";
        case GET_ENCODERS:
            return "Get Encoders";
        case GET_SPEED:
            return "Get Speed";
        case SET_SPEED:
            return "Set Speed";
        case GET_FEED_OVERRIDE:
            return "Get Feed Override";
        case GET_ACCELERATION:
            return "Get Acceleration";
        case GET_JOINT_MIN_ANGLE:
            return "Get Joint Min Angle";
        case GET_JOINT_MAX:
            return "Get Joint Max Angle";
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
        case SET_COLOR:
            return "Set Color";
        case SET_BASIC_OUTPUT:
            return "Set Basic Output";
        case GET_BASIC_INPUT:
            return "Get Basic Output";
        case SET_GPIO_MODE:
            return "Set GPIO Mode";
        case SET_GPIO_UP:
            return "Set GPIO Up";
        case SET_GPIO_OUTPUT:
            return "Set GPIO Output";
        case GET_GPIO_IN:
            return "Get GPIO In";
        case SET_SSID_PWD:
            return "Set SSID Password";
        case GET_SSID_PWD:
            return "Get SSID Password";
        case SET_SERVER_PORT:
            return "Set Server Port";
        case GET_TOF_DISTANCE:
            return "Get ToF Distance";
        default:
            return "Unknown";
    };
}

const char *const MyCobotParser::getFrameStateName(DataFrameState state) {
    switch (state) {
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

bool MyCobotParser::findFrameHeader(void) {
    int prev = -1;
    int cur = -1;
    while (available()) {
        cur = recv();
        if (prev == FRAME_HEADER && cur == FRAME_HEADER) {
            return true;
        }
        prev = cur;
    }
    return false;
}

bool MyCobotParser::parseAnglesReply(float *angles, size_t n_angles) {
    const int data_len = recv();
    if (data_len == -1) {
        return false;
    }
    const int cmd = recv();
    if (cmd != GET_ANGLES) {
        return false;
    }
    int high = 0;
    int low = 0;
    for (int i = 0; i < n_angles; ++i) {
        high = recv();
        low = recv();
        angles[i] = (short)((high << 8 | low) & 0xFFFF) / 100.0;
    }
    return recv() == FRAME_FOOTER;
}

uint16_t MyCobotParser::getCommandCounter() const {
    return command_counter;
}

uint8_t MyCobotParser::getParsePosition(void) const {
    return parse_position;
}

void MyCobotParser::send(int b) {
    transponder->send(b);
}

uint8_t MyCobotParser::recv(void) {
    return transponder->recv();
}

bool MyCobotParser::available(void) {
    return transponder->is_received();
}

void MyCobotParser::flush(void) {
    transponder->flush();
}

void MyCobotParser::flush_received(void) {
    transponder->flush_received();
}

MyCobotTransponder::~MyCobotTransponder(void) {
}

MyCobot::MyCobot(MyCobotTransponder *transponder) : parser(transponder) {
}

MyCobot::~MyCobot(void) {
}

MyCobotParser &MyCobot::getParser(void) {
    return parser;
}

const char *const MyCobot::getCommandName(int cmd) {
    return parser.getCommandName(cmd);
}

void MyCobot::setLED(byte r, byte g, byte b) {
    parser.send(FRAME_HEADER);
    parser.send(FRAME_HEADER);
    parser.send(CMD_SET_COLOR_LEN);
    parser.send(SET_COLOR);
    parser.send(r);
    parser.send(g);
    parser.send(b);
    parser.send(FRAME_FOOTER);
    parser.flush();
}

void MyCobot::releaseAllServos(void) {
    parser.send(FRAME_HEADER);
    parser.send(FRAME_HEADER);
    parser.send(CMD_RELEASE_ALL_SERVOS_LEN);
    parser.send(RELEASE_ALL_SERVOS);
    parser.send(FRAME_FOOTER);
    parser.flush();
    delay(100);
    parser.flush_received();
}

bool MyCobot::getAngles(float *angles, const size_t n_angles) {
    parser.send(FRAME_HEADER);
    parser.send(FRAME_HEADER);
    parser.send(CMD_GET_ANGLES_LEN);
    parser.send(GET_ANGLES);
    parser.send(FRAME_FOOTER);
    parser.flush();
    delay(10);
    return parser.findFrameHeader() &&
           parser.parseAnglesReply(angles, n_angles);
}

bool MyCobot::isFrameState(DataFrameState state) const {
    return parser.isFrameState(state);
}

bool MyCobot::isInFrame(void) const {
    return !isFrameState(STATE_NONE) && !isFrameState(STATE_ILLEGAL);
}

DataFrameState MyCobot::getFrameState(void) const {
    return parser.getFrameState();
}

uint16_t MyCobot::getCommandCounter(void) const {
    return parser.getCommandCounter();
}

uint8_t MyCobot::getParsePosition(void) const {
    return parser.getParsePosition();
}

DataFrameState MyCobot::parse(int b) {
    return parser.parse(b);
}
