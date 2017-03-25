#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD 0x20
#define ADDU 0x21
#define SUB 0x22
#define AND 0x24
#define OR 0x25
#define XOR 0x26
#define NOR 0x27
#define NAND 0x28
#define SLT 0x2A
#define SLL 0x00
#define SRL 0x02
#define SRA 0x03
#define JR 0x08
#define MULT 0x18
#define MULTU 0x19
#define MFHI 0x10
#define MFLO 0x12

#define ADDI 0x08
#define ADDIU 0x09
#define LW 0x23
#define LH 0x21
#define LHU 0x25
#define LB 0x20
#define LBU 0x24
#define SW 0x2B
#define SH 0x29
#define SB 0x28
#define LUI 0x0F
#define ANDI 0x0C
#define ORI 0x0D
#define NORI 0x0E
#define SLTI 0x0A
#define BEQ 0x04
#define BNE 0x05
#define BGTZ 0x07

#define J 0x02
#define JAL 0x03

#define WordToAdd(x) ((x)<<2)
#define AddToWord(x) ((x)>>2)

#define SIGN_BIT	0x80000000

#define RFORM 1
#define IFORM 2
#define JFORM 3
