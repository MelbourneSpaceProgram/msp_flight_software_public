#include <Board.h>
#include <src/init/init.h>
#include <src/tasks/tasks.h>
#include <test_runners/master_runner.h>
#include <ti/sysbios/BIOS.h>

int main(void) {
    PreBiosInit();
    BIOS_start();  // Infinite Loop
    return (0);
}
