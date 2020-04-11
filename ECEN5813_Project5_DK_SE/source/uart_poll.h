/*
 * uart_poll.h
 *
 *  Created on: Apr 9, 2020
 *      Author: sagar
 */

#ifndef UART_POLL_H_
#define UART_POLL_H_

#include <stdint.h>
#include <MKL25Z4.H>



#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6)
#define SYS_CLOCK							(48e6)

void Init_UART0(uint32_t baud_rate);
void UART0_Transmit_Poll(uint8_t data);
uint8_t UART0_Receive_Poll(void);

void Send_String_Poll(uint8_t * str);

uint8_t receive_check(void);
uint8_t transmit_check(void);
uint8_t echofunc(void);
uint8_t appfunc(void);
#endif /* UART_POLL_H_ */
