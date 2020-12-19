#include "header.h"
#include "enum.h"

void disAsm(FILE *code);

void *disAsmComand(void *adress, int *c);

void *disAsmArg(void *adress, int arg, int *c);


void disAsm(FILE *code) {

$	size_t len = fileLen(code);
$
$    char *s = (char*) calloc(len, sizeof(char));

	printf("%s\n", s);
$    len = fread(s, sizeof(char), len, code);
	 int c = 0;

	while (c < len) {
		disAsmComand(s, &c);
	}
}

void *disAsmComand(void *adress, int *c) {
	int cn = *(int*) (adress + *c);
	*c += sizeof(int);

	#define DEF_CMD(name, num, arg, code) \
	if (cn == num) {\
		printf("%d "#name " ", cn);\
		adress = disAsmArg(adress, arg, c);\
	}
	#include "commands.h"
	#undef DEF_CMD
	printf("\n");
	return adress;
}


void *disAsmArg(void *adress, int arg, int *c) {
	for (int i = 0; i < arg; i++) {
		printf("%d ", *(arg_t*) (adress + *c));
		*c += sizeof(arg_t);
	}
	return adress;
}
