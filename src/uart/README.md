Sample code for UART tasks.

The UART object should only ever be used in a thread, it cannot be used anywhere within the main function.

```
// Open the UART stream
UARTConfiguration uart_config(UARTConfiguration::kBaud115200);
UART uart_bus(uart_config, UARTA0);

void * test_task(void *){
    // Test task for reading back to UART
    while(1){
        byte input = 0xF0;
        uint16_t write_buffer_length = 1;
        uart_bus.PerformWriteTransaction(&input, write_buffer_length);
        Task_sleep(500);
    }
}

void * test_reader(void *){
    byte input;
    while(1){
    	uart_bus.PerformReadTransaction(&input, 1);
        GPIO_toggle(Board_GPIO_LED1);
    }
}
```