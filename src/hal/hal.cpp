/*
 * hal.c
 *
 *  Created on: 3Mar.,2017
 *      Author: Anthony
 */

#include "hal.h"

// Wrap the GPIOToggle function
void msp_GPIO_toggle(unsigned int index){
    // In here, we will fetch a config option that will direct our execution
    // If real, we just call GPIO_toggle
    // If we are debugging, we might write to file, or display to user in another way

    int action = 0; // TODO Turn this into an ENUM

    switch(action){
        case 0:
            GPIO_toggle(index);
            break;
        case 1:
            // TODO Eventually create a logger function to handle this, and timestamping etc
            System_printf("Toggling pin %u\n", index);
            System_flush();
            break;
    }
}
