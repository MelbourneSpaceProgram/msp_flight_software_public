/*
 * Task.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */


#ifndef TASK_H_
#define TASK_H_

// TODO Should this be here?
#include <pthread.h>

class Task
{
private:
    int taskStackSize; // TODO This should be an argument from the constructor
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;
public:
    Task();
    virtual ~Task();

    void start();
    void stop();
    virtual void action() = 0;
};

#endif /* TASK_H_ */
