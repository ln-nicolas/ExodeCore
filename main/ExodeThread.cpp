#include "ExodeThread.h"


ExodeThread::ExodeThread(byte o[], void (*callback)(byte o[]), unsigned long _interval):Thread(){

  op = o;

  onRun(callback);

  enabled = true;
  _cached_next_run = 0;
  last_run = 0;

  ThreadID = (int)this;
  #ifdef USE_THREAD_NAMES
    ThreadName = "Thread ";
    ThreadName = ThreadName + ThreadID;
  #endif

  setInterval(_interval);

};

void ExodeThread::onRun(void (*callback)(byte o[])){
  _onRun = callback;
}

void ExodeThread::run(){
  if(_onRun != NULL)
    _onRun(op);

  runned();
}

int ExodeThread::getId(){
  return ThreadID;
}
