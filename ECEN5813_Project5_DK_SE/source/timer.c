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

bool timeout = false;

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

// systick interrupt handler
void SysTick_Handler(){
    timeout = true;	// set timeout bool to true
}

