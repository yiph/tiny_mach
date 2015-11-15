
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "machine.h"
#include "assembler.h"

#define MAX_LINE_SIZE 1000

#define DEBUG 0

#define FLAG(S) printf(#S"\n")
#define PRINT(I) printf("%d\n", (I))

typedef struct SYMBOL_TABLE ST;
ST *make_ST(void);
void free_ST(ST *st);
int contains(ST *st, char *key);
void put(char *key, uint32_t val, ST *st);
uint32_t get(char *key, ST *st);


typedef struct LINKED_LIST LIST;
LIST *make_list();
void free_list(LIST *list);
void append(uint32_t val, LIST *list);
void init_iterator(LIST *list);
int has_next(LIST *list);
uint32_t next(LIST *list);

typedef struct DYNAMIC_ARRAY DYNARRAY;
DYNARRAY *make_dynarray();
void free_dynarray(DYNARRAY *dynarray);
void assign_dynarray(DYNARRAY *dynarray, int index, int val);
int size_dynarray(DYNARRAY *dynarray);
int *static_array(DYNARRAY *dynarray);

char *get_token(char **token, char *src);
int is_end(char *line);
void report_and_abort(int line_num, char *line);
uint32_t lookup_table_and_report_error(char *token, ST *st, char *errormsg, int line_num, char *line);

