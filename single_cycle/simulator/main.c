#include "define.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE *error_dump;
int error_cycle[4];
int data_i[256], data_d[256], reg[32];
int *i_code, *d_code;
unsigned int PC;
static int op, rs, rt, rd, shamt, funct, imm, addr;
int type;

void error_detect(int cycle)
{
    if(error_cycle[0]) fprintf(error_dump,"In cycle %d: Write $0 Error\n",cycle);
    if(error_cycle[1]) fprintf(error_dump,"In cycle %d: Number Overflow\n",cycle);
    if(error_cycle[2]) fprintf(error_dump,"In cycle %d: Address Overflow\n",cycle);
    if(error_cycle[3]) fprintf(error_dump,"In cycle %d: Misalignment Error\n",cycle);

    memset(error_cycle, 0, sizeof(int)*4);
}

static void Mult(int a, int b, bool sign, int *hiPtr, int *loPtr)
{
    bool negative;
    unsigned int bLo, bHi, lo, hi;
    int i;
    
    if ((a == 0) || (b == 0)) 
    {
	    *hiPtr = *loPtr = 0;
	    return;
    }
    
    negative = FALSE;
    
    if (sign) {
	    if (a<0) {
	        negative = !negative;
	        a = -a;
	    }
	    if (b<0) {
	        negative = !negative;
	     b = -b;
	    }
    }

    bLo=b;
    bHi=0;
    lo=0;
    hi=0;
    for (i=0; i<32; i++) {
	    if (a & 1) {
	        lo += bLo;
	        if (lo < bLo) hi += 1;
	        hi += bHi;
	        if ((a & 0xfffffffe) == 0) break;
	    }
	    bHi <<= 1;
	    if (bLo & 0x80000000) bHi |= 1;
	    bLo <<= 1;
	    a >>= 1;
    }
    
    if (negative) {
	    hi = ~hi;
	    lo = ~lo;
	    lo++;
	    if (lo == 0) hi++;
    }
    
    *hiPtr = (int) hi;
    *loPtr = (int) lo;
}

