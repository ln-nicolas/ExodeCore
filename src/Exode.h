/*
  Exode.h -

  ExodeArduinoCore is an interface between Arduino Board and
  an other devices (computer, Rasberry Pi, phone..).

  From theses devices you send instructions to the
  Arduino board, and Exode will interpret your
  instructions, and execute them on the board.

  Exode use a specific protocol, see the link below
  http://sne3ks.github.io/ExodeDoc/#how-it-works

  Created by Lenselle Nicolas, december, 2015
  APACHE License 2.0

*/

#ifndef Exode_h
#define Exode_h

#include "utility/ThreadController.h"
#include "utility/ExodeThread.h"
#include "instructionSet.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

class Exode{

protected:

  byte len_input; // incomming instruction's length (byte array)
	int  cursor_input;
  byte *_INPUT; // byte array to hold the current instruction

  // Multithread
  ThreadController* controller;
  Thread* listener;

  InstructionSet* instructionSet[10];

public:

  Exode();

  void setup();

  void listen();
  void execute(byte *bytecode);

  void addInstructionSet(char id, InstructionSet* set);

  void sendUnsignedInt(byte key, int value);

  void addThread(ExodeThread *th);
  void addThread(Thread *th);

  void deleteThread(ExodeThread *th);
  void deleteThread(int id);


  void run();
};

#endif

extern Exode _exode;
