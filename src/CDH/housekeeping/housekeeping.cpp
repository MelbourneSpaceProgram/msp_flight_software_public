#include <src/public_headers/logger.h>

#include <ti/sysbios/knl/Task.h>
#include <src/public_headers/systems.hpp>
#include <src/public_headers/request.hpp>
#include <src/public_headers/allocator.hpp>

#include <src/public_headers/housekeeping.hpp>
#include <src/CDH/housekeeping/housekeeping_p.hpp>

request_short CDH_requests(){
    request_short request;

    //custom.n_executions = 10;
    //custom.priority = 99;

    request.n_executions = 10;
    request.period = 3000;
    request.func = (Task_FuncPtr) output;


    return request;
}

void CDH_task_callback(Task_Handle created_task){
    log_info("Received a task handle");

    // Delete the task
    Task_delete(&created_task);

}

void output(UArg arg0, UArg arg1){
    // Quickly prototype dynamically creating tasks. We also demonstrate how these tasks still run, even when the main allocator task is 'running'.
    while(1){
        // Request some tasks from everyone else
        log_info("My custom logger\n");
        //log_warning("My custom logger2\n");
        //log_error("My custom logger3\n");

        Task_sleep((UInt) arg0);
    }

}
