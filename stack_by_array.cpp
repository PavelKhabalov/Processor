#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <signal.h>
#include <conio.h>

#define STK_DEBUG 0
#define DEF_LVL 1

#define ASSERT_STACK(stack) if (int flag = stackOk(stack)) {printf("Error status: %d\nFile: '%s'\nLine: %d, in '%s' function\n-------------\n", flag, __FILE__, __LINE__, __func__); stackDump(stack, flag); assert(!"OK");}
#define ARR_END(cap) ((char*) stack->array_ + (cap) * sizeof(elem_t) + 2 * sizeof(canary_t))


//=============================================================================
#if STK_DEBUG > 0
#define $ printLog(logfile, __FILE__, __func__, __LINE__); setLog(__FILE__, __func__, __LINE__); LOG.test_str = (char*) calloc(100, 1); free(LOG.test_str);
#else
#define $
#endif

const char *log_file_name = "C:/Users/Pasha/my_programs/Stack/stack_by_array/logfile.txt";
FILE *logfile;

struct ProcessInfo {
    const char *_FILE = "";
    const char *_FUNC = "";
    size_t _LINE = 0;

    char *test_str;
} LOG;

void listener(int sig) {
    fprintf(logfile, "\nError: access violation\nFile: %s\nFunction: %s\nLine: %zu\n", LOG._FILE, LOG._FUNC, LOG._LINE);
    fflush(logfile);
    fclose(logfile);
}

void setLog(const char *file, const char *func, int line)  {
    LOG._FILE = file;
    LOG._FUNC = func;
    LOG._LINE = line;
}

void printLog(FILE *logfile_, const char *file, const char *func, size_t line) {
    if (file != LOG._FILE) {
        fprintf(logfile_, "file: '%s' \n", file);
    }
    if (func != LOG._FUNC) {
        fprintf(logfile_, "func: %s \n", func);
    }
    fprintf(logfile_, "line: %zu\n", line);
}


//=============================================================================



enum stackErrors {
    NDERRORS           = 0,
    INV_STACK_ADRESS   = 1,
    INV_ARR_ADRESS     = 1 << 1,
    INV_SIZE           = 1 << 2,
    INV_CAP            = 1 << 3,
    INV_SIZE_AND_CAP   = 1 << 4,
    INV_OP_CANARY      = 1 << 5,
    INV_END_CANARY     = 1 << 6,
    INV_OP_ARR_CANARY  = 1 << 7,
    INV_END_ARR_CANARY = 1 << 8,
    INV_HASH_FOR_STK   = 1 << 9,
    INV_HASH_FOR_ARR   = 1 << 10,
    WA_INV_POISON      = 1 << 11,
    TOO_BIG_CAP        = 1 << 12,
    TOO_BIG_SIZE       = 1 << 13,
    REALLOC_ERROR = 3
};


typedef long long int64_t;

typedef int elem_t;///< this typedef defines type of elements of stack
typedef long long hash_t;///< this typedef defines type of hashs of stack
typedef long long canary_t;///< this typedef defines type of canary of stack
typedef long long poison_t;///< this typedef defines type of poison of stack

const poison_t POISON = 0xBADEDAD11ADEDA;///< this const defines poison of stack

const canary_t OP_CANARY      = 211884564llu;///< this const defines opening canary of stack
const canary_t END_CANARY     = 480320020llu;///< this const defines ending canary of stack
const canary_t OP_ARR_CANARY  = 748755476llu;///< this const defines opening canary of array of stack
const canary_t END_ARR_CANARY = 1017190932llu;///< this const defines ending canary of array of stack


/*!
\brief struct of stack
*/
typedef struct {
#if DEF_LVL > 0
    canary_t opening_canary_ = OP_CANARY;
#endif

    elem_t *array_ = NULL;
    int64_t size_ = 0;
    int64_t capacity_ = 0;

#if DEF_LVL > 1
    hash_t hash_for_stk_ = 0;
    hash_t hash_for_arr_ = 0;
#endif

#if DEF_LVL > 0
    canary_t ending_canary_ = END_CANARY;
#endif
} stack_t;



/*!
\brief construct stack for adress and return it
\param stack Adress of stack which will be constructed
\param capacity Capacity of stack
\return constructed stack
*/
stack_t *stackConstruct(stack_t *stack, int64_t capacity);

/*!
\brief make and construct new stack and return adress of it
\param capacity Capacity of stack that will be constructed
\return new constructed stack
*/
stack_t *newStack(int64_t capacity);

/*!
\brief check stack for errors
\param stack Adress of stack that will be checked
\return status of errors
*/
long long stackOk(stack_t *stack);

