#ifndef TRANSPONDER_H_
#define TRANSPONDER_H_

#include <Preferences.h>
#include "m5stack/UITask.h"
#ifdef ENABLE_ESP_NOW
#include "m5stack/EspNowReceiver.h"
#endif

class Transponder
{
public:
    static const char *const getVersion(void);

    Transponder(void);
    virtual ~Transponder(void);

    virtual void begin(byte r, byte g, byte b);
    virtual void send(void);
    virtual void recv(void);

    virtual bool toggleDumped(void);
    virtual void drawJointAngles(void);
    virtual void setFreeMove(void);

protected:
    virtual bool readDumped(void);
    virtual void drawSendStatus(bool isOn);
    virtual void drawRecvStatus(bool isOn);
    virtual void sendCommandByte(int b);

private:
    static const char *const VERSION;
    static const char *const NAME;
    static const char *const PREFS_NAMESPACE;
    static const char *const PREFS_IS_DUMPED;
    static const int SERIAL_BAUD_RATE = 115200;   // PC -> Basic
    static const int SERIAL2_BAUD_RATE = 1000000; // Basic -> ATOM

    Preferences prefs;
    MyCobot mycobot;
    UITask uiTask;
#ifdef ENABLE_ESP_NOW
    EspNowReceiver receiver;
#endif
    bool dumped;
};

#endif