#include <src/isr/power.hpp>
#include <xdc/runtime/Log.h>
#include "Board.h"

void power_bms_isr(uint_least8_t index)
{
    // TODO
    // This will eventually be replaced with code that takes action when the BMS is broken.
    // For now, do nothing

    Log_info0("Power interrupt triggered");
}
