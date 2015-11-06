
#include <stdlib.h>
#include <stdio.h>
#include "machine.h"

#define OP_SET_FUND(SENT)                               \
    SENT##_OP(rfmt);                                    \
    SENT##_OP(j);                                       \
    SENT##_OP(jal);                                     \
    SENT##_OP(syscall);                                 \
    SENT##_OP(addi);                                    \
    SENT##_OP(subi);                                    \
    SENT##_OP(andi);                                    \
    SENT##_OP(ori);                                     \
    SENT##_OP(slti);                                    \
    SENT##_OP(seqi);                                    \
    SENT##_OP(lui);                                     \
    SENT##_OP(lw);                                      \
    SENT##_OP(sw);                                      \
    SENT##_OP(bze);                                     \
    SENT##_OP(bnz)
    

#define OP_SET_RFMT(SENT)                               \
    SENT##_OP(move);                                    \
    SENT##_OP(add);                                     \
    SENT##_OP(sub);                                     \
    SENT##_OP(mul);                                     \
    SENT##_OP(div);                                     \
    SENT##_OP(mflo);                                    \
    SENT##_OP(mfhi);                                    \
    SENT##_OP(sll);                                     \
    SENT##_OP(srl);                                     \
    SENT##_OP(sra);                                     \
    SENT##_OP(and);                                     \
    SENT##_OP(or);                                      \
    SENT##_OP(not);                                     \
    SENT##_OP(slt);                                     \
    SENT##_OP(seq);                                     \
    SENT##_OP(jr)
    

#define OP_SET_ALL(SENT)                                \
    OP_SET_FUND(SENT);                                  \
    OP_SET_RFMT(SENT)

#define DECLARE_FUNCT_OP(OP_NAME)                                 \
    void OP_NAME##_f(uint32_t ins, struct machine *mach)

#define ASSIGN_FUND_DISPATCH_OP(OP_NAME)        \
    fund_dispatch[_##OP_NAME##_] = OP_NAME##_f

#define INIT_FUND_DISPATCH                                              \
    void (*fund_dispatch[MAX_OPCODE])(uint32_t ins, struct machine *mach); \
    OP_SET_FUND(ASSIGN_FUND_DISPATCH)

#define ASSIGN_RFMT_DISPATCH_OP(OP_NAME)        \
    rfmt_dispatch[_##OP_NAME##_] = OP_NAME##_f

#define INIT_RFMT_DISPATCH                                              \
    void (*rfmt_dispatch[MAX_FUNCT])(uint32_t ins, struct machine *mach); \
    OP_SET_RFMT(ASSIGN_RFMT_DISPATCH)

/* Declare all functions corresponding to instructions */
OP_SET_ALL(DECLARE_FUNCT);


int run(struct machine *mach);



int main(void)
{
        //struct machine mach = malloc(sizeof(struct machine));
    
    return 0;
}

int run(struct machine *mach)
{
    INIT_FUND_DISPATCH;
    INIT_RFMT_DISPATCH;
    
    return 0;
    
}

/* Implementions of instructions */

void rfmt_f(uint32_t ins, struct machine *mach)
{}

void j_f(uint32_t ins, struct machine *mach)
{}

void jal_f(uint32_t ins, struct machine *mach)
{}

void syscall_f(uint32_t ins, struct machine *mach)
{}

void addi_f(uint32_t ins, struct machine *mach)
{}

void subi_f(uint32_t ins, struct machine *mach)
{}

void andi_f(uint32_t ins, struct machine *mach)
{}

void ori_f(uint32_t ins, struct machine *mach)
{}

void slti_f(uint32_t ins, struct machine *mach)
{}

void seqi_f(uint32_t ins, struct machine *mach)
{}

void lui_f(uint32_t ins, struct machine *mach)
{}

void lw_f(uint32_t ins, struct machine *mach)
{}

void sw_f(uint32_t ins, struct machine *mach)
{}

void bze_f(uint32_t ins, struct machine *mach)
{}

void bnz_f(uint32_t ins, struct machine *mach)
{}

void move_f(uint32_t ins, struct machine *mach)
{}

void add_f(uint32_t ins, struct machine *mach)
{}

void sub_f(uint32_t ins, struct machine *mach)
{}

void mul_f(uint32_t ins, struct machine *mach)
{}

void div_f(uint32_t ins, struct machine *mach)
{}

void mflo_f(uint32_t ins, struct machine *mach)
{}

void mfhi_f(uint32_t ins, struct machine *mach)
{}

void sll_f(uint32_t ins, struct machine *mach)
{}

void srl_f(uint32_t ins, struct machine *mach)
{}

void sra_f(uint32_t ins, struct machine *mach)
{}

void and_f(uint32_t ins, struct machine *mach)
{}

void or_f(uint32_t ins, struct machine *mach)
{}

void not_f(uint32_t ins, struct machine *mach)
{}

void slt_f(uint32_t ins, struct machine *mach)
{}

void seq_f(uint32_t ins, struct machine *mach)
{}

void jr_f(uint32_t ins, struct machine *mach)
{}


