#include "Transponder.h"

const char *const Transponder::VERSION = "v0.0.5";
const char *const Transponder::NAME = "Transponder";
const char *const Transponder::PREFS_NAMESPACE = "transponder";
const char *const Transponder::PREFS_IS_DUMPED = "is_dumped";

xQueueHandle send_queue = xQueueCreate(128, sizeof(uint8_t));
xQueueHandle recv_queue = xQueueCreate(128, sizeof(uint8_t));

const char *const Transponder::getVersion(void)
{
    return VERSION;
}

void transpond(void *arg)
{
    int b;
    while (1)
    {
        while (Serial.available() > 0)
        {
            b = Serial.read();
            Serial2.write(b);
            xQueueSend(send_queue, &b, 0);
        }
        while (Serial2.available() > 0)
        {
            b = Serial2.read();
            Serial.write(b);
            xQueueSend(recv_queue, &b, 0);
        }
        vTaskDelay(1 / portTICK_RATE_MS);
    }
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

bool Transponder::begin(BaseType_t core, byte r, byte g, byte b)
{
    Serial.begin(SERIAL_BAUD_RATE);
    Serial2.begin(SERIAL2_BAUD_RATE);

    const BaseType_t result = xTaskCreatePinnedToCore(
        transpond, NAME, 8192, NULL, 1, NULL, core);

    mycobot.setLED(r, g, b);

    uiTask.drawTitle(NAME, VERSION);
    uiTask.drawButtonA(readDumped());
    uiTask.drawButtonB();
    uiTask.drawButtonC();
#ifdef ENABLE_ESP_NOW
    uiTask.drawEspNowStatus(receiver.begin());
#endif
    return result == pdPASS;
}

void Transponder::send(void)
{
    uint8_t b = 0;
    const bool is_sent = xQueuePeek(send_queue, &b, 0) == pdPASS;
    uiTask.drawSendStatus(is_sent);
    while (xQueueReceive(send_queue, &b, 0) == pdPASS)
    {
        if (dumped)
        {
            mycobot.parse(b);
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
                uiTask.drawDataFrame(mycobot.getFrameState(),
                                     mycobot.getParsePosition(),
                                     b);
            }
        }
    }
    if (is_sent)
    {
        uiTask.drawSendStatus(false);
    }
}

void Transponder::recv(void)
{
    uint8_t b = 0;
    const bool is_recv = xQueuePeek(recv_queue, &b, 0) == pdPASS;
    uiTask.drawRecvStatus(is_recv);
    while (xQueueReceive(recv_queue, &b, 0) == pdPASS)
    {
    }
    if (is_recv)
    {
        uiTask.drawRecvStatus(false);
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

void Transponder::getJointAngles(void)
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
        mycobot.setFreeMove();
    }
}
