/*
  Instructions.h -

  All functions implemented by Exode

  // instructions

  Created by Lenselle Nicolas, december, 2015
  // licence

*/

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


// Instructions's array

extern void (*_FUNCTIONS[])(byte o[]);
