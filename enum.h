#ifndef ENUM
#define ENUM

#include "header.h"

enum commands {

#define DEF_CMD(name, num, arg, code) CMD_##name = num,

#include "commands.h"

#undef DEF_CMD

};

enum reg_adresses {

#define DEF_RX(name, adr) RX_##name = adr,
#include "rx.h"
#undef DEF_RX

};

#endif
