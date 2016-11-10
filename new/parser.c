#include "lexer.h"
#include "parser.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TABLE_SIZE 100
#define CODE_SIZE 100

extern char *sourceCode;
token_type token;
LVAL lval;
symbol_type symbolTable[MAX_TABLE_SIZE];
int tablePos = 0;
instruction code[CODE_SIZE];
int cx = 0;

void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
symbol_type *get_symbol(char* name);
void put_symbol(int kind, char* name, int num, int level, int modifier);

// get symbol from symbol table
symbol_type *get_symbol(char* name)
{
    int i = 0;
    for(i = 0; i <= tablePos; i++)
    {
        if(strcmp(symbolTable[i].name,name) == 0)
            return symbolTable+i;
    }
    return NULL;
}

// put symbol to symbol table
void put_symbol(int kind, char* name, int num, int level, int modifier)
{
    symbolTable[tablePos].kind = kind;
    symbolTable[tablePos].name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(symbolTable[tablePos].name,name);
    symbolTable[tablePos].num = num;
    symbolTable[tablePos].level = level;
    symbolTable[tablePos].modifier = modifier;
    tablePos++;
}

// write PM0 code
void emit(int op, int l, int m)
{
    if(cx > CODE_SIZE)
        error(25); //"Code too long."
    else
    {
        code[cx].op = op; // opcode
        code[cx].l = l; // lexicographical level
        code[cx].m = m; // modifier
        cx++;
    }
}

// update global variable token and lval
void getNextToken()
{
    tokenUnit temp = lex();
    token = temp.type;
    lval = temp.lval;
}


void program()
{
    getNextToken();
    block();
    if (token != periodsym)
        error(8); //Period expected.
    emit(SIO,0,2);
}

void block()
{
    int num_vars = 0;
    if(token == constsym)
    {
        do
        {
            getNextToken();
            if(token != identsym)
                error(3); // const must be followed by identifier.
            char* id = lval.id;
            getNextToken();
            if(token != eqsym)
                error(2); // Identifier must be followed by =.
            getNextToken();
            if(token != numbersym)
                error(1); // = must be followed by a number.
            put_symbol(constant, id, lval.num, 0, 0);
            getNextToken();
        }while(token == commasym);
        if(token != semicolonsym) 
            error(4); // Semicolon missing.
        getNextToken();
    }
    if(token == varsym)
    {
        do
        {
            getNextToken();
            if(token != identsym)
                error(3); // var must be followed by identifier.
            num_vars++;
            put_symbol(variable, lval.id, 0, 0, 3 + num_vars);
            getNextToken();
        }while(token == commasym);
        if(token != semicolonsym)
            error(4); // Semicolon missing.
        getNextToken();
    }
    while(token == procsym) 
    {
        getNextToken();
        if(token != identsym)
            error(3); // procedure must be followed by identifier.
        getNextToken();
        if(token != semicolonsym)
            error(4); // Semicolon missing.
        getNextToken();
        block();
        if(token != semicolonsym) 
            error(4); // Semicolon missing.
        getNextToken();
    }
    emit(INC, 0, 4 + num_vars);
    statement();
}

void statement()
{
    if(token == identsym)
    {
        symbol_type* temp = get_symbol(lval.id);
        if(temp->kind == constant)
            error(26); // Cannot assign value to a constant.
        getNextToken();
        if(token != becomessym)
            error(27);
        getNextToken();
        expression();
        emit(STO,0,temp->modifier);
    }
    else if(token == callsym)
    {
        getNextToken();
        if(token != identsym)
            error(13); // call must be followed by an identifier.
        getNextToken();
    }
    else if(token == beginsym)
    {
        getNextToken();
        statement();
        while(token == semicolonsym)
        {
            getNextToken();
            statement();
        }
        if(token != endsym)
            error(28); // end expected.
        getNextToken();
    }
    else if(token == ifsym)
    {
        getNextToken();
        condition();
        if(token != thensym)
            error(15); // then expected.
        getNextToken();
        
        int ctemp = cx;
        emit(JPC, 0, 0);
        statement();
        code[ctemp].m = cx;
        
    }
    else if(token == whilesym)
    {
        int cx1 = cx;
        getNextToken();
        condition();
        int cx2 = cx;
        emit(JPC, 0, 0);
        if(token != dosym)
            error(17); // do expected.
        getNextToken();
        statement();
        emit(JMP, 0, cx1);
        code[cx2].m = cx;
    }
    else if(token == readsym)
    {
        getNextToken();
        if(token != identsym)
            error(29); // "read must be followed by identifier."
        emit(SIO,0,1);
        symbol_type* temp = get_symbol(lval.id);
        if(temp == NULL)
            error(30); // variable is not declared.
        if(temp->kind == constant)
            error(26); // Cannot assign value to a constant.
        emit(STO,0,temp->modifier);
        getNextToken();
    }
    else if(token == writesym)
    {
        getNextToken();
        if(token != identsym)
            error(31);

        symbol_type* temp = get_symbol(lval.id);
        if(temp == NULL)
            error(30); // variable is not declared.
        if(temp->kind == constant)
            emit(LIT,0,temp->num);
        else if(temp->kind == variable)
            emit(LOD,0,temp->modifier);
        emit(SIO,0,0);    
        getNextToken();        
    }
}

void condition()
{
    if(token == oddsym)
    {
        getNextToken();
        expression();
        emit(OPR,0,ODD);
    }
    else
    {
        expression();
        if(token < eqsym || token > geqsym )
            error(19); // Relational operator expected.
        int cond = token;
        int offset = eqsym - EQL;
        getNextToken();
        expression();
        emit(OPR,0, cond-offset);
    }
}

void expression()
{
    int addop;
    if(token == plussym || token == minussym)
    {
        addop = token;
        getNextToken();
        term();
        if(addop == minussym)
            emit(OPR, 0, NEG);
    }
    else
        term();
    while(token == plussym || token == minussym)
    {
        addop = token;
        getNextToken();
        term();
        if(addop == plussym)
            emit(OPR, 0, ADD);
        else
            emit(OPR, 0, SUB);
    }
}

void term()
{
    int mulop;
    factor();
    while(token == multsym || token == slashsym)
    {
        mulop = token;
        getNextToken();
        factor();
        if(mulop == multsym)
            emit(OPR, 0, MUL);
        else
            emit(OPR, 0, DIV);
    }
}

void factor()
{
    if(token == identsym)
    {
        symbol_type *temp = get_symbol(lval.id);
        if(temp->kind == constant)
            emit(LIT,0,temp->num);
        else if(temp->kind == variable)
            emit(LOD,0,temp->modifier);
        getNextToken();
    }
    else if(token == numbersym)
    {
        emit(LIT,0,lval.num);
        getNextToken();
    }
    else if(token = lparentsym)
    {
        getNextToken();
        expression();
        if(token != rparentsym)
            error(21); // Right parenthesis missing.
        getNextToken();
    }
    else
        error(23); // An expression cannot begin with this symbol.
}

void ouputPM0(const char *fileName)
{
    int i = 0;
    FILE *fp = fopen(fileName,"w");
    for(i=0; i<cx; i++)
        fprintf(fp,"%d %d %d\n",code[i].op,code[i].l,code[i].m);
    fclose(fp);
}

// int main(int argc, char *argv[])
// {
//     sourceCode = readInput(argv[1]);
//     if(sourceCode == NULL)
//     {
//         printf("error!\n");
//         exit(1);
//     }
//     program();
//     printf("No errors, program is syntactically correct.\n");
//     ouputPM0(argv[2]);
//     return 0;
// }