/*
  Exode.h -

  Exode is an interface between Arduino Board and
  an other device (computer, Rasberry Pi, phone..).

  From this device you send instructions to the
  Arduino board, and Exode will interpret your
  instructions, and execute them on the board.

  Exode use a specific protocol, see the link below.

  Created by Lenselle Nicolas, december, 2015
  APACHE License 2.0

*/

#ifndef Exode_h
#define Exode_h

#include <ThreadController.h>
#include "ExodeThread.h"
#include "instructions.h"
#include "Servo.h"


#define NB_PIN 20
#define DEBUG 0

#define MAX_PPM 8

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

class Exode{

protected:


  byte inst_len; // incomming instruction's length (byte array)
	int  cursor_input;
  byte *_INPUT; // byte array to hold the current instruction

  void execute(); // execute the incomming instruction

  // Multithread
  ThreadController* controller;
  Thread* listener;

  // ServoUser
  Servo ppmUser[MAX_PPM];

public:

  Exode();

  void listen(); // listen the serial port
  void sendUnsignedInt(byte key, int value);

  void run();

  void addThread(ExodeThread *th);
  void deleteThread(ExodeThread *th);
  void deleteThread(int id);

  int addPPM(int pin, int us); // return the servo id
  void writePPM(int id, int us);
  void removePPM(int id);

};

#endif

extern Exode _exode; // unique Exode's instance
void callback_listen();

// Global Board Values
extern int _pinMod[NB_PIN];
extern int _digLvl[NB_PIN];
extern double _anaVal[NB_PIN];
