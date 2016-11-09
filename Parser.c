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
#define SIZE 500

//Struct for a symbol
typedef struct symbol
{
    int kind;
    char name[12];
    int val;
    int level;
    int addr;
} symbol;

//token type enum
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
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
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
void statement(symbol *symbolList);
void expression(symbol *symbolList);
void condition(symbol *symbolList);
void term(symbol *symbolList);
void factor(symbol *symbolList);
int isVar(char *name);

void getToken(symbol *symbolList);
void makeSymbol(symbol *symbolList);
void error(int value);

void printCode(FILE *out);
void emit(int op, int l, int m);
void getOldinstruction(int index, int code, int m);
int addLine();
int getLine();

symbol *createList(FILE *in);


int main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    //Create new symbol list at the start
    symbol *symbolList = createList(in);

    makeSymbol(symbolList);

    //Start scanner
    program(symbolList);

    //Generate the m code
    printCode(out);


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

    emit(SIO, 0, 2);


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


    //Add m code
    emit(INC, 0, numVars);

    //Call statement
    statement(symbolList);;
}


//Function to get the next token
void getToken(symbol *symbolList)
{
    token = symbolList[numToken].kind;
    numToken++;
}


//error function, prints based on which error occurs
void error(int value)
{
    switch(value)
    {
        case 1:
            printf("error 1: Use = instead of :=.\n");
            break;
        case 2:
            printf("error 2: = must be followed by a number.\n");
            break;
        case 3:
            printf("error 3: Identifier myst be followed by =.\n");
            break;
        case 4:
            printf("error 4: Const, Var, Procedure must be followed by an identifier.\n");
            break;
        case 5:
            printf("error 5: Semicolon or comma missing.\n");
            break;
        case 6:
            printf("error 6: Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("error 7: Statement expected.\n");
            break;
        case 8:
            printf("error 8: Incorrect symbol after statement part in block.\n");
            break;
        case 9:
            printf("error 9: Period expected.\n");
            break;
        case 10:
            printf("error 10: Semicolon between statements missing.\n");
            break;
        case 11:
            printf("error 11: Undeclared identifier.\n");
            break;
        case 12:
            printf("error 12: Assignment to constant or precedure not allowed.\n");
            break;
        case 13:
            printf("error 13: Assignment operator expected.\n");
            break;
        case 14:
            printf("error 14: Call must be folloqws by an identifier.\n");
            break;
        case 15:
            printf("error 15: Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("error 16: Then expected.\n");
            break;
        case 17:
            printf("error 17: Semicolon or } expected.\n");
            break;
        case 18:
            printf("error 18: Do expected.\n");
            break;
        case 19:
            printf("error 19: Incorrect symbol following statement.\n");
            break;
        case 20:
            printf("error 20: Relational operator expected.\n");
            break;
        case 21:
            printf("error 21: Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            printf("error 22: Right parenthesis is missing.\n");
            break;
        case 23:
            printf("error 23: Preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
            printf("error 24: Expression cannot bewin with this symbol.\n");
            break;
        case 25:
            printf("error 25: end expected.\n");
            break;
        case 26:
            printf("error 26: read must be followed by identifier.\n");
            break;
        case 27:
            printf("error 27: write must be followed by identifier.\n");
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


symbol *createList(FILE *in)
{
    int i = 0;
    int x = 1;

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


void statement(symbol *symbolList)
{
    int temp = 0;
    int tempLine[2];

    if(token == identSym)
    {
        if(!isVar(symbolList[numToken-1].name))
        {

            if(!isConstant(symbolList[numToken-1].name))
                error(12);
            else
                error(11);
        }

        getToken(symbolList);
        if(token != becomesSym)
            error(19);

        temp = m1;
        getToken(symbolList);
        expression(symbolList);

        emit(STO, 0, temp);

    }
    else if(token == callSym)
    {
        getToken(symbolList);
        if(token != identSym)
            error(23);

        emit(CAL, 0, m1);

        getToken(symbolList);
    }
    else if(token == beginSym)
    {
        getToken(symbolList);
        statement(symbolList);
        while(token == semicolonSym)
        {
            getToken(symbolList);
            statement(symbolList);
        }
        if(token != endSym)
            error(25);
        getToken(symbolList);
    }
    else if(token == ifSym)
    {
        tempLine[1] = 0;

        getToken(symbolList);
        condition(symbolList);

        tempLine[0] = addLine();

        if(token != thenSym)
            error(16);
        getToken(symbolList);
        statement(symbolList);

        if(token == elseSym)
        {
            tempLine[1] = addLine();
            line--;
            getToken(symbolList);
        }

        getOldinstruction(tempLine[0], JPC, getLine());

        if(tempLine[1])
        {
            statement(symbolList);
            getOldinstruction(tempLine[1], JMP, getLine());
        }
    }
    else if(token == whileSym)
    {
        getToken(symbolList);

        tempLine[0] = getLine();

        condition(symbolList);

        tempLine[1] = addLine();

        if(token != doSym)
            error(12);
        getToken(symbolList);
        statement(symbolList);

        emit(JMP, 0, line);
        getOldinstruction(tempLine[1], JPC, getLine());

    }
    else if(token == readSym)
    {
        emit(SIO, 0, 1);

        getToken(symbolList);
        //Comment for now
        /*if(token != identsym)
            error(26);
        */
        if(token == identSym)
        {
            if(isVar(symbolList[numToken-1].name))
                emit(STO, 0, m1);
            else if(isConstant(symbolList[numToken-1].name))
            {
                printf("Can't store\n");
                exit(1);
            }
            else
                error(11);
        }

        getToken(symbolList);
    }
    else if(token == writeSym)
    {
        getToken(symbolList);

        //Comment for now
        /*
        if(token != identsym)
            error(27);
            */
        if(token == identSym)
        {
            if(isVar(symbolList[numToken-1].name))
                emit(LOD, 0, m1);
            else if(isConstant(symbolList[numToken-1].name))
                emit(LIT, 0, m1);
            else
                error(11);
        }

        getToken(symbolList);
        emit(SIO, 0, 0);


    }
    else if(token == endSym)
        return;
    else
    {
        if(token == periodSym);
        else
            error(7);
    }
}

void condition(symbol *symbolList)
{
    if(token == oddSym)
    {
        getToken(symbolList);
        expression(symbolList);
        emit(OPR, 0, ODD);
    }
    else
    {
        int temp;
        expression(symbolList);
        if(token < eqSym || token > geqSym )
        {
            //error(20);
            if(token == becomesSym)
                error(1);
            error(13);

        }
        else
        {
            temp = token;
        }


        getToken(symbolList);
        expression(symbolList);

        if(temp == eqSym)
            emit(OPR, 0, EQL);
        else if(temp == neqSym)
            emit(OPR, 0, NEQ);
        else if(temp == lesSym)
            emit(OPR, 0, LSS);
        else if(temp == leqSym)
            emit(OPR, 0, LEQ);
        else if(temp == gtrSym)
            emit(OPR, 0, GTR);
        else if(temp == geqSym)
            emit(OPR, 0, GEQ);

    }
}

void expression(symbol *symbolList)
{
    int temp;


    if(token != plusSym && token != minusSym && token != numberSym && token != lparentSym && token != identSym)
        error(21);

    if(token == plusSym || token == minusSym)
        getToken(symbolList);



    term(symbolList);

    if(token == minusSym)
        emit(OPR, 0, NEG);

    while(token == plusSym || token == minusSym)
    {
        temp = token;


        getToken(symbolList);
        term(symbolList);

        if(temp == plusSym)
            emit(OPR, 0, ADD);
        if(temp == minusSym)
            emit(OPR, 0, SUB);

    }
}

void term(symbol *symbolList)
{
    int temp;

    factor(symbolList);
    while(token == multSym || token == slashSym)
    {
        temp = token;

        getToken(symbolList);
        factor(symbolList);

        if(temp == multSym)
            emit(OPR, 0, MUL);
        if(temp == slashSym)
            emit(OPR, 0, DIV);

    }
}

void factor(symbol *symbolList)
{
    if(token == identSym || token == numberSym)
    {
        if(isConstant(symbolList[numToken-1].name))
            emit(LIT, 0, m1);
        else if(isVar(symbolList[numToken-1].name))
            emit(LOD, 0, m1);
        else
            error(11);

        getToken(symbolList);
    }
    else if(token == numberSym)
    {
        emit(LIT, 0, symbolList[numToken-1].val);

        getToken(symbolList);
    }
    else if(token = lparentSym)
    {
        getToken(symbolList);
        expression(symbolList);
        if(token != rparentSym)
            error(22);
        getToken(symbolList);
    }
    else
        error(24);
}

void emit(int op, int l, int m)
{
    code[line].op = op;
    code[line].l = l;
    code[line].m = m;

    line++;
}

void getOldinstruction(int index, int op, int m)
{
    code[index].op = op;
    code[index].l = 0;
    code[index].m = m;
}

int isVar(char *name)
{
    int x = 0;
    while(symbolTable[x].kind != 0)
    {
        if(strcmp(symbolTable[x].name, name) == 0 && symbolTable[x].kind == 2)
        {
            m1 = symbolTable[x].addr;
            return 1;
        }
        x++;
    }
    return 0;
}

int addLine()
{
    return line++;
}

int getLine()
{
    return line;
}

void printCode(FILE *out)
{
    int i;



    for(i = 0; i < line; i++)
    {
       fprintf(out, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }

    fclose(out);
}
