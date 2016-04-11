#include "Exode.h"

// Initialisation of the single instance
Exode _exode = Exode();

// Global board's values
int _pinMod[NUM_DIGITAL_PINS+NUM_ANALOG_INPUTS];
int _digLvl[NUM_DIGITAL_PINS+NUM_ANALOG_INPUTS];
double _anaVal[NUM_DIGITAL_PINS+NUM_ANALOG_INPUTS];

// Callbacking function for multithreading
void callback_listen(){  _exode.listen(); }

Exode::Exode(){  
  inst_len = 0;
  cursor_input = 0;

  // Initialisation of the multithreading
  controller = new ThreadController();
  listener = new Thread();
  listener->onRun(callback_listen);
  listener->setInterval(1);
  controller->add(listener);

}


void Exode::execute(){
  // We callback the function asked
  void (*foo)(byte o[]) = _FUNCTIONS[ (int) _INPUT[0]];
  foo(_INPUT);
  free(_INPUT);
}


void Exode::listen(){
  /* Read the input and execute the instruction
   * the first byte reading define the next instruction's
   * number of bytes.
   */

  while ( Serial.available()>0 ){
    byte b= Serial.read();
    if(inst_len == 0){ inst_len = b; _INPUT = (byte*)malloc(inst_len); }
    else{
      _INPUT[cursor_input] = b;
      cursor_input++;
    }

    if(inst_len != 0 && inst_len == cursor_input){
      cursor_input = 0;
      inst_len = 0;
      execute();
    }

  }

}


void Exode::sendUnsignedInt(byte key, int value){
  /* Each value send to the device is coded on 5 byte,
     We don't need a "stop" byte.
  */
  // Id between 0 and 255 (1 byte )
  Serial.write(key);
  unsigned long int n = value;

  unsigned char bytes[4];
  bytes[0] = (n >> 24) & 0xFF;
  bytes[1] = (n >> 16) & 0xFF;
  bytes[2] = (n >> 8) & 0xFF;
  bytes[3] = n & 0xFF;

  Serial.write(bytes[3]);
  Serial.write(bytes[2]);
  Serial.write(bytes[1]);
  Serial.write(bytes[0]);


}

void Exode::run(){
  controller->run();
}

void Exode::addThread(ExodeThread *th){
  controller->add(th);
}

void Exode::deleteThread(ExodeThread *th){
  controller->remove(th);
}

void Exode::deleteThread(int id){
  controller->remove(id);
}

int Exode::addPPM(int pin, int us){

  // we search a free place
  for(int i=0; i < MAX_PPM; i++){
   if( ! ppmUser[i].attached() ){
    ppmUser[i].attach(pin);
    ppmUser[i].writeMicroseconds(us);
    return i;
   }
  }

  return -1;
}

void Exode::writePPM(int id, int us){
  ppmUser[id].writeMicroseconds(us);
}

void Exode::removePPM(int id){
  ppmUser[id].detach();
}