void decode(int inst)
{
  int sum, diff, tmp;
  
  op=(inst>>26) & 0x3f;
  if(op == 0x3f) exit(0);
  PC = PC+4;
  
  if(op == 0x00) type=RFORM;
  else if(op == 0x02 || op == 0x03) type=JFORM;
  else type=IFORM;
  
  switch(type)
  {
    case 1:
      funct=inst & 0x3f;
      shamt=(inst>>6) & 0x1f;
      rd=(inst>>11) & 0x1f;
      rt=(inst>>16) & 0x1f;
      rs=(inst>>21) & 0x1f;
      
      if(rd == 0 && funct!=JR) 
      {
        if((funct==SLL) && shamt==0) error_detect(0);
        else error_detect(1);
      }
       else
       {
         switch(funct)
         {
             case ADD:
              sum=reg[rs]+reg[rt];
              if (!((reg[rs] ^ reg[rt]) & SIGN_BIT) && ((reg[rs] ^ sum) & SIGN_BIT)) error_detect(2);
              reg[rd]=sum;
              break;
             case ADDU:
              reg[rd]=reg[rs]+reg[rt];
              break;
             case SUB:
              diff=reg[rs]-register[rt];
              if (((reg[rs] ^ reg[rt]) & SIGN_BIT) && ((reg[rs] ^ diff) & SIGN_BIT)) error_detect(2);
              reg[rd]=diff;
              break;
             case AND:
              reg[rd]=reg[rs]+reg[rt];
              break;
             case OR:
              reg[rd]=reg[rs] | reg[rt];
              break;
             case XOR:
              reg[rd]=reg[rs] ^ reg[rt];
             case NOR:
              reg[rd]=~(reg[rs] | reg[rt]);
              break;
             case NAND:
              reg[rd]=~(reg[rs]+reg[rt]);
              break;
             case SLT:
              if(reg[rs] < reg[rt]) reg[rd]=1;
              else reg[rd]=0;
              break;
             case SLL:
              reg[rd]=reg[rt] << shamt;
              break;
             case SRL:
              tmp=reg[rt];
              tmp >>= shamt;
              reg[rd]=tmp;
              break;
             case SRA:
              reg[rd]=reg[rt] >> shamt;
              break;
             case JR:
              PC=reg[rs];
              break;
            case MULT:
              Mult(reg[rs], reg[rt], TRUE, &reg[HiReg], &reg[LoReg]);
              break;
            case MULTU:
              Mult(reg[rs], reg[rt], FALSE, &reg[HiReg], &reg[LoReg]);
              break;
            case MFHI:
              reg[rd]=reg[HiReg];
              break;
            case MFLO:
              reg[rd]=reg[LoReg];
              break;
         }
       }
    case 2:
      imm = (inst & 0xffff) << 16) >>16;
      rt=(inst>>16) & 0x1f;
      rs=(inst>>21) & 0x1f;
      if(rt==0 && op!=SW && op!=SH && op!=SB && op!=BEQ && op!=BNE) 
      {
        error_detect(1);
        if(flag) exit(1);
      }
      else
      {
        switch(op)
        {
            case ADDI:
              sum=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              reg[rt]=sum;
              break;
            case ADDIU:
              reg[rt]=reg[rs]+imm;
              break;
            case LW:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x3) error_detect(3);
              if(tmp%4 !=0) error_detect(4);
              if(flag) exit(1);
              reg[rt]=data_d[tmp/4];
              break;
            case LH:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(tmp%2 !=0) error_detect(4);
              if(flag) exit(1);
              if(tmp%4 == 0) reg[rt]=data_d[tmp/4] >> 16;
              else reg[rt]=data_d[tmp/4] << 16 >> 16;
              break;
            case LHU:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(tmp%2 !=0) error_detect(4);
              if(flag) exit(1);
              if(tmp%4 == 0) reg[rt]=data_d[tmp/4] >> 16 & 0x0000ffff;
              else reg[rt]=data_d[tmp/4] << 16 >> 16 & 0x0000ffff;
              break;
            case LB:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(flag) exit(1);
              if(tmp%4 == 0) reg[rt]=data_d[tmp/4] >> 24;
              else if(tmp%4 == 1) reg[rt]=data_d[tmp/4] << 8 >> 24;
              else if(tmp%4 == 2) reg[rt]=data_d[tmp/4] << 16 >> 24;
              else reg[rt]=data_d[tmp/4] << 24 >> 24;
              break;
            case LBU:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(flag) exit(1);
              if(tmp%4 == 0) reg[rt]=data_d[tmp/4] >> 24 & 0x0000ffff;
              else if(tmp%4 == 1) reg[rt]=data_d[tmp/4] << 8 >> 24 & 0x0000ffff;
              else if(tmp%4 == 2) reg[rt]=data_d[tmp/4] << 16 >> 24 & 0x0000ffff;
              else reg[rt]=data_d[tmp/4] << 24 >> 24 & 0x0000ffff;
              break;
            case SW:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(tmp%2 !=0) error_detect(4);
              if(flag) exit(1);
              data_d[tmp/4] = reg[rt];
              break;
            case SH:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(tmp%2 !=0) error_detect(4);
              if(flag) exit(1);
              if(tmp%4 == 0)
              {
                data_d[tmp/4] &= 0x0000ffff;
                data_d[tmp/4] |= ((reg[rt] & 0x0000ffff)<< 16);
                }
              else
              {
                data_d[tmp/4] &= 0xffff0000;
                data_d[tmp/4] |= (reg[rt] & 0x0000ffff);
               }
              break;
            case SB:
              tmp=reg[rs]+imm;
              if (!((reg[rs] ^ imm) & SIGN_BIT) && ((imm ^ sum) & SIGN_BIT)) error_detect(2);
              if(tmp & 0x1) error_detect(3);
              if(tmp%2 !=0) error_detect(4);
              if(flag) exit(1);
              if(tmp%4 == 0)
                {
                  data_d[tmp/4] &= 0x0000ffff;
                  data_d[tmp/4] |= ((reg[rt] & 0x000000ff)<< 24);
                }
              else if(tmp%4 == 1)
                {
                  data_d[tmp/4] &= 0xff0000ff;
                  data_d[tmp/4] |= ((reg[rt] & 0x000000ff)<< 16);
              }
              else if(tmp%4 == 2)
              {
                data_d[tmp/4] &= 0xffff00ff;
                data_d[tmp/4] |= ((reg[rt] & 0x000000ff)<< 16);
              }
              else
              {
                data_d[tmp/4] &= 0xffffff00;
                data_d[tmp/4] |= (reg[rt] & 0x000000ff);
               }
              break;
          case LUI:
            reg[rt]=imm << 16;
            break;
          case ANDI:
            reg[rt]=reg[rs] & (imm & 0xffff);
            break;
          case ORI:
            reg[rt]=reg[rs] | (imm & 0xffff);
            break;
          case NORI:
           reg[rt]=~(reg[rs] | (imm & 0xffff);
           break;
          case SLTI:
            if(reg[rs] < imm) reg[rt]=1;
            else reg[rt]=0;
            break;
          case BEQ: 
            if(reg[rs]==reg[rt]) PC+=WordToAdd(imm);
            break;
          case BNE:                        
            if(reg[rs]!=reg[rt]) PC+=WordToAdd(imm);
            break;
          case BGTZ:                        
            if(reg[rs]>0) PC+=WordToAdd(imm);
            break;
         }
    }
   case 3:                  
      addr=inst & 0x3fffffff;
      switch(op)
      {
        case J:                        
          PC=(PC & 0xf0000000) | WordToAdd(addr);
          break;
        case JAL:                        
          reg[R31]=PC;
          PC=(PC & 0xf0000000) | WordToAdd(addr);
          break;
       }                        
}                            
                              
