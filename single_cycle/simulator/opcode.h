enum RegType { NONE, RS, RT, RD, EXTRA}
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
  
