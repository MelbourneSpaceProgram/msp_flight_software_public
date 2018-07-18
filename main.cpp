#include <src/init/init.h>
#include <src/version.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>

int main(void) {
    System_printf("Built at: %s %s\n", __DATE__, __TIME__);
#if defined GIT_INFO_PRESENT
        System_printf("Branch: %s\n", TOSTRING(GIT_REFNAME));
#endif

    PreBiosInit();
    BIOS_start();  // Infinite Loop
    return (0);
}
