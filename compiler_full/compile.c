#include "parser.h"
#include "lexer.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

extern char *sourceCode;

int main(int argc, char *argv[])
{
    sourceCode = readInput(argv[1]);
    if(sourceCode == NULL)
        error(32);
    program();
    printf("No errors, program is syntactically correct.\n");
    outputPM0(argv[2]);
    return 0;
}