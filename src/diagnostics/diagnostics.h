#include <ti/sysbios/knl/Task.h>
#include "Board.h"
#include <ti/drivers/GPIO.h>

#define HEARTBEATPIN Board_LED0
#define HEARTBEATFREQ 1 // Number of blink cycles / second
#define TASKSTACKSIZE   512
Void heartbeatFxn(UArg arg0, UArg arg1);
int initHeartbeat();
