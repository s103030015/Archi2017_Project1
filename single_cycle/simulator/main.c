#include "instruc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    case RT:
      return instr->rt;
    case RD:
      return instr->rd;
    case EXTRA:
      return instr->extra;
    default:
      return -1;
   }
}

void Instruction(Instructioin *instr)
{
  int byte, raw, nextLoadReg, nextLoadValue;
  nextLoadReg=0;
  nextLoadValue=0;
  
  if(ReadMem(registers[PCReg], 4, &raw)==0) return;
  instr->value = raw;
  instr->Decode();
  
  struct OpString 
