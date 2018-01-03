#include <src/tasks/task_holder.h>
#include <src/util/data_types.h>

static Task_Struct kEmptyTaskStruct;

TaskHolder::TaskHolder(uint16_t stack_size, char* name, int priority,
                       Runnable* runnable)
    : stack_size(stack_size),
      name(name),
      priority(priority),
      stack(new byte[stack_size]),
      runnable(runnable),
      task_struct(kEmptyTaskStruct),
      handle(NULL) {}

TaskHolder::~TaskHolder() { delete[] stack; }

void TaskHolder::Init() {
    Task_Params task_params;
    Task_Params_init(&task_params);
    task_params.stackSize = stack_size;
    task_params.instance->name = name;
    task_params.stack = stack;
    task_params.priority = priority;

    // TODO(dingbenjamin): Figure out initialisation of task struct
    // TODO(dingbenjamin): Figure out last argument "xdc_runtime_Error_Block
    // *__eb"
    Task_construct(&task_struct, (Task_FuncPtr)runnable->GetRunnablePointer(),
                   &task_params, NULL);
    handle = Task_handle(&task_struct);
}

Task_Handle TaskHolder::GetTaskHandle() const {
    // TODO(dingbenjamin): Figure out if this is acceptable to copy, or if it
    // should be returned by reference
    return handle;
}
