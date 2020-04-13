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
#ifdef TESTING_MODE
#include "circ_buf_test.h"
#endif

#include "UART.h"

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
    // enable UART
    Init_UART0(115200);

#if USE_UART_INTERRUPTS==0
    Send_String_Poll((uint8_t *)"Using Poll\r\n");
#else// if UART_INT
    Send_String((uint8_t *)"Using Interrupt\r\n");
#endif

#ifdef TESTING_MODE
    // run circular buffer tests
    run_tests();
#endif

#if ECHO_MODE
    while(1){
#if USE_UART_INTERRUPTS==0
    	echofunc();
#else
    	echo_function();
#endif
    }
#else// if APP_MODE
    while(1){
    	if(application_mode()){
    		break;
    	}
    }
#endif

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
        Delay(8000000);
#if USE_UART_INTERRUPTS==0
        Send_String_Poll((uint8_t *)"Finished\r\n");
#else
        Send_String((uint8_t *)"Finished\r\n");
#endif
//        Log_string("testing\r\n", MAIN, LOG_STATUS);
    }
    return 0 ;
}
