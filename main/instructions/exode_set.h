/*
  exode_instructions.h -

  Created by Lenselle Nicolas, december, 2015
  APACHE License 2.0
*/
#include "instructionSet.h"

#include "Servo.h"
#define  MAX_SERVO 12
Servo* servo[MAX_SERVO];

#include "lib/AccelStepper.h"
#define MAX_STEPPER 12
AccelStepper* stepper[MAX_STEPPER];

/*
 * INSTRUCTION'S ID
 */

#define PINMODE   0

#define DIGWRITE  1
#define DIGREAD   2
#define DIGSWITCH 3

#define ANAWRITE  4
#define ANAREAD   5

#define ADDSERVO  6
#define RMSERVO   7
#define WRTSERVO  8

#define PULSE     9
#define PULSEIN   10

#define EXCTHRD   11
#define INITHRD   12
#define DELTHRD   13

#define CHKEXD    14

#define ADDSTEP   15
#define ACCSTEP   16
#define SPDSTEP   17
#define MOVSTEP   18

/*
 * DEFINITIONS
 */

void _pinMode(byte o[]){
  /*
    o[1]: pin
    o[2]: mode
    o[3]: isAnalogic
  */

  if(o[3]==1)
    o[1]= analogInputToDigitalPin(o[1]);

  pinMode(o[1],o[2]);
}



void _digitalWrite(byte o[]){
  /*
    o[1]: pin
    o[2]: level
  */

  digitalWrite(o[1],o[2]);
}



void _digitalRead(byte o[]){
  /*
    o[1]: pin
    o[2]: key
  */

  int val = 0;
  if (digitalRead(o[1]) == HIGH) val = 1;
  _exode.sendUnsignedInt(o[2], val);
}



void _digitalSwitch(byte o[]){
  /*
    o[1]: pin
  */

  digitalWrite(o[1], digitalRead(o[1])^1 );
}



void _analogWrite(byte o[]){
  /*
    o[1]: pin
    o[2]: value
  */

  analogWrite(o[1], o[2]);
}



void _analogRead(byte o[]){
  /*
    o[1]: pin
    o[2]: key
  */
  _exode.sendUnsignedInt(o[2], analogRead(analogInputToDigitalPin(o[1])));
}



void _addServo(byte o[]){
  /*
    o[1]: pin
    o[2]: key
    o[3]: us
  */
  int us = *((unsigned long*)(o+3));

  char id= 0;
  for(; id < MAX_SERVO; id++){
   if( servo[id] == nullptr ){
    servo[id]= new Servo();
    servo[id]->attach(o[1]);
    servo[id]->writeMicroseconds(us);
    break;
   }
  }

  _exode.sendUnsignedInt(o[2], id);
}




void _removeServo(byte o[]){
  /*
    o[1]: id
  */
  servo[o[1]]->detach();
  free(servo[o[1]]);
  servo[o[1]]= nullptr;
}



void _writeServo(byte o[]){
  /*
    o[1]: id
    o[2]: us
  */
  int us = *((unsigned long*)(o+2));
  servo[o[1]]->writeMicroseconds(us);
}



void _pulse(byte o[]){
  /*
   o[1]: pin
   o[2]: us
  */
   int us= *((unsigned long*)(o+2));
   digitalWrite(o[1], 1);
   delay(us);
   digitalWrite(o[1], 0);
}




void _pulseIn(byte o[]){
  /*
   o[1]: pin
   o[2]: key
  */
  _exode.sendUnsignedInt(o[2], pulseIn(o[1], 1));
}




void _executeThread(byte o[]){
  /*
   o[1]: bytecode len
  */

  int stop_bit = o[1]+1;

  // place cursor on the first instruction
  for( int cursor= 2; cursor <= stop_bit;){

    //cursor+1 the position of the function's id
    _exode.execute(o+(cursor+1));

    //update the cursor position
    //      curentPos + block lenght + 1
    cursor= cursor+o[cursor]+1;
  }

}




