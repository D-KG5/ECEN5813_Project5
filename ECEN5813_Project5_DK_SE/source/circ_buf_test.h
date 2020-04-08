/*
 * circ_buf_test.h
 *
 *  Created on: Apr 7, 2020
 *      Author: Dhruva
 */

#ifndef CIRC_BUF_TEST_H_
#define CIRC_BUF_TEST_H_

void run_tests(void);
void buf_destroy_test(void);
void buf_full_test(void);
void buf_not_full_test(void);
void buf_create_test(void);
void buf_overfill_test(void);
void buf_overempty_test(void);
void buf_data_access_test(void);

#endif /* CIRC_BUF_TEST_H_ */
