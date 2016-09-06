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
struct
{
	int op;  // opcode 
	int l;   // L            
	int m;   // M         
} instruction;

struct
{
	int bp;  // base pointer
	int sp;  // stack pointer
	int pc;  // program counter
	int ir;  // instruction register
} cpu;