#include "header.h"
#include "enum.h"


CPU_INFO CPU;

int do_asm(FILE *in) {
$	CPUconstruct(&CPU, in);

$	while (CPU.PC < CPU.code_len) {
$		CPU.cmds[CPU.cmd_num++] = CPU.PC;
$		switch (*(int*) (CPU.code + CPU.PC)) {
$
			#define DEF_CMD(name, num, arg, code) case CMD_##name: CPU.PC += sizeof(int); code break;
			#include "commands.h"
			#undef DEF_CMD
$
$			default: fprintf(stderr, "Wrong command: %d, PC = %d\n", *(int*) (CPU.code + CPU.PC), CPU.PC); return 2;
		}
	}
	return 0;
}

void CPUconstruct(CPU_INFO *cpu, FILE *in) {
$	*cpu = {};
$	cpu->stk = newStack(10);
$
$	size_t len = fileLen(in);
$
$   cpu->code = (char*) calloc(len, sizeof(char));
$   len = fread(cpu->code, sizeof(char), len, in);
$
$	cpu->code_len = len;
$
$	cpu->cmds = (int*) calloc(len, 1);
$
$	cpu->ram = (arg_t*) calloc(1000, sizeof(arg_t));
}

size_t fileLen(FILE* text) {
    assert(text);

    struct stat buf;
    int status = fstat(fileno(text), &buf);

    assert(!status);

    return buf.st_size;
}

char* readf(FILE* text) {
$    assert(text);
$
$    size_t len = fileLen(text);
$
$    char* buffer = (char*) calloc(len + 1, sizeof(char));
$    size_t size = fread(buffer, sizeof(char), len, text);
$    buffer = (char*) realloc(buffer, size + 1);
$    buffer[size] = '\0';
$
$    return buffer;
}
