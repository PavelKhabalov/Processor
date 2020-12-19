#ifndef HEADER
#define HEADER

#include "stack_by_array.cpp"
#include "header.h"
#include "dict\dict.c"
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <sys\stat.h>

typedef elem_t arg_t;
typedef int adress_t;

typedef struct {
	int PC = 0;
	char *code;
	int code_len = 0;
	
	int cmd_num = 0;
	int *cmds;

	stack_t *stk;

	arg_t *ram;
	arg_t rx[4];
} CPU_INFO;

typedef struct {
	int PC;
	int len;

	int cmd_num;
	int cmd_count;

	dict *marks;

} code_info;


void CPUconstruct(CPU_INFO *cpu, FILE *in);

int do_asm(FILE *in);

void binintfprintf(FILE *out, int a);

void binfprintf(FILE *out, void *adress, size_t size);

void skipLexemes(size_t n, FILE *out);

int Asm(FILE *in, FILE *out);

void readPrintfArgs(size_t n, FILE *out);

void make_code_info(code_info *ci, char *program_code);

bool isValidName(char *s);

bool isNum(char *s);

size_t fileLen(FILE* text);

char* readf(FILE* text);

#endif
