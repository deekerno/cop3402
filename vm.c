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

// Fucntion to print stack results
void printStack(int stack[], cpu cpuList, int AR[], int sizeAR);

// execution function to execute instructions
int execute(instruction instruction, cpu *cpuList, int stack[]);

// Function to find base l levels down
int base(int l, int base, int stack[]);

char INSTRUCTION[][4] = {"","LIT","OPR","LOD","STO","CAL","INC","JMP","JPC","SIO"};

char OPR[][4] = {"RET","NEG","ADD","SUB","MUL","DIV","ODD","MOD","EQL","NEQ","LSS","LEQ","GTR","GEQ"};

char SIO[][4] = {"OUT","INP","HLT"};

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
    printf("                      pc   bp   sp   stack\n");
    printf("                  %6d%5d%5d\n", cpuList.pc, cpuList.bp, cpuList.sp);

	instruction ins;
	int condition = 2;
	int AR[MAX_LEXI_LEVELS] = {0};
	int currAR = 0;
	// condition list:
	// 0 halt
	// 1 add one activation record
	// -1 delete one activation record
	while(condition)
	{
		// fetch Cycle:
		cpuList.ir = cpuList.pc;
		cpuList.pc++;
		ins = instructionList[cpuList.ir];
		// execution Cycle:
		printf(" %2d  ",cpuList.ir);
		printInstruction(ins);
		condition = execute(ins,&cpuList,stack);
		if(condition == 1)
		{
			AR[currAR] = cpuList.bp;
			currAR++;			
		}
		else if(condition == -1)
		{
			AR[currAR] = 0;
			currAR--;
		}		
		// print current instruction and stack result		
		printf("%6d%5d%5d   ", cpuList.pc, cpuList.bp, cpuList.sp);
		printStack(stack,cpuList,AR,currAR);
		puts("");
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

int execute(instruction instruction, cpu *cpuList, int stack[])
{
	switch(instruction.op)
	{
		// 01 LIT 0 M
		// Pushes M onto the stack
		case 1:
			cpuList->sp++;
			stack[cpuList->sp] = instruction.m;
			break;
		// 02 OPR 0 M
		// Arithmetic operations
		case 2:
			switch(instruction.m)
			{
				// 0 RET
				case 0:
					cpuList->sp = cpuList->bp - 1;
					cpuList->pc = stack[cpuList->sp + 4];
					cpuList->bp = stack[cpuList->sp + 3];
					return -1;
					break;
				// 1 NEG
				case 1:
					stack[cpuList->sp] *= -1;
					break;
				// 2 ADD
				case 2:
					cpuList->sp -= 1;
					stack[cpuList->sp] += stack[cpuList->sp + 1];
					break;
				// 3 SUB
				case 3:
					cpuList->sp -= 1;
					stack[cpuList->sp] -= stack[cpuList->sp + 1];
					break;
				// 4 MUL
				case 4:
					cpuList->sp -= 1;
					stack[cpuList->sp] *= stack[cpuList->sp + 1];
					break;
				// 5 DIV
				case 5:
					cpuList->sp -= 1;
					stack[cpuList->sp] /= stack[cpuList->sp + 1];
					break;
				// 6 ODD
				case 6:
					stack[cpuList->sp] = stack[cpuList->sp] % 2;
					break;
				// 7 MOD
				case 7:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] % stack[cpuList->sp + 1];
					break;
				// 8 EQL
				case 8:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] == stack[cpuList->sp + 1] ? 1 : 0;
					break;
				// 9 NEQ
				case 9:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] != stack[cpuList->sp + 1] ? 1 : 0;
					break;
				// 10 LSS
				case 10:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] < stack[cpuList->sp + 1] ? 1 : 0;
					break;
				// 11 LEQ
				case 11:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] <= stack[cpuList->sp + 1] ? 1 : 0;
					break;
				// 12 GTR
				case 12:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] > stack[cpuList->sp + 1] ? 1 : 0;
					break;
				// 13 GEQ
				case 13:
					cpuList->sp -= 1;
					stack[cpuList->sp] = stack[cpuList->sp] >= stack[cpuList->sp + 1] ? 1 : 0;
					break;
				// Shouldn't hit this, but including it just in case.
				default:
					break;
			}
			break;
		// 03 LOD L M
		// Loads value L levels down at M to the top of the stack
		case 3:
			cpuList->sp++;
			stack[cpuList->sp] = stack[base(instruction.l,cpuList->bp,stack) + instruction.m];
			break;
		// 04 STO L M
		// Stores the value at the top of the stack at M, L levels down
		case 4:
			stack[base(instruction.l,cpuList->bp,stack) + instruction.m] = stack[cpuList->sp];
			cpuList->sp--;
			break;
		// 05 CAL L M
		// Creates a new Activation Record and sets the program counter to M
		case 5:
			stack[cpuList->sp + 1] = 0;
			stack[cpuList->sp + 2] = base(instruction.l,cpuList->bp,stack);
			stack[cpuList->sp + 3] = cpuList->bp;
			stack[cpuList->sp + 4] = cpuList->pc;
			cpuList->bp = cpuList->sp + 1;
			cpuList->pc = instruction.m;
			return 1;
			break;
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
					break;
				// System input
				// Read in input from user and push it
				case 1:
					cpuList->sp++;
					scanf("%d",stack+cpuList->sp);
					break;
				// Halt the machine
				case 2:
					return 0;
			}
			break;
		// Shouldn't hit this, but including it just in case.
		default:
			break;
	}
	return 2;
}

int base (int l, int base, int stack[])
{
	// Loops until we have reached the desired lexicographical level
	for(int i = l; i>0; i--)
	{
		base = stack[base + 1];
	}

	return base;
}

void printInstruction(instruction ins)
{
	// SIO instruction
	if(ins.op == 9)
		printf("%s          ", SIO[ins.m]);
	// Arithmetic/logical instructions
	else if(ins.op == 2)
		printf("%s          ", OPR[ins.m]);
	// instructions that need to display level
	else if(ins.op == 3 || ins.op == 4 || ins.op == 5)
		printf("%s  %3d %4d", INSTRUCTION[ins.op],ins.l,ins.m);
	else
		printf("%s  %3s %4d", INSTRUCTION[ins.op],"",ins.m);
}

void printStack(int stack[], cpu cpuList, int AR[], int sizeAR)
{
	int i = 0;
	for(i=1;i<=cpuList.sp;i++)
	{
		int j;
		for(j=0;j<sizeAR;j++)
		{
			if(i == AR[j])
				printf("| ");
		}
		printf("%d ",stack[i]);
	}
}
