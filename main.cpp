#include <src/init/init.h>
#include <src/version.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>

int main(void) {
    System_printf("Built at: %s %s\n", __DATE__, __TIME__);
#if defined GIT_INFO_PRESENT
        System_printf("Branch: %s, Hash: %s\n", GIT_REFNAME, GIT_HASH);
        System_printf("Authored at: %s\n", GIT_DATE);
#endif

    PreBiosInit();
    BIOS_start();  // Infinite Loop
    return (0);
}
