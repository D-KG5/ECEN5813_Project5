/*
 * led_control.c
 *
 *  Created on: Feb 8, 2020
 *      Author: Dhruva
 */
#include <led_control.h>
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "MKL25Z4.h"

#include "global_defines.h"
#include "logger.h"
#include "fsl_debug_console.h"

// initialize RGB LEDs
void LED_init(void){
	uint32_t masking_state;
	masking_state = __get_PRIMASK();
	START_CRITICAL();
    // https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_2/Source/main.c
	// start clock for PORT B and PORT D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Make 3 pins GPIO
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(1);

	// set ports to outputs
	PTB->PDDR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PDDR |= MASK(BLUE_LED_SHIFT);
	// clear ports
	PTB->PCOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PCOR |= MASK(BLUE_LED_SHIFT);

	// turn off LED
	PTB->PSOR = MASK(RED_LED_SHIFT);
	PTB->PSOR = MASK(GREEN_LED_SHIFT);
	PTD->PSOR = MASK(BLUE_LED_SHIFT);
	END_CRITICAL(masking_state);
	Log_string("LED Initialized\r\n", LED_INIT, LOG_DEBUG, 1);
}

/**
 * turn off LED by colour
 * @args unit8_t color: pass in desired color of RGB LED
 */
void LED_off(uint8_t color){
	// clear RGB LED Pins
	switch(color){
	case RED:
		PTB->PSOR = MASK(RED_LED_SHIFT);
		Log_string("RED OFF\r\n", LED_OFF, LOG_DEBUG, 1);
		break;
	case GREEN:
		PTB->PSOR = MASK(GREEN_LED_SHIFT);
		Log_string("GREEN OFF\r\n", LED_OFF, LOG_DEBUG, 1);
		break;
	case BLUE:
		PTD->PSOR = MASK(BLUE_LED_SHIFT);
		Log_string("BLUE OFF\r\n", LED_OFF, LOG_DEBUG, 1);
		break;
	case ALL:
		PTB->PSOR = MASK(RED_LED_SHIFT);
		PTB->PSOR = MASK(GREEN_LED_SHIFT);
		PTD->PSOR = MASK(BLUE_LED_SHIFT);
//		Log_string("ALL OFF\r\n", LED_OFF, LOG_DEBUG);
		break;
	default:
		Log_string("ERROR: Couldn't turn off LED\r\n", LED_OFF, LOG_STATUS, 1);
		break;
	}
}

/**
 * turn on LED by colour
 * @args unit8_t color: pass in desired color of RGB LED
 */
void LED_on(uint8_t color){
	// set RGB LED Pins
	LED_off(ALL);
	switch(color){
	case RED:
		PTB->PCOR = MASK(RED_LED_SHIFT);
		PTB->PSOR = MASK(GREEN_LED_SHIFT);
		PTD->PSOR = MASK(BLUE_LED_SHIFT);
		Log_string("RED ON\r\n", LED_ON, LOG_DEBUG, 1);
		break;
	case GREEN:
		PTB->PSOR = MASK(RED_LED_SHIFT);
		PTB->PCOR = MASK(GREEN_LED_SHIFT);
		PTD->PSOR = MASK(BLUE_LED_SHIFT);
		Log_string("GREEN ON\r\n", LED_ON, LOG_DEBUG, 1);
		break;
	case BLUE:
		PTB->PSOR = MASK(RED_LED_SHIFT);
		PTB->PSOR = MASK(GREEN_LED_SHIFT);
		PTD->PCOR = MASK(BLUE_LED_SHIFT);
		Log_string("BLUE ON\r\n", LED_ON, LOG_DEBUG, 1);
		break;
	case ALL:
		PTB->PCOR = MASK(RED_LED_SHIFT);
		PTB->PCOR = MASK(GREEN_LED_SHIFT);
		PTD->PCOR = MASK(BLUE_LED_SHIFT);
		Log_string("ALL ON\r\n", LED_ON, LOG_DEBUG, 1);
		break;
	default:
		Log_string("ERROR: Couldn't turn off LED\r\n", LED_ON, LOG_STATUS, 1);
	}
}

// flash LED num_flashes times
int LED_flash(uint8_t color, uint8_t num_flashes){
	int flash = 0;
	Log_string("Flashing X times: ", LED_FLASH, LOG_DEBUG, 1);
	Log_integer(num_flashes, EMPTY_NAME, LOG_DEBUG, 1);
	for(int i = 0; i < num_flashes; i++){
		LED_on(color);
		Delay(1000000);
		LED_off(color);
		Delay(1000000);
		flash++;
	}
	return flash;
}
