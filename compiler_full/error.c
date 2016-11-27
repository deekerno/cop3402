#include "error.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH_ERROR  55
char errorMessage[][MAX_LENGTH_ERROR] = {
    "Use = instead of :=.",                                     //0
    "= must be followed by a number.",                          //1
    "Identifier must be followed by =.",                        //2
    "const, var, procedure must be followed by identifier.",    //3
    "Semicolon or comma missing.",                              //4
    "Incorrect symbol after procedure declaration.",            //5
    "Statement expected.",                                      //6
    "Incorrect symbol after statement part in block.",          //7
    "Period expected.",                                         //8
    "Semicolon between statements missing.",                    //9 
    "Undeclared identifier.",                                   //10
    "Assignment to constant or procedure is not allowed.",      //11
    "Assignment operator expected.",                            //12
    "call must be followed by an identifier.",                  //13
    "Call of a constant or variable is meaningless.",           //14
    "then expected.",                                           //15
    "Semicolon or } expected.",                                 //16
    "do expected.",                                             //17
    "Incorrect symbol following statement.",                    //18
    "Relational operator expected.",                            //19
    "Expression must not contain a procedure identifier.",      //20
    "Right parenthesis missing.",                               //21
    "The preceding factor cannot begin with this symbol.",      //22
    "An expression cannot begin with this symbol.",             //23
    "This number is too large.",                                //24
    "Code too long.",                                           //25
    "undefined behavior in statement.",                         //26
    "end expected.",                                            //27
    "read must be followed by identifier.",                     //28
    "variable is not declared.",                                //29
    "write must be followed by identifier.",                    //30
    "Cannot read input file."                                   //31
};

void error(int num)
{
    printf("Error number %d, %s\n",num,errorMessage[num]);
    exit(1);
}