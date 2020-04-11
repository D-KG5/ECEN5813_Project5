/*
 * uart_interrupt.h
 *
 *  Created on: Mar 31, 2020
 *      Author: Dhruva
 */

#ifndef UART_INTERRUPT_H_
#define UART_INTERRUPT_H_

#include <stdint.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6)
#define SYS_CLOCK							(48e6)


void Init_UART0_(uint32_t baud_rate);

void UART0_IRQHandler(void);

uint32_t Rx_Chars_Available(void);
uint8_t	Get_Rx_Char(void);
void Send_String(uint8_t * str);
void Receive_String(void);
uint8_t echo_function();
uint8_t application_mode();

#endif /* UART_INTERRUPT_H_ */
