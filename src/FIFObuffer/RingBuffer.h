/**
 * @Name    : RingBuffer.h
 * @Date    : 2020年1月14日 下午8:35:41
 * @Author  : Rick Kimball
 * @brief   :
 */

#ifndef FIFOBUFFER_RINGBUFFER_H_
#define FIFOBUFFER_RINGBUFFER_H_

#include "Arduino/Basic.h"



/**
 * ringbuffer_t - provide a circular_buffer without disabling interrupts
 * expects a power of 2 container, and only one reader and one writer
 * container capacity SIZE-1
 */
template <
	typename T, 								/* works with any type */
	uint32_t SIZE, 								/* how many elements-1 must be power of 2 */
	typename POP_T = int16_t, 					/* return type of pop_front */
	POP_T EMPTY_ELEM = static_cast<POP_T>(-1)	/* default return value when empty */
>
struct ringbuffer_t {

		template <uint32_t SIZE_pow>		// 使用模板的方式檢查是否為2的次方
		struct is_power_of_two {
				enum {
					val = (SIZE_pow >= 2) & (SIZE_pow > 1) & !(SIZE_pow & (SIZE_pow - 1))
				};
				static const unsigned badSIZE[(val == 1) ? 1 : -1]; // SIZE is not a power of 2 if you an error here.
		};

		/**
		 * uint16x2_t - a union containing 16 bit head and tail offsets into the ring buffer. The union
		 * allows the c code to grab both values with one assembler instruction access.
		 *
		 */
		union uint16x2_t {
				// access as 32 bit
				uint32_t both;
				// -- or as 2 16 bit values --
				struct {
						uint16_t head;
						uint16_t tail;
				};
		};


		// --- private structure data ---
		// although variables are accessible because this is a struct

		volatile uint16x2_t offsets; // comes first so we can use 0 offset to variables
									 // for fastest access
		T elements[SIZE];

		enum {
			CAPACITY = SIZE - 1
		}; // leave one slot open

		is_power_of_two<SIZE> check_buffer_size; // your SIZE is not a power of 2, if you get an error here

		// --- public methods ---

		// write access zeros out head and tail
		inline void clear(void) {
			offsets.both = 0;
		}

		// return the count of used slots
		size_t available() {
			register uint16x2_t temp = { offsets.both };

			temp.both = (temp.head - temp.tail) & CAPACITY;

			return temp.both;
		}

		// return maximum number of slots available
		size_t capacity() {
			return CAPACITY;
		}

		// returns true when there is no used slots
		bool inline empty() {
			return !available();
		}

		// returns true when all slots used
		bool inline full() {
			return available() == capacity();
		}

		/**
		 *	@brief:	push_back() - adds an element to the end of the queue
		 *
		 *	@Note:	affects head, reads tail, element ignored if overflow ~300 ns @72MHz
		 */
		void push_back(const T element) {
			register uint16_t temp_head = offsets.head;

			elements[temp_head++] = element;
			temp_head &= CAPACITY;
			if (temp_head != offsets.tail) { // !full
				offsets.head = temp_head;
			}

			return;
		}

		// no bounds check version, affects head ~250 ns @72MHz
		void push_back_nc(const T element) {
			register uint16_t temp_head = offsets.head;

			elements[temp_head++] = element;
			offsets.head = temp_head & CAPACITY;
			return;
		}

		// affects tail, reads head
		POP_T pop_front(void) {
			register uint16x2_t temp = { offsets.both };

			if ((temp.head - temp.tail) & CAPACITY) { // !empty
				POP_T elem = elements[temp.tail++];
				offsets.tail = temp.tail & CAPACITY;
				return elem;
			}

			return EMPTY_ELEM; // on underflow return default element
		}

		// no bounds check, affects tail
		POP_T pop_front_nc(void) {
			register uint16_t temp_tail = offsets.tail;

			POP_T elem = elements[temp_tail++];
			offsets.tail = temp_tail & CAPACITY;
			return elem;
		}

		POP_T peek(void){
			return elements[offsets.tail];
		}

};

#endif /* FIFOBUFFER_RINGBUFFER_H_ */
