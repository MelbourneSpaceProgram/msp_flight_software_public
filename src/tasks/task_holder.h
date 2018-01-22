#ifndef SRC_TASKS_TASK_HOLDER_H_
#define SRC_TASKS_TASK_HOLDER_H_

#include <src/tasks/runnable.h>
#include <ti/sysbios/knl/Task.h>

class TaskHolder {
   public:
    TaskHolder(uint16_t stack_size, char *name, int priority,
               Runnable *runnable);
    ~TaskHolder();
    Task_Handle GetTaskHandle() const;
    void Init();

   private:
    uint16_t stack_size;
    char *name;
    int priority;
    Runnable *runnable;
    Task_Params task_params;
    Task_Handle handle;

    TaskHolder(const TaskHolder &);
    TaskHolder &operator=(const TaskHolder &);
};

#endif  // SRC_TASKS_TASK_HOLDER_H_
