#include "InstructionSet.h"

InstructionSet::InstructionSet(int len){
  // Initialisation of the instruction's set
  _len= len;
  _SET= (_inst*) malloc(len*sizeof(_inst));
}

void InstructionSet::set(char id, _inst foo){
  this->_SET[id]= foo;
}

void InstructionSet::exec(char id, byte o[]){
  if (this->_SET[id] != NULL)this->_SET[id](o);
}
