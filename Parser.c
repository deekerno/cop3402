// Team Name: Compiler Group 13
// Alexander deCurnou
// Peter Hoang
// Shuaicheng Ma
// Stephen Rigdon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_TABLE_SIZE 100
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

//Struct for a symbol
typedef struct symbol
{
    int kind;
    char name[12];
    int val;
    int level;
    int addr;
} symbol;

//Token type enum
typedef enum
{
     nulsym = 1, identSym, numberSym, plusSym, minusSym,
    multSym,  slashSym, oddSym, eqSym, neqSym, lesSym, leqSym,
    gtrSym, geqSym, lparentSym, rparentSym, commaSym, semicolonSym,
    periodSym, becomesSym, beginSym, endSym, ifSym, thenSym,
    whileSym, doSym, callSym, constSym, varSym, procSym, writeSym,
    readSym , elseSym
} token_type;

//instruction
typedef struct instruction
{
    int op;
    int l;
    int m;
} instruction;

//OP codes
typedef enum
{
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO1, SIO2, SIO3
} op_code;

//Operator codes
typedef enum
{
    RET = 0, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} operate_code;


//Global variables
token_type token;
int numToken = 0;
int line = 0;
int numVars = 0;
int m1 = 0;
instruction code[SIZE];
symbol symbolTable[MAX_TABLE_SIZE];

//Functions
void program(symbol *symbolList);
void block(symbol *symbolList);


void getToken(symbol *symbolList);
void makeSymbol(symbol *symbolList);
void error(int value);


symbol *createList();


int main()
{
    //Create new symbol list at the start
    symbol *symbolList = createList();

    makeSymbol(symbolList);

    //Start scanner
    program(symbolList);

    return 0;
}

void program(symbol *symbolList)
{
    getToken(symbolList);

    //Call block function
    block(symbolList);

    //Check if it ends in a period
    if (token != periodSym)
        error(9);

    else
        printf("No errors, program is syntactically correct.\n");


}


void block(symbol *symbolList)
{
    //Depending on what the next token is determines what block should do
    if(token == constSym)
    {
        do
        {
            getToken(symbolList);

            if(token != identSym)
                error(4);

            getToken(symbolList);

            if(token != eqSym)
            {
                if(token == becomesSym)
                    error(1);

                else
                    error(3);
            }

            getToken(symbolList);

            if(token != numberSym)
                error(2);

            getToken(symbolList);

        } while (token == commaSym);

        if(token != semicolonSym)
            error(5);

        getToken(symbolList);
    }

    if(token == varSym)
    {
        do
        {
            getToken(symbolList);
            if(token != identSym)
                error(4);

            getToken(symbolList);


        } while (token == commaSym);

        if(token != semicolonSym)
            error(5);

        getToken(symbolList);
    }

    if(token == procSym)
    {
        do
        {
            getToken(symbolList);

            if(token != identSym)
                error(4);

            getToken(symbolList);

            if(token != semicolonSym)
                error(5);

            block(symbolList);

            if(token != semicolonSym)
                error(5);

            getToken(symbolList);
        } while (token == procSym);
    }


    //Call statement
    statement(symbolList);
}


//Function to get the next token
void getToken(symbol *symbolList)
{
    token = symbolList[numToken].kind;
    numToken++;
}


