
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "assembler.h"
#include "machine.h"


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Need *ONE* input file.\n");
        return 0;
    }
    FILE *src = fopen(argv[1], "r");
    if (src == NULL) {
        printf("Cannot open file: %s\n", argv[1]);
        return 0;
    }
    init_environment();
    printf(">> Running assembler ...\n");
    OBJ_CODE *obj_code = assemble(src);
    MACHINE *mach = make_machine();
    init_machine(mach);
    load_memory(obj_code->ins, obj_code->ins_count, mach, 0);
    printf(">> Running Tiny Machine ...\n");
    run_machine(mach);
    printf("\n>> Done.\n");
    free_machine(mach);
    free_obj_code(obj_code);
    return 0;
}

