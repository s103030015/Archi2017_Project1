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

#define RFORM 1
#define IFORM 2
#define JFORM 3

struct OpType {
  int opCode;
  int format;
};

static OpType optype[] = {
  {ADD, RFORM}, {ADDU, RFORM}, {SUB, RFORM}, {AND, RFORM},
  {OR, RFORM}, {XOR, RFORM}, {NOR, RFORM}, {NAND, RFORM},
  {SLT, RFORM}, {SLL, RFORM}, {SRL, RFORM}, {SRA, RFORM},
  {JR, RFORM}, {MULT, RFORM}, {MULTU, RFORM}, {MFHI, RFORM},
  {MFLO, RFORM}, {ADDI, IFORM},{ADDIU, IFORM}, {LW, IFORM},
  {LH, IFORM}, {LHU, IFORM}, {LB, IFORM}, {LBU, IFORM},
  {SW, IFORM}, {SH, IFORM}, {SB, IFORM}, {LUI, IFORM},
  {ANDI, IFORM}, {ORI, IFORM}, {NORI, IFORM}, {SLTI, IFORM},
  {BEQ, IFORM}, {BNE, IFORM}, {BGTZ, IFORM}, {J, JFORM},
  {JAL, JFORM}
};

enum RegType { NONE, RS, RT, RD, EXTRA};

struct OpString{
  char *format;
  RegType arg[3];
};

static struct OpString opstrings[] = {
  {"ADD r%d,r%d,r%d", {RD, RS, RT}},
  {"ADDU r%d,r%d,r%d", {RD, RS, RT}},
  {"SUB r%d,r%d,r%d", {RD, RS, RT}},
  {"AND r%d,r%d,r%d", {RD, RS, RT}},
  {"OR r%d,r%d,r%d", {RD, RS, RT}},
  {"XOR r%d,r%d,r%d", {RD, RS, RT}},
  {"NOR r%d,r%d,r%d", {RD, RS, RT}},
  {"NAND r%d,r%d,r%d", {RD, RS, RT}},
  {"SLT r%d,r%d,r%d", {RD, RS, RT}},
  {"SLL r%d r%d %d", {RD, RT, EXTRA}},
  {"SRL r%d r%d %d", {RD, RT, EXTRA}},
  {"SRA r%d,r%d,%d", {RD, RT, EXTRA}},
  {"JR r%d r%d", {RD, RS, NONE}},
  {"MULT r%d,r%d", {RS, RT, NONE}},
  {"MULTU r%d,r%d", {RS, RT, NONE}},
  {"MFHI r%d", {RD, NONE, NONE}},
  {"MFLO r%d", {RD, NONE, NONE}},
  {"ADDI r%d,r%d,%d", {RT, RS, EXTRA}},
  {"ADDIU r%d,r%d,%d", {RT, RS, EXTRA}},
  {"LW r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"LH r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"LHU r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"LB r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"LBU r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"SW r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"SH r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"SB r%d,%d(r%d)", {RT, EXTRA, RS}},
  {"LUI r%d,%d", {RT, EXTRA, NONE}},
  {"ANDI r%d,r%d,%d", {RT, RS, EXTRA}},
  {"ORI r%d,r%d,%d", {RT, RS, EXTRA}},
  {"NORI r%d,r%d,%d", {RT, RS, EXTRA}},
  {"SLTI r%d,r%d,%d", {RT, RS, EXTRA}},
  {"BEQ r%d,r%d,%d", {RS, RT, EXTRA}},
  {"BNE r%d,r%d,%d", {RS, RT, EXTRA}},
  {"BGTZ r%d,%d", {RS, EXTRA, NONE}},
  {"J %d", {EXTRA, NONE, NONE}},
  {"JAL %d", {EXTRA, NONE, NONE}}
};
  
