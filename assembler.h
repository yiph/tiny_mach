#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

typedef struct OBJECT_CODE 
{
    int *undetermined_addr;
    int ins_count;
    uint32_t *ins;
} OBJ_CODE;

OBJ_CODE *assemble(FILE *src);
void free_obj_code(OBJ_CODE *obj);

#endif
