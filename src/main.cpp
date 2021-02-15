#include "Transponder.h"

Transponder transponder;

void setup(void)
{
  M5.begin(true, false, true, false); // LCD, SD, Serial, I2C
  M5.Power.begin();
  dacWrite(25, 0); // disable mic

  transponder.begin();
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
