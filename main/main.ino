#include "Exode.h"
#include "instructions/exode_set.h"
#include "instructions/adaServoShield_set.h"
#include "instructions/lib/AccelStepper.h"


AccelStepper s(AccelStepper::FULL4WIRE, 9, 10, 11, 12);

void setup() {
  _exode.setup();
  load_exode_instructions(0);
  load_adaServoShield_instructions(1);
}

void loop() {
  // Enjoy it !
 _exode.run();
}

