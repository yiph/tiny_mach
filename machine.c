
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "machine.h"

#define FLAG(S) printf(#S"\n")
#define PRINT(I) printf("%d\n", (I))



#define DECLARE_FUNCT_OP(OP_NAME)                                 \
    void OP_NAME##_f(uint32_t ins, MACHINE *mach)
#define DECLARE_INS_F OP_SET_ALL(DECLARE_FUNCT)

#define ASSIGN_FUND_DISPATCH_OP(OP_NAME)        \
    fund_dispatch[_##OP_NAME##_] = OP_NAME##_f
#define INIT_FUND_DISPATCH() do { OP_SET_FUND(ASSIGN_FUND_DISPATCH); } while(0)

#define ASSIGN_RFMT_DISPATCH_OP(OP_NAME)        \
    rfmt_dispatch[_##OP_NAME##_] = OP_NAME##_f
#define INIT_RFMT_DISPATCH() do { OP_SET_RFMT(ASSIGN_RFMT_DISPATCH); } while(0)



/* Declaration */
DECLARE_INS_F; // Declare all functions of form INS_f
static void (*fund_dispatch[MAX_OPCODE + 1])(uint32_t ins, MACHINE *mach);
static void (*rfmt_dispatch[MAX_FUNCT + 1])(uint32_t ins, MACHINE *mach);
static void (*syscall_dispatch[MAX_SYSCALL + 1])(uint32_t ins, MACHINE *mach);

// Declare syscall functions
void sys_halt(uint32_t ins, MACHINE *mach);
void sys_newline(uint32_t ins, MACHINE *mach);
void sys_print_int(uint32_t ins, MACHINE *mach);
void sys_print_hex(uint32_t ins, MACHINE *mach);
void sys_read_int(uint32_t ins, MACHINE *mach);

/* function bodies */
void init_environment()
{
    INIT_FUND_DISPATCH();
    INIT_RFMT_DISPATCH();
    syscall_dispatch[_sys_halt_]      = sys_halt;
    syscall_dispatch[_sys_newline_]   = sys_newline;
    syscall_dispatch[_sys_print_int_] = sys_print_int;
    syscall_dispatch[_sys_print_hex_] = sys_print_hex;
    syscall_dispatch[_sys_read_int_]  = sys_read_int;
}

void init_machine(MACHINE *mach)
{
    mach->pc = 0;
    mach->lo = 0;
    mach->hi = 0;
    int i;
    for (i = 0; i < 32; i++)
        mach->gpr[i] = 0;
    mach->gpr[_sp_] = 0xFF;
    for (i = 0; i < MEMORY_SIZE; i++)
        mach->memory[i] = 0;
}

MACHINE *make_machine()
{
    MACHINE *mach =  (MACHINE *) malloc(sizeof(MACHINE));
    if (mach == NULL) {                                        \
        printf("Fail in malloc at line: %d in "__FILE__"\n", __LINE__);       \
        exit(0);                                                \
    }
    return mach;
}

void free_machine(MACHINE *mach)
{
    free(mach);
}

void load_memory(int32_t words[], int length, MACHINE *mach, int start_addr)
{
    for (int addr = start_addr, i = 0; i < length && addr < MEMORY_SIZE; addr++, i++)
        mach->memory[addr] = words[i];
}


int run_machine(MACHINE *mach)
{
    uint32_t *pc_ptr = &mach->pc;
    for (*pc_ptr = 0; *pc_ptr < MEMORY_SIZE; (*pc_ptr)++) {
        #if 0
        printf(">>> Running Line %d ... ra: %d\n",*pc_ptr, mach->gpr[_ra_]);
        for (int i = 1; i < 11; i++)
            printf("%2d ", mach->memory[0xFF-i]);
        printf("\n");
        for (int i = 1; i < 11; i++)
            printf((mach->gpr[_sp_] == 0xFF-i) ? " ^ " : "   ");
        printf("\n");
        #endif
        uint32_t ins = mach->memory[*pc_ptr];
        fund_dispatch[OPCODE(ins)](ins, mach);
    }
    return 0;
}

/* Implementations of instructions */
void rfmt_f(uint32_t ins, MACHINE *mach)
{ rfmt_dispatch[FUNCT(ins)](ins, mach); }

void j_f(uint32_t ins, MACHINE *mach)
{ mach->pc = ADDR(ins) - 1; }

void jal_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[_ra_] = mach->pc + 1; mach->pc = ADDR(ins) - 1; }

void syscall_f(uint32_t ins, MACHINE *mach)
{ syscall_dispatch[ADDR(ins)](ins, mach); }

void addi_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] + ((int16_t) CONST(ins)); }