#define CHECK_MALLOC(ptr)                                               \
    do {                                                                \
        if ((ptr) == NULL) {                                            \
            printf("Fail to allocate memory for "#ptr" at line: %d in "__FILE__"\n", __LINE__); \
            exit(0);                                                    \
        }                                                               \
    } while(0)

#define PUT_OPCODE_OP(OP_NAME)                  \
    put(#OP_NAME, _##OP_NAME##_, fund_table)
#define INIT_FUND_TABLE_I                       \
    OP_SET_FUND(PUT_OPCODE)
#define PUT_OPCODE_RFMT_OP(OP_NAME)             \
    put(#OP_NAME, _rfmt_, fund_table)
#define INIT_FUND_TABLE_II                      \
    OP_SET_RFMT(PUT_OPCODE_RFMT)
#define INIT_FUND_TABLE                         \
    INIT_FUND_TABLE_I;                          \
    INIT_FUND_TABLE_II

#define PUT_FUNCT_OP(OP_NAME)                   \
    put(#OP_NAME, _##OP_NAME##_, rfmt_table)
#define INIT_RFMT_TABLE                         \
    OP_SET_RFMT(PUT_FUNCT)

#define PUT_CODE_REG(REG_NAME)                  \
    put(#REG_NAME, _##REG_NAME##_, reg_table)
#define INIT_REG_TABLE                          \
    REG_SET(PUT_CODE)

#define PUT_CODE_SYSCALL(SYSCALL_NAME)                          \
    put(#SYSCALL_NAME, _sys_##SYSCALL_NAME##_, syscall_table)
#define INIT_SYSCALL_TABLE                      \
    SYSCALL_SET(PUT_CODE)

OBJ_CODE *assemble(FILE *src)
{
    ST *fund_table = make_ST();
    ST *rfmt_table = make_ST();
    ST *reg_table  = make_ST();
    ST *syscall_table = make_ST();
    INIT_FUND_TABLE;
    INIT_RFMT_TABLE;
    INIT_REG_TABLE;
    INIT_SYSCALL_TABLE;
    
    int tags_count = 0;
    ST *tags_num = make_ST();
    DYNARRAY *tags_addr = make_dynarray();
    int ins_count = 0;
    LIST *ins_list = make_list();
    LIST *pending_ins = make_list();
    char *buffer = malloc(MAX_LINE_SIZE * sizeof(char));
    CHECK_MALLOC(buffer);
    int line_num = 0;

    char *line;
    while (!feof(src)) {
        line_num++;
        line = fgets(buffer, MAX_LINE_SIZE, src);
        #if DEBUG
        printf("line %d: %s", line_num, buffer);
        #endif

        while (isspace(*line)) line++;
        if (is_end(line))      continue;
        char *token;
        line = get_token(&token, line);
        
            /* check tags */
        if (*line == ':') {
            int tag_code;
            if (contains(tags_num, token)) {
                tag_code = get(token, tags_num);
                free(token);
            }
            else {
                tag_code = tags_count++;
                put(token, tag_code, tags_num);
            }
            assign_dynarray(tags_addr, tag_code, ins_count);
            line++;
            while (isspace(*line)) line++;
            if (is_end(line))      continue;
            line = get_token(&token, line);
        }
        
            /* check instructions */
        if (isspace(*line)) {
            uint32_t ins_code;
            uint32_t opcode
                = lookup_table_and_report_error(token, fund_table,
                                                "Unknown instruction", line_num, buffer);
            uint32_t reg0 = 0;
            uint32_t reg1 = 0;
            uint32_t reg2 = 0;
            uint32_t shamt = 0;
            uint32_t funct = 0;
            uint32_t c = 0;
            uint32_t syscall_code = 0;

            switch (opcode) {
            case _rfmt_:
                reg2 = 0;
                shamt = 0;
                funct = lookup_table_and_report_error(token, rfmt_table,
                                                      "Unknown instruction", line_num, buffer);
                free(token);

                if (funct == _nop_) {
                    while (isspace(*line)) line++;
                    if (!is_end(line))      report_and_abort(line_num, buffer);
                    goto rfmt_done;
                }

                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                reg0 = lookup_table_and_report_error(token, reg_table,
                                                     "Unknown register", line_num, buffer);
                free(token);
                
                if (funct == _jr_ || funct == _mfhi_ || funct == _mflo_) {
                    while (isspace(*line)) line++;
                    if (!is_end(line))      report_and_abort(line_num, buffer);
                    goto rfmt_done;
                }
                

                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                reg1 = lookup_table_and_report_error(token, reg_table, "Unknown register", line_num, buffer);
                free(token);

                if (funct == _move_ || funct == _mul_ || funct == _div_ || funct == _not_) {
                    while (isspace(*line)) line++;
                    if (!is_end(line))      report_and_abort(line_num, buffer);
                    goto rfmt_done;
                }
                
                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                if (funct == _sll_ || funct == _srl_ || funct == _sra_) {
                    shamt = atoi(token);
                }
                else {
                    reg2 = lookup_table_and_report_error(token, reg_table,
                                                         "Unknown register", line_num, buffer);
                }
                free(token);

                rfmt_done:
                ins_code = opcode << OPCODE_OFFSET
                    | reg0 << REG0_OFFSET
                    | reg1 << REG1_OFFSET
                    | reg2 << REG2_OFFSET
                    | shamt << SHAMT_OFFSET
                    | funct << FUNCT_OFFSET;
                break;

            case _j_:
            case _jal_:
                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                int tag_code;
                if (contains(tags_num, token)) {
                    tag_code = get(token, tags_num);
                    free(token);
                }
                else {
                    put(token, tags_count, tags_num);
                    tag_code = tags_count++;
                }
                append(ins_count, pending_ins);
                ins_code = opcode << OPCODE_OFFSET | tag_code << ADDR_OFFSET;
                break;
                
            case _syscall_:
                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                syscall_code = lookup_table_and_report_error(token, syscall_table,
                                                             "Unknown syscall", line_num, buffer);
                ins_code = opcode << OPCODE_OFFSET | syscall_code << ADDR_OFFSET;
                break;
                
            default: // I-format
                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                reg0 = lookup_table_and_report_error(token, reg_table,
                                                     "Unknown register", line_num, buffer);
                free(token);

                if (opcode != _li_ && opcode != _lli_ && opcode != _lui_
                    && opcode != _bze_ && opcode != _bnz_) {
                    while (isspace(*line)) line++;
                    if (is_end(line))      report_and_abort(line_num, buffer);
                    line = get_token(&token, line);
                    reg1 = lookup_table_and_report_error(token, reg_table,
                                                         "Unknown register", line_num, buffer);
                    free(token);
                }
                
                while (isspace(*line)) line++;
                if (is_end(line))      report_and_abort(line_num, buffer);
                line = get_token(&token, line);
                if (opcode == _bze_ || opcode == _bnz_) {
                    if (contains(tags_num, token)) {
                        c = get(token, tags_num);
                        free(token);
                    }
                    else {
                        put(token, tags_count, tags_num);
                        c = tags_count++;
                    }
                    append(ins_count, pending_ins);
                }
                else {
                    c = strtol(token, NULL, 0);
                    free(token);
                }
                ins_code = opcode << OPCODE_OFFSET
                    | reg0 << REG0_OFFSET
                    | reg1 << REG1_OFFSET
                    | (c & CONST_FIELD ) << CONST_OFFSET;
                break;
            }
            
            while (isspace(*line)) line++;
            if (!is_end(line))      report_and_abort(line_num, buffer);
            
            append(ins_code, ins_list);
            ins_count++;
        }
        else {
            report_and_abort(line_num, buffer);
        }
        
    }
    
    free_ST(fund_table);
    free_ST(rfmt_table);
    free_ST(reg_table);
    free_ST(syscall_table);
    free_ST(tags_num);
    free(buffer);
     
        /* rearrange and adjust instructions */
    uint32_t *ins = malloc(ins_count * sizeof(uint32_t));
    CHECK_MALLOC(ins);
    init_iterator(ins_list);
    int i = 0;
    for (int i = 0; has_next(ins_list); i++)
        ins[i] = next(ins_list);

    LIST *undetermined_addr_list = make_list();
    int undetermined_addr_count = 0;
    int *tags_addr_query = static_array(tags_addr);
//    int tags_addr_num = size_dynarray(tags_addr);
    init_iterator(pending_ins);
    while (has_next(pending_ins)) {
        int pos = next(pending_ins);
        int opcode = OPCODE(ins[pos]);
        if (opcode == _bze_ || opcode == _bnz_) {
            uint32_t temp_addr = CONST(ins[pos]);
            uint32_t real_addr = tags_addr_query[temp_addr];
            int32_t offset = real_addr - pos - 1;
            ins[pos] = (ins[pos] & (~CONST_FIELD))
                | ((((uint32_t) offset) & CONST_FIELD) << CONST_OFFSET);
        }
        else { // J-format
            uint32_t temp_addr = ADDR(ins[pos]);
            uint32_t real_addr = tags_addr_query[temp_addr];
            ins[pos] = (ins[pos] & (~ADDR_FIELD))
                | ((real_addr & ADDR_FIELD) << ADDR_OFFSET);
            append(pos, undetermined_addr_list);
        }
    }
    
    free_dynarray(tags_addr);
    free_list(ins_list);
    free_list(pending_ins);
        
        /* make object code */
    int *undetermined_addr = malloc(undetermined_addr_count * sizeof(int));
    CHECK_MALLOC(undetermined_addr);
    init_iterator(undetermined_addr_list);
    for (i = 0; has_next(undetermined_addr_list); i++)
        undetermined_addr[i] = next(undetermined_addr_list);

    #if DEBUG
    printf("\nMachine code:\n");
    for (i = 0; i < ins_count; i++)
        printf("ins %d: %08x\n", i, ins[i]);
    #endif
    
    OBJ_CODE *obj_code = malloc(sizeof(OBJ_CODE));
    CHECK_MALLOC(obj_code);
    obj_code->ins = ins;
    obj_code->ins_count = ins_count;
    obj_code->undetermined_addr = undetermined_addr;
    return obj_code;
}

void free_obj_code(OBJ_CODE *obj)
{
    free(obj->ins);
    free(obj->undetermined_addr);
    free(obj);
}

int is_end(char *line)
{
    return *line == '\0' || *line == '#';
}

char *get_token(char **token, char *src)
{
    int i = 0;
    while (src[i] != '\0' && !isspace(src[i]) && src[i] != ':' && src[i] != '#')
        i++;
    *token = malloc((i+1) * sizeof(char));
    CHECK_MALLOC(*token);
    strncpy(*token, src, i);
    (*token)[i] = '\0';
        //printf("%s\n", *token);
    return src + i;
}
void report_and_abort(int line_num, char *line)
{
    printf("Error: Bad instruction at line %d: \n\t%s\n", line_num, line);
    exit(0);
}

uint32_t lookup_table_and_report_error(char *token, ST *st, char *errormsg, int line_num, char *line)
{
    if (contains(st, token)) {
        return get(token, st);
    }
    else {
        printf("Error: %s at line %d: %s\n\t%s\n", errormsg, line_num, token, line);
        exit(0);
    }
}

/* Symbol table */
typedef struct TREE_NODE 
{
    char *key;
    uint32_t val;
    struct TREE_NODE *left;
    struct TREE_NODE *right;
} T_NODE;


struct SYMBOL_TABLE
{
    T_NODE *root;
};
    
ST *make_ST(void)
{
    ST *newST = malloc(sizeof(ST));
    CHECK_MALLOC(newST);
    newST->root = NULL;
    return newST;
}

void free_ST_node(T_NODE *node)
{
    if (node == NULL) return;
    free(node->key);
    if (node->left != NULL)
        free_ST_node(node->left);
        //printf("key: %s  \t val: %d\n", node->key, node->val);
    if (node->right != NULL)
        free_ST_node(node->right);
    free(node);
}

void free_ST(ST *st)
{
    free_ST_node(st->root);
    free(st);
}

int contains(ST *st, char *key)
{
    T_NODE *current = st->root;
    while (current != NULL) {
        int cmp = strcmp(key, current->key);
        if      (cmp < 0) current = current->left;
        else if (cmp > 0) current = current->right;
        else              return 1;
    }
    return 0;
}

T_NODE *recursively_put(char *key, uint32_t val, T_NODE *node)
{
    if (node != NULL) {
        int cmp = strcmp(key, node->key);
        if      (cmp < 0) node->left  = recursively_put(key, val, node->left);
        else if (cmp > 0) node->right = recursively_put(key, val, node->right);
        else {
            free(node->key);
            node->key = key;
            node->val = val;
        }
        return node;
    }
    else {
        T_NODE *new_node = malloc(sizeof(T_NODE));
        CHECK_MALLOC(new_node);
        new_node->key = key;
        new_node->val = val;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }
}

void put(char *key, uint32_t val, ST *st)
{
    st->root = recursively_put(key, val, st->root);
}

uint32_t get(char *key, ST *st)
{
    T_NODE *current = st->root;
    while (current != NULL) {
        int cmp = strcmp(key, current->key);
        if      (cmp < 0) current = current->left;
        else if (cmp > 0) current = current->right;
        else              return current->val;
    }
    return -1;
}


/* List */
typedef struct LIST_NODE
{
    uint32_t val;
    struct LIST_NODE *next;
} L_NODE;

struct LINKED_LIST
{
    L_NODE *head;
    L_NODE *tail;
    L_NODE *current;
};

LIST *make_list()
{
    LIST *list = malloc(sizeof(LIST));
    CHECK_MALLOC(list);
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    return list;
}

void free_list(LIST *list)
{
    if (list->head != NULL) {
        L_NODE *current = list->head;
        while (current != NULL) {
            L_NODE *next = current->next;
            free(current);
            current = next;
        }
    }
    free(list);
}

void append(uint32_t val, LIST *list)
{
    L_NODE *new_node = malloc(sizeof(L_NODE));
    CHECK_MALLOC(new_node);
    new_node->val = val;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        list->tail->next = new_node;
        list->tail = list->tail->next;
    }
}

void init_iterator(LIST *list)
{
    list->current = list->head;
}

int has_next(LIST *list)
{
    return list->current != NULL;
}

uint32_t next(LIST *list)
{
    uint32_t temp = list->current->val;
    list->current = list->current->next;
    return temp;
}

/* Dynamic array */
struct DYNAMIC_ARRAY
{
    int *array;
    int size;
    int capacity;
};

DYNARRAY *make_dynarray()
{
    DYNARRAY *dynarray = malloc(sizeof(DYNARRAY));
    CHECK_MALLOC(dynarray);
    dynarray->size = 0;
    dynarray->capacity = 100;
    dynarray->array = malloc(dynarray->capacity * sizeof(int));
    CHECK_MALLOC(dynarray->array);
    for (int i = 0; i < dynarray->capacity; i++)
        dynarray->array[i] = -1;
    return dynarray;
}
        
void free_dynarray(DYNARRAY *dynarray)
{
    free(dynarray->array);
    free(dynarray);
}

void assign_dynarray(DYNARRAY *dynarray, int index, int val)
{
    if (index >= dynarray->size)
        dynarray->size = index + 1;
    if (index >= dynarray->capacity) {
        int *temp = malloc(2 * dynarray->capacity * sizeof(int));
        CHECK_MALLOC(temp);
        for (int i = 0; i < dynarray->capacity; i++)
            temp[i] = dynarray->array[i];
        dynarray->capacity *= 2;
        for (int i = dynarray->capacity / 2; i < dynarray->capacity; i++)
            temp[i] = -1;
    }
    dynarray->array[index] = val;
}

int size_dynarray(DYNARRAY *dynarray)
{
    return dynarray->size;
}

int *static_array(DYNARRAY *dynarray)
{
    return dynarray->array;
}

    


