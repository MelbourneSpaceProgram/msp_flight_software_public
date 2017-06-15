#ifndef MSP_ARCH_TASK_H
#define MSP_ARCH_TASK_H

typedef int UArg;
typedef int Task_FuncPtr;
typedef int Task_Handle;
typedef int UInt;

typedef char Char;

#define Task_Params_init

// TODO Define proper
#define Task_sleep

/*
struct Task_Params {
    int __size;
    const void *__self;
    void *__fxns;
    int *instance;
    UArg arg0;
    UArg arg1;
    int priority;
    int stack;
    int stackSize;
    int stackHeap;
    int env;
    int vitalTaskFlag;
    UInt affinity;
    int __iprms;
};
*/
struct Task_Params {
    int stackSize;
    int priority;
    int *instance;
    UArg arg0;
    UArg arg1;
    void * stack;

};

#define Task_Struct int
/*
Task_Handle Task_create(Task_FuncPtr fxn, Task_Params * asd, void *__eb ){
    // Do the pthread business here.
}

*/

#endif
