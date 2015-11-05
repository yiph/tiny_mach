#ifndef _INSTRSET_H_
#define _INSTRSET_H_

#include <stdint.h>

/* define data type for instructions: */

#define ins_t uint32_t

#define OPCODE(ins)                             \
    (((uint32_t) ((ins) & 0xFC000000)) >> 26)

#define REG0(ins)                               \
    (((uint32_t) ((ins) & 0x03E00000)) >> 21)

#define REG1(ins)                               \
    (((uint32_t) ((ins) & 0x001F0000)) >> 16)

#define REG2(ins)                               \
    (((uint32_t) ((ins) & 0x0000F800)) >> 11)

#define SHAMT(ins)                              \
    (((uint32_t) ((ins) & 0x000007C0)) >> 6)

#define FUNCT(ins)                              \
    ((uint32_t) ((ins) & 0x0000003F))

#define CONST(ins)                              \
    ((uint32_t) ((ins) & 0x0000FFFF))

#define ADDR(ins)                               \
    ((uint32_t) ((ins) & 0x03FFFFFF))


/* codes for instructions: */

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



#endif
