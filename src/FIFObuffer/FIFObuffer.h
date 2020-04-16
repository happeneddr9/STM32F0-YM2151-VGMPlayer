/*
 * FIFObuffer.h
 *
 *  Created on: 2019年11月17日
 *      Author: happe
 */

#ifndef FIFOBUFFER_FIFOBUFFER_H_
#define FIFOBUFFER_FIFOBUFFER_H_

#include "main.h"
#include <stdio.h>
#include <array>


/**
 * @brief   : like Queue
 *
 */
template <typename DataType, size_t len, typename headerType = uint8_t>
class FIFO {
	public:
		FIFO(void) {
			buff_head = 0;
			buff_tail = 0;
		}

		void push(DataType n) {
			buff[buff_head] = n;
			buff_head++;                         //接收指針
			if (buff_head >= len)
				buff_head = 0;
		}

		inline void clear(){
			buff_tail = buff_head;
		}

		inline size_t size(void){
			return ((size_t) (len + buff_head - buff_tail)) % len;
		}

		inline size_t max_size(void){
			return len;
		}

		inline bool not_empty(void){
			return (buff_head != buff_tail);
		}

		DataType get(void) {
			if (buff_head == buff_tail) {
				return -1;
			} else {
				DataType& c = buff[buff_tail];
				buff_tail = (buff_tail + 1) % len;
				return c;
			}
		}

		DataType peek(void) {
			if (buff_head == buff_tail) {
				return -1;
			} else {
				return buff[buff_tail];
			}
		}

		DataType at(size_t n){
			if(n < len)
				return buff[n];
			else
				return 0;
		}

		DataType operator[](size_t n){
			return buff[n];
		}

	private:

	protected:
		volatile headerType buff_head;
		volatile headerType buff_tail;
		DataType buff[len];

		/*if constexpr (len < 256) {
		 uint8_t buff_head;
		 uint8_t buuff_tail;
		 } else if constexpr (len < 65536) {
		 uint16_t buff_head;
		 uint16_t buuff_tail;
		 } else constexpr{
		 uint32_t buff_head;
		 uint32_t buuff_tail;
		 }*/
};

#endif /* FIFOBUFFER_FIFOBUFFER_H_ */
