#include "RVInstDec.h"

int main( void )
{
   printf( "\n********** RISC-V RV32I Instruction Decode **********" );
   uint32_t instr;
   while( 1 ) {
      printf( "\n Please input Instruction ( input 0 to quit ): " );
      scanf( "%x", &instr );

      if( instr == 0 )
      {
         printf( "\n Quit.\n" );
         break;
      }
         
      printf( "\n" );
      printf( " Your Instruction is : 0x%08x\n", instr );
      printf( " Binary Type is      : " );
      print_binary( instr );

      // instruction struct declaration and init
      struct Instruction struInstr;
      memset( &struInstr, 0, sizeof( struct Instruction ) );

      // parse every seg to instruction struct
      parse_instr_to_struct( instr, &struInstr );
      // parse instruction struct
      parse_instr_struct( struInstr );
   }
   

   return 0;
}
