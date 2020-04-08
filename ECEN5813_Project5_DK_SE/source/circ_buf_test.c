/*
 * circ_buf_test.c
 *
 *  Created on: Apr 7, 2020
 *      Author: Dhruva
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "global_defines.h"
#include "circ_buffer.h"

void buf_destroy_test(void);
void buf_full_test(void);
void buf_not_full_test(void);
void buf_create_test(void);
void buf_overfill_test(void);
void buf_overempty_test(void);
void buf_data_access_test(void);

void run_tests(void){
	// run tests
	buf_create_test();
	buf_destroy_test();
	buf_data_access_test();
	buf_full_test();
	buf_not_full_test();
	buf_overfill_test();
	buf_overempty_test();

    UCUNIT_WriteSummary();
    UCUNIT_Shutdown();
}

void buf_destroy_test(void){
	UCUNIT_TestcaseBegin("Buffer Destroy Test\r\n");
//    destroy buffer
    circ_buf_t * buffer1;
    int len = 9;
    uint8_t *foo = malloc((len + 1) * sizeof(uint8_t));

    buffer1 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer1));

    // true
    UCUNIT_CheckIsEqual(true, is_empty(buffer1));
    insert_item(buffer1, 'A'); // 1
    insert_item(buffer1, 'B'); // 2
    insert_item(buffer1, 'C'); // 3
    insert_item(buffer1, 'D'); // 4
    insert_item(buffer1, 'E'); // 5
    insert_item(buffer1, 'F'); // 6
    insert_item(buffer1, 'G'); // 7
    insert_item(buffer1, 'H'); // 8
    insert_item(buffer1, 'I'); // 9

    // true
    UCUNIT_CheckIsEqual(true, is_full(buffer1));
    remove_item(buffer1, foo);
    remove_item(buffer1, foo);
    remove_item(buffer1, foo);
    remove_item(buffer1, foo);

    destroy_buf(buffer1);
    // true
    UCUNIT_CheckIsEqual(true, check_buf_ptr(buffer1));
    free(foo);
    UCUNIT_TestcaseEnd();
}

void buf_full_test(void){
	UCUNIT_TestcaseBegin("Buffer Full Test\r\n");
//  buffer is full
    circ_buf_t * buffer2;
    int len = 9;

    buffer2 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer2));

    // true
    UCUNIT_CheckIsEqual(true, is_empty(buffer2));
    insert_item(buffer2, 'A'); // 1
    insert_item(buffer2, 'B'); // 2
    insert_item(buffer2, 'C'); // 3
    insert_item(buffer2, 'D'); // 4
    insert_item(buffer2, 'E'); // 5
    insert_item(buffer2, 'F'); // 6
    insert_item(buffer2, 'G'); // 7
    insert_item(buffer2, 'H'); // 8
    insert_item(buffer2, 'I'); // 9
    //true
    UCUNIT_CheckIsEqual(true, is_full(buffer2));

    destroy_buf(buffer2);
	UCUNIT_TestcaseEnd();
}

void buf_not_full_test(void){
	UCUNIT_TestcaseBegin("Buffer Not Full Test\r\n");
	// buffer3 is not full
    circ_buf_t * buffer3;
    int len = 9;

    buffer3 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer3));

    // true
    UCUNIT_CheckIsEqual(true, is_empty(buffer3));
    insert_item(buffer3, 'A'); // 1
    insert_item(buffer3, 'B'); // 2
    insert_item(buffer3, 'C'); // 3
    insert_item(buffer3, 'D'); // 4
    insert_item(buffer3, 'E'); // 5
    insert_item(buffer3, 'F'); // 6
    insert_item(buffer3, 'G'); // 7
    insert_item(buffer3, 'H'); // 8
    // false
    UCUNIT_CheckIsEqual(false, is_full(buffer3));

    destroy_buf(buffer3);
	UCUNIT_TestcaseEnd();
}

void buf_create_test(void){
	UCUNIT_TestcaseBegin("Buffer Create Test\r\n");
    // create buffer
    circ_buf_t * buffer4;
    int len = 9;
    buffer4 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer4));
    destroy_buf(buffer4);
	UCUNIT_TestcaseEnd();
}

void buf_overfill_test(void){
	UCUNIT_TestcaseBegin("Buffer Overfill Test\r\n");
//     overfill
    circ_buf_t * buffer5;
    int len = 9;
    int ret = 0;

    buffer5 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer5));

    // true
    UCUNIT_CheckIsEqual(true, is_empty(buffer5));
    ret = insert_item(buffer5, 'A'); // 1
    ret = insert_item(buffer5, 'B'); // 2
    ret = insert_item(buffer5, 'C'); // 3
    ret = insert_item(buffer5, 'D'); // 4
    ret = insert_item(buffer5, 'E'); // 5
    ret = insert_item(buffer5, 'F'); // 6
    ret = insert_item(buffer5, 'G'); // 7
    ret = insert_item(buffer5, 'H'); // 8
    ret = insert_item(buffer5, 'I'); // 9
    ret = insert_item(buffer5, 'J'); // 10 (overfill)
    UCUNIT_CheckIsEqual(-1, ret);

    destroy_buf(buffer5);
	UCUNIT_TestcaseEnd();
}

void buf_overempty_test(void){
	UCUNIT_TestcaseBegin("Buffer Overempty Test\r\n");
    // overempty
    circ_buf_t * buffer6;
    int len = 9;
    int ret = 0;
    uint8_t *foo = malloc((len + 1) * sizeof(uint8_t));

    buffer6 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer6));

    // true
    UCUNIT_CheckIsEqual(true, is_empty(buffer6));
    ret = insert_item(buffer6, 'A'); // 1
    ret = insert_item(buffer6, 'B'); // 2
    ret = insert_item(buffer6, 'C'); // 3
    ret = insert_item(buffer6, 'D'); // 4
    ret = insert_item(buffer6, 'E'); // 5
    ret = insert_item(buffer6, 'F'); // 6
    ret = insert_item(buffer6, 'G'); // 7
    ret = insert_item(buffer6, 'H'); // 8
    ret = insert_item(buffer6, 'I'); // 9
    UCUNIT_CheckIsEqual(0, ret);

    //true
    UCUNIT_CheckIsEqual(true, is_full(buffer6));
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    ret = remove_item(buffer6, foo);
    UCUNIT_CheckIsEqual(-1, ret);

    destroy_buf(buffer6);
    free(foo);
    UCUNIT_TestcaseEnd();
}

void buf_data_access_test(void){
	UCUNIT_TestcaseBegin("Buffer data Access Test\r\n");
//     test data access
    circ_buf_t * buffer7;
    int len = 9;
    int ret = 0;
    uint8_t *foo = malloc((len + 1) * sizeof(uint8_t));

    buffer7 = init_buf((len + 1));
    UCUNIT_CheckIsEqual(true, check_buf(buffer7));
    // true
    UCUNIT_CheckIsEqual(true, is_empty(buffer7));
    ret = insert_item(buffer7, 'A'); // 1
    ret = insert_item(buffer7, 'B'); // 2
    ret = insert_item(buffer7, 'C'); // 3
    ret = insert_item(buffer7, 'D'); // 4
    ret = insert_item(buffer7, 'E'); // 5
    ret = insert_item(buffer7, 'F'); // 6
    UCUNIT_CheckIsEqual(0, ret);

    ret = remove_item(buffer7, foo);
    UCUNIT_CheckIsEqual('A', *foo);
    ret = remove_item(buffer7, foo);
    UCUNIT_CheckIsEqual('B', *foo);
	ret = remove_item(buffer7, foo);
	UCUNIT_CheckIsEqual('C', *foo);

	UCUNIT_CheckIsEqual(0, ret);
	destroy_buf(buffer7);
	free(foo);
	UCUNIT_TestcaseEnd();
}
