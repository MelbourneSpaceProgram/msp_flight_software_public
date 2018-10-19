#include <src/init/init.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>

int main(void) {
    // TODO(akremor): Add in logic to detect reboots/other limp mode reasons
    bool enter_limp_mode = false;

    if (enter_limp_mode) {
        EnterLimpMode();
    } else {
        PreBiosInit();
    }

    BIOS_start();  // Infinite Loop
    return (0);
}
