# Tiny Machine (tiny_mach)

This is a tiny virtual machine (based on MIPS) as a practice for myself.

Compile tiny machine

> gcc -std=c99 launcher.c assembler.c machine.c -o tm

Run tiny machine

First sample:

> tm factorial.tms

Then enter an integer greater than 0

> >>> Running assembler ...
> >>> Running Tiny Machine ...
> 3
> 6
> >>> Done.

The second sample is fibonacci.tms. User entering n, it will compute first n terms in fibnacci sequence.


## Registers

  * GPR

  | NUMBER		| NAME		|
  | ----------- | --------- |  
  |	0			| at		|
  |	1-2			| k0-k1		|
  |	3-4			| v0-v1		|
  |	5-9		 	| a0-a4		|
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


  | NAME	  | FORMAT		| ARG NUM	| OPCODE/FUNCT(hex)	|
  | --------- |:-----------:|:---------:| -----------------:|
  | nop		  | R			| 0			| 00/00				|
  | move	  | R			| 2			| 00/01				|
  | add		  | R			| 3			| 00/02				|
  | addi	  | I			| 3			| 05				|
  | sub		  | R			| 3			| 00/03				|
  | mul		  | R			| 2			| 00/04				|
  | div		  | R			| 2			| 00/05				|
  | mflo	  | R			| 1			| 00/06				|
  | mfhi	  | R			| 1			| 00/07				|
  | sll		  | R			| 3			| 00/08				|
  | srl		  | R			| 3			| 00/09				|
  | sra		  | R			| 3			| 00/0a				|
  | and		  | R			| 3 		| 00/0b				|
  | andi	  | I			| 3 		| 06				|
  | or		  | R			| 3 		| 00/0c				|
  | ori		  | I			| 3			| 07				|
  | not		  | R			| 2			| 00/0d				|
  | slt		  | R			| 3			| 00/0e				|
  | slti	  | I			| 3			| 08				|
  | seq		  | R			| 3			| 00/0f				|
  | seqi	  | I			| 3			| 09				|
  | li		  | I			| 2			| 0a				|
  | lli		  | I			| 2			| 0b				|
  | lui		  | I			| 2			| 0c				|
  | lw		  | I			| 3			| 0d				|
  | sw		  | I			| 3			| 0e				|
  | bze		  | I			| 2 		| 0f				|
  | bnz		  | I			| 2			| 10				|
  | j		  | J			| 1			| 01				|
  | jal		  | J			| 1			| 02				|
  | jr		  | R			| 1			| 00/10				|
  | syscall	  | J			| 1			| 03				|

