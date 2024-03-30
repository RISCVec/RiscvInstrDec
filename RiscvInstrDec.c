#include "RiscvInstrDec.h"

int main()
{
   printf("\n********** RISC-V Instruction Decode **********\n");
   unsigned int instr;
   printf(" Please input Instruction: ");
   scanf("%x", &instr);

   printf("\n");
   printf(" Your Instruction is : 0x%08x\n", instr);
   printf(" Binary Type is      : ");
   print_binary(instr);

   // instruction struct declaration and init
   struct Instruction struInstr;
   memset(&struInstr, 0, sizeof(struct Instruction));

   // parse every seg to instruction struct
   parse_instr_to_struct(instr, &struInstr);
   // parse instruction struct
   parse_instr_struct(struInstr);

   return 0;
}
