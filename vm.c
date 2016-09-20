// Team Name: Compiler Group 13
// Alexander deCurnou
// Peter Hoang
// Shuaicheng Ma
// Stephen Rigdon

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

// Given in Wocjan's wiki
typedef struct
{
	int op;  // opcode
	int l;   // L
	int m;   // M
} instruction;

typedef struct
{
	int bp;  // base pointer
	int sp;  // stack pointer
	int pc;  // program counter
	int ir;  // instruction register
} cpu;

//Function to read the file and store data into instruction array
int readFile(FILE* reader,instruction list[], int counter);

//Function to print the results
void printList(instruction list[], int counter);




int main(int argc, char *argv[])
{
    //Initial values
    int counter = 0;
    int instructionCount = 0;

    //Initial setup
    instruction instructionList[MAX_CODE_LENGTH];
    cpu cpuList;
    int stack[MAX_STACK_HEIGHT];

    //File pointer
    //Create and set the file pointer
    FILE* reader;
    reader= fopen(argv[1], "r");


    //Initial Values
    cpuList.sp = 0;
    cpuList.bp = 1;
    cpuList.pc = 0;
    cpuList.ir = 0;

    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;

    //Read in the program and obtain the instruction
    instructionCount = readFile(reader ,instructionList, counter);

    //Print out the read-in instructions
    printList(instructionList, instructionCount);
    /*
    //Initial execution
    printf("\n");
    printf("Execution.\n");
    printf("\t\t\t\tpc\tbp\tsp\stack\n");
    printf("\t\t\t\t%d\t%d\t\%d\n", cpuList.pc, cpuList.bp, cpuList.sp);

    //Execution
    */

    return 0;



}

int readFile(FILE* reader, instruction list[], int counter)
{


    //Continue reading the file until there's nothing left
    while(!feof(reader))
    {
        fscanf(reader, "%d", &list[counter].op);
        fscanf(reader, "%d", &list[counter].l);
        fscanf(reader, "%d", &list[counter].m);
        counter++;

        //If there are too many instructions, stop the program
        if(counter > MAX_CODE_LENGTH)
            return -1;

    }

    fclose(reader);

    return counter;

}

void printList(instruction list[], int counter)
{
    int i;

    printf("PL/0 code:\n");
    printf("\n");
    for(i = 0; i<counter; i++)
    {

        printf("%d\t", i);

        //Switch case to determine what will be printed
        switch(list[i].op)
        {
            case 1:
                printf("LIT\t\t%d\n", list[i].m);
                break;
            //Since OPR can have many cases, we have a switch function here
            case 2:
                switch(list[i].m)
                {
                    case 0:
                        printf("RET\n");
                        break;
                    case 1:
                        printf("NEG\n");
                        break;
                    case 2:
                        printf("ADD\n");
                        break;
                    case 3:
                        printf("SUB\n");
                        break;
                    case 4:
                        printf("MUL\n");
                        break;
                    case 5:
                        printf("DIV\n");
                        break;
                    case 6:
                        printf("ODD\n");
                        break;
                    case 7:
                        printf("MOD\n");
                        break;
                    case 8:
                        printf("EQL\n");
                        break;
                    case 9:
                        printf("NEQ\n");
                        break;
                    case 10:
                        printf("LSS\n");
                        break;
                    case 11:
                        printf("LEQ\n");
                        break;
                    case 12:
                        printf("GTR\n");
                        break;
                    case 13:
                        printf("GEQ\n");
                        break;
                }
                break;

            case 3:
                printf("LOD\t%d\t%d\n", list[i].l, list[i].m);
                break;
            case 4:
                printf("STO\t%d\t%d\n", list[i].l, list[i].m);
                break;
            case 5:
                printf("CAL\t%d\t%d\n", list[i].l, list[i].m);
                break;
            case 6:
                printf("INC\t\t%d\n", list[i].m);
                break;
            case 7:
                printf("JMP\t\t%d\n", list[i].m);
                break;
            case 8:
                printf("JPC\t\t%d\n", list[i].m);
                break;
            case 9:
                if(list[i].m == 2)
                    printf("HLT\n");
                break;

        }
    }



}
