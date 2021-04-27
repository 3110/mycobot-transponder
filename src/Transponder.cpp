#include "Transponder.h"

const char *const Transponder::PREFS_NAMESPACE = "transponder";
const char *const Transponder::PREFS_IS_DUMPED = "is_dumped";

const char *const Transponder::getName(void)
{
    return TRANSPONDER_NAME;
}

const char *const Transponder::getVersion(void)
{
    return TRANSPONDER_VERSION;
}

void transpond(void *arg)
{
    uint8_t b;
    while (Serial.read() != -1)
        ;
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

SerialQueueTransponder::SerialQueueTransponder(HardwareSerial &sendSerial,
                                               xQueueHandle &sendHandle,
                                               xQueueHandle &recvHandle)
    : sendSerial(sendSerial), sendHandle(sendHandle), recvHandle(recvHandle)
{
}

SerialQueueTransponder::~SerialQueueTransponder(void)
{
}

bool SerialQueueTransponder::is_received(void)
{
    return uxQueueMessagesWaiting(recvHandle) > 0;
}

void SerialQueueTransponder::send(uint8_t b)
{
    sendSerial.write(b);
    xQueueSend(sendHandle, &b, 0);
}

uint8_t SerialQueueTransponder::recv(void)
{
    uint8_t b = 0;
    xQueueReceive(recvHandle, &b, portMAX_DELAY);
    return b;
}

void SerialQueueTransponder::flush(void)
{
    sendSerial.flush();
}

void SerialQueueTransponder::flush_received(void)
{
    uint8_t b;
    while (xQueueReceive(recvHandle, &b, 0) == pdTRUE)
        ;
}

#ifdef ENABLE_ESP_NOW
Transponder::Transponder(void)
    : receiver(MYCOBOT_CHANNEL),
      mycobot(new SerialQueueTransponder(Serial2, send_queue, recv_queue))
{
}
#else
Transponder::Transponder(void)
    : mycobot(new SerialQueueTransponder(Serial2, send_queue, recv_queue))
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
        transpond, getName(), 8192, NULL, 1, NULL, core);

    uiTask.drawTitle(getName(), getVersion());
    uiTask.drawButtonA(readDumped());
    uiTask.drawButtonB();
    uiTask.drawButtonC();
#ifdef ENABLE_ESP_NOW
    uiTask.drawEspNowStatus(receiver.begin());
#endif
    mycobot.setLED(r, g, b);

    pinMode(15, OUTPUT);
    // for Suction Pump
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    pinMode(2, OUTPUT);

    return result == pdPASS;
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
        uiTask.clearJointAngles();
    }
    return dumped;
}

void Transponder::getJointAngles(void)
{
    if (mycobot.isFrameState(STATE_NONE) || mycobot.isFrameState(STATE_FOOTER))
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
    mycobot.setFreeMove();
}

void Transponder::updateUI(byte b)
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
        uiTask.drawDataFrame(mycobot.getFrameState(),
                             mycobot.getParsePosition(),
                             b);
    }
}

void Transponder::parseSetBasic(void) {
    uint8_t b = 0;
    uint8_t pinNo = 0;
    uint8_t pinData = 0;

    if (xQueueReceive(send_queue, &pinNo, 0) != pdTRUE) {
        return;
    }
    mycobot.parse(pinNo);
    if (dumped)
    {
        updateUI(pinNo);
    }
    if (!mycobot.isFrameState(STATE_DATA))
    {
        return;
    }

    if (xQueueReceive(send_queue, &pinData, 0) != pdTRUE)
    {
        return;
    }
    mycobot.parse(pinData);
    if (dumped)
    {
        updateUI(pinData);
    }
    if (!mycobot.isFrameState(STATE_DATA))
    {
        return;
    }

    if (xQueueReceive(send_queue, &b, 0) != pdTRUE)
    {
        return;
    }
    mycobot.parse(b);
    if (dumped)
    {
        updateUI(b);
    }
    if (mycobot.isFrameState(STATE_FOOTER))
    {
        digitalWrite(pinNo, pinData);
    }
}

void Transponder::update(void)
{
    uint8_t b = 0;
    while (xQueueReceive(send_queue, &b, 0) == pdTRUE)
    {
        mycobot.parse(b);
        if (dumped)
        {
            updateUI(b);
        }
        if (b == SET_BASIC_OUTPUT)
        {
            parseSetBasic();
        }
    }
}
