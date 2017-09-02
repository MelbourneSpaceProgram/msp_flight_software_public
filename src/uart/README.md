Sample code for UART tasks.

```

// Open the UART stream
UARTConfiguration uart_config(UARTConfiguration::BLOCKING_MODE, UARTConfiguration::HIGH, &uartReceiveFinished);
UART uart_bus(&uart_config, Board_UART0);


void * test_task(void *){
    // Test task for reading back to UART
    uart_bus.open();


    int i = 0;
    while(1){
        char input = 97 + (i % 26);
        int write_buffer_length = 1;
        uart_bus.perform_write_transaction(&input, write_buffer_length);
        i++;

        Task_sleep(500);
    }
}

void * test_reader(void *){
    char input;
    while(1){
        // This is a blocking read
        UART_read(uart_bus.get_handle(), &input, 1);

        GPIO_toggle(Board_GPIO_LED1);
    }
}
```