#include "instructions.h"
#include "Exode.h"

// o <-> operande

void _pinMode(byte o[]){
  //int pin = o[1];
  //int mode = o[2];

  _pinMod[o[1]] = o[2];
  pinMode(o[1],o[2]);
}

void _digitalWrite(byte o[]){
  int pin = o[1];
  int level = o[2];
  _digLvl[pin] = level;
  digitalWrite(pin,level);
}

void _digitalRead(byte o[]){
  int pin = o[1];
  byte key = o[2];

  int val = 0;
  if (digitalRead(pin) == HIGH) val = 1;

  _exode.sendUnsignedInt(key, val);
}

void _digitalSwitch(byte o[]){
  int pin = o[1];
  int lvl = (_digLvl[pin]+1)%2;
  _digLvl[pin] = lvl;
  digitalWrite(pin,_digLvl[pin]);
}

void _analogWrite(byte o[]){
  int pin = o[1];
  int value = o[2];
  analogWrite(pin, value);
}

void _analogRead(byte o[]){
  int pin = o[1];
  int key = o[2];
  _exode.sendUnsignedInt(key, analogRead(pin));
}


void _addPPM(byte o[]){
  int pin = o[1];
  byte key_thread = o[2];
  int us = *((unsigned long*)(o+3));
  int id = _exode.addPPM(pin, us);
  _exode.sendUnsignedInt(key_thread, id);
}

void _removePPM(byte o[]){
  int id = o[1];
  _exode.removePPM(id);
}

void _writePPM(byte o[]){
  int id = o[1];
  int us = *((unsigned long*)(o+2));
  _exode.writePPM(id, us);
}

void _pulse(byte o[]){

  int pin = o[1];
  int us = *((unsigned long*)(o+2));
  digitalWrite(pin,HIGH);
  delayMicroseconds(us);
  digitalWrite(pin,LOW);


}

void _pulseIn(byte o[]){

  int pin = o[1];
  int key = o[2];
  int duration = pulseIn(pin, HIGH);

  _exode.sendUnsignedInt(key, duration);

}

void _executeThread(byte o[]){

  int block_len = o[1];

  int inst_len = 0;
  byte *inst;
  int  inst_cursor = 0;


  for( int i= 1; i <= block_len; i++){

    byte b = o[i+1];

    if(inst_len == 0){ inst_len = b; inst = (byte*)malloc(inst_len); }
    else{
      inst[inst_cursor] = b;
      inst_cursor++;
    }

    if(inst_len != 0 && inst_len == inst_cursor){
      inst_cursor = 0;
      inst_len = 0;
      void (*foo)(byte o[]) = _FUNCTIONS[ (int) inst[0]];
      foo(inst);
      free(inst);
    }

  }

}

void _initThread(byte o[]){

  int key = o[1];
  int period = *((unsigned long*)(o+2));
  int len = o[6];

  byte *op = (byte*)malloc(len+1);

  ExodeThread *ex_th = new ExodeThread(op, &_executeThread, period);
  _exode.addThread(ex_th);
  _exode.sendUnsignedInt(key, ex_th->getId());

  // i don't why, but i've to copy the op here, else it doesn't work
  for(int i= 0; i <= len; i++) op[1+i] = o[6+i];

}

void _deleteThread(byte o[]){
  int ID = *((unsigned long*)(o+1));
  _exode.deleteThread(ID);
}

void _reset(byte o[]){
  //not defined yet
}

void _checkExode(byte o[]){
  // Call this function to check if Exode is installed
  // on the board (autosearch board)
  _exode.sendUnsignedInt(202, 404);
}

void (*_FUNCTIONS[])(byte o[]) = {
  &_pinMode,          // 0

  &_digitalWrite,     // 1
  &_digitalRead,      // 2
  &_digitalSwitch,    // 3

  &_analogWrite,      // 4
  &_analogRead,       // 5

  &_addPPM,           // 6
  &_removePPM,        // 7
  &_writePPM,         // 8

  &_pulse,            // 9
  &_pulseIn,          // 10

  &_executeThread,   // 11
  &_initThread,      // 12
  &_deleteThread,    // 13

  &_reset,           // 14
  &_checkExode       // 15
};