//Error function, prints based on which error occurs
void error(int value)
{
    switch(value)
    {
        case 1:
            printf("Error 1: Use = instead of :=.\n");
            break;
        case 2:
            printf("Error 2: = must be followed by a number.\n");
            break;
        case 3:
            printf("Error 3: Identifier myst be followed by =.\n");
            break;
        case 4:
            printf("Error 4: Const, Var, Procedure must be followed by an identifier.\n");
            break;
        case 5:
            printf("Error 5: Semicolon or comma missing.\n");
            break;
        case 6:
            printf("Error 6: Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("Error 7: Statement expected.\n");
            break;
        case 8:
            printf("Error 8: Incorrect symbol after statement part in block.\n");
            break;
        case 9:
            printf("Error 9: Period expected.\n");
            break;
        case 10:
            printf("Error 10: Semicolon between statements missing.\n");
            break;
        case 11:
            printf("Error 11: Undeclared identifier.\n");
            break;
        case 12:
            printf("Error 12: Assignment to constant or precedure not allowed.\n");
            break;
        case 13:
            printf("Error 13: Assignment operator expected.\n");
            break;
        case 14:
            printf("Error 14: Call must be folloqws by an identifier.\n");
            break;
        case 15:
            printf("Error 15: Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("Error 16: Then expected.\n");
            break;
        case 17:
            printf("Error 17: Semicolon or } expected.\n");
            break;
        case 18:
            printf("Error 18: Do expected.\n");
            break;
        case 19:
            printf("Error 19: Incorrect symbol following statement.\n");
            break;
        case 20:
            printf("Error 20: Relational operator expected.\n");
            break;
        case 21:
            printf("Error 21: Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            printf("Error 22: Right parenthesis is missing.\n");
            break;
        case 23:
            printf("Error 23: Preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
            printf("error 24: Expression cannot bewin with this symbol.\n");
            break;



    }

    //End program
    exit(1);
}


void makeSymbol(symbol *symbolList)
{
    int counter = 0;
    int symbolCounter = 0;



    while(1)
    {
        if(symbolList[counter].kind == 29)
        {
            counter++;

            while(symbolList[counter].kind != semicolonSym)
            {
                if(symbolList[counter].kind == 17)
                    counter++;
                symbolTable[symbolCounter].kind = 2;
                strcpy(symbolTable[symbolCounter].name, symbolList[counter].name);
                symbolTable[symbolCounter].level = 0;
                symbolTable[symbolCounter].addr = numVars;
                numVars++;
                symbolCounter++;
                counter++;
            }
        }

        else if(symbolList[counter].kind == 28)
        {
            counter++;

            while(symbolList[counter].kind != semicolonSym)
            {
                if(symbolList[counter].kind == 17)
                    counter++;
                symbolTable[symbolCounter].kind = 1;
                strcpy(symbolTable[symbolCounter].name, symbolList[counter].name);
                counter++;
                counter++;
                symbolTable[symbolCounter].val = symbolList[counter].val;
                symbolCounter++;
                counter++;
            }
        }

        else if(symbolTable[counter].kind == 0)
            break;

        counter++;
    }
}


symbol *createList()
{
    int i = 0;
    int x = 1;
    FILE *in = fopen("lexemelist.txt", "r");
    symbol *symbolList = malloc(sizeof(symbol) *1000);

    int c = 0;
    int counter = -1;
    int contin = 0;
    int position = 0;

    while(1)
    {
        c = fgetc(in);
        if(c == EOF)
            break;

        if(c == ' ' || c == '\n')
        {
            if(symbolList[counter].kind == 3)
            {
                c = fgetc(in);
                symbolList[counter].val = (c - '0');
                c = fgetc(in);

                while(!i)
                {
                    if(c == ' ' || c == '\n')
                        i = 1;
                    else
                    {
                        symbolList[counter].val = symbolList[counter].val *10 + (c - '0');
                        c = fgetc(in);
                    }
                }
            }

            i = 0;

            contin = 0;
        }
        else
        {
            if(isdigit(c))
            {
                position = 0;
                if(contin == 0)
                {
                    counter++;
                    contin = 1;
                    symbolList[counter].kind = c - '0';
                }

                else
                {
                    symbolList[counter].kind = symbolList[counter].kind * 10 + (c - '0');
                }
            }
            else
            {
                symbolList[counter].name[position] = c;
                position++;

                while(x)
                {
                    c = fgetc(in);
                    if(c == ' ' || c == '\n')
                        x = 0;
                    else
                    {
                        symbolList[counter].name[position] = c;
                        position++;
                    }
                }
                contin = 0;
            }
        }
    }

    return symbolList;
}

int isConstant(char *name)
{
    int x = 0;
    while(symbolTable[x].kind != 0)
    {
        if(strcmp(symbolTable[x].name, name) == 0 && symbolTable[x].kind == 1)
        {
            m1 = symbolTable[x].val;
            return 1;
        }
        x++;
    }
    return 0;
}


