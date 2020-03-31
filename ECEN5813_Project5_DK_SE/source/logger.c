/*
 * logger.c
 *
 *  Created on: Feb 20, 2020
 *      Author: Dhruva
 */

#ifndef LOGGING_C_
#define LOGGING_C_

#include "logger.h"
#include <stdint.h>
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "global_defines.h"

log_status_t log_status;

// log level enum and accompanying strings
log_level_t log_level;
static const char* log_levels[] = {"Status:", "Debug:", "Test:", ""};

// function name enum and accompanying strings
func_names_t func_name;
static const char* func_names[] = {"main:", "POST:", "Log_enable:", "Log_level:", "LED_init:", "LED_off:",
							"LED_off:", "LED_flash:", "I2C_init:", "Sensor_enable:",
							"Sensor_disable:", "Read_xyz:", "Slider_init:", "Slider_poll:", "state_event_handler:",
							"SPI_init", "spi_event_handler", "UCUNIT_WriteString", "UCUNIT_WriteInt", "UCUNIT_Init",
							"UCUNIT_Shutdown", ""};

static bool enabled = false;

// enable logging
void Log_enable(void){
#if LOGGING
	// begin printing log messages when called
	enabled = true;
#endif
}

// disable logging
void Log_disable(void){
#if LOGGING
	// ignore any log messages until re-enabled
	enabled = false;
#endif
}

// set logging level TEST > DEBUG > STATUS
void Log_level(log_level_t level){
#if LOGGING
	switch(level){
	case LOG_TEST:
		log_level = LOG_TEST;
		break;
	case LOG_DEBUG:
		log_level = LOG_DEBUG;
		break;
	case LOG_STATUS:
		log_level = LOG_STATUS;
		break;
	case LOG_EMPTY:
	default:
		// error
		break;
	}
#endif
}

// returns a flag to indicate whether the logger is enabled or disabled
bool Log_status(void){
	bool ret = false;
#if LOGGING
	if(enabled){
		ret = true;
	} else{
		ret = false;
	}
#endif
	return ret;
}

/* display in hexadecimal an address and contents of a memory location,
 * arguments are a pointer to a sequence of bytes and a specified length
 */
int Log_data(uint8_t * seq, uint8_t len, func_names_t func, log_level_t level){
	log_status = LOG_FAILED;
#if LOGGING
	if(enabled){
		if(log_level >= level){
			int ret = PRINTF("%s: %s: Memory allocated at 0x%08X	Data: ", log_levels[level], func_names[func], seq);
			for(int i = 0; i < len; ++i){
				ret = PRINTF("|%d| ", seq[i]);
			}
			ret = PRINTF("\r\n");
			if(ret >= 0){
				log_status = LOG_SUCCESS;
			}
		}
	}
#endif
	return log_status;
}

// display a string
int Log_string(char * string, func_names_t func, log_level_t level){
	log_status = LOG_FAILED;
#if LOGGING
	if(enabled){
		if(log_level >= level){
			int ret = PRINTF("%s %s %s", log_levels[level], func_names[func], string);
			if(ret >= 0){
				log_status = LOG_SUCCESS;
			}
		}
	}
#endif
	return log_status;
}

// display an integer
int Log_integer(int32_t integer, func_names_t func, log_level_t level){
	log_status = LOG_FAILED;
#if LOGGING
	if(enabled){
		if(log_level >= level){
			int ret = PRINTF("%s %s %d\r\n", log_levels[level], func_names[func], integer);
			if(ret >= 0){
				log_status = LOG_SUCCESS;
			}
		}
	}
#endif
	return log_status;
}


#endif /* LOGGING_C_ */