int main()
{                            
  FILE *ptr, *err, *iimage, *dimage;
  int i_size, d_size, i_instr, d_instr;
  int cycle;
  int i,j;
  
  ptr=fopen("snapshot.rpt", "w");
  err=fopen("error_dump.rpt", "w");
  iimage=fopen("iimage.bin", "rb");
  dimage=fopen("dimage.bin", "rb");
  
  if((!iimage)||(!dimage))
  {
    printf("Can't find your testcase.\n");
    exit(2);
  }
  
  fseek(iimage, 0, SEEK_END);
  fseek(dimage, 0, SEEK_END);
  i_size=ftell(iimage);
  d_size=ftell(dimage);
  
  rewind(iimage);
  rewind(dimage);
  
  i_code=malloc(i_size);
  d_code=malloc(d_size);
  
  fread(i_code, 1, i_size, iimage);
  fread(d_code, 1, d_size, dimage);

  for(i=0;i<256;i++)
  {
    data_i[i]=0;
    data_d[i]=0;
  }
  for(i=0;i<32;i++) reg[i]=0;
  
  PC=i_code[0];
  i_instr=i_code[1];
  d_instr=d_code[1];
  if(PC+i_instr>1024) exit(1);
  if(d_instr>1024) exit(1);
  
  for(i=AddToWord(PC), j=2; j<(2+i_instr); i++, j++) i_data[i]=i_code[j];
  for(i=0, j=2; j<(2+d_instr); i++, j++) d_data[i]=d_code[j];
  
  cycle=0;
  while(1)
  {
    fprintf(ptr, "cycle %d\n", cycle);
    for(i=0;i<32;i++) fprintf(ptr, "$%02d: 0x%08X\n", i, reg[i]);
    fprintf(ptr, "PC: 0x%08X\n\n", PC);
    cycle++;
    
    if(PC%4!=0 || PC>1024 || PC<0) exit(1);
    decode(i_data[AddToWord]);
    if(op == 0x3f) break;
  }  
    fclose(ptr);
    fclose(err);
    return 0;
}                                          

  
      
      
      
