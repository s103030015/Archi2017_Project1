#include "opcode.h"

class Instrction {
  public:
    void Decode();
    unsigned int value;
    char opCode;
    char rs,rt,rd;
    int extra;
};

static int TypeToReg(RegType reg, Instruction *instr)
{
  switch(reg){
    case RS:
      return instr->rs;
