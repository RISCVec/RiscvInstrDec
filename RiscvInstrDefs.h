// RiscvInstrDefines.h

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
#define SLTIU 0x3        // 011
#define XORI 0x4         // 100
#define SRLI_SRAI_F3 0x5 // 101
#define ORI 0x6       // 110
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

// fun7 seg
// use 7-bit unsigned char to store
// MSB 1-bit is useless
#define SLLI 0x00  // 0000000
#define SRLI 0x00  // 0000000
#define SRAI 0x20  // 0100000
#define SLLIW 0x00 // 0000000
#define SRLIW 0x00 // 0000000
#define SRAIW 0x20 // 0100000
#define ADD 0x00   // 0000000
#define SUB 0x20   // 0100000
#define SLL 0x00   // 0000000
#define SLT 0x00   // 0000000
#define SLTU 0x00  // 0000000
#define XOR 0x00   // 0000000
#define SRL 0x00   // 0000000
#define SRA 0x20   // 0100000
#define OR 0x00    // 0000000
#define AND 0x00   // 0000000
#define ADDW 0x00  // 0000000
#define SUBW 0x20  // 0100000
#define SLLW 0x00  // 0000000
#define SRLW 0x00  // 0000000
#define SRAW 0x20  // 0100000
// define end

// Instruction struct
typedef struct Instruction
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
   unsigned int imm_z; //
} StruInstr;

// Function Head

void print_binary(unsigned int n);
void parse_asl_f3f7(StruInstr struInstr);
void parse_asli_f3f7(StruInstr struInstr);
