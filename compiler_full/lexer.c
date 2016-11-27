// Simple Lexer
// Author: Shuaicheng Ma
// 10/02/2016

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_LIST_SIZE 1024
#define MAX_TOKEN_SIZE 12
#define MAX_NUMBER 65535
#define FALSE 0
#define TRUE 1
typedef unsigned char BOOL;


char *sourceCode;
tokenUnit * tokenList;


const char *keyword[] = {"null", "begin", "call", "const", "do", "else", "end", "if",
"odd", "procedure", "read", "then", "var", "while", "write"};

const int keyword_type[] = {nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym,
ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

// arithmetic operators
// comparison operators (note that = is also used to assign the value to a constant)
// assignment and some special symbols
const char *symbols[] = {"+","-","*","/","=","<>","<","<=",">",">=",
"(",")",",",";",".",":="};

const int symbols_type[] = {plussym, minussym, multsym, slashsym, eqsym, neqsym,
lessym, leqsym, gtrsym, geqsym,lparentsym, rparentsym, 
commasym, semicolonsym,periodsym, becomessym};

char* removeComment(char* sourceCode);
int getToken(char* code);
char* splitToken(char* token,int *index);
void regToken(char* token, int isSymbol, int pos);
int isKeyword(char* token);
BOOL isIdent(char* token);
BOOL isNum(char* token);

// read input file
// put everything which includes newline, space to 1D char array
// build 1D char array, so we can use build-in string function
char* readInput(const char* fileName)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    pFile = fopen ( fileName , "rb" );
    if (pFile==NULL) {fputs ("File error\n",stderr); exit (1);}
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error\n",stderr); exit (1);}
    
    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error\n",stderr); exit (1);}
    
    /* the whole file is now loaded in the memory buffer. */
    // terminate
    fclose (pFile);
    
    return buffer;
}


// replace comments with space from source code
// output raw source code
char* removeComment(char* sourceCode)
{
    // make a copy of source code 
    char *raw = (char*)malloc(strlen(sourceCode)+1);
    strncpy(raw,sourceCode,strlen(sourceCode)+1);

    if(strstr(raw,"*/") && !strstr(raw,"/*"))
    {
        fprintf(stderr,"missing open comment symbol /*\n");
        exit(1);
    }
    //search occurrence of open comment symbol /*
    char *open = strstr(raw,"/*");
    char *close = strstr(raw,"*/");
    while(open)
    {
        
        // if close comment symbol exist,
        // replce comment with space
        // else throw error message
        if(close)
        {
            // if close comment symnol */ occurrs before open comment symbol /*
            // throw error message
            if(open > close)
            {
                fprintf(stderr,"close comment symnol */ occurrs before open comment symbol /*\n");
                exit(1);
            }
            // memset(open,' ',strlen(open)-strlen(close)+2);
            // replace all character in comment with space character
            // except new line character
            int size = strlen(open)-strlen(close)+2;
            int i;
            for(i=0;i<size;i++)
            {
                if(open[i] != '\n')
                    open[i] = ' ';
            }
        }
        else
        {
            fprintf(stderr,"missing close comment symbol */\n");
            exit(1);
        }
        close = strstr(open+2,"*/");
        open = strstr(open+2,"/*");
        if(close && !open)
        {
            fprintf(stderr,"missing open comment symbol /*\n");
            exit(1);
        }
    }
    return raw;
}

// tokenize the string
int getToken(char* code)
{
    // make a copy of original code, since strtok() modifies string
    char *raw = (char*)malloc(strlen(code)+1);
    strncpy(raw,code,strlen(code)+1);
    
    char* token = NULL;
    char* pch = NULL;
    int pos = 0;
    int index = 0;
    char temp[MAX_TOKEN_SIZE+1] = {0};
    token = strtok(raw," \n\t\r");
    while(token)
    {
        //printf("%s\n",token);
        strncpy(temp,token,MAX_TOKEN_SIZE+1);
        pch = splitToken(temp,&index);
        while(pch)
        {
            // pch == 0 when leading characters are symbols
            if(*pch != 0)
                regToken(pch,-1,pos++);
            // index == 1 when no symbol in token
            if(index != -1)
                regToken((char*)symbols[index],symbols_type[index],pos++);
            pch = splitToken(NULL,&index);
        }
        token = strtok(NULL," \n\t\r");
    }
    return pos;
}    

