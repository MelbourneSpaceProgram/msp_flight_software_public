/*
 * Task.cpp
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#include <stdint.h>

#include <src/CDH/util/Task.h>

Task::Task(){

    pthread_attr_init(&attrs);
    priParam.sched_priority = 8;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, taskStackSize);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    retc = pthread_create(&thread, &attrs, NULL, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }
}

