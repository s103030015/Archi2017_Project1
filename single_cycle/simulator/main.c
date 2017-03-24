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
  
  int pcAfter=register[NextPCReg]+4;
  int sum, diff, tmp, value;
  unsigned int rs, rt, imm;
  
  switch(instr->opCode)
  {
    case ADD:
      sum=register[instr->rs]+instr->extra;
      register[instr->rt]=sum;
      break;
    case ADDU:
      register[instr->rd]=register[instr->rs]+register[instr->rt];
      break;
    case SUB:
      diff=register[instr->rs]-register[instr->rt];
      register[instr->rd]=diff;
      break;
    case AND:
      register[instr->rd]=register[instr->rs]+register[instr->rt];
      break;
    case OR:
      register[instr->rd]=register[instr->rs] | register[instr->rt];
      break;
    case XOR:
      register[instr->rd]=register[instr->rs] ^ register[instr->rt];
    case NOR:
      register[instr->rd]=~(register[instr->rs] | register[instr->rt]);
      break;
    case NAND:
      register[instr->rd]=~(register[instr->rs]+register[instr->rt]);
      break;
    case SLT:
      if(register[instr->rs] < register[instr->rt]) register[instr->rd]=1;
      else register[instr->rd]=0;
      break;
    case SLL:
      register[instr->rd]=register[instr->rt] << instr->extra;
      break;
    case SRL:
      tmp=register[instr->rt];
      tmp >>= instr->extra;
      register[instr->rd]=tmp;
      break;
    case SRA:
      register[instr->rd]=register[instr->rt] >> instr->extra;
      break;
    case JR:
      pcAfter=register[instr->rs];
      break;
    case MULT:
      Mult(register[instr->rs], register[instr->rt], TRUE, &register[HiReg], &register[LoReg]);
      break;
    case MULTU:
      Mult(register[instr->rs], register[instr->rt], FALSE, &register[HiReg], &register[LoReg]);
      break;
    case MFHI:
      register[instr->rd]=register[HiReg];
      break;
    case MFLO:
      register[instr->rd]=register[LoReg];
      break;
    case ADDI:
      sum=register[instr->rs]+instr->extra;
      register[instr->rt]=sum;
      break;
    case ADDIU:
      register[instr->rt]=register[instr->rs]+instr->extra;
      break;
    case LW:
      tmp=register[instr->rs]+instr->extra;
      if(tmp & 0x3)
      {
        RaiseException (AddressErrorException, tmp);
        return;
      }
      if(!ReadMem(tmp, 4, &value)) return;
      nextLoadReg = instr->rt;
      nextLoadValue = value;
      break;
    case LH:
    case LHU:
      tmp=register[instr->rs]+instr->extra;
      if(tmp & 0x1)
      {
        RaiseException(AddressErrorException, tmp);
        return;
      }
      if(!ReadMem(tmp, 2, &value)) return;
      if((value & 0x8000) && (instr->opCode == LH)) value |= 0xffff0000;
      else value &= 0xffff;
      nextLoadReg=instr->rt;
      nextLoadValue=value;
      break;
    case LB:
    case LBU:
      tmp=register[instr->rs]+instr->extra;
      
      
      
      
      
