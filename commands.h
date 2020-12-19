DEF_CMD(PUSH, 1, 2, {if (*(arg_t*)(CPU.code + CPU.PC) == 0) {stackPush(CPU.stk, *((arg_t*)(CPU.code + CPU.PC) + 1)); CPU.PC += 2 * sizeof(arg_t);} else {CPU.PC += sizeof(arg_t); if (*(adress_t*) (CPU.code + CPU.PC) >= 0) {stackPush(CPU.stk, CPU.rx[*(adress_t*) (CPU.code + CPU.PC)]);} else {stackPush(CPU.stk, CPU.rx[-(*(adress_t*) (CPU.code + CPU.PC)) - 1]);} CPU.PC += sizeof(adress_t);}})
DEF_CMD(ADD, 2, 0, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); arg_t b = stackTop(CPU.stk); stackPop(CPU.stk); stackPush(CPU.stk, a + b);})
DEF_CMD(SUB, 3, 0, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); arg_t b = stackTop(CPU.stk); stackPop(CPU.stk); stackPush(CPU.stk, -a + b);})
DEF_CMD(MUL, 4, 0, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); arg_t b = stackTop(CPU.stk); stackPop(CPU.stk); stackPush(CPU.stk, a * b);})
DEF_CMD(DIV, 5, 0, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); arg_t b = stackTop(CPU.stk); stackPop(CPU.stk); if (a != 0) {stackPush(CPU.stk, a + b);} else {printf("error: zero devision\n");};})
DEF_CMD(SQRT, 6, 0, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); stackPush(CPU.stk, sqrt(a));})
DEF_CMD(POW, 7, 0, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); arg_t b = stackTop(CPU.stk); stackPop(CPU.stk); stackPush(CPU.stk, pow(b, a));})
DEF_CMD(IN, 8, 0, {arg_t a; scanf("%d", &a); stackPush(CPU.stk, a);})
DEF_CMD(OUT, 9, 0, {printf("%d\n", stackTop(CPU.stk));})
DEF_CMD(END, 10, 0, {return 1;})

DEF_CMD(JE, 11, 1, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); if (a == 0) {/*printf(">>%d\n", *(adress_t*) (CPU.code + CPU.PC))*/; CPU.PC = *(adress_t*) (CPU.code + CPU.PC);} else {CPU.PC += sizeof(adress_t);}})
DEF_CMD(JNE, 12, 1, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); if (a != 0) {/*printf(">>%d\n", *(adress_t*) (CPU.code + CPU.PC))*/; CPU.PC = *(adress_t*) (CPU.code + CPU.PC);} else {CPU.PC += sizeof(adress_t);}})
DEF_CMD(JG, 13, 1, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); if (a > 0) {/*printf(">>%d\n", *(adress_t*) (CPU.code + CPU.PC))*/; CPU.PC = *(adress_t*) (CPU.code + CPU.PC);} else {CPU.PC += sizeof(adress_t);}})
DEF_CMD(JL, 14, 1, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); if (a < 0) {/*printf(">>%d\n", *(adress_t*) (CPU.code + CPU.PC))*/; CPU.PC = *(adress_t*) (CPU.code + CPU.PC);} else {CPU.PC += sizeof(adress_t);}})

DEF_CMD(POP, 15, 1, {arg_t a = stackTop(CPU.stk); stackPop(CPU.stk); if (*(adress_t*) (CPU.code + CPU.PC) >= 0) {CPU.rx[*(adress_t*) (CPU.code + CPU.PC)] = a;} else {CPU.rx[-(*(adress_t*) (CPU.code + CPU.PC)) - 1] = a;} CPU.PC += sizeof(adress_t);})

DEF_CMD(PASS, 16, 0, {})
