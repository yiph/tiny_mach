#ifndef _MACHINE_H_
#define _MACHINE_H_

#include <stdint.h>

/* define the machine */

typedef uint32_t memo_t; /* require an unsigned type */
#define MEMORY_SIZE 0x1000000

struct machine {
    memo_t pc;
    memo_t gpr[32];
    memo_t lo, hi;
    memo_t memory[MEMORY_SIZE];
};

/* define operations for instructions: */

#define OPCODE_FIELD  0xFC000000
#define OPCODE_OFFSET 26
#define REG0_FIELD    0x03E00000
#define REG0_OFFSET   21
#define REG1_FILED    0x001F0000
#define REG1_OFFSET   16
#define REG2_FIELD    0x0000F800
#define REG2_OFFSET   11
#define SHAMT_FIELD   0x000007C0
#define SHAMT_OFFSET  6
#define FUNCT_FIELD   0x0000003F
#define FUNCT_OFFSET  0
#define CONST_FIELD   0x0000FFFF
#define CONST_OFFSET  0
#define ADDR_FIELD    0x03FFFFFF
#define ADDR_OFFSET   0

#define FIELD(FIELD_NAME, ins)                                          \
    (((memo_t) ((ins) & FIELD_NAME##_FIELD)) >> FIELD_NAME##_OFFSET)
#define OPCODE(ins) FIELD(OPCODE, ins)
#define REG0(ins)   FIELD(REG0,   ins)
#define REG1(ins)   FIELD(REG1,   ins)
#define REG2(ins)   FIELD(REG2,   ins)
#define SHAMT(ins)  FIELD(SHAMT,  ins)
#define FUNCT(ins)  FIELD(FUNCT,  ins)
#define CONST(ins)  FIELD(CONST,  ins)
#define ADDR(ins)   FIELD(ADDR,   ins)
#undef  FIELD

/* codes for instructions: */

#define MAX_OPCODE 0x10
#define MAX_FUNCT  0x0E

/* opcode */
#define R_FMT   0x00
#define J       0x01
#define JAL     0x02
#define SYSCALL 0x03
#define ADDI    0x05
#define SUBI    0x06
#define ANDI    0x07
#define ORI     0x08
#define SLTI    0x09
#define SEQI    0x0A
#define LUI     0x0B
#define LW      0x0C
#define SW      0x0D
#define BZE     0x0E
#define BNZ     0x0F
#define JR      0x10

/* R-format funct. */
#define MOVE    0x00
#define ADD     0x01
#define SUB     0x02
#define MUL     0x03
#define DIV     0x04
#define MFLO    0x05
#define MFHI    0x06
#define SLL     0x07
#define SRL     0x08
#define SRA     0x09
#define AND     0x0A
#define OR      0x0B
#define NOT     0x0C
#define SLT     0x0D
#define SEQ     0x0E


#endif /* _MACHINE_H_ */
