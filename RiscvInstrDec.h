#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Instruction Defines
// OP end: instruction should look up func3 or func7 seg
// F3 end: instruction should look up func7 seg
// other end: Good luck! instruction matched

// opcode seg
// use 8-bit unsigned char to store
// MSB is useless
#define LOAD_OP 0x03                // 0000011 lb lh lw ld lbu lhu lwu
#define FENCE_OP 0x0f               // 0001111 fence fence.i
#define ADD_SHIFT_LOGIC_IMM_OP 0x13 // 0010011 addi slli slti sltiu xori srli srai ori andi
#define AUIPC_OP 0x17               // 0010111 auipc
#define ADD_SHIFT_IMM_WORD_OP 0x1b  // 0011011 addiw slliw srliw sraiw
#define STORE_OP 0x23               // 0100011 sb sh sw sd
#define ADD_SHIFT_LOGIC_OP 0x33     // 0110011 add sub sli slt xor srl sra or and
#define LUI 0x37                    // 0110111 lui
#define ADD_SUB_SHIFT_WORD_OP 0x3b  // 0111011 addw subw sllw srlw sraw
#define BRANCH_OP 0x63              // 1100011 beq bne blt bge bltu bgeu
#define JALR_OP 0x67                // 1100111 jalr
#define JAL 0x6F                    // 1101111 jal
#define ECALL_BREAK_OP 0x73         // 1110011 ecall ebreak
#define CSR_OP 0x73                 // 1110011 csrrw csrrs csrrc csrrwi csrrsi csrrci

// fun3 seg
// use 1-bit unsigned char to store
// MSB 5-bit is useless
// LOAD
#define LB 0x0  // 000
#define LH 0x1  // 001
#define LW 0x2  // 010
#define LD 0x3  // 011
#define LBU 0x4 // 100
#define LHU 0x5 // 101
#define LWU 0x6 // 110
// FENCE
#define FENCE 0x0   // 000
#define FENCE_I 0x1 // 001
// ADD SUB SHIFT LOGIC IMM
#define ADDI 0x0         // 000
#define SLLI_F3 0x1      // 001
#define SLTI 0x2         // 010
#define SLTUI 0x3        // 011
#define XORI 0x4         // 100
#define SRLI_SRAI_F3 0x5 // 101
#define ORI_F3 0x6       // 110
#define ANDI 0x7         // 111
// AND SHIFT WORD
#define ADDIW 0x0          // 000
#define SLLIW_F3 0x1       // 001
#define SRLIW_SRAIW_F3 0x5 // 101
// STORE
#define SB 0x0 // 000
#define SH 0x1 // 001
#define SW 0x2 // 020
#define SD 0x3 // 011
// ADD SUB LOGIC
#define ADD_SUB_F3 0x0 // 000
#define SLL_F3 0x1     // 001
#define SLT_F3 0x2     // 010
#define SLTU_F3 0x3    // 011
#define XOR_F3 0x4     // 100
#define SRL_SRA_F3 0x5 // 101
#define OR_F3 0x6      // 110
#define AND_F3 0x7     // 111
// ADD SUB SHIFT WORD
#define ADDW_SUBW_F3 0x0 // 000
#define SLLW_F3 0x1      // 001
#define SRLW_SRAW_F3 0x5 // 101
// BRANCH
#define BEQ 0x0  // 000
#define BNE 0x1  // 001
#define BLT 0x4  // 100
#define BGE 0x5  // 101
#define BLTU 0x6 // 110
#define BGEU 0x7 // 111
// JALR
#define JALR 0x0 // 000
// ECALL EBREAK
#define ECALL_EBREAK_F3 0x0 // 000
// CSR
#define CSRRW 0x1  // 001
#define CSRRS 0x2  // 010
#define CSRRC 0x3  // 011
#define CSRRWI 0x5 // 101
#define CSRRSI 0x6 // 110
#define CSRRCI 0x7 // 111

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

// Instruction struct
struct Instruction
{
   unsigned char func7;
   unsigned char rs2;
   unsigned char rs1;
   unsigned char func3;
   unsigned char rd;
   unsigned char opcode;
   unsigned int imm_i; // use low 12-bit
   unsigned int imm_s; // use low 12-bit
   unsigned int imm_b; // use low 12-bit, lsb is 0
   unsigned int imm_u; // use high 21-bit
   unsigned int imm_j; // use low 21-bit, lsb is 0
};

// Parse Instruction to Instruction struct
void parse_instr_to_struct(unsigned int instr, struct Instruction *struInstr)
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
   struInstr->imm_s = (unsigned int)(((instr >> 20) & 0x0003f800) + ((instr >> 7) & 0x0000001f));
}

// base on RISC-V instruction struct, parse instruction option.
void parse_instr_struct(struct Instruction struInstr)
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
         break;
      case LH:
         printf(" OPCODE : LH\n");
         break;
      case LW:
         printf(" OPCODE : LW\n");
         break;
      case LD:
         printf(" OPCODE : LD\n");
         break;
      case LBU:
         printf(" OPCODE : LBU\n");
         break;
      case LHU:
         printf(" OPCODE : LHU\n");
         break;
      case LWU:
         printf(" OPCODE : LWU\n");
         break;
      default:
         break;
      }
      printf(" RD    : %d\n", struInstr.rd);
      printf(" RS1   : %d\n", struInstr.rs1);
      printf(" imm_i : %d\n", struInstr.imm_i);
      printf(" Function: x[%d] = M[x[%d] + sext(%d)]\n", struInstr.rd, struInstr.rs1, struInstr.imm_i);
      break;
   }

      // STORE opcode matched, look at func3
   case STORE_OP:
   {
      // func3 look up
      switch (struInstr.func3)
      {
      case SB:
         printf(" OPCODE : SB\n");
         break;
      case SH:
         printf(" OPCODE : SH\n");
         break;
      case SW:
         printf(" OPCODE : SW\n");
         break;
      case SD:
         printf(" OPCODE : SD\n");
         break;
      default:
         break;
      }
      printf(" RS1   : %d\n", struInstr.rs1);
      printf(" RS2   : %d\n", struInstr.rs2);
      printf(" imm_s : %d\n", struInstr.imm_s);
      printf(" Function: M[x[%d] + sext(%d)] = x[%d]\n", struInstr.rs1, struInstr.imm_s, struInstr.rs2);
      break;
   }
   // ADD SHIFT LOGIC opcode mached, look at func3
   case ADD_SHIFT_LOGIC_OP:
   {
      // func3 look up
      switch (struInstr.func3)
      {
         // ADD SUB matched, look at func7
      case ADD_SUB_F3:
      {
         // func7
         if (struInstr.func7 == 0x00)
         {
            printf(" OPCODE : ADD\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] + x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
         else if (struInstr.func7 == 0x20)
         {
            printf(" OPCODE : SUB\n");
            printf(" RD : %d\n", struInstr.rd);
            printf(" RS1   : %d\n", struInstr.rs1);
            printf(" RS2   : %d\n", struInstr.rs2);
            printf(" Function: x[%d] = x[%d] - x[%d] \n", struInstr.rd, struInstr.rs1, struInstr.rs2);
         }
      }
      break;

      case SH:
         printf(" OPCODE : SH\n");
         break;
      case SW:
         printf(" OPCODE : SW\n");
         break;
      case SD:
         printf(" OPCODE : SD\n");
         break;
      default:
         break;
      }

      break;
   }
   default:
      break;
   }
}