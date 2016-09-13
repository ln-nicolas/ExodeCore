/*
  adaServoShield_instructions.h

  Created by Lenselle Nicolas, december, 2015
  APACHE License 2.0
*/

#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver ada_PWM_ServoDriver;

/*
 * INSTRUCTION'S ID
 */

#define SETPWM 0

/*
 *  DEFINITIONS
 */


void setPWM(byte o[]){
  /*
    o[1]: n
    o[2]: us
  */
  int us = *((unsigned long*)(o+2));
  ada_PWM_ServoDriver.setPWM(o[1], 0, us);
}

/*
 *  LOADER
 */

void load_adaServoShield_instructions(char id){
  ada_PWM_ServoDriver= Adafruit_PWMServoDriver();
  ada_PWM_ServoDriver.begin();
  ada_PWM_ServoDriver.setPWMFreq(50);

  InstructionSet *ada_inst= new InstructionSet(1);
  ada_inst->set(SETPWM, &setPWM);

  _exode.addInstructionSet(id, ada_inst);
}
