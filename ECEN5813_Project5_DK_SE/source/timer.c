/*
 * timer.c
 *
 *  Created on: Mar 18, 2020
 *      Author: Dhruva
 */

#include "timer.h"
#include "logger.h"
#include "global_defines.h"
#include "MKL25Z4.h"
#include <stdio.h>

// timestamp counter vars for tenths, seconds, minutes, hours
uint8_t timestamp_counter_n = 0;
uint8_t timestamp_counter_s = 0;
uint8_t timestamp_counter_m = 0;
uint8_t timestamp_counter_h = 0;

// initialize systick timer for ~0.1 second interval and interrupts
void SysTick_init(void) {
	SysTick->LOAD = (48000000L/320U); // 10Hz
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
}

// enable systick interrupts
void SysTick_enable(void){
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
}

// disable systick interrupts
void SysTick_disable(void){
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_DisableIRQ(SysTick_IRQn);
}

// systick interrupt handler to increment timestamp counter vars in a critical section
void SysTick_Handler(){
	START_CRITICAL();
	timestamp_counter_n++;
	if(timestamp_counter_n == 10){
		timestamp_counter_n = 0;
		timestamp_counter_s++;
	}
	if(timestamp_counter_s == 60){
		timestamp_counter_s = 0;
		timestamp_counter_m++;
	}
	if(timestamp_counter_m == 60){
		timestamp_counter_m = 0;
		timestamp_counter_h++;
	}
    END_CRITICAL();
}

