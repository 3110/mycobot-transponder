#ifndef UI_TASK_H_
#define UI_TASK_H_

#include <M5Stack.h>
#include "MyCobot.h"

typedef uint32_t coord_t;
typedef uint16_t length_t;
typedef uint32_t color_t;
typedef uint8_t font_size_t;
typedef uint8_t font_datum_t;

class UITask;

class UITitle
{
public:
    static const coord_t X_POS = 320;
    static const coord_t Y_POS = 0;
    static const length_t WIDTH = 320;
    static const length_t HEIGHT = 28;

    static const color_t BG_COLOR = TFT_BLUE;

    static const color_t TITLE_COLOR = TFT_WHITE;
    static const font_size_t TITLE_FONT_SIZE = 4;
    static const font_datum_t TITLE_DATUM = MC_DATUM;
    static const coord_t TITLE_X_POS = WIDTH / 2;
    static const coord_t TITLE_Y_POS = HEIGHT / 2 + 1;

    static const color_t VERSION_COLOR = TFT_WHITE;
    static const font_size_t VERSION_FONT_SIZE = 2;
    static const font_datum_t VERSION_DATUM = TL_DATUM;
    static const coord_t VERSION_X_POS = 5;
    static const coord_t VERSION_Y_POS = 10;

    UITitle(void);
    virtual ~UITitle(void);

    void draw(TFT_eSprite &sprite,
              const char *const title, const char *const version);
};

class UIStatus
{
public:
    static const coord_t RADIUS = 10;

    UIStatus(coord_t x, coord_t y, coord_t radius,
             color_t onColor, color_t offColor);
    UIStatus(coord_t x, coord_t y, color_t onColor, color_t offColor);
    virtual ~UIStatus(void);

    bool begin(void);
    virtual void draw(TFT_eSprite &sprite, bool isOn);

private:
    const coord_t x;
    const coord_t y;
    const coord_t radius;
    const color_t onColor;
    const color_t offColor;
};

class UIRecvStatus : public UIStatus
{
public:
    static const coord_t X_POS = 305;
    static const coord_t Y_POS = 14;
    static const color_t ON_COLOR = TFT_RED;
    static const color_t OFF_COLOR = TFT_BLACK;

    UIRecvStatus();
    virtual ~UIRecvStatus();
};

class UISendStatus : public UIStatus
{
public:
    static const coord_t X_POS = 280;
    static const coord_t Y_POS = 14;
    static const color_t ON_COLOR = TFT_GREEN;
    static const color_t OFF_COLOR = TFT_BLACK;

    UISendStatus();
    virtual ~UISendStatus();
};

#ifdef ENABLE_ESP_NOW
class UIEspNowStatus : public UIStatus
{
public:
    static const coord_t X_POS = 255;
    static const coord_t Y_POS = 14;
    static const color_t ON_COLOR = TFT_YELLOW;
    static const color_t OFF_COLOR = TFT_BLACK;

    UIEspNowStatus();
    virtual ~UIEspNowStatus();
};
#endif

class UICommandName
{
public:
    const coord_t X_POS = 0;
    const coord_t Y_POS = 50;
    const length_t WIDTH = 320;
    const length_t HEIGHT = 26 * 2;

    const font_size_t FONT_SIZE = 4;
    const font_datum_t TEXT_DATUM = TL_DATUM;
    const color_t BG_COLOR = TFT_BLACK;
    const color_t TEXT_COLOR = TFT_WHITE;

    const char *const COMMAND_LABEL = "Send:";
    const coord_t COMMAND_LABEL_X_POS = 0;
    const coord_t COMMAND_LABEL_Y_POS = 0;

    const coord_t COMMAND_COUNTER_X_POS = 80;
    const coord_t COMMAND_COUNTER_Y_POS = 0;

    const coord_t COMMAND_NAME_X_POS = 16;
    const coord_t COMMAND_NAME_Y_POS = 26;
    const length_t COMMAND_NAME_WIDTH = 320 - COMMAND_NAME_X_POS;
    const length_t COMMAND_NAME_HEIGHT = 26;

    UICommandName(void);
    virtual ~UICommandName(void);

    virtual void draw(TFT_eSprite &sprite, int cmd, uint16_t counter);
    void clear(TFT_eSprite &sprite);
};

class UIJointAngles
{
public:
    static const coord_t X_POS = 5;
    static const coord_t Y_POS = 155;
    static const length_t WIDTH = 320 - X_POS * 2;
    static const length_t HEIGHT = 40;

