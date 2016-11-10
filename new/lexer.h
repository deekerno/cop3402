#pragma once
#include <stddef.h>

typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
}token_type;


typedef union {
    char* id;
    int num;
}LVAL;

typedef struct tokenUnit
{
    LVAL lval;
    token_type type;
}tokenUnit;

char* readInput(const char* fileName);
tokenUnit lex();