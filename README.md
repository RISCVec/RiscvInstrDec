Hi, I am a RISC-V learner.

During learning RISC-V, I found it's difflicult to lear RISC-V instruction.

So, I write a parser in c.

This code was written in C, and will print the RISC-V instruction function.

Input shoud be hex format in 8 chars.
Output like this:

********** RISC-V Instruction Decode **********

 Please input Instruction: 00802303

 Your Instruction is : 0x00802303

 Binary Type is      : 0000000-01000-00000-010-00110-0000011
 
 OPCODE : LW
 
 RD    : 6
 
 RS1   : 0
 
 imm_i : 8
 
 Function: x[6] = M[x[0] + sext(8)]

If you have any good idea about it, please mail me.

2024-04-12
finish load/store add sub logic branch jal jalr
