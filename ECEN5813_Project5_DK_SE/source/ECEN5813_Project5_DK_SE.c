/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ECEN5813_Project5_DK_SE.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "global_defines.h"
#include "timer.h"
#include "led_control.h"
#include "logger.h"

#include "circ_buffer.h"
#include "uart_poll.h"
#include "uart_interrupt.h"

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    // enable peripherals
    SysTick_init();

    // enable logging
    Log_enable();
#ifdef TESTING_MODE
    Log_level(LOG_TEST);
    UCUNIT_Init();
#else
    Log_level(LOG_DEBUG);
#endif

    // enable peripherals
    LED_init();
    LED_off(ALL);
#if UART_POLL
    //
#else// if UART_INT
    //
#endif

//    PRINTF("Hello World\n");

#if ECHO_MODE
    //
#else// if APP_MODE
    //
#endif
    circ_buf_t * buffer;
    int len = 10;
    char *foo = malloc(len * sizeof(char));
    buffer = init_buf(len);
    insert_item(buffer, 'A');
    insert_item(buffer, 'B');
    insert_item(buffer, 'C');
    remove_item(buffer, foo);
    PRINTF("foo is %c\r\n", *foo);
	remove_item(buffer, foo);
	PRINTF("foo is %c\r\n", *foo);
	remove_item(buffer, foo);
	PRINTF("foo is %c\r\n", *foo);

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
//        Delay(8000000);
//        Log_string("testing\r\n", MAIN, LOG_STATUS);
    }
    return 0 ;
}