void andi_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] & ((int32_t) CONST(ins)); }

void ori_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] | ((int32_t) CONST(ins)); }

void slti_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] < ((int16_t) CONST(ins)); }

void seqi_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] == ((int16_t) CONST(ins)); }

void li_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = CONST(ins); }

void lli_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = (mach->gpr[REG0(ins)] & 0xFFFF0000) | CONST(ins); }

void lui_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = (CONST(ins) << 16) | (mach->gpr[REG0(ins)] & 0x0000FFFF); }

void lw_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->memory[CONST(ins) + mach->gpr[REG1(ins)]]; }

void sw_f(uint32_t ins, MACHINE *mach)
{ mach->memory[CONST(ins) + mach->gpr[REG1(ins)]] = mach->gpr[REG0(ins)]; }

void bze_f(uint32_t ins, MACHINE *mach)
{ if (mach->gpr[REG0(ins)] == 0)  mach->pc = mach->pc + ((int16_t) CONST(ins)); }

void bnz_f(uint32_t ins, MACHINE *mach)
{ if (mach->gpr[REG0(ins)] != 0)  mach->pc = mach->pc + ((int16_t) CONST(ins)); }

// R-format
void nop_f(uint32_t ins, MACHINE *mach)
{ /* No operation */}

void move_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] =  mach->gpr[REG1(ins)]; }

void add_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] + mach->gpr[REG2(ins)]; }

void sub_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] - mach->gpr[REG2(ins)]; }

void mul_f(uint32_t ins, MACHINE *mach)
{
    uint64_t product = mach->gpr[REG0(ins)] * mach->gpr[REG1(ins)];
    mach->lo = product & 0x00000000FFFFFFFF;
    mach->hi = product >> 32;
}

void div_f(uint32_t ins, MACHINE *mach)
{
    mach->lo = mach->gpr[REG0(ins)] / mach->gpr[REG1(ins)];
    mach->hi = mach->gpr[REG0(ins)] % mach->gpr[REG1(ins)];
}

void mflo_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->lo; }

void mfhi_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->hi; }

void sll_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] << SHAMT(ins); }

void srl_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] >> SHAMT(ins); }

void sra_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = ((int32_t) mach->gpr[REG1(ins)]) >> SHAMT(ins); }

void and_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] & mach->gpr[REG2(ins)]; }

void or_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] | mach->gpr[REG2(ins)]; }

void not_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = ~mach->gpr[REG1(ins)]; }

void slt_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] < mach->gpr[REG2(ins)]; }

void seq_f(uint32_t ins, MACHINE *mach)
{ mach->gpr[REG0(ins)] = mach->gpr[REG1(ins)] == mach->gpr[REG2(ins)]; }

void jr_f(uint32_t ins, MACHINE *mach)
{ mach->pc = mach->gpr[REG0(ins)] - 1; }

// Syscall
void sys_halt(uint32_t ins, MACHINE *mach)
{ j_f(JFMT_INS(j, MEMORY_SIZE), mach); }

void sys_newline(uint32_t ins, MACHINE *mach)
{ putchar('\n'); }

void sys_print_int(uint32_t ins, MACHINE *mach)
{ printf("%d", mach->gpr[_a0_]); }

void sys_print_hex(uint32_t ins, MACHINE *mach)
{ printf("%08x", mach->gpr[_a0_]); }

void sys_read_int(uint32_t ins, MACHINE *mach)
{ scanf("%d", mach->gpr + _v0_); }
