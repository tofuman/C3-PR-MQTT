#include "robo_brain.h"
static RoboBrain *brain;

void setup()
{
  brain = new RoboBrain();
}

void loop()
{
  brain->tick();
}
