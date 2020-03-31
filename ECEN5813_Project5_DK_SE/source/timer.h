/*
 * timer.h
 *
 *  Created on: Mar 18, 2020
 *      Author: Dhruva
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdbool.h>
#include <stdint.h>

void SysTick_init(void);
void SysTick_Handler();
void SysTick_enable(void);
void SysTick_disable(void);

#endif /* TIMER_H_ */
