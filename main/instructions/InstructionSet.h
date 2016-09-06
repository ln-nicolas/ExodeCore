/*
  InstructionSet.h -

  A set of Instructions

  Created by Lenselle Nicolas, december, 2015
  APACHE License 2.0
*/

#ifndef InstructionSet_h
#define InstructionSet_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

// _inst : a pointer to a function with an
// byte array as argument
typedef void (*_inst)(byte*);

class InstructionSet{

  protected:
    _inst* _SET;
    int    _len;

  public:

    InstructionSet(int len);

    void set(char id, _inst foo);
    void exec(char id, byte o[]);
};

#endif
