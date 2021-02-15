#include "Transponder.h"

const char *const Transponder::VERSION = "v0.0.5";
const char *const Transponder::NAME = "Transponder";
const char *const Transponder::PREFS_NAMESPACE = "transponder";
const char *const Transponder::PREFS_IS_DUMPED = "is_dumped";

const char *const Transponder::getVersion(void)
{
    return VERSION;
}

#ifdef ENABLE_ESP_NOW
Transponder::Transponder(void) : receiver(MYCOBOT_CHANNEL)
{
}
#else
Transponder::Transponder(void)
{
}
#endif

Transponder::~Transponder(void)
{
}

void Transponder::begin(byte r, byte g, byte b)
{
    Serial.begin(SERIAL_BAUD_RATE);
    Serial2.begin(SERIAL2_BAUD_RATE);

    mycobot.setLED(r, g, b);

    uiTask.begin(1);
    uiTask.drawTitle(NAME, VERSION);
    uiTask.drawButtonA(readDumped());
    uiTask.drawButtonB();
    uiTask.drawButtonC();
#ifdef ENABLE_ESP_NOW
    uiTask.drawEspNowStatus(receiver.begin());
#endif
}

void Transponder::send(void)
{
    uiTask.drawSendStatus(Serial.available() > 0);
    int b;
    while (Serial.available() > 0)
    {
        b = Serial.read();
        sendCommandByte(b);
    }
}

void Transponder::recv(void)
{

    uiTask.drawRecvStatus(Serial2.available() > 0);
    while (Serial2.available() > 0)
    {
        Serial.write(Serial2.read());
    }
}

bool Transponder::readDumped(void)
{
    prefs.begin(PREFS_NAMESPACE, true);
    dumped = prefs.getBool(PREFS_IS_DUMPED, false);
    prefs.end();
    return dumped;
}

bool Transponder::toggleDumped(void)
{
    dumped = !dumped;
    prefs.begin(PREFS_NAMESPACE, false);
    prefs.putBool(PREFS_IS_DUMPED, dumped);
    prefs.end();

    uiTask.drawButtonA(dumped);
    if (!dumped)
    {
        uiTask.clearCommandName();
        uiTask.clearDataFrame();
    }
    return dumped;
}

void Transponder::sendCommandByte(int b)
{
    Serial2.write(b);
    mycobot.parse(b);
    if (dumped)
    {
        dumpCommand(b);
    }
}

void Transponder::drawJointAngles(void)
{
    if (mycobot.isFrameState(STATE_NONE))
    {
        float angles[MyCobot::N_JOINTS] = {
            0.0,
        };
        mycobot.getAngles(angles, MyCobot::N_JOINTS);
        uiTask.drawJointAngles(angles, MyCobot::N_JOINTS);
    }
}

void Transponder::setFreeMove(void)
{
    if (mycobot.isFrameState(STATE_NONE))
    {
        setFreeMove();
    }
}

void Transponder::dumpCommand(int b)
{
    if (mycobot.isFrameState(STATE_CMD))
    {
        uiTask.drawCommandName(mycobot.getCommandName(b),
                               mycobot.getCommandCounter());
    }
    if (mycobot.isFrameState(STATE_HEADER_START))
    {
        uiTask.clearDataFrame();
    }
    if (mycobot.isInFrame())
    {
        uiTask.drawDataFrame(mycobot.getFrameState(), mycobot.getParsePosition(), b);
    }
}
