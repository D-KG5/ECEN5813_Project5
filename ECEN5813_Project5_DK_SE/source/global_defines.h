/*
 * global defines.h
 *
 *  Created on: Feb 8, 2020
 *      Author: Dhruva
 */

#ifndef GLOBAL_DEFINES_H_
#define GLOBAL_DEFINES_H_

#include "fsl_debug_console.h"
#include "MKL25Z4.h"
#include "logger.h"

// critical section macro functions
#define START_CRITICAL()	__disable_irq()
#define END_CRITICAL(x)	__set_PRIMASK(x)

// macros to enable/disable logging, testing
#define LOGGING_MODE
//#define TESTING_MODE

#ifdef TESTING_MODE
#include "uCUnit/uCUnit.h"
#endif

#define UART_POLL (1)
//#define UART_INT (1)

#define ECHO_MODE (1)
#define APP_MODE (1)


// enable/disable logging
#ifdef LOGGING_MODE
#define LOGGING (1)
#else
#define LOGGING (0)
#endif

// define macros for LED_init function
#define RED_LED_SHIFT   (18)	// on port B
#define GREEN_LED_SHIFT (19)	// on port B
#define BLUE_LED_SHIFT  (1)		// on port D
#define MASK(x) (1UL << (x))

// define macros for LED_on and LED_off functions
#define RED (1U)
#define GREEN (2U)
#define BLUE (3U)
#define ALL (4U)

// simple delay function
void Delay(volatile uint32_t number);

#endif /* GLOBAL_DEFINES_H_ */
