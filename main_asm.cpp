#include "header.h"
#include "enum.h"
#include "do_program.cpp"
#include "disAsm.cpp"
#define $

code_info CI;



int Asm(FILE *in, FILE *out) {
$	char *program_code = readf(in);
$	printf("%s\n", program_code);
$
$	size_t l = strlen(program_code);
$   char *code_copy = (char*) calloc(1, l + 1);
$   memcpy(code_copy, program_code, l);
$
$	make_code_info(&CI, code_copy);

$	char *cmd = strtok(program_code, "\r\n ");
$
$	while (cmd != NULL) {
$
#define DEF_CMD(name, num, arg, code)\
if (strcmp(cmd, #name) == 0) {\
	binintfprintf(out, CMD_##name);\
	readPrintfArgs(num, out);\
} else

#include "commands.h"
#undef DEF_CMD
		{
$			int slen = strlen(cmd);
$			if (cmd[slen - 1] == ':') {
$				cmd[slen - 1] = '\0';
				int *arg = (int*) dict_find(CI.marks, cmd);
$				if (arg != NULL) {
$					binintfprintf(out, CMD_PASS);
$				}
$			} else {
$				fprintf(stderr, "Syntax error: '%s'\n", cmd);
$			}
$		}
$
$		cmd = strtok(NULL, "\r\n ");
$	}
$
$
$	return 0;
$}


void readPrintfArgs(size_t num_cmd, FILE *out) {
$	if (num_cmd == CMD_PUSH) {
$		char *arg = strtok(NULL, "\r\n ");

		#define DEF_RX(name, adr)\
		if (strcmp(#name, arg) == 0) {\
			binintfprintf(out, 1);\
			binintfprintf(out, -RX_##name);\
		} else
		#include "rx.h"
		#undef DEF_RX

		if (isNum(arg)) {
			binintfprintf(out, 0);
			binintfprintf(out, atoi(arg));
		} else if (arg[0] = '[' && arg[strlen(arg) - 1] == ']') {
			arg++;
			arg[strlen(arg) - 1] = '\0';

			#define DEF_RX(name, adr)\
			if (strcmp(#name, arg) == 0) {\
				binintfprintf(out, 1);\
				binintfprintf(out, -RX_##name);\
			} else
			#include "rx.h"
			#undef DEF_RX

$			if (isNum(arg)) {
$				int adr = atoi(arg);
$				if (adr >= 0) {
					binintfprintf(out, 1);
$					binintfprintf(out, adr);
$				} else {
					fprintf(stderr, "Error: invalid adress: '%d'\n", adr);
				}
$			} else {
$				fprintf(stderr, "Error: invalid adress: '%s'\n", arg);
$			}
		} else {
			fprintf(stderr, "Error: invalid argument: '%s'\n", arg);
		}

$	} else if (num_cmd == CMD_JG || num_cmd == CMD_JL || num_cmd == CMD_JE || num_cmd == CMD_JNE) {
		char *mark = strtok(NULL, "\r\n ");
		int *arg = (int*) dict_find(CI.marks, mark);
$		if (arg != NULL) {
			printf(">%s\n", mark);
$			binintfprintf(out, *arg);
$		} else {
$			fprintf(stderr, "Error: in code there is not this mark: %s\n", mark);
$		}

$	} else if (num_cmd == CMD_POP) {
$		char *arg = strtok(NULL, "\r\n ");
$
		#define DEF_RX(name, adr)\
		if (strcmp(#name, arg) == 0) {\
			binintfprintf(out, -RX_##name);\
		} else
		#include "rx.h"
		#undef DEF_RX

		{
		if (arg[0] = '[' && arg[strlen(arg) - 1] == ']') {
			arg++;
			arg[strlen(arg) - 1] = '\0';
$			if (isNum(arg)) {
$				int adr = atoi(arg);
$				if (adr >= 0) {
$					binintfprintf(out, adr);
$				}
$			} else {
$				fprintf(stderr, "Error: invalid adress: '%s'\n", arg);
$			}
$		} else {
			fprintf(stderr, "Error: invalid argument: '%s'\n", arg);
		}
		}
$	}
$}

void skipLexemes(size_t n) {
$	for (int i = 0; i < n; i++) {
$		strtok(NULL, "\r\n ");
$	}
$}

void binfprintf(FILE *out, void *adress, size_t size) {
$	for (int i = 0; i < size; i++) {
$		fprintf(out, "%c", *((char*) adress + i));
$	}
$}

void binintfprintf(FILE *out, int a) {
$	for (int i = 0; i < sizeof(int); i++) {
$		fprintf(out, "%c", a % 256);
$		a /= 256;
$	}
$}

void make_code_info(code_info *ci, char *program_code) {
$	*ci = {};
$	ci->marks = dict_new(100, sizeof(int));
$
$	char *cmd = strtok(program_code, "\r\n ");
$
$	while (cmd != NULL) {
$
#define DEF_CMD(name, num, arg, code)\
if (strcmp(cmd, #name) == 0) {\
	ci->cmd_num++;\
	ci->PC += arg * sizeof(arg_t);\
	if (strcmp(#name, "PUSH") == 0) {\
		skipLexemes(1);\
	} else {\
		skipLexemes(arg);\
	}\
} else

#include "commands.h"

#undef DEF_CMD

		if (cmd[strlen(cmd) - 1] == ':') {
$			cmd[strlen(cmd) - 1] = '\0';
$			if (isValidName(cmd)) {
				printf("%s\n", cmd);
$				dict_push(ci->marks, cmd, &ci->PC);
$			} else {
$				fprintf(stderr, "Syntax error: %s\n", cmd);
$				assert(!"OK");
$			}
$		}
$		ci->PC += sizeof(int);
$		cmd = strtok(NULL, "\r\n ");
$	}
$	ci->len = ci->PC;
$	ci->cmd_count = ci->cmd_num;
$	ci->PC = 0;
$	ci->cmd_num = 0;
$}

bool isValidName(char *s) {
$
$	for (int i = 0; s[i] != '\0'; i++) {
$		if (!((s[i] == '_') || (s[i] >= '0' && s[i] <= '9') || isalpha(s[i]))) {
$			return false;
$		}
$		if ((s[i] >= '0' && s[i] <= '9') && i == 0) {
$			return false;
$		}
$	}
$	return true;
$}

bool isNum(char *s) {
$	for (int i = 0; s[i] != '\0'; i++) {
$		if (!(isdigit(s[i]) || ((s[i] == '-' || s[i] == '+') && i == 0))) {
$			return false;
$		}
$	}
$	return true;
}


int main() {
$	FILE *in = fopen("input.txt", "rb");
$	FILE *out = fopen("byte_code.txt", "wb");

	printf("%p %p\n", in, out);
$
$	assert(in);
$	assert(out);
$	Asm(in, out);
$
$	fclose(in);
$	fclose(out);
$
$	in = fopen("byte_code.txt", "rb");

$	do_asm(in);
	//disAsm(in);
$
	fclose(in);
	return 0;
}
