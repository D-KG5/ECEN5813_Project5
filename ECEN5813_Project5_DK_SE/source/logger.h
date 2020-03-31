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
	I2C_INIT,
	SENSOR_ENABLE,
	SENSOR_DISABLE,
	READ_XYZ,
	SLIDER_INIT,
	SLIDER_POLL,
	STATE_EVENT_HANDLER,
	SPI_INIT,
	SPI_EVENT_HANDLER,
	UCUNIT_WRITESTRING,
	UCUNIT_WRITEINT,
	UCUNIT_INIT,
	UCUNIT_SHUTDOWN,
	EMPTY_NAME
} func_names_t;

void Log_enable(void); // begin printing log messages when called
void Log_disable(void); // ignore any log messages until re-enabled
void Log_level(log_level_t level);	// set logging level
bool Log_status(void); // returns a flag to indicate whether the logger is enabled or disabled
int Log_data(uint8_t * seq, uint8_t len, func_names_t func, log_level_t level); // display in hexadecimal an address and contents of a memory location, arguments are a pointer to a sequence of bytes and a specified length
int Log_string(char * string, func_names_t func, log_level_t level); // display a string
int Log_integer(int32_t integer, func_names_t func, log_level_t level); // display an integer

// The previous three commands should include a new line after each display

#endif /* LOGGER_H_ */
