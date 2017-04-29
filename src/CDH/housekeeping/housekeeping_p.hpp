typedef struct output_args
{
    system_type system_id;
    int subsystem_id; // The subsystem that is requesting this event
    int request_id; // A unique identifier. This is only unique in the context of a subsystem, so subsystem_id and request_id is required to globally identify.
    int priority; // The priority of this task, from a range 0 - X.
    int period; // In ms
    int n_executions; // Number of executions
    char name[30];
} output_args_type;

void output(UArg arg0, UArg arg1);
