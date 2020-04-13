/*
 * logger.h
 *
 *  Created on: Feb 20, 2020
 *      Author: Dhruva
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum log_status_t{
	LOG_SUCCESS,	// no error
	LOG_FAILED	// failure case
} log_status_t;

// enum for log level
typedef enum log_level_t{
	LOG_STATUS,
	LOG_DEBUG,
	LOG_TEST,
	LOG_EMPTY
} log_level_t;

// enum for function names
typedef enum func_names_t{
	MAIN,
	POST_FUNC,
	lOG_ENABLE,
	LOG_LEVEL,
	LED_INIT,
	LED_OFF,
	LED_ON,
	LED_FLASH,
	ECHOFUNC,
	APPLICATION_MODE,
	SYSTICK_INIT,
	INIT_UART0,
	UART0_TRANSMIT_POLL,
	UART0_RECEIVE__POLL,
	SEND_STRING_POLL,
	RECEIVE_CHECK,
	TRANSMIT_CHECK,
	UCUNIT_WRITESTRING,
	UCUNIT_WRITEINT,
	UCUNIT_INIT,
	UCUNIT_SHUTDOWN,
	UART0_IRQHANDLER,
	SEND_STRING,
	RECEIVE_STRING,
	INSERT_ITEM,
	REMOVE_ITEM,
	DESTROY_BUFF,
	SYSTICK_ENABLE,
	SYSTICK_DISABLE,
	SYSTICK_HANDLER,
	EMPTY_NAME
} func_names_t;

char * Log_timestamp(void); // return formatted timestamp for logging
void Log_enable(void); // begin printing log messages when called
void Log_disable(void); // ignore any log messages until re-enabled
void Log_level(log_level_t level);	// set logging level
bool Log_status(void); // returns a flag to indicate whether the logger is enabled or disabled
int Log_data(uint8_t * seq, uint8_t len, func_names_t func, log_level_t level); // display in hexadecimal an address and contents of a memory location, arguments are a pointer to a sequence of bytes and a specified length
int Log_string(char * string, func_names_t func, log_level_t level, uint8_t console); // display a string
int Log_integer(int32_t integer, func_names_t func, log_level_t level, uint8_t console); // display an integer

// The previous three commands should include a new line after each display

#endif /* LOGGER_H_ */