/*!
\brief push new element in stack
\param stack Adress of stack in which will be pushed new element
\param value Value of new element
*/
void stackPush(stack_t *stack, elem_t value);

/*!
\brief return last element of stack
\param stack Adress of stack
\return last element of stack
*/
elem_t stackTop(stack_t *stack);

/*!
\brief delete last element
\param stack Adress of stack
\return 0 if success, 1 else
*/
int stackPop(stack_t *stack);

/*!
\brief return size of stack
\param stack Adress of stack
\return size of stack
*/
size_t sizeOfStack(stack_t *stack);

/*!
\brief destroy stack
\param stack Adress of stack
*/
void destroyStack(stack_t *stack);

/*!
\brief clear stack
\param stack Adress of stack
*/
void clearStack(stack_t *stack);

/*!
\brief output all possible information of stack (depence on STK_DEBUG) with errors
\param stack Adress of stack
\param status status of errors (variable that conteins information of errors
\return ND_ERRORS if it was successful and numbers of fatal errors else
*/
stackErrors stackDump(stack_t *stack, long long status);

/*!
\brief change capacity of stack
\param stack Adress of stack
\param new_capacity New capacity
\return ND_ERRORS if it was successful and numbers of fatal errors else
*/
stackErrors changeStackCapacity(stack_t *stack, int64_t new_capacity);

/*!
\brief count hash of segment of memory
\param begin, end Adress begin and end of segment of memory
\return hash of segment of memory
*/
hash_t hFunc(void *begin, void *end);

/*!
\brief check if hash is correct
\param begin, end Adress begin and end of segment of memory
\param h Adress of hash that will be checked
\return true if hash is correct false else
*/
bool checkHash(void* begin, void* end, hash_t *h);

/*!
\brief count hash for stack
\param stack Adress of stack
*/
void countHashForStack(stack_t *stack);

/*!
\brief set canary for segment of memory
\param begin, end Adress begin and end of segment of memory
\param op_canary, end_canary Begin and end canary
*/
void setCanary(void *begin, void *end, canary_t op_canary, canary_t end_canary);

/*!
\brief print segment of memory
\param begin, end Adress begin and end of segment of memory
*/
void printMem(void *begin, void *end);

/*!
\brief set segment of memory
\param begin, end Adress begin and end of segment of memory
\param filler Variable that will filled segment of memory
\param size Size of part of filler that will filled segment of memory
*/
void setMem(void *begin, void *end, long long filler, size_t size);

/*!
\brief return max of two elements
\param a, b Comparing elements
\return max of two elements
*/
int64_t max(int64_t a, int64_t b);

void setLog(const char *file, const char *func, int line);

void listener(int sig);

void printLog(FILE *logfile, const char *file, const char *func, size_t line);



/*
//-----------------------------------------------------------------------------
int main() {
    logfile = fopen(log_file_name, "w");
//=Signal==========================
$    signal(SIGSEGV, listener);
//=================================

$    /*stack_t* stk = newStack(10);

$    *(stk + 999999999999999) = {};
$    //stk->size_ = 2;

$    stackPush(stk, 1);
$    stackPush(stk, 8);
$    stackPush(stk, 5);
$    stackPush(stk, 255);
$    stackPush(stk, 2);
$    stackPush(stk, 1);
$    stackPush(stk, 8);
$    stackPush(stk, 15);
$    stackPush(stk, 1);
$    stackPush(stk, 2);
$    stackPush(stk, 1);

$    stackDump(stk, 0);

$    stackPop(stk);
$    stackPop(stk);
$    stackPop(stk);
$    stackPop(stk);
$    stackPop(stk);
$    stackPop(stk);
$    stackPop(stk);
$    stackPop(stk);

$    stackPush(stk + 1000, 12);


$    //stackPop(stk);



$    stackDump(stk, 0);

$    destroyStack(stk);

$    stk->capacity_ = 12;
$    stackPop(stk + 10000);

    //fclose(logfile);
    long k = -1;
    long long kk = -345;
    printf("%lu %lu %lu %lu\n", k, kk, sizeof(k), sizeof(kk));
$    return 0;
}
*/
//-----------------------------------------------------------------------------


stack_t *newStack(int64_t capacity) {
$    stack_t *stack = (stack_t*) calloc(1, sizeof(stack_t));

$    stackConstruct(stack, capacity);

$    return stack;
}

//-----------------------------------------------------------------------------

stack_t *stackConstruct(stack_t *stack, int64_t capacity) {
$    stack->array_ = (elem_t*) calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));



$    stack->size_ = 0;
$    stack->capacity_ = capacity;

