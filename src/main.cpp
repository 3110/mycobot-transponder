#include "Transponder.h"

const byte ATOM_LED_R = 0xff;
const byte ATOM_LED_G = 0xff;
const byte ATOM_LED_B = 0xff;

Transponder transponder;

void setup(void)
{
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  transponder.begin(ATOM_LED_R, ATOM_LED_G, ATOM_LED_B);
}

void loop(void)
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    transponder.toggleDumped();
  }

  if (M5.BtnB.wasPressed())
  {
    transponder.drawJointAngles();
  }

  if (M5.BtnC.wasPressed())
  {
    transponder.setFreeMove();
  }

  transponder.send();
  transponder.recv();
}
