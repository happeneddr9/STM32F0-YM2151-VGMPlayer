/*
 * strconv.h
 *
 *  Created on: 2020年3月24日
 *      Author: user
 */


#ifndef PRINT_STRCONV_H_
#define PRINT_STRCONV_H_

#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

constexpr const uint8_t DEC = 10;
constexpr const uint8_t HEX = 16;
constexpr const uint8_t OCT = 8;
constexpr const uint8_t BIN = 2;

namespace happe {
	void reverse(char str[], int length);
	char* itoa(int num, char* str, uint8_t base);
	char* utoa(unsigned int num, char* str, uint8_t base);
	char* ltoa(long long num, char* str, uint8_t base);
	char* ultoa(unsigned long long num, char* str, uint8_t base);
	char* ftostrf(float val, signed char width, unsigned char prec, char *sout);
	char* dtostrf(double val, signed char width, unsigned char prec,
			char *sout);

	uint32_t atou(const char * str);
	int atoi(const char *buff);

	static constexpr const char * FloatLabel[] = { "Nan", "Inf", "Ovf" };

	template<typename T>
	constexpr void swap(T& a, T& b) {
		T tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
	}

}



#endif /* PRINT_STRCONV_H_ */