// C strtok() style function call
// first call takes a string
// in sequential call, replace string with NULL
// This function delimits string by symbols:
// Method: replace delimiter by '\0'
// return a pointer which point at beginning of string(or last string)
// if symbol is found in string, set symbol_index and function will return ""ake
// notice "" is not equivalent to NULL
char* splitToken(char* token, int *symbol_index)
{
    // static variable to store last position internally
    static char* last;
    int size = sizeof(symbols)/sizeof(char*);
    char* pch = token;
    // save the beginning postion of string
    char* origin = token;
    if(token == NULL)
    {
        pch = last;
        origin = last;
    }
    int i = -1;
    while(pch && *pch)
    {
        for(i = 0; i < size; i++)
        {
            *symbol_index = i;
            // 2-character symbol
            if(symbols[i][0] == pch[0] && pch[1] != 0 && symbols[i][1] == pch[1])
            {
                last = pch + 2;
                pch[0] = 0;
                pch[1] = 0;
                return origin;
                
            }
            // 1-character symbol
            else if(symbols[i][0] == pch[0] && symbols[i][1] == 0 )
            {
                last = pch + 1;
                pch[0] = 0;
                return origin;
            }
        }
        pch++;
    }
    // no delimiter symbol found
    *symbol_index = -1;
    last = NULL;
    return origin;
}

void regToken(char* token, int isSymbol, int pos)
{
    if(pos >= MAX_LIST_SIZE)
    {
        fprintf(stderr,"out of memory.\n");
        exit(1);
    }
    int type = 0;
    int temp = 0;
    if(isSymbol != -1)
        type = isSymbol;
    else if((temp = isKeyword(token)) != -1)
        type = temp;
    else if(isNum(token))
    {
        type = numbersym;
        tokenList[pos].lval.num = atoi(token);
    }
    else if(isIdent(token))
    {
        type = identsym;
        int size = strlen(token)+1;
        tokenList[pos].lval.id = (char*)malloc(sizeof(char)*size);
        memcpy(tokenList[pos].lval.id,token,size);
    }
    else
    {
        fprintf(stderr,"%s: Invalid token\n",token);
        exit(1);
    }
    tokenList[pos].type = type;
}

int isKeyword(char* token)
{
    int size = sizeof(keyword)/sizeof(char*);
    int i = 0;
    for(i = 0; i < size; i++)
    {
        if(strcmp(keyword[i],token) == 0)
            return keyword_type[i];
    }
    return -1;
}

BOOL isIdent(char* token)
{
    int i = 0;
    int len = strlen(token);
    if(len > MAX_TOKEN_SIZE)
    {
        fputs("Identifier is too long.\n",stderr);
        exit(1);
    }
    for(i = 1; i < len; i++)
    {
        if(!isalnum(token[i]))
            return FALSE;
    }
    if(isdigit(token[0]))
    {
        fputs("Identifier does not start with a letter.\n",stderr);
        exit(1);
    }
    return TRUE;
}

BOOL isNum(char* token)
{
    int i = 0;
    int len = strlen(token);
    for(i = 0; i < len; i++)
    {
        if(!isdigit(token[i]))
            return FALSE;
    }
    char maxNumber[MAX_TOKEN_SIZE];
    sprintf(maxNumber,"%d", MAX_NUMBER);
    if(len > strlen(maxNumber) || atoi(token) > MAX_NUMBER)
    {
        fputs("Number is too large.\n",stderr);
        exit(1);
    }
    return TRUE;
}


/*
 usage: ./lexer [option(s)] filename
    options:
    --source: view source code
    --clean: view source code without comment
*/

// int main(int argc, char *argv[])
// {
//     BOOL clean = FALSE;
//     BOOL source = FALSE;
//     if(argc < 2) 
//     {
//         fprintf(stderr, "usage: ./lexer filename\n"); 
//         exit(1);
//     }
    
//     int i = 0;
//     for(i = 1; i < argc - 1; i++)
//     {
//         if(strcmp(argv[i],"--source") == 0)
//             source = TRUE;
//         else if(strcmp(argv[i],"--clean") == 0)
//             clean = TRUE;
//         else
//         {
//             fprintf(stderr,"unknown argument\n");
//             exit(1);
//         }
        
//     }
    
//     sourceCode = readInput(argv[argc-1]);
//     char *raw = removeComment(sourceCode);
//     tokenList = (tokenUnit*)malloc(sizeof(tokenUnit)*MAX_LIST_SIZE);
    
//     if(source)
//         printf("source code:\n------------\n%s\n",sourceCode);
//     if(clean)
//         printf("source code without comments:\n-----------------------------\n%s\n",raw);
    
//     int size = getToken(raw);
    
//     printf("tokens:\n-------\n");
//     for(i = 0; i < size; i++)
//         printf("%-12s    %d\n",tokenList[i].str,tokenList[i].type);
    
//     free(sourceCode);
//     free(raw);
//     free(tokenList);
    
//     return 0;
// }


tokenUnit lex()
{
    static int pos;
    if(tokenList == NULL)
    {
        pos = 0;
        char *raw = removeComment(sourceCode);
        tokenList = (tokenUnit*)malloc(sizeof(tokenUnit)*MAX_LIST_SIZE);
        getToken(raw);
        free(raw);
    }
    return tokenList[pos++];
}
