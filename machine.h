#ifndef _MACHINE_H_
#define _MACHINE_H_

#include <stdint.h>

/* define the machine */

#define MEMORY_SIZE 0x1000000

typedef struct machine {
    uint32_t pc;
    uint32_t gpr[32];
    uint32_t lo, hi;
    uint32_t memory[MEMORY_SIZE];
} MACHINE;

/* Interfaces to runmachine */
void init_environment();
MACHINE *make_machine();
void free_machine(MACHINE *mach);
void init_machine(MACHINE *mach);
void load_memory(int32_t words[], int length, MACHINE *mach, int start_addr);
int run_machine(MACHINE *mach);

#define _nil_         0 // for unused part of instruction
#define _at_          0
#define _k0_          1
#define _k1_          2
#define _v0_          3
#define _v1_          4
#define _a0_          5
#define _a1_          6
#define _a2_          7
#define _a3_          8
#define _a4_          9
#define _t0_          10
#define _t1_          11
#define _t2_          12
#define _t3_          13
#define _t4_          14
#define _t5_          15
#define _t6_          16
#define _t7_          17
#define _t8_          18
#define _t9_          19
#define _s0_          20
#define _s1_          21
#define _s2_          22
#define _s3_          23
#define _s4_          24
#define _s5_          25
#define _s6_          26
#define _s7_          27
#define _gp_          28
#define _sp_          29
#define _fp_          30
#define _ra_          31

#define IFMT_INS(OP_NAME, R0, R1, C)                     \
    ((uint32_t) ((_##OP_NAME##_ << OPCODE_OFFSET)        \
               | (_##R0##_ << REG0_OFFSET)               \
               | (_##R1##_ << REG1_OFFSET)               \
               | ((((uint32_t) (C)) << CONST_OFFSET) & 0x0000FFFF)))

#define RFMT_INS(FUNCT_NAME, R0, R1, R2, SHA)            \
    ((uint32_t) ((_rfmt_ << OPCODE_OFFSET)               \
               | ((_##FUNCT_NAME##_ << FUNCT_OFFSET))    \
               | (_##R0##_ << REG0_OFFSET)               \
               | (_##R1##_ << REG1_OFFSET)               \
               | (_##R2##_ << REG2_OFFSET)               \
               | (((uint32_t) (SHA)) << SHAMT_OFFSET)))

#define JFMT_INS(OP_NAME, A)                             \
    ((uint32_t) ((_##OP_NAME##_ << OPCODE_OFFSET)        \
               | (((uint32_t) (A)) << ADDR_OFFSET)))

#define SYSCALL_INS(OP_NAME)                             \
    JFMT_INS(syscall, _##OP_NAME##_)

/* define operations for instructions: */
#define OPCODE_FIELD  0xFC000000
#define OPCODE_OFFSET 26
#define REG0_FIELD    0x03E00000
#define REG0_OFFSET   21
#define REG1_FIELD    0x001F0000
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


/* codes for instructions: */
#define MAX_OPCODE  0x11
#define MAX_FUNCT   0x10
#define MAX_SYSCALL 100

/* opcode */
#define _rfmt_    0x00
#define _j_       0x01
#define _jal_     0x02
#define _syscall_ 0x03
#define _addi_    0x05
#define _andi_    0x06
#define _ori_     0x07
#define _slti_    0x08
#define _seqi_    0x09
#define _li_      0x0A
#define _lli_     0x0B
#define _lui_     0x0C
#define _lw_      0x0D
#define _sw_      0x0E
#define _bze_     0x0F
#define _bnz_     0x10

/* R-format funct. */
#define _nop_     0x00
#define _move_    0x01
#define _add_     0x02
#define _sub_     0x03
#define _mul_     0x04
#define _div_     0x05
#define _mflo_    0x06
#define _mfhi_    0x07
#define _sll_     0x08
#define _srl_     0x09
#define _sra_     0x0A
#define _and_     0x0B
#define _or_      0x0C
#define _not_     0x0D
#define _slt_     0x0E
#define _seq_     0x0F
#define _jr_      0x10

/* Syscall */
#define _sys_newline_      0
#define _sys_print_int_    1
#define _sys_print_hex_    2
#define _sys_read_int_     3

#endif /* _MACHINE_H_ */