void _initThread(byte o[]){
  /*
   o[1]: key
   o[2-5]: us (period)
   o[6]: len
  */
  byte *op = (byte*)malloc(o[6]+1);
  int   us = *((unsigned long*)(o+2));
  ExodeThread *ex_th = new ExodeThread(op, &_executeThread, us);
  _exode.addThread(ex_th);
  _exode.sendUnsignedInt(o[1], ex_th->getId());

  // i don't why, but i've to copy the op here...
  for(int i= 0; i <= o[6]; i++) op[1+i] = o[6+i];

}




void _deleteThread(byte o[]){
  /*
   o[1]: id
  */
  _exode.deleteThread(*((unsigned long*)(o+1)));
}




void _checkExode(byte o[]){
  _exode.sendUnsignedInt(202, 404);
}



void _addStepper(byte o[]){
  /*
    o[1]: key
    o[2]: type (cf AccelStepper documentation)
    o[3]: pin1
    o[4]: pin2
    o[5]: pin3
    o[6]: pin4
  */

  char id= 0;
  for(; id < MAX_STEPPER; id++){
   if( stepper[id] == nullptr ){
        stepper[id]= new AccelStepper(o[2], o[3], o[4], o[5], o[6]);
        stepper[id]->setAcceleration(50);
        stepper[id]->setMaxSpeed(100);
        break;
    }
  }

  _exode.sendUnsignedInt(o[1], id);
}



void _setStepperAcceleration(byte o[]){
  /*
    o[1]: stepper id
    o[2]: acceleration (steps.s-2)
  */
  float acc = (*((float *)(o+2)));
  stepper[o[1]]->setAcceleration(acc);
}


void _setStepperSpeed(byte o[]){
  /*
    o[1]: stepper id
    o[2]: speed * 100 (steps.s-1)
  */
  float speed = (*((float *)(o+2)));
  stepper[o[1]]->setMaxSpeed(speed);
}



void _moveStepper(byte o[]){
  /*
    o[1]: stepper id
    o[2]: relative (signed long)
  */
  long relative = (*((long*)(o+2)));
  stepper[o[1]]->moveTo(relative);
}





/*
 *  CONTROLLER
 */

 void stepper_controller(){
   for(char id=0; id < MAX_STEPPER; id++)
   if (stepper[id] != NULL)
   stepper[id]->run();
 }





/*
 *  LOADER
 */

void load_exode_instructions(char id){

  InstructionSet* exode_inst= new InstructionSet(19);

  exode_inst->set(PINMODE,   &_pinMode);
  exode_inst->set(DIGWRITE,  &_digitalWrite);
  exode_inst->set(DIGREAD,   &_digitalRead);
  exode_inst->set(DIGSWITCH, &_digitalSwitch);
  exode_inst->set(ANAWRITE,  &_analogWrite);
  exode_inst->set(ANAREAD,   &_analogRead);
  exode_inst->set(ADDSERVO,  &_addServo);
  exode_inst->set(RMSERVO,   &_removeServo);
  exode_inst->set(WRTSERVO,  &_writeServo);
  exode_inst->set(PULSE,     &_pulse);
  exode_inst->set(PULSEIN,   &_pulseIn);
  exode_inst->set(EXCTHRD,   &_executeThread);
  exode_inst->set(INITHRD,   &_initThread);
  exode_inst->set(DELTHRD,   &_deleteThread);
  exode_inst->set(CHKEXD,    &_checkExode);
  exode_inst->set(ADDSTEP,   &_addStepper);
  exode_inst->set(ACCSTEP,   &_setStepperAcceleration);
  exode_inst->set(SPDSTEP,   &_setStepperSpeed);
  exode_inst->set(MOVSTEP,   &_moveStepper);

  _exode.addInstructionSet(id, exode_inst);

  Thread* stepper_thread= new Thread();
  stepper_thread->setInterval(1);
  stepper_thread->onRun(stepper_controller);

  _exode.addThread(stepper_thread);

}
