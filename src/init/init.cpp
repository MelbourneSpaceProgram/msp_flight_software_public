#include <Board.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/tasks/task_holder.h>
#include <test_runners/master_runner.h>

// Initialises the core MSP432 drivers provided by TI and loads the post BIOS
// init tasks. Should be called once at system startup, and prior to the BIOS
// starting.
void PreBiosInit() {
    Board_initGeneral();
    GPIO_init();
    I2C_init();
    SPI_init();
    UART_init();
    PWM_init();

    TaskHolder *post_bios_initialiser_task =
        new TaskHolder(1536, "Initialiser", 10, new PostBiosInitialiser());
    post_bios_initialiser_task->Init();
}
