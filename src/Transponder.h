#ifndef TRANSPONDER_H_
#define TRANSPONDER_H_

#define TRANSPONDER_VERSION "v0.1.0"
#define TRANSPONDER_NAME "Transponder"

#include <Preferences.h>
#include "m5stack/UITask.h"
#include "CommandQueue.h"
#ifdef ENABLE_ESP_NOW
#include "m5stack/EspNowReceiver.h"
#endif

class SerialQueueTransponder : public MyCobotTransponder
{
public:
    SerialQueueTransponder(HardwareSerial &sendSerial,
                           xQueueHandle &sendHandle,
                           xQueueHandle &recvHandle);
    virtual ~SerialQueueTransponder(void);

    virtual bool is_received(void);
    virtual void send(uint8_t b);
    virtual uint8_t recv(void);
    virtual void flush(void);
    virtual void flush_received(void);

private:
    HardwareSerial &sendSerial;
    xQueueHandle &sendHandle;
    xQueueHandle &recvHandle;
};

class Transponder
{
public:
    static const byte DEFAULT_LED_R = 0xff;
    static const byte DEFAULT_LED_G = 0xff;
    static const byte DEFAULT_LED_B = 0xff;
    static const BaseType_t DEFAULT_TRANSPOND_CORE = 0;

    static const char *const getVersion(void);
    static const char *const getName(void);

    Transponder(void);
    virtual ~Transponder(void);

    virtual bool begin(BaseType_t core = DEFAULT_TRANSPOND_CORE,
                       byte r = DEFAULT_LED_R,
                       byte g = DEFAULT_LED_G,
                       byte b = DEFAULT_LED_B);

    virtual bool toggleDumped(void);
    virtual void getJointAngles(void);
    virtual void setFreeMove(void);
    virtual void update(void);

protected:
    virtual bool readDumped(void);

private:
    static const char *const VERSION;
    static const char *const NAME;
    static const char *const PREFS_NAMESPACE;
    static const char *const PREFS_IS_DUMPED;
    static const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
    static const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

    Preferences prefs;
#ifdef ENABLE_ESP_NOW
    EspNowReceiver receiver;
#endif
    MyCobot mycobot;
    UITask uiTask;
    bool dumped;
};

#endif