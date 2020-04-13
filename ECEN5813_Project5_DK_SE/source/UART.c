/*
 * UART.c
 *
 *  Created on: Apr 12, 2020
 *      Author: sagar
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "global_defines.h"
#include "logger.h"
#include "led_control.h"
#include "MKL25Z4.h"
#include "circ_buffer.h"
#include "UART.h"



static char * report;
circ_buf_t * TxQ;
circ_buf_t * RxQ;
int len = 128;
static char * report;

uint8_t c, * bp;
int frr[10];



//Function for UART hardware initialization as above with addition of interrupt configuration and enable
void Init_UART0(uint32_t baud_rate) {
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)(( BUS_CLOCK )/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

#if USE_UART_INTERRUPTS
	// in Tx/Rx buffers
	TxQ = init_buf((len + 1));
	RxQ = init_buf((len + 1));
	// Enable interrupts. Listing 8.11 on p. 234
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);
#endif
	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}

//polling functions

#if USE_UART_INTERRUPTS==0
//Function to transmit a character assuming transmitter is available

uint8_t Transmit_char(uint8_t data)
{
	UART0->D = data;
	return 1; // Transmit success
}


//Function waits for the transmitter to be available and then once available transmit a character and return
void UART0_Transmit_Poll(uint8_t data) {
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		Transmit_char(data);
	//	printf("%d\n\r",data);
}

//Function to receive the character assuming receiver has data
uint8_t Receive_char(void)
{
	return UART0->D;
}

//Function to wait  for the receiver to receive a new character and then return that character
uint8_t UART0_Receive_Poll(void) {
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		return Receive_char();
}

void Send_String_Poll(uint8_t * str) {
	// enqueue string
	while (*str != '\0') { // Send characters up to null terminator
		UART0_Transmit_Poll(*str++);
	}
}

//Function to check whether the transmitter is available to accept a new character for transmission
uint8_t transmit_check(void)
{
	if(UART0->C2 & 0x08)
	{
//		PRINTF("UART ready to transmit\n\r");
		return 1;

	}
	else
	{
//		PRINTF("UART not ready to transmit\n\r");
		return 0;
	}

}


//Function to check whether the receiver has a new character to receive
uint8_t receive_check(void)
{
	//UART0->C2 &=~ UART0_C2_TE_MASK;
	if(UART0->C2 & 0x04)
	{
//		PRINTF("UART ready to receive\n\r");
		return 1;

	}
	else
	{
//		PRINTF("UART not ready to receive\n\r");
		return 0;
	}

}

uint8_t echofunc()
{
	char inputchar;

	receive_check();
	inputchar=UART0_Receive_Poll();
	transmit_check();
	UART0_Transmit_Poll(inputchar);

	return 1;

}

uint8_t appfunc()
{
	char inputchar;
	report = malloc(sizeof(char) * 9);
	uint32_t masking_state;
	int arr[10];
    int i=0;
    int j=0;
    uint32_t arr_[128]={0};
    //Receives the five character and triggers to printing
	while(i<5)
	{
		receive_check();
		inputchar=UART0_Receive_Poll();
		arr[i]=inputchar;
		arr_[arr[i]]+=1;
		i++;
	}

//Transmit the five characters and prints
	transmit_check();

	   for(j=0;j<i;j++)
	    {

	    	UART0_Transmit_Poll(arr[j]);
	    }

//sorting the array in ASCI order
	int k;
	int p;
	char temp;
    int Size=5;
	for (k = 0; k < Size; k++)
	{
		for (p = k + 1; p < Size; p++)
		{
			if(arr[k] > arr[p])
			{
				temp = arr[k];
				arr[k] = arr[p];
				arr[p] = temp;
			}

		}
	}
//http://www.crazyforcode.com/write-code-to-remove-duplicates-in-a-sorted-array/
	int x,y=0;
	   // Remove the duplicates ...
	   for (x = 0; x < 5; x++)
	   {
	     if (arr[x] != arr[y])
	     {
	       y++;
	       arr[y] = arr[x]; // Move it to the front
	     }
	   }

	   // The new array size..
	   Size = (y + 1);



//For count of the unique characters that have been received by the UART device driver
    for(i=0;i<Size;i++)
    {
    	masking_state = __get_PRIMASK();
    	START_CRITICAL();
    	snprintf(report, 9, "\n\r%c:%d\n\r", arr[i], arr_[arr[i]]);
    	Send_String_Poll((uint8_t *)report);
    	END_CRITICAL(masking_state);
    }




return 1;
}

#endif

#if USE_UART_INTERRUPTS

//interrupt functions

void UART0_IRQHandler(void) {
	uint8_t ch;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			ch = UART0->D;
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		// received a character
		ch = UART0->D;
		if (!is_full(RxQ)) {
			insert_item(RxQ, ch);
		} else {
			// error - queue full.
			// discard character
		}
	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) ) { // tx buffer empty
		// can send another character
		if (!is_empty(TxQ)) {
			UART0->D = remove_item(TxQ);
		} else {
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

uint32_t Rx_Chars_Available(void) {
	return RxQ->size;
}

uint8_t	Get_Rx_Char(void) {
	return remove_item(RxQ);
}


//Transmit character
void Send_String(uint8_t * str) {

	// enqueue string
	//printf("UART Ready to transmit\n\r");
	while (*str != '\0') { // copy characters up to null terminator
		while (is_full(TxQ))
			; // wait for space to open up

		insert_item(TxQ, *str);
		str++;
	}

	// start transmitter if it isn't already running
	if (!(UART0->C2 & UART0_C2_TIE_MASK)) {
		UART0->D = remove_item(TxQ);
		UART0->C2 |= UART0_C2_TIE(1);
	}

}

//Receive string
void Receive_String(void)
{

//	printf("UART Ready to Receive\n\r");
//	printf("Ready to receive");
	while ((RxQ->size) == 0)
		; // wait for character to arrive

	c = remove_item(RxQ);//reads the character recevied

	//for application mode

	// Blocking transmit

	bp = &c; //adds the character to the bp

}

uint8_t echo_function()
{
	Receive_String();

	if(c == '\0') //if no character received then it fails
	{
	//	printf("Receive failed\n\r");
		Stop_Transmitting; //if failed stop transmitting
		return 0;
	}

	Send_String(bp);

	printf("You pressed %c\n\r", c);
	return 1;
}


uint8_t application_mode()
{
	int count=0;
	uint32_t frr_[128]={0};
	report = malloc(sizeof(char) * 9);
	uint32_t masking_state;

	while(count<5)
	{
		Receive_String();
		Send_String(bp);
		frr[count]= c;
		frr_[frr[count]]+=1;

		count++;

	}

	//sorting the array in ASCI order
	int k;
	int p;
	char temp;
    int Size=5;
	for (k = 0; k < Size; k++)
	{
		for (p = k + 1; p < Size; p++)
		{
			if(frr[k] > frr[p])
			{
				temp = frr[k];
				frr[k] = frr[p];
				frr[p] = temp;
			}

		}
	}
	//http://www.crazyforcode.com/write-code-to-remove-duplicates-in-a-sorted-array/
	int x,y=0;
	   // Remove the duplicates ...
	   for (x = 0; x < 5; x++)
	   {
	     if (frr[x] != frr[y])
	     {
	       y++;
	       frr[y] = frr[x]; // Move it to the front
	     }
	   }

	   // The new array size..
	   Size = (y + 1);

	   //For count of the unique characters that have been received by the UART device driver
    for(count=0;count<Size;count++)
    {
    	masking_state = __get_PRIMASK();
    	START_CRITICAL();
    	snprintf(report, 9, "\n\r%c:%d\n\r", frr[count], frr_[frr[count]]);
    	Send_String((uint8_t *)report);
    	END_CRITICAL(masking_state);
//	printf("\n\r%c:%d\n\r",frr[count] ,frr_[frr[count]]);
    }
    return 1;
}





#endif
