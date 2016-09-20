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

// Function to print the instruction
void printInstruction(instruction ins);

//Function to print the results
void printList(instruction list[], int counter);

// execution function to execute instructions
void execute(instruction instruction, cpu *cpuList, int stack[]);

char INSTRUCTION[][4] = {"","LIT","OPR","LOD","STO","CAL","INC","JMP","JPC","SIO"};

char OPR[][4] = {"RET","NEG","ADD","SUB","MUL","DIV","ODD","MOD","EQL","NEQ","LSS","LEQ","GTR","GEQ"};

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
    //Initial execution
    printf("\n");
    printf("Execution:\n");
    printf("\t\t\t\tpc\tbp\tsp\tstack\n");
    printf("\t\t\t\t%2d\t%2d\t\%2d\n", cpuList.pc, cpuList.bp, cpuList.sp);

    //Execution
	
	// demo for now
	// change later	
	int i;
	for(i=0;i<instructionCount;i++)
	{
		printf(" %2d  ",i);
		printInstruction(instructionList[i]);
		puts("");
		execute(instructionList[i],&cpuList,stack);
	}

		
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
		printf(" %2d  ",i);
		printInstruction(list[i]);
		puts("");
    }
}

void execute(instruction instruction, cpu *cpuList, int stack[])
{
	switch(instruction.op)
	{
		// 06 INC 0 M 
		// Allocate M locals on stack
		case 6:
			cpuList->sp += instruction.m;
			break;
		// 07 JMP 0 M
		// Jump to M
		case 7:
			cpuList->pc = instruction.m;
			break;
		// 08 JPC 0 M
		// Pop stack and jump to M if value is equal to 0
		case 8:
			if (stack[cpuList->sp] == 0)
			{
				cpuList->pc = instruction.m;
				cpuList->sp--;
			}
			break;
		// 09 SIO System input & ouput operation
		case 9:
			switch(instruction.m)
			{
				// System output
				// Pop stack and print out value
				case 0:
					printf("%d",stack[cpuList->sp]);
					cpuList->sp--;
				// System input
				// Read in input from user and push it
				case 1:
					cpuList->sp++;
					scanf("%d",stack+cpuList->sp);
				// Halt the machine
				case 2:
					exit(0);
			}
		
	}
}

void printInstruction(instruction ins)
{
	// Halt instruction
	if(ins.op == 9 && ins.m ==2)
		printf("HLT");
	// Arithmetic/logical instructions
	else if(ins.op == 2)
		printf("%s", OPR[ins.m]);
	// instructions that need to display level
	else if(ins.op == 3 || ins.op == 4 || ins.op == 5)
		printf("%s  %3d %4d", INSTRUCTION[ins.op],ins.l,ins.m);
	else
		printf("%s  %3s %4d", INSTRUCTION[ins.op],"",ins.m);	
}