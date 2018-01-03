#ifndef SRC_TASKS_RUNNABLE_H_
#define SRC_TASKS_RUNNABLE_H_

typedef void (*fnptr)(void);

class Runnable {
   public:
    virtual fnptr GetRunnablePointer() = 0;
    virtual ~Runnable();
};

#endif  // SRC_TASKS_RUNNABLE_H_
