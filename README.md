# ECEN5813_Project5
PES Project 5 for ECEN 5813. By Dhruva Koley and Sagar Eligar

# Contents
- ECEN5813_Project5_DK_SE
  - source
    - ECEN5813_Project5_DK_SE.c: main source file containing application and echo mode
	- global_defines.h: header file that contains logging, testing, echo/app mode flags to turn them on or off
	- led_control.c: source file to initialize and control the LED
	- led_control.h: header file for led_control.c
	- logger.c: source file that contains logging utility functions
	- logger.h: header file for logger.c
	- circ_buffer.c: source file that contains circular buffer functions
	- circ_buffer.h: header file for circ_buffer.c
	- circ_buf_test.c: source file that contains uCUnit tests for the circular buffer
	- circ_buf_test.h: header file for circ_buf_test.c
	- UART.c: source file that contains UART (interrupt/polling) functions and application mode function
	- UART.h: header file for UART.c
	- timer.c: source file that contains SysTick timer functions
	- timer.h: header file for timer.c
- uCUnit
	- System.c: source file that containsKL25Z specific system changes to allow uCUnit to work
	- System.h: header file for System.h
	- uCUnit.h: header file containing unit test functions
- FMEA_project5.pdf: pdf with FMEA

# Notes
- If the circular buffer is full, it will extend the buffer memory by twice its original length (extra credit)
- Application mode processes `NUM_CHAR` characters before printing out the report
- `NUM_CHAR` controls how many characters are processed in application mode before the readout report is printed
- The UART serial console contains application and echo mode traffic.
- The semihost console contains the logging for everything else
- The while loop at the end of the main function will be entered after application mode has finished and demonstrates logging to the UART serial console as well as the timestamp.
- Logging strings and integers can be done within the semihost console or on the UART serial console depending on `uint8_t console`
- Test mode can be enabled in global_defines.h
- The type of UART driver (polled or interrupts) that is used can be toggled in global_defines.h
- The slider touch state transitions are extremely glitchy. 
	- Right touch must be quick and forceful (leave the finger on it). 
	- Left touch must be extremely quick (quickly remove your finger).

# Environment
 - Windows 10
 - MCUXpresso v11.1.0