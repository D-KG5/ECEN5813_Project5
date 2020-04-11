/*
 * circ_buffer.h
 *
 *  Created on: Mar 31, 2020
 *      Author: Dhruva
 */

#ifndef CIRC_BUFFER_H_
#define CIRC_BUFFER_H_

#include <stdint.h>

typedef enum buf_status_t{
	BUF_SUCCESS,	// no error
	BUF_FAILED,		// failure case
	BUF_INIT,
	BUF_NOTINIT
} buf_status_t;

typedef struct{
	uint8_t *buffer;
	int head;
	int tail;
	int length;	// total length of buffer
	int size;	// current size of buffer
	buf_status_t buf_status;
} circ_buf_t;

// extend buffer if its full
bool extend_buf(circ_buf_t *buf);
// add item
int insert_item(circ_buf_t *buf, uint8_t data);
// remove oldest item
int remove_item(circ_buf_t *buf);
// check buffer full
bool is_full(circ_buf_t *buf);
// check buffer empty
bool is_empty(circ_buf_t *buf);
// check buffer init
bool check_buf(circ_buf_t *buf);
// check buffer pointer valid
bool check_buf_ptr(circ_buf_t *buf);
// buffer init
circ_buf_t *init_buf(int length);
// buffer destroy
int destroy_buf(circ_buf_t *buf);
#endif /* CIRC_BUFFER_H_ */
