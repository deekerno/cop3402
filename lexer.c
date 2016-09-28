// Team Name: Compiler Group 13
// Alexander deCurnou
// Peter Hoang
// Shuaicheng Ma
// Stephen Rigdon

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAX_LINE 256
#define MAX_CHAR_PER_LINE 256


typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;

const char *keyword[] = {"null", "begin", "call", "const", "do", "else", "end", "if",
"odd", "procedure", "read", "then", "var", "while", "write"};

const int keyword_type[] = {nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym,
ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

// arithmetic operators
// comparison operators (note that = is also used to assign the value to a constant)
// assignment and some special symbols
const char *symbols[] = {"+","-","*","/","=","<>","<=","<",">",">=",":=",
",",";",".","(",")"};


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
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    
    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
    
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
    char *raw = (char*)malloc(strlen(sourceCode));
    memcpy(raw,sourceCode,strlen(sourceCode));

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
            memset(open,' ',strlen(open)-strlen(close)+2);
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


int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: ./lextest filename\n"); 
        exit(1);
    }
    
    char *sourceCode = readInput(argv[1]);
    char *raw = removeComment(sourceCode);
    printf("source code:\n------------\n%s",sourceCode);
    printf("\nsource code without comments:\n-----------------------------\n%s",raw);
    
    free(sourceCode);
    free(raw);
    
    
	return 0;
}
