#include <src/board/board.h>
#include <src/util/flash.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>

void Flash::FlashLed() {
    GPIO_write(SYS_LED, 1);
    TaskUtils::SleepMilli(200);
    GPIO_write(SYS_LED, 0);
    TaskUtils::SleepMilli(200);
}
