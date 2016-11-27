#pragma once

typedef struct symbol {
    int kind; // constant = 1, variable = 2
    char* name; // name of constant or variable
    int num; // number number is only set for constant
    int level; // L level level and modifier are only set for variable,
    int modifier; // M modifier but level is always 0 for tiny PL/0
} symbol_type;

typedef enum {
    constant = 1, variable, procedure
}symbol_kind;

typedef struct instruction
{
    int op;
    int l;
    int m;
}instruction;

//OP codes
typedef enum
{
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
} op_code;

//Operator codes
typedef enum
{
    RET = 0, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} operate_code;

void outputPM0(const char *fileName);

void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
symbol_type *get_symbol(char* name);
void put_symbol(int kind, char* name, int num, int level, int modifier);