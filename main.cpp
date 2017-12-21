#include <pthread.h>
#include <ti/sysbios/BIOS.h>
#include <src/public_headers/init/init.hpp>

/* Example/Board Header files */
#include "Board.h"

int main(void) {
    init_core();
    init_time();

    // All basic systems should now be operational. Start the main loop of the
    // satellite.
    init_satellite();

    // Start BIOS
    BIOS_start();

    return (0);
}
