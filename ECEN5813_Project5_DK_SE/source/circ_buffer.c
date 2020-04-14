/*
 * circ_buffer.c
 *
 *  Created on: Mar 31, 2020
 *      Author: Dhruva
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "global_defines.h"
#include "logger.h"
#include "MKL25Z4.h"
#include "circ_buffer.h"


// add item to buffer
int insert_item(circ_buf_t *buf, uint8_t data){
	// check if not full
	if(((buf->tail + 1) % buf->length) == buf->head){
		Log_string("ERROR: Buffer is full\r\n", INSERT_ITEM, LOG_DEBUG, 1);
		// if buffer is full then realloc buffer to twice the original length
		if(!extend_buf(buf)){
			Log_string("ERROR: Buffer extension failed\r\n", INSERT_ITEM, LOG_DEBUG, 1);
			return -1;
		}
	}
	// store data to tail of buffer and increment tail and current size of buffer
	buf->buffer[buf->tail] = data;
	buf->tail = (buf->tail + 1) % buf->length;
	buf->size++;
	return 0;
}

// remove oldest item from buffer (FIFO)
int remove_item(circ_buf_t *buf){
	uint8_t data = 0;
	// check if not empty
	if(buf->head == buf->tail){
		Log_string("ERROR: Buffer is empty\r\n", REMOVE_ITEM, LOG_DEBUG, 1);
		return -1;
	}
	// load data from head of buffer and decrement current size of buffer
	data = buf->buffer[buf->head];
	buf->head = (buf->head + 1) % buf->length;
	buf->size--;
	return data;
}

// check if buffer is full
inline
bool is_full(circ_buf_t *buf){
	if(((buf->tail + 1) % buf->length) == buf->head){
		return true;
	}else{
		return false;
	}
}

// check if buffer is empty
inline
bool is_empty(circ_buf_t *buf){
	if(buf->head == buf->tail){
		return true;
	} else{
		return false;
	}
}

// check buffer init
inline
bool check_buf(circ_buf_t *buf){
	if(buf->buf_status == BUF_INIT){
		return true;
	}else{
		return false;
	}
}

// check if buffer pointer is valid
inline
bool check_buf_ptr(circ_buf_t *buf){
	if(buf != NULL){
		return true;
	}else{
		return false;
	}
}

// initialize buffer of size length (plus null terminator)
circ_buf_t *init_buf(int length){
	circ_buf_t newBuf;
	circ_buf_t * newBufPtr = malloc(sizeof(newBuf));
	if(!check_buf_ptr(newBufPtr)){
		return NULL;
	}
	newBufPtr->length = length + 1;	// includes space for null terminator
	// calloc buffer of size length
	uint8_t* buf_arr = (uint8_t*)calloc(length, sizeof(uint8_t));
	newBufPtr->buffer = buf_arr;
	// set up head and tail
	newBufPtr->head = 0;
	newBufPtr->tail = 0;
	newBufPtr->size = 0;
	newBufPtr->buf_status = BUF_INIT;
	return newBufPtr;
}

// buffer destroy
int destroy_buf(circ_buf_t *buf){
	if(buf != NULL){
		buf->buf_status = BUF_NOTINIT;
		free(buf->buffer);
		free(buf);
		buf = NULL;
	}
	return 0;
}

// extend buffer to 2 times its original length if its full
// return false if it fails
inline
bool extend_buf(circ_buf_t *buf){
	buf->buffer = realloc(buf->buffer, (2 * buf->length));
	buf->length *= 2;
	if(check_buf_ptr(buf)){
		return true;
	} else{
		return false;
	}
}

