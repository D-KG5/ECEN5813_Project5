/*
 * UART.h
 *
 *  Created on: Apr 12, 2020
 *      Author: sagar
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"


#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6)
#define SYS_CLOCK							(48e6)
#define Start_Transmitting UART0->C2 |= UART_C2_RIE(1)
#define Stop_Transmitting  UART0->C2 &= ~UART_C2_TIE_MASK

void Init_UART0(uint32_t baud_rate);

//polling fucntions
void UART0_Transmit_Poll(uint8_t data);
uint8_t UART0_Receive_Poll(void);
void Send_String_Poll(uint8_t * str);
uint8_t receive_check(void);
uint8_t transmit_check(void);
uint8_t echofunc(void);


//interrupt functions
void UART0_IRQHandler(void);
uint32_t Rx_Chars_Available(void);
uint8_t	Get_Rx_Char(void);
void Send_String(uint8_t * str);
void Receive_String(void);
uint8_t echo_function();

//application mode for poll and interrupt mode
uint8_t application_mode();


#endif /* UART_H_ */
