/*
 	ExodeThread.h - An runnable object

	ExodeThread inherited from Thread
  See here : https://github.com/ivanseidel/ArduinoThread

  ExodeThread is adapted for Exode, callback function
  use a byte array to stock the operandes
*/

#ifndef ExodeThread_h
#define ExodeThread_h

#include <Thread.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

class ExodeThread : public Thread{

  protected :
  void (*_onRun)(byte o[]);

  public :

  ExodeThread(byte o[], void (*callback)(byte o[]) = NULL, unsigned long _interval = 0);

  // Associated operandes
  byte *op;
  void setOperande(byte o[]);

  int getId();

  void onRun(void (*callback)(byte o[]));
  void run();
};

#endif
