#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RiscvInstrDefs.h"

// Print Binary Style Instruction
void print_binary(unsigned int n)
{
   for (int i = sizeof(n) * 8 - 1; i >= 0; i--)
   {
      switch (i)
      {
      case 6:         // 0-6 7-bit, opcode
      case 11:        // 7-11 5-bit, rd
      case 14:        // 12-14 3-bit, func3
      case 19:        // 15-19 5-bit, rs2
      case 24:        // 20-24 5-bit, rs1, remaining is func7 25-31, 7-bit
         printf("-"); // ???????-?????-?????-???-??????? format
      defalut:;
      }
      printf("%d", (n >> i) & 1);
   }
   printf("\n");
}

// Parse Instruction to Instruction struct
void parse_instr_to_struct(unsigned int instr, StruInstr *struInstr)
{

   // opcode, only save low 7-bit, convert to unsigned char, MSB is useless.
   struInstr->opcode = (unsigned char)(instr & 0x0000007f);
   // rd, shift right 7-bit (discard opcode) and only save 5-bit
   // convert to unsigned char, MSB 3-bit is useless.
   struInstr->rd = (unsigned char)((instr >> 7) & 0x0000001f);
   // func3, shift right 12-bit (discard opcode and rd) and only save 3-bit
   // convert to unsigned char, MSB 5-bit is useless.
   struInstr->func3 = (unsigned char)((instr >> 12) & 0x00000007);
   // rs1, shift right 15-bit (discard opcode, rd, func3) and only save 5-bit
   // convert to unsigned char, MSB 3-bit is useless.
   struInstr->rs1 = (unsigned char)((instr >> 15) & 0x0000001f);
   // rs2, shift right 20-bit (discard opcode, rd, func3, rs1) and only save 5-bit
   // convert to unsigned char, MSB 3-bit is useless.
   struInstr->rs2 = (unsigned char)((instr >> 20) & 0x0000001f);
   // rs2, shift right 25-bit (discard opcode, rd, func3, rs1, rs2) and only save 5-bit
   // convert to unsigned char, MSB 3-bit is useless.
   struInstr->func7 = (unsigned char)(instr >> 25);
   // imm_i, shift right 20-bit (discard opcode, rd, func3, rs1) and only save 20-bit
   // convert to unsigned int, MSB 12-bit is useless.
   struInstr->imm_i = (unsigned int)(instr >> 20);
   // imm_s, shift right 20-bit (discard opcode, rd, func3, rs1) and only save 20-bit
   // convert to unsigned int, MSB 12-bit is useless.
   struInstr->imm_s = (unsigned int)(((instr >> 20) & 0x00000fe0) + ((instr >> 7) & 0x0000001f));
   // imm_b, instr 31, 7, 30-25, 11-8 bit with sign bit extention
   // convert to unsigned int, LSB is 0,
   struInstr->imm_b = (unsigned int)(((instr >> 29) & 0x00001000) + ((instr << 4) & 0x00000800) + ((instr >> 20) & 0x000007e0) + ((instr >> 7) & 0x0000001e));
   if ((instr >> 31) == 1)
      struInstr->imm_b += 0xffffe000; // sign bit extention
   // imm_j, instr 31, 19-12, 20, 30-21 bit with sign bit extention
   // convert to unsigned int, LSB is 0,
   struInstr->imm_j = (unsigned int)(((instr >> 12) & 0x00080000) + ((instr) & 0x00007800) + ((instr >> 9) & 0x00000400) + ((instr >> 20) & 0x000007fe));
   if ((instr >> 31) == 1)
      struInstr->imm_j += 0xffe00000; // sign bit extention
   // imm_u, instr 31-12 bit, with low 12-bit set to 0
   // convert to unsigned int
   struInstr->imm_u = (unsigned int)(instr & 0xfffff000);
   // imm_z, instr 19-15 bit, with high 27-bit set to 0
   // convert to unsigned int
   struInstr->imm_z = (unsigned int)((instr >> 15) & 0x0000001f);
}

