#include <src/init/init.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>

int main(void) {
    PreBiosInit();
    BIOS_start();  // Infinite Loop
    return (0);
}
