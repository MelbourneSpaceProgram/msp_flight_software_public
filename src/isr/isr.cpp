#include <src/isr/isr.hpp>
#include <src/isr/power.hpp>
#include "Board.h"

void InitInterrupts() {

    /* Set up a button callback, in lieu of having no real power interrupts yet */
    GPIO_setCallback(Board_GPIO_BUTTON0, power_bms_isr);
    GPIO_enableInt(Board_GPIO_BUTTON0);
}