// base on RISC-V instruction struct, parse instruction option.
void parse_instr_struct(StruInstr struInstr)
{
   // opcode
   switch (struInstr.opcode)
   {

   // LOAD opcode matched, look at func3
   case LOAD_OP:
   {
      // func3 look up
      switch (struInstr.func3)
      {
      case LB:
         printf(" OPCODE : LB\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)][0:7]\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      case LH:
         printf(" OPCODE : LH\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)][0:15]\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      case LW:
         printf(" OPCODE : LW\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)][0:31]\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      case LD:
         printf(" OPCODE : LD\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)]\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      case LBU:
         printf(" OPCODE : LBU\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)][0:7], zero-extends\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      case LHU:
         printf(" OPCODE : LHU\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)][0:15], zero-extends\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      case LWU:
         printf(" OPCODE : LWU\n");
         printf(" RD    : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = M[x[%d] + sext(%d)][0:31], zero-extends\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      default:
         printf(" Error Instruction Format!!!");
         break;
      }

      break;
   }
   // LOAD_OP end

   // STORE opcode matched, look at func3
   case STORE_OP:
   {
      // func3 look up
      switch (struInstr.func3)
      {
      case SB:
         printf(" OPCODE : SB\n");
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" RS2   : %d\n", struInstr.rs2);
         printf(" imm_s : %d\n", struInstr.imm_s);
         printf(" Function: M[x[%d] + sext(%d)][0:7] = x[%d][0:7]\n", struInstr.rs1, struInstr.imm_s, struInstr.rs2);
         break;
      case SH:
         printf(" OPCODE : SH\n");
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" RS2   : %d\n", struInstr.rs2);
         printf(" imm_s : %d\n", struInstr.imm_s);
         printf(" Function: M[x[%d] + sext(%d)][0:15] = x[%d][0:15]\n", struInstr.rs1, struInstr.imm_s, struInstr.rs2);
         break;
      case SW:
         printf(" OPCODE : SW\n");
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" RS2   : %d\n", struInstr.rs2);
         printf(" imm_s : %d\n", struInstr.imm_s);
         printf(" Function: M[x[%d] + sext(%d)][0:31] = x[%d][0:31]\n", struInstr.rs1, struInstr.imm_s, struInstr.rs2);
         break;
      case SD:
         printf(" OPCODE : SD\n");
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" RS2   : %d\n", struInstr.rs2);
         printf(" imm_s : %d\n", struInstr.imm_s);
         printf(" Function: M[x[%d] + sext(%d)] = x[%d]\n", struInstr.rs1, struInstr.imm_s, struInstr.rs2);
         break;
      default:
         printf(" Error Instruction Format!!!");
         break;
      }
      
      break;
   }
   // STORE_OP end

   // ADD SHIFT LOGIC opcode mached, look at func3
   case ADD_SHIFT_LOGIC_OP:
   {
      parse_asl_f3f7(struInstr);
      break;
   }
      // ADD_SHIFT_LOGIC_OP end

   case ADD_SHIFT_LOGIC_IMM_OP:
   {
      parse_asli_f3f7(struInstr);
      break;
   }
      // ADD_SHIFT_LOGIC_IMM_OP end

   case BRANCH_OP:
   {
      switch (struInstr.func3)
      {
         case BEQ:
         {
            printf(" OPCODE : BEQ\n");         
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" imm_b : %d\n", struInstr.imm_b);
            printf(" Function: if(x[%d] == x[%d]) PC += %d \n", struInstr.rs1, struInstr.rs2, struInstr.imm_b);
            break;
         }
         case BNE:
         {
            printf(" OPCODE : BNE\n");         
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" imm_b : %d\n", struInstr.imm_b);
            printf(" Function: if(x[%d] != x[%d]) PC += %d \n", struInstr.rs1, struInstr.rs2, struInstr.imm_b);
            break;
         }
         case BLT:
         {
            printf(" OPCODE : BLT\n");         
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" imm_b : %d\n", struInstr.imm_b);
            printf(" Function: if(x[%d] < x[%d]) PC += %d \n", struInstr.rs1, struInstr.rs2, struInstr.imm_b);
            break;
         }
         case BGE:
         {
            printf(" OPCODE : BLT\n");         
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" imm_b : %d\n", struInstr.imm_b);
            printf(" Function: if(x[%d] >= x[%d]) PC += %d \n", struInstr.rs1, struInstr.rs2, struInstr.imm_b);
            break;
         }
         case BLTU:
         {
            printf(" OPCODE : BLT\n");         
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" imm_b : %d\n", struInstr.imm_b);
            printf(" Function: if(x[%d] < x[%d]) PC += %d zero-extends\n", struInstr.rs1, struInstr.rs2, struInstr.imm_b);
            break;
         }
         case BGEU:
         {
            printf(" OPCODE : BLT\n");         
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" imm_b : %d\n", struInstr.imm_b);
            printf(" Function: if(x[%d] >= x[%d]) PC >= %d zero-extends\n", struInstr.rs1, struInstr.rs2, struInstr.imm_b);
            break;
         }
         default:
            printf(" Error Instruction Format!!!");
            break;
      }
      break;
   }
   // BRANCH_OP end

   case JAL:
   {
      printf(" OPCODE : JAL\n");         
      printf(" imm_j : %d\n", struInstr.imm_j);
      printf(" Function: x[%d] = PC + 4, PC += %d\n", struInstr.rd, struInstr.imm_j);
      break;
   }

   case JALR_OP:
   {
      if(struInstr.func3 != JALR)
      {
         printf(" Error Instruction Format!!!\n");
         break;
      }
      printf(" OPCODE : JALR\n");         
      printf(" RS1   : %d\n", struInstr.rs1);
      printf(" imm_j : %d\n", struInstr.imm_j);
      printf(" Function: x[%d] = PC + 4, PC = x[%d] + %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_j);
      break;
   }
   
   case LUI:
   {
      printf(" OPCODE: LUI\n");
      printf(" RD   : %d\n", struInstr.rd);
      printf(" imm_u: %u\n", struInstr.imm_u);
      printf(" Function: x[%d] = %u\n", struInstr.rd, struInstr.imm_u);
      break;
   }

   case AUIPC:
   {
      printf(" OPCODE: AUIPC\n");
      printf(" RD   : %d\n", struInstr.rd);
      printf(" imm_u: %u\n", struInstr.imm_u);
      printf(" Function: x[%d] = PC + %u\n", struInstr.rd, struInstr.imm_u);
      break;
   }

   case ECALL_BREAK_OP:
   {
      parse_ebec_f3f7(struInstr);
      break;
   }
   default: 
      printf(" Error Instruction Format!!!\n");
      break;
   }
}
// parse Add Shift Logic func3 and func7
void parse_asl_f3f7(StruInstr struInstr)
{
   // opcode is ADD_SHIFT_LOGIC_OP
   switch (struInstr.func3)
   {
      case ADD_SUB_F3:
      {
         if (struInstr.func7 == ADD)
         {
            printf(" OPCODE : ADD\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] + x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         else if(struInstr.func7 == SUB)
         {
            printf(" OPCODE : SUB\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] - x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         
         break;
      }
      // ADD_SUB_F3 end
      case XOR_F3:
      {
         if (struInstr.func7 == XOR)
         {
            printf(" OPCODE : XOR\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] ^ x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // XOR_F3 end
      case OR_F3:
      {
         if (struInstr.func7 == OR)
         {
            printf(" OPCODE : OR\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] | x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // OR_F3 end
      case AND_F3:
      {
         if (struInstr.func7 == AND)
         {
            printf(" OPCODE : AND\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] & x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // AND_F3 end
      case SLL_F3:
      {
         if (struInstr.func7 == SLL)
         {
            printf(" OPCODE : SLL\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] << x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // SLL_F3 end
      case SRL_SRA_F3:
      {
         if (struInstr.func7 == SRL)
         {
            printf(" OPCODE : SRL\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] >> x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         else if (struInstr.func7 == SRA)
         {
            printf(" OPCODE : SRA\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] >> x[%d] msb-extends\n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // SRL_SRA_F3 end
      case SLT_F3:
      {
         if (struInstr.func7 == SLT)
         {
            printf(" OPCODE : SLT\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = (x[%d] < x[%d]) ? 1 : 0 \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // SLT_F3 end
      case SLTU_F3:
      {
         if (struInstr.func7 == SLTU)
         {
            printf(" OPCODE : SLT\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = (x[%d] < x[%d]) ? 1 : 0 zero-extends\n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         break;
      }
      // SLTU_F3 end
      default:
         printf(" Error Instruction Format!!!\n");
         break;
   }
}

void parse_asli_f3f7(StruInstr struInstr)
{
   switch (struInstr.func3)
   {
      case ADDI:
      {
         printf(" OPCODE : ADDI\n");
         printf(" RD : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = x[%d] + %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      }
      case XORI:
      {
         printf(" OPCODE : XORI\n");
         printf(" RD : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = x[%d] ^ %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      }
      case ORI:
      {
         printf(" OPCODE : ORI\n");
         printf(" RD : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = x[%d] | %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      }
      case ANDI:
      {
         printf(" OPCODE : ANDI\n");
         printf(" RD : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = x[%d] & %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      }
      case SLLI_F3:
      {
         if (struInstr.func7 == SLLI)
         {
            printf(" OPCODE : SLLI\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" imm_i : %d\n", struInstr.imm_i);
            printf(" Function: x[%d] = x[%d] << %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         }
         break;
      }
      case SRLI_SRAI_F3:
      {
         if (struInstr.func7 == SRLI)
         {
            printf(" OPCODE : SRLI\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" imm_i : %d\n", struInstr.imm_i);
            printf(" Function: x[%d] = x[%d] >> %d \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         }
         else if (struInstr.func7 == SRAI)
         {
            printf(" OPCODE : SRAI\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" imm_i : %d\n", struInstr.imm_i);
            printf(" Function: x[%d] = x[%d] >> %d msb-extends\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         }
         break;
      }
      case SLTI:
      {
         printf(" OPCODE : SLTI\n");
         printf(" RD : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = (x[%d] < %d) ? 1 : 0 \n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      }
      case SLTIU:
      {
         printf(" OPCODE : SLTIU\n");
         printf(" RD : %d\n", struInstr.rd);
         printf(" RS1   : %d\n", struInstr.rs1);
         printf(" imm_i : %d\n", struInstr.imm_i);
         printf(" Function: x[%d] = (x[%d] < %d) ? 1 : 0 zero-extends\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
         break;
      }
      default:
         printf(" Error Instruction Format!!!\n");
         break;
   } // end switch
}

void parse_ebec_f3f7(StruInstr struInstr)
{
   if(struInstr.func3 != 0x0)
      printf(" Error Instruction Format!!!\n");
   else
   {
      switch(struInstr.func7)
      {
         case ECALL:
         {
            printf(" OPCODE : ECALL\n");
            printf(" Transfer control to OS.\n");
            break;
         }
         case EBREAK:
         {
            printf(" OPCODE : EBREAK\n");
            printf(" Transfer control to debugger.\n");
            break;
         }
         default:
            printf(" Error Instruction Format!!!");
            break;
      }
   }
}
