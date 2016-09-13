#include <Exode.h>
#include <exode_set.h>

void setup() {
  _exode.setup();
  load_exode_instructions(0);
}

void loop() {
  // Enjoy it !
 _exode.run();
}
