#include <src/tasks/task_holder.h>
#include <src/util/data_types.h>

TaskHolder::TaskHolder(uint16_t stack_size, char* name, int priority,
                       Runnable* runnable)
    : stack_size(stack_size),
      name(name),
      priority(priority),
      runnable(runnable),
      task_params(),
      handle(NULL) {}

TaskHolder::~TaskHolder() {
    Task_delete(&handle);
    if (runnable != NULL) {
        // Runnable should always be heap allocated or there
        // will be double deletion!
        delete runnable;
    }
}

void TaskHolder::Init() {
    Task_Params_init(&task_params);
    task_params.stackSize = stack_size;
    task_params.instance->name = name;
    task_params.priority = priority;
    handle = Task_create((Task_FuncPtr)runnable->GetRunnablePointer(),
                         &task_params, NULL);
    if (handle == NULL) {
        // TODO(dingbenjamin): Throw an error
    }
}

Task_Handle TaskHolder::GetTaskHandle() const { return handle; }
