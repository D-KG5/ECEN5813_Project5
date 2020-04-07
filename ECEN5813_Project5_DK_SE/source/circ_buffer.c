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
#include "led_control.h"
#include "MKL25Z4.h"
#include "circ_buffer.h"

buf_status_t buf_status;

// add item
int insert_item(circ_buf_t *buf, char data){
	// check if not full
	if(((buf->tail + 1) % buf->length) == buf->head){
		return -1;
	}
	buf->buffer[buf->tail] = data;
	buf->tail = (buf->tail + 1) % buf->length;
	return 0;
}

// remove oldest item
int remove_item(circ_buf_t *buf, char *data){
	// check if not empty
	if(buf->head == buf->tail){
		return -1;
	}
	*data = buf->buffer[buf->head];
	buf->head = (buf->head + 1) % buf->length;
	PRINTF("Removed %c\r\n", *data);
	return 0;
}

// check buffer full
bool is_full(circ_buf_t *buf){
	if(((buf->tail + 1) % buf->length) == buf->head){
		return true;
	}else{
		return false;
	}
}
// check buffer empty
bool is_empty(circ_buf_t *buf){
	if(buf->head == buf->tail){
		return true;
	} else{
		return false;
	}
}
// check buffer init
bool check_buf(void){
	if(buf_status == BUF_SUCCESS){
		return true;
	}else{
		return false;
	}
}
// check buffer pointer valid
bool check_buf_ptr(circ_buf_t *buf){
	if(buf != NULL){
		return true;
	}else{
		return false;
	}
}
// buffer init
circ_buf_t *init_buf(int length){
	circ_buf_t newBuf;
	circ_buf_t * newBufPtr = malloc(sizeof(newBuf));
	if(!check_buf_ptr(newBufPtr)){
		PRINTF("Failed to init buffer\r\n");
		buf_status = BUF_FAILED;
		return NULL;
	}
	newBufPtr->length = length;
	char *buf_arr = malloc(length * sizeof(char));
	newBufPtr->buffer = buf_arr;
	for(int i = 0; i < newBufPtr->length; i++){
		newBufPtr->buffer[i] = '0';
	}
	newBufPtr->head = 0;
	newBufPtr->tail = 0;
	buf_status = BUF_SUCCESS;
	PRINTF("Init buffer success\r\n");
	return newBufPtr;
}
// buffer destroy
int destroy_buf(circ_buf_t *buf){
	if(buf != NULL){
		free(buf->buffer);
		free(buf);
		buf = NULL;
	}
	return 0;
}
