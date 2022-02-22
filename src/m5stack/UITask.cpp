#include "UITask.h"

const char *const UITask::NAME = "UI Task";
const char *const UICommandName::COMMAND_LABEL = "Send:";

const size_t JOINT_NAME_LEN = 4;
const char JOINT_NAME_LABELS[][JOINT_NAME_LEN] = {
    "J1:", "J2:", "J3:", "J4:", "J5:", "J6:",
};

UITitle::UITitle(void) {
}

UITitle::~UITitle(void) {
}

void UITitle::draw(TFT_eSprite &sprite, const char *const title,
                   const char *const version) {
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);

    sprite.setTextColor(VERSION_COLOR);
    sprite.setTextDatum(VERSION_DATUM);
    sprite.drawString(version, VERSION_X_POS, VERSION_Y_POS, VERSION_FONT_SIZE);

    sprite.setTextColor(TITLE_COLOR);
    sprite.setTextDatum(TITLE_DATUM);
    sprite.drawString(title, TITLE_X_POS, TITLE_Y_POS, TITLE_FONT_SIZE);

    sprite.pushSprite(0, 0);
    sprite.deleteSprite();
}

UIStatus::UIStatus(coord_t x, coord_t y, coord_t radius, color_t onColor,
                   color_t offColor)
    : x(x), y(y), radius(radius), onColor(onColor), offColor(offColor) {
}

UIStatus::UIStatus(coord_t x, coord_t y, color_t onColor, color_t offColor)
    : UIStatus(x, y, RADIUS, onColor, offColor) {
}

UIStatus::~UIStatus(void) {
}

bool UIStatus::begin(void) {
    return true;
}

void UIStatus::draw(TFT_eSprite &sprite, bool isOn) {
    M5.Lcd.fillCircle(x, y, radius, isOn ? onColor : offColor);
}

#ifdef ENABLE_ESP_NOW
UIEspNowStatus::UIEspNowStatus() : UIStatus(X_POS, Y_POS, ON_COLOR, OFF_COLOR) {
}

UIEspNowStatus::~UIEspNowStatus() {
}
#endif

UICommandName::UICommandName(void) {
}

UICommandName::~UICommandName(void) {
}

void UICommandName::draw(TFT_eSprite &sprite, const char *const name,
                         uint16_t counter) {
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);
    sprite.setTextColor(TEXT_COLOR);
    sprite.setTextDatum(TEXT_DATUM);
    sprite.drawString(COMMAND_LABEL, COMMAND_LABEL_X_POS, COMMAND_LABEL_Y_POS,
                      FONT_SIZE);
    sprite.drawString(name, COMMAND_NAME_X_POS, COMMAND_NAME_Y_POS, FONT_SIZE);
    sprite.drawNumber(counter, COMMAND_COUNTER_X_POS, COMMAND_COUNTER_Y_POS,
                      FONT_SIZE);
    sprite.pushSprite(X_POS, Y_POS);
    sprite.deleteSprite();
}

void UICommandName::clear(TFT_eSprite &sprite) {
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);
    sprite.pushSprite(X_POS, Y_POS);
    sprite.deleteSprite();
}

UIJointAngles::UIJointAngles(void) {
}

UIJointAngles::~UIJointAngles(void) {
}

void UIJointAngles::draw(TFT_eSprite &sprite, float *angles, size_t n_angles) {
    int32_t x = 0;
    int32_t y = 0;
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);
    sprite.setTextColor(TEXT_COLOR);
    sprite.setTextDatum(DATUM);
    for (int i = 0; i < n_angles; ++i) {
        x = (i % N_LABELS_IN_A_ROW) * LABEL_WIDTH;
        y = i / N_LABELS_IN_A_ROW * LABEL_HEIGHT;
        sprite.drawString(JOINT_NAME_LABELS[i], x, y, FONT_SIZE);
        sprite.drawFloat(angles[i], 2, x + VALUE_INDENT, y, FONT_SIZE);
    }
    sprite.pushSprite(X_POS, Y_POS);
    sprite.deleteSprite();
}

void UIJointAngles::clear(TFT_eSprite &sprite) {
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);
    sprite.pushSprite(X_POS, Y_POS);
    sprite.deleteSprite();
}

UIDataFrame::UIDataFrame(void) {
}

UIDataFrame::~UIDataFrame(void) {
}

