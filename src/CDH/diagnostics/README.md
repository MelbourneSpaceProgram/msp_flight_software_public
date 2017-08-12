# Logging

Log types supported are: 
- Log_printX()
- Log_infoX()
- Log_warningX()
- Log_errorX()
, where X is a number from 0 to 5. This number specifies the number of values that need to be entered into the string. For example:

```
Log_info0("No parameters");
Log_info1("One parameter %d", 123);
Log_error2("The value of system %d was %f", system, value);
```

The logger function automatically places a newline (`\n`) at the end of each message, so there is no need to add one in the log message.
Log messages are capped at 161 characters in length, including any values that are entered into the message.

Log messages can be accessed via the serial port. The baud rate is 115200, and the port is named `XDS110 Class Application/User UART`. Use Device Manager or similar to identify the port. Alternatively, trial and error can be used, as the LaunchPad only presents two serial devices.
