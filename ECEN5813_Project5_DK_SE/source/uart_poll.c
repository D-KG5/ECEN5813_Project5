/*
 * uart_poll.c
 *
 *  Created on: Apr 9, 2020
 *      Author: sagar
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "uart_poll.h"

//in main

 //   Init_UART0(115200);

 //   echofunc();
 //  appfunc();


//Function for UART hardware initialization including baud rate and serial message format
void Init_UART0(uint32_t baud_rate) {
	uint16_t sbr;
	uint8_t temp;

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
	sbr = (uint16_t)((BUS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
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



	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

//	UART0->C2 |= UART_C2_RE_MASK; //enable receive
//	UART0->C2 |= UART_C2_TE_MASK; //enable transmit

	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;
	// Clear the UART RDRF flag
//	temp = UART0->D;

}

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
		PRINTF("UART ready to transmit\n\r");
		return 1;

	}
	else
	{
		PRINTF("UART not ready to transmit\n\r");
		return 0;
	}

}


//Function to check whether the receiver has a new character to receive
uint8_t receive_check(void)
{
	//UART0->C2 &=~ UART0_C2_TE_MASK;
	if(UART0->C2 & 0x04)
	{
		PRINTF("UART ready to receive\n\r");
		return 1;

	}
	else
	{
		PRINTF("UART not ready to receive\n\r");
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

	printf("\n\r%c:%d\n\r",arr[i] ,arr_[arr[i]]);
    }




return 1;
}

