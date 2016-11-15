#include "error.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH_ERROR  55
char errorMessage[][MAX_LENGTH_ERROR] = {
    "",                                                         //0
    "Use = instead of :=.",                                     //1
    "= must be followed by a number.",                          //2
    "Identifier must be followed by =.",                        //3
    "const, var, procedure must be followed by identifier.",    //4
    "Semicolon or comma missing.",                              //5
    "Incorrect symbol after procedure declaration.",            //6
    "Statement expected.",                                      //7
    "Incorrect symbol after statement part in block.",          //8
    "Period expected.",                                         //9
    "Semicolon between statements missing.",                    //10 
    "Undeclared identifier.",                                   //11
    "Assignment to constant or procedure is not allowed.",      //12
    "Assignment operator expected.",                            //13
    "call must be followed by an identifier.",                  //14
    "Call of a constant or variable is meaningless.",           //15
    "then expected.",                                           //16
    "Semicolon or } expected.",                                 //17
    "do expected.",                                             //18
    "Incorrect symbol following statement.",                    //19
    "Relational operator expected.",                            //20
    "Expression must not contain a procedure identifier.",      //21
    "Right parenthesis missing.",                               //22
    "The preceding factor cannot begin with this symbol.",      //23
    "An expression cannot begin with this symbol.",             //24
    "This number is too large.",                                //25
    "Code too long.",                                           //26
    "Cannot assign value to a constant.",                       //27
    "end expected.",                                            //28
    "read must be followed by identifier.",                     //29
    "variable is not declared.",                                //30
    "write must be followed by identifier.",                    //31
    "Cannot read input file."                                   //32
};

void error(int num)
{
    printf("Error number %d, %s\n",num,errorMessage[num]);
    exit(1);
}