#if DEF_LVL > 0
$    stack->opening_canary_ = OP_CANARY;
$    stack->ending_canary_ = END_CANARY;

$    setCanary(stack->array_, ARR_END(stack->capacity_), OP_ARR_CANARY, END_ARR_CANARY);
#endif

#if DEF_LVL > 1
$    countHashForStack(stack);
#endif

$    return stack;
}

//-----------------------------------------------------------------------------

void stackPush(stack_t *stack, elem_t v) {
$    ASSERT_STACK(stack)


$    if (stack->size_ == stack->capacity_) {
$        long long check = changeStackCapacity(stack, max(stack->capacity_ * 2, 4));
$        if (check) {
$            ASSERT_STACK(stack)
$            assert(!"OK");
$        }
$    }
$    *(elem_t*)((char*) (stack->array_ + stack->size_++) + sizeof(canary_t)) = v;
$
#if DEF_LVL > 1
$    countHashForStack(stack);
#endif
$    ASSERT_STACK(stack)
$}

//-----------------------------------------------------------------------------

stackErrors changeStackCapacity(stack_t *stack, int64_t new_capacity) {
$    assert(new_capacity >= 0);
#if DEF_LVL > 0
$    setCanary(stack->array_, ARR_END(stack->capacity_), 0, 0);
#endif
$    elem_t *new_array = (elem_t*) realloc(stack->array_, new_capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

$    if (!new_array) {
$        return REALLOC_ERROR;
$    }
$    stack->array_ = new_array;

$    setMem(ARR_END(stack->capacity_), ARR_END(new_capacity), 0, 1);

$    stack->capacity_ = new_capacity;

#if DEF_LVL > 0
$    setCanary(stack->array_, ARR_END(new_capacity), OP_ARR_CANARY, END_ARR_CANARY);
#endif

#if DEF_LVL > 1
$    countHashForStack(stack);
#endif

$    ASSERT_STACK(stack)
$    return NDERRORS;
}

//-----------------------------------------------------------------------------

elem_t stackTop(stack_t *stack) {
$    ASSERT_STACK(stack)
$    return *((elem_t*)((char*) stack->array_ + sizeof(canary_t)) + stack->size_ - 1);
}

//-----------------------------------------------------------------------------

int stackPop(stack_t *stack) {
$    ASSERT_STACK(stack)

$    if (stack->size_ > 0) {
$        *(elem_t*) ((char*) (stack->array_ + --stack->size_) + sizeof(canary_t)) = 0;

$        if (stack->size_ * 4 < stack->capacity_ && stack->capacity_ >= 8) {
$            changeStackCapacity(stack, stack->capacity_ / 2);
$        }
#if DEF_LVL > 1
$        countHashForStack(stack);
#endif
$        ASSERT_STACK(stack)
$        return 0;
$    } else {
$        return 1;
$    }
}

//-----------------------------------------------------------------------------

size_t sizeOfStack(stack_t *stack) {
$    ASSERT_STACK(stack)
$    return stack->size_;
}

//-----------------------------------------------------------------------------

stackErrors stackDump(stack_t *stack, long long status) {

$    printf("\n---------begin-of-dump--------\n");
#if STK_DEBUG == 1
$    if (stack) {
$        printf("\nAdress: %p\n\n", stack);
$    } else {
$        fprintf(stderr, "Error: nullptr of stack\n");
$        return INV_STACK_ADRESS;
$    }

$    printf("Adress of array: %p\n", stack->array_);
$    printf("Size: %lld\n", stack->size_);
$    printf("Capacity: %lld\n", stack->capacity_);

#if DEF_LVL > 0
$    printf("Opening stack canary: %llu\n", stack->opening_canary_);
$    printf("Ending stack canary: %llu\n", stack->ending_canary_);
#endif

#if DEF_LVL > 1
$    printf("Hash for stack: %llu\n", stack->hash_for_stk_);
$    printf("Hash for array: %llu\n\n", stack->hash_for_arr_);
#endif

#if DEF_LVL > 1
$    hash_t old_h = stack->hash_for_stk_;

$    stack->hash_for_stk_ = 0;
$    stack->hash_for_stk_ = hFunc(stack, stack + 1);

$    if (old_h != stack->hash_for_stk_) {
$        fprintf(stderr, "Error: incorrect hash for stack: %llu, must be %llu\n", stack->hash_for_stk_, old_h);
$        stack->hash_for_stk_ = old_h;
$        return INV_HASH_FOR_STK;
$    }

$    old_h = stack->hash_for_arr_;

$    stack->hash_for_arr_ = 0;
$    stack->hash_for_arr_ = hFunc(stack->array_, ARR_END(stack->capacity_));

$    if (status & INV_HASH_FOR_ARR) {
$        fprintf(stderr, "Error: incorrect hash for array: %llu, must be %llu\n", stack->hash_for_arr_, old_h);
$        stack->hash_for_arr_ = old_h;
$    }
$    printf("\n");
#endif

#if DEF_LVL > 0
$    if (status & INV_OP_CANARY) {
$        fprintf(stderr, "Error: incorrect opening canary of stack: %llu, must be %llu\n", stack->opening_canary_, OP_CANARY);
$    }
$    if (status & INV_END_CANARY) {
$        fprintf(stderr, "Error: incorrect ending canary of stack: %llu, must be %llu\n", stack->ending_canary_, END_CANARY);
$    }
$    printf("\n");
#endif

$    if (!stack->array_) {
$        fprintf(stderr, "Error: nullptr of stack->array_\n");
$        return INV_ARR_ADRESS;
$    }

$    if (status & INV_SIZE) {
$        printf("Error: invalid size: %lld\n", stack->size_);
$    }

$    if (status & INV_CAP) {
$        fprintf(stderr, "Error: invalid capacity: %lld\n", stack->capacity_);
$    }

$    if (status & INV_SIZE_AND_CAP) {
$        fprintf(stderr, "Error: invalid size and capacity: %lld > %lld\n", stack->size_, stack->capacity_);
$    }
$    printf("\n");
#if DEF_LVL > 0
$    if (status & INV_OP_ARR_CANARY) {
$        fprintf(stderr, "Error: incorrect opening canary of array: %llu, must be %llu\n", *((canary_t*) stack->array_), OP_ARR_CANARY);
$    } else {
$        printf("Opening arr canary: %llu\n", *((canary_t*) stack->array_));
$    }
$    if (status & INV_END_ARR_CANARY) {
$        fprintf(stderr, "Error: incorrect ending canary of array: %llu, must be %llu\n", *((canary_t*) (ARR_END(stack->capacity_)) - 1), END_ARR_CANARY);
$    } else {
$        printf("Ending arr canary: %llu\n", *((canary_t*) (ARR_END(stack->capacity_)) - 1));
$    }
#endif
$    printf("\nData of array: ");
$    printMem(stack->array_, ARR_END(stack->capacity_));
#else
$    if (status & INV_STACK_ADRESS) {
$        printf("Error: invalid adress of stack: %p\n", stack);
$        return INV_STACK_ADRESS;
$    }
#if DEF_LVL > 0
$    if (status & INV_OP_CANARY) {
$        printf("Error: incorrect opening canary for stack: %llu\n", stack->opening_canary_);
$    }
$    if (status & INV_END_CANARY) {
$        printf("Error: incorrect ending canary for stack: %llu\n", stack->ending_canary_);
$    }
#endif
$    if (status & INV_ARR_ADRESS) {
$        printf("Error: invalid adress of array of stack: %p\n", stack->array_);
$    }
$    if (status & INV_SIZE) {
$        printf("Error: invalid size: %lld\n", stack->size_);
$    }
$    if (status & INV_CAP) {
$        printf("Error: invalid capacity: %lld\n", stack->capacity_);
$    }
$    if (status & INV_SIZE_AND_CAP) {
$        printf("Error: invalid size and capacity: %lld > %lld\n", stack->size_, stack->capacity_);
$    }
$    if (status & INV_OP_ARR_CANARY) {
$        printf("Error: incorrect opening canary for array: %llu\n", *(canary_t*) stack->array_);
$    }
$    if (status & INV_END_ARR_CANARY) {
$        printf("Error: incorrect ending canary for array: %llu\n", *(canary_t*) (ARR_END(stack->capacity_)));
$    }
#if DEF_LVL > 1
$    if (status & INV_HASH_FOR_STK) {
$        hash_t old_h = stack->hash_for_stk_;
$        stack->hash_for_stk_ = 0;
$        printf("Error: incorrect hash for stack: %llu, must be %llu\n", hFunc(stack, stack + 1), old_h);
$        stack->hash_for_stk_ = old_h;
$    }
$    if (status & INV_HASH_FOR_ARR) {
$        hash_t old_h = stack->hash_for_arr_;
$        stack->hash_for_arr_ = 0;
$        printf("Error: incorrect hash for array: %llu, must be %llu\n", hFunc(stack->array_, ARR_END(stack->capacity_)), old_h);
$        stack->hash_for_arr_ = old_h;
$    }
#endif
$
#endif
$    printf("Elems:\n");
$    for (int i = 0; i < stack->capacity_; i++) {
$        printf("%d  %p: ", i, (elem_t*) ((char*) stack->array_ + sizeof(canary_t)) + i);
$        for (int j = sizeof(elem_t) - 1; j >= 0; j--) {
$            printf("%02X", *((unsigned char*) ((elem_t*) ((char*) stack->array_ + sizeof(canary_t)) + i) + j));
$        }
$        printf("\n");

$    }
$    printf("\n---------end-of-dump--------\n");

$    return NDERRORS;
}

//-----------------------------------------------------------------------------

long long stackOk(stack_t *stack) {

$    long long status = 0;

$    if (stack == NULL) {
$        status = status | INV_STACK_ADRESS;
$        return status;
$    }

$    if (stack->size_ < 0) {
$        status = status | INV_SIZE;
$    }
$    if (stack->capacity_ < 0) {
$        status = status | INV_CAP;
$    }
$    if (stack->size_ > stack->capacity_) {
$        status = status | INV_SIZE_AND_CAP;
$    }
$    if (stack->array_ == NULL) {
$        status = status | INV_ARR_ADRESS;
$        return status;
$    }

#if DEF_LVL > 1
$    if (!checkHash(stack, stack + 1, &(stack->hash_for_stk_))) {
$        status = status | INV_HASH_FOR_STK;
$        return status;
$    }
#endif

#if DEF_LVL > 0
$    if (stack->opening_canary_ != OP_CANARY) {
$        status = status | INV_OP_CANARY;
$    }
$    if (stack->ending_canary_ != END_CANARY) {
$        status = status | INV_END_CANARY;
$    }
#endif


#if DEF_LVL > 0
$    if (*((canary_t*) stack->array_) != OP_ARR_CANARY) {
$        status = status | INV_OP_ARR_CANARY;
$    }
$    if (*((canary_t*) (ARR_END(stack->capacity_)) - 1) != END_ARR_CANARY) {
$        status = status | INV_END_ARR_CANARY;
$    }
#endif

#if DEF_LVL > 1
$    if (!checkHash(stack->array_, ARR_END(stack->capacity_), &(stack->hash_for_arr_))) {
$        status = status | INV_HASH_FOR_ARR;
$    }
#endif

$    return status;
}

//-----------------------------------------------------------------------------

void clearStack(stack_t *stack) {
$    ASSERT_STACK(stack)

$    changeStackCapacity(stack, 0);
$    stack->size_ = 0;

#if DEF_LVL > 1
$    countHashForStack(stack);
#endif
}

//-----------------------------------------------------------------------------

void destroyStack(stack_t *stack) {
$    ASSERT_STACK(stack)
$    free(stack->array_);
$    *stack = {};
}

//-----------------------------------------------------------------------------

hash_t hFunc(void *begin, void *end) {
$    hash_t h = 0;
    hash_t base = 257;
    hash_t mod = 2103179;

$    for (size_t i = 0; (char*) begin + i < end; i++) {
$        h = ((h * base) % mod + (hash_t) (*((unsigned char*) begin + i))) % mod;
$    }

$    return h;
}

//-----------------------------------------------------------------------------

bool checkHash(void *begin, void *end, hash_t *h) {
$    hash_t old_h = *h;
$    *h = 0;

$    *h = hFunc(begin, end);

$    if (*h == old_h) {
$        return true;
$    } else {
$        *h = old_h;
$        return false;
$    }
}

//-----------------------------------------------------------------------------
#if DEF_LVL > 1
void countHashForStack(stack_t *stack) {
$    stack->hash_for_arr_ = 0;
$    stack->hash_for_arr_ = hFunc(stack->array_, ARR_END(stack->capacity_));

$    stack->hash_for_stk_ = 0;
$    stack->hash_for_stk_ = hFunc(stack, stack + 1);
}
#endif
//-----------------------------------------------------------------------------

void setCanary(void *begin, void *end, canary_t op_canary, canary_t end_canary) {
$    *((canary_t*) begin) = op_canary;
$    *((canary_t*) end - 1) = end_canary;
}

//-----------------------------------------------------------------------------

void printMem(void *begin, void *end) {
$    for (unsigned char *j = (unsigned char*) end - 1; j >= begin; j--) {
$        printf("%02X", *j);
$    }
$    printf("\n");
}

//-----------------------------------------------------------------------------

void setMem(void *begin, void *end, long long filler, size_t size) {
$    for (size_t i = 0; (char*) begin + i < end; i++) {
$        *((char*) begin + i) = *((char*) &filler + i % size);
$    }
}

//-----------------------------------------------------------------------------

int64_t max(int64_t a, int64_t b) {
$    return (a > b) ? a: b;
}
