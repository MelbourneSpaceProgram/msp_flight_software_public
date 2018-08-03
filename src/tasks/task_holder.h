#ifndef SRC_TASKS_TASK_HOLDER_H_
#define SRC_TASKS_TASK_HOLDER_H_

class Runnable;
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Task.h>

class TaskHolder {
   public:
    // Will allocate memory on the heap for the task stack. Does not require the
    // user to worry about memory management (except for general issues involved
    // with dynamic memory).
    TaskHolder(uint16_t stack_size, const char *name, int priority,
               Runnable *runnable);
    ~TaskHolder();
    Task_Handle GetTaskHandle() const;
    void Start();

   private:
    uint16_t stack_size;
    const char *name;
    int priority;
    Runnable *runnable;
    Task_Params task_params;
    Task_Handle handle;
    Task_Struct task_struct;

    TaskHolder(const TaskHolder &);
    TaskHolder &operator=(const TaskHolder &);
};

#endif  // SRC_TASKS_TASK_HOLDER_H_
