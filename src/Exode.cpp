#include "Exode.h"

Exode _exode= Exode();
void callback_listen(){  _exode.listen(); }

Exode::Exode(){
  len_input    = 0;
  cursor_input = 0;

  // Initialisation of the multithreading
  controller = new ThreadController();
  listener = new Thread();
  listener->onRun(callback_listen);

  listener->setInterval(1);
  controller->add(listener);
}

void Exode::setup(){
  Serial.begin(9600);
}

void Exode::execute(byte *_in){
  //this->instSet->exec(_in[0], _in);
  instructionSet[_in[0]]->exec(_in[1], (_in+1));
}


void Exode::listen(){
  /* Read the input and execute the instruction
   * the first byte reading define the next instruction's
   * number of bytes.
   */

  while ( Serial.available() > 0 ){
    byte b= Serial.read();

    if(len_input == 0){ 

      len_input_pointer[0] = b;
      len_input_pointer[1] = Serial.read();
      len_input_pointer[2] = Serial.read();
      len_input_pointer[3] = Serial.read();
      len_input = *((unsigned long*)(len_input_pointer));

      this->sendUnsignedInt(42, len_input);

      _INPUT = (byte*)malloc(len_input); 
    }
    else{
      _INPUT[cursor_input] = b;
      cursor_input++;
    }

    if(len_input != 0 && len_input == cursor_input){
      cursor_input = 0;
      len_input = 0;
      execute(_INPUT); free(_INPUT);
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

void Exode::addThread(Thread *th){
  controller->add(th);
}

void Exode::deleteThread(ExodeThread *th){
  free(th->op);
  controller->remove(th);
}

void Exode::deleteThread(int id){
  controller->remove(id);
}

void Exode::addInstructionSet(char id, InstructionSet* set){
    instructionSet[id]= set;
}