void UIDataFrame::draw(TFT_eSprite &sprite, DataFrameState state, uint8_t pos,
                       int b) {
    static char s[3] = {
        '\0',
    };
    sprite.setColorDepth(8);
    sprite.createSprite(BYTE_LENGTH, BYTE_HEIGHT);
    sprite.setTextColor(TEXT_COLOR);
    sprite.setTextDatum(DATUM);
    sprite.fillSprite(state == DataFrameState::STATE_CMD ? COMMAND_BG_COLOR
                                                         : BG_COLOR);
    snprintf(s, sizeof(s), "%02X", b);
    sprite.drawString(s, BYTE_LENGTH / 2, BYTE_HEIGHT / 2, FONT_SIZE);
    sprite.pushSprite(X_POS + (pos % N_BYTES_IN_A_ROW) * BYTE_LENGTH,
                      Y_POS + pos / N_BYTES_IN_A_ROW * BYTE_HEIGHT);
    sprite.deleteSprite();
}

void UIDataFrame::clear(TFT_eSprite &sprite) {
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);
    sprite.pushSprite(X_POS, Y_POS);
    sprite.deleteSprite();
}

UIHardButton::UIHardButton(void) {
}

UIHardButton::~UIHardButton(void) {
}

void UIHardButton::draw(TFT_eSprite &sprite) {
    sprite.setColorDepth(8);
    sprite.createSprite(WIDTH, HEIGHT);
    sprite.fillSprite(BG_COLOR);
    sprite.setTextColor(TEXT_COLOR);
    sprite.setTextDatum(DATUM);
    sprite.drawString(getLabel(), WIDTH / 2, HEIGHT / 2, FONT_SIZE);
    sprite.pushSprite(getX(), Y_POS);
    sprite.deleteSprite();
}

UIHardButtonA::UIHardButtonA() : dumped(false) {
}

UIHardButtonA::~UIHardButtonA(void) {
}

const char *const UIHardButtonA::getLabel(void) const {
    return this->dumped ? "ON" : "OFF";
}

coord_t UIHardButtonA::getX(void) const {
    return X_POS;
}

void UIHardButtonA::setDumped(bool dumped) {
    this->dumped = dumped;
}

bool UIHardButtonA::isDumped(void) const {
    return dumped;
}

UIHardButtonB::UIHardButtonB(void) {
}

UIHardButtonB::~UIHardButtonB(void) {
}

const char *const UIHardButtonB::getLabel(void) const {
    return "Angle";
}

coord_t UIHardButtonB::getX(void) const {
    return X_POS;
}

UIHardButtonC::UIHardButtonC(void) {
}

UIHardButtonC::~UIHardButtonC(void) {
}

const char *const UIHardButtonC::getLabel(void) const {
    return "Free";
}

coord_t UIHardButtonC::getX(void) const {
    return X_POS;
}

UITask::UITask() : sprite(&M5.Lcd) {
}

UITask::~UITask(void) {
}

void UITask::drawTitle(const char *const title, const char *const version) {
    this->title.draw(getSprite(), title, version);
}

#ifdef ENABLE_ESP_NOW
void UITask::drawEspNowStatus(bool isOn) {
    espNowStatus.draw(getSprite(), isOn);
}
#endif

void UITask::drawCommandName(const char *const cmd, uint16_t counter) {
    commandName.draw(getSprite(), cmd, counter);
}

void UITask::clearCommandName(void) {
    commandName.clear(getSprite());
}

void UITask::drawJointAngles(float *angles, size_t n_angles) {
    jointAngles.draw(getSprite(), angles, n_angles);
}

void UITask::clearJointAngles(void) {
    jointAngles.clear(getSprite());
}

void UITask::drawDataFrame(DataFrameState state, uint8_t pos, int b) {
    dataFrame.draw(getSprite(), state, pos, b);
}

void UITask::clearDataFrame(void) {
    dataFrame.clear(getSprite());
}

void UITask::drawButtonA(bool dumped) {
    buttonA.setDumped(dumped);
    buttonA.draw(getSprite());
}

void UITask::drawButtonB(void) {
    buttonB.draw(getSprite());
}

void UITask::drawButtonC(void) {
    buttonC.draw(getSprite());
}

bool UITask::isDumped(void) const {
    return buttonA.isDumped();
}

void UITask::setDumped(bool dumped) {
    buttonA.setDumped(dumped);
}

TFT_eSprite &UITask::getSprite(void) {
    return sprite;
}
