#ifndef _MACHINE_H_
#define _MACHINE_H_

#include <stdint.h>

/* define the machine */

#define MEMORY_SIZE 0x1000000

struct machine {
    uint32_t pc;
    uint32_t gpr[32];
    uint32_t lo, hi;
    uint32_t memory[MEMORY_SIZE];
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
    (((uint32_t) ((ins) & FIELD_NAME##_FIELD)) >> FIELD_NAME##_OFFSET)
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
#define _rfmt_    0x00
#define _j_       0x01
#define _jal_     0x02
#define _syscall_ 0x03
#define _addi_    0x05
#define _subi_    0x06
#define _andi_    0x07
#define _ori_     0x08
#define _slti_    0x09
#define _seqi_    0x0A
#define _lui_     0x0B
#define _lw_      0x0C
#define _sw_      0x0D
#define _bze_     0x0E
#define _bnz_     0x0F


/* R-format funct. */
#define _move_    0x00
#define _add_     0x01
#define _sub_     0x02
#define _mul_     0x03
#define _div_     0x04
#define _mflo_    0x05
#define _mfhi_    0x06
#define _sll_     0x07
#define _srl_     0x08
#define _sra_     0x09
#define _and_     0x0A
#define _or_      0x0B
#define _not_     0x0C
#define _slt_     0x0D
#define _seq_     0x0E
#define _jr_      0x0F


#endif /* _MACHINE_H_ */