    static const color_t TEXT_COLOR = TFT_WHITE;
    static const color_t BG_COLOR = TFT_BLACK;
    static const font_size_t FONT_SIZE = 2;
    static const font_datum_t DATUM = TL_DATUM;

    static const int16_t N_LABELS_IN_A_ROW = 3;
    static const int16_t LABEL_WIDTH = 115;
    static const int16_t LABEL_HEIGHT = 20;
    static const int16_t VALUE_INDENT = 30;

    UIJointAngles(void);
    virtual ~UIJointAngles(void);

    virtual void draw(TFT_eSprite &sprite, float *angles, size_t n_angles);
};

class UIDataFrame
{
public:
    static const coord_t X_POS = 6;
    static const coord_t Y_POS = 110;
    static const length_t WIDTH = 320 - X_POS * 2;
    static const length_t HEIGHT = 40;

    static const color_t TEXT_COLOR = TFT_WHITE;
    static const color_t BG_COLOR = TFT_BLACK;
    static const font_size_t FONT_SIZE = 2;
    static const font_datum_t DATUM = MC_DATUM;

    static const uint8_t N_BYTES_IN_A_ROW = 11;
    static const length_t BYTE_LENGTH = WIDTH / N_BYTES_IN_A_ROW;
    static const length_t BYTE_HEIGHT = HEIGHT / 2;

    static const color_t COMMAND_BG_COLOR = TFT_BLUE;

    UIDataFrame(void);
    virtual ~UIDataFrame(void);

    virtual void draw(TFT_eSprite &sprite, MyCobot::FrameState state,
                      uint8_t pos, int b);
    virtual void clear(TFT_eSprite &sprite);
};

class UIHardButton
{
public:
    static const coord_t Y_POS = 204;
    static const length_t WIDTH = 16 * 5;
    static const length_t HEIGHT = 26;
    static const color_t TEXT_COLOR = TFT_WHITE;
    static const color_t BG_COLOR = TFT_BLACK;
    static const font_size_t FONT_SIZE = 4;
    static const font_datum_t DATUM = MC_DATUM;

    UIHardButton(void);
    virtual ~UIHardButton(void);

    virtual const char *const getLabel(void) const = 0;
    virtual coord_t getX(void) const = 0;

    virtual void draw(TFT_eSprite &sprite);
};

class UIHardButtonA : public UIHardButton
{
public:
    UIHardButtonA(void);
    virtual ~UIHardButtonA(void);

    virtual const char *const getLabel(void) const;
    virtual coord_t getX(void) const;
    virtual void setDumped(bool dumped);

private:
    static const coord_t X_POS = 65 - WIDTH / 2;
    bool dumped;
};

class UIHardButtonB : public UIHardButton
{
public:
    UIHardButtonB(void);
    virtual ~UIHardButtonB(void);

    virtual const char *const getLabel(void) const;
    virtual coord_t getX(void) const;

private:
    static const coord_t X_POS = 160 - WIDTH / 2;
};

class UIHardButtonC : public UIHardButton
{
public:
    UIHardButtonC(void);
    virtual ~UIHardButtonC(void);

    virtual const char *const getLabel(void) const;
    virtual coord_t getX(void) const;

private:
    static const coord_t X_POS = 255 - WIDTH / 2;
};

class UITask
{
public:
    UITask(void);
    virtual ~UITask(void);

    bool begin(const BaseType_t coreId);
    virtual void drawTitle(const char *const title, const char *const version);
    virtual void drawRecvStatus(bool isOn);
    virtual void drawSendStatus(bool isOn);
#ifdef ENABLE_ESP_NOW
    virtual void drawEspNowStatus(bool isOn);
#endif
    virtual void drawCommandName(int cmd, uint16_t counter);
    virtual void clearCommandName(void);
    virtual void drawJointAngles(float *angles, size_t n_angles);
    virtual void drawDataFrame(MyCobot::FrameState state, uint8_t pos, int b);
    virtual void clearDataFrame(void);
    virtual void drawButtonA(bool dumped);
    virtual void drawButtonB(void);
    virtual void drawButtonC(void);

    TFT_eSprite &getSprite(void);

private:
    static const char *const NAME;

    TFT_eSprite sprite;

    UITitle title;
    UIRecvStatus recvStatus;
    UISendStatus sendStatus;
#ifdef ENABLE_ESP_NOW
    UIEspNowStatus espNowStatus;
#endif
    UICommandName commandName;
    UIJointAngles jointAngles;
    UIDataFrame dataFrame;
    UIHardButtonA buttonA;
    UIHardButtonB buttonB;
    UIHardButtonC buttonC;
};
#endif