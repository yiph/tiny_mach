# Tiny Machine (tiny_mach)

This is a tiny virtual machine (based on MIPS) as a practice for myself.

## Registers

  * GPR

  | NUMBER		| NAME		|
  | ----------- | --------- |  
  |	0			| at		|
  |	1-2			| k0-k1		|
  |	3-4			| v0-v1		|
  |	5-9		 	| a0-a5		|
  |	10-19		| t0-t9		|
  |	20-27		| s0-s7		|
  | 28			| gp		|
  |	29			| sp		|
  |	30			| fp		|
  |	31			| ra		|


  * PC

  * LO, HI


## The Instruction Set

  * **R-format**

    6(opcode) + 5(reg0) + 5(reg1) + 5(reg2) + 5(shamt) + 6(funct) = 32 bits
    
  * **I-format**

    6(opcode) + 5(reg0) + 5(reg1) + 16(const) = 32 bits
    
  * **J-format**

    6(opcode) + 26(addr) = 32 bits


  | NAME	  | FORMAT		| OPCODE/FUNCT(hex)	|
  | --------- |:-----------:| -----------------:|
  | move	  | R			| 00/00				|
  | add		  | R			| 00/01				|
  | addi	  | I			| 05				|
  | sub		  | R			| 00/02				|
  | subi	  | I			| 06				|
  | mul		  | R			| 00/03				|
  | div		  | R			| 00/04				|
  | mflo	  | R			| 00/05				|
  | mfhi	  | R			| 00/06				|
  | sll		  | R			| 00/07				|
  | srl		  | R			| 00/08				|
  | sra		  | R			| 00/09				|
  | and		  | R			| 00/0a				|
  | andi	  | I			| 07				|
  | or		  | R			| 00/0b				|
  | ori		  | I			| 08				|
  | not		  | R			| 00/0c				|
  | slt		  | R			| 00/0d				|
  | slti	  | I			| 09				|
  | seq		  | R			| 00/0e				|
  | seqi	  | I			| 0a				|
  | lui		  | I			| 0b				|
  | lw		  | I			| 0c				|
  | sw		  | I			| 0d				|
  | bze		  | I			| 0e				|
  | bnz		  | I			| 0f				|
  | j		  | J			| 01				|
  | jal		  | J			| 02				|
  | jr		  | I			| 10				|
  | syscall	  | J			| 03				|

