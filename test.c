
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "machine.h"



uint32_t factorial_ins[1000] = {
    SYSCALL_INS(sys_read_int),
    RFMT_INS(move, a0, v0, nil, 0),
    JFMT_INS(jal, 7), // call proc.
    RFMT_INS(move, a0, v0, nil, 0),
    SYSCALL_INS(sys_print_int),
    SYSCALL_INS(sys_newline),
    JFMT_INS(j, MEMORY_SIZE-1), // exit
    IFMT_INS(addi, sp, sp, -2), // factorial proc.
    IFMT_INS(sw,   ra, sp,  0),
    IFMT_INS(sw,   a0, sp,  1),
    IFMT_INS(bnz,  a0,nil,  3),
    IFMT_INS(li,   v0,nil,  1), // when n = 0
    IFMT_INS(addi, sp, sp,  2),
    RFMT_INS(jr,   ra,nil, nil, 0),
    IFMT_INS(addi, a0, a0, -1), // when n > 0
    JFMT_INS(jal, 7), // recursively call factorial proc.
    IFMT_INS(lw,   a0, sp,  1),
    RFMT_INS(mul,  v0, a0, nil, 0),
    RFMT_INS(mflo, v0,nil, nil, 0),
    RFMT_INS(move, a0, v0, nil, 0),
    IFMT_INS(lw,   ra, sp,  0),
    IFMT_INS(addi, sp, sp,  2),
    RFMT_INS(jr,   ra,nil, nil, 0)
};

int main(void)
{
    init_environment();
    MACHINE *mach = make_machine();
    
    init_machine(mach);
    load_memory(factorial_ins, 100, mach, 0);
    run_machine(mach);
    
    free_machine(mach);
    return 0;
}
