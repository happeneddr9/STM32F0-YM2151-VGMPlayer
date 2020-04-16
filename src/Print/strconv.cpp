/*
 * strconv.cpp
 *
 *  Created on: 2020年3月24日
 *      Author: user
 */

#include "strconv.h"

namespace happe {

	/* A utility function to reverse a string  */
	void reverse(char str[], int length) {
		int start = 0;
		int end = length - 1;
		while (start < end) {
			swap(*(str + start), *(str + end));
			start++;
			end--;
		}
	}

	// Implementation of utoa()
	char* utoa(unsigned int num, char* str, uint8_t base) {
		size_t i = 0;

		// prevent crash if called with base == 1
		if (base < BIN)
			base = DEC;
		/* Handle 0 explicitely, otherwise empty string is printed for 0 */
		if (num == 0) {
			str[i++] = '0';
			str[i] = '\0';
			return str;
		}

		// Process individual digits
		while (num != 0) {
			char rem = num % base;
			str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
			num = num / base;
		}

		str[i] = '\0'; // Append string terminator

		// Reverse the string
		reverse(str, i);

		return str;
	}

	// Implementation of itoa()
	char* itoa(int num, char* str, uint8_t base) {
		size_t i = 0;
		if (num < 0 && base == DEC) {
			str[i++] = '-';
			num = -num;
		}

		utoa(num, &str[i], base);
		return str;
	}

	// Implementation of ultoa()
	char* ultoa(unsigned long long num, char* str, uint8_t base) {
		size_t i = 0;

		// prevent crash if called with base == 1
		if (base < BIN)
			base = DEC;
		/* Handle 0 explicitely, otherwise empty string is printed for 0 */
		if (num == 0) {
			str[i++] = '0';
			str[i] = '\0';
			return str;
		}

		// Process individual digits
		while (num != 0) {
			char rem = num % base;
			str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
			num = num / base;
		}

		str[i] = '\0'; // Append string terminator

		// Reverse the string
		reverse(str, i);

		return str;
	}

	// Implementation of ltoa()
	char* ltoa(long long num, char* str, uint8_t base) {
		size_t i = 0;
		if (num < 0 && base == DEC) {
			str[i++] = '-';
			num = -num;
		}

		ultoa(num, &str[i], base);
		return str;
	}

	char* ftostrf(float val, signed char width, unsigned char prec,
			char *sout) {
		size_t n = 0;

		if (isnan(val))
			return strcat(sout, FloatLabel[0]);
		if (isinf(val))
			return strcat(sout, FloatLabel[1]);
		if (val > 4294967040.0f) 				// If use MCU System,
			return strcat(sout, FloatLabel[2]);	// float to unsigned long max =  4294967040.0
												// constant determined empirically
		if (val < -4294967040.0f)
			return strcat(sout, FloatLabel[2]);	// constant determined empirically

		// Handle negative numbers
		if (val < 0.0) {
			sout[n++] = '-';
			val = -val;
		}

		// Round correctly so that print(1.999, 2) prints as "2.00"
		float rounding = 0.5;
		for (uint8_t i = 0; i < prec; ++i)
			rounding /= 10.0;

		val += rounding;

		// Extract the integer part of the number and print it
		unsigned long int_part = (unsigned long) val;
		float remainder = val - (float) int_part;
		utoa(int_part, &sout[n++], DEC);
		n += strlen(&sout[n]);

		// Print the decimal point, but only if there are digits beyond
		if (prec > 0) {
			sout[n++] = '.';
		}

		// Extract digits from the remainder one at a time
		while (prec-- > 0) {
			remainder *= 10.0;
			unsigned int toPrint = (unsigned int) (remainder);
			utoa(toPrint, &sout[n++], DEC);
			remainder -= toPrint;
		}

		return sout;
	}

	char * dtostrf(double val, signed char width, unsigned char prec,
			char *sout) {
		size_t n = 0;

		if (isnan(val))
			return strcat(sout, FloatLabel[0]);
		if (isinf(val))
			return strcat(sout, FloatLabel[1]);
		if (val > 18446744073709551615.0) 				// If use MCU System,
			return strcat(sout, FloatLabel[2]);	// float to unsigned long max =  4294967040.0
												// constant determined empirically
		if (val < -18446744073709551615.0)
			return strcat(sout, FloatLabel[2]);	// constant determined empirically

		// Handle negative numbers
		if (val < 0.0) {
			sout[n++] = '-';
			val = -val;
		}

		// Round correctly so that print(1.999, 2) prints as "2.00"
		double rounding = 0.5;
		for (uint8_t i = 0; i < prec; ++i)
			rounding /= 10.0;

		val += rounding;

		// Extract the integer part of the number and print it
		unsigned long long int_part = (unsigned long long) val;
		double remainder = val - (double) int_part;
		ultoa(int_part, &sout[n++], DEC);
		n += strlen(&sout[n]);

		// Print the decimal point, but only if there are digits beyond
		if (prec > 0) {
			sout[n++] = '.';
		}

		// Extract digits from the remainder one at a time
		while (prec-- > 0) {
			remainder *= 10.0;
			unsigned int toPrint = (unsigned int) (remainder);
			utoa(toPrint, &sout[n++], DEC);
			remainder -= toPrint;
		}

		return sout;
	}

	uint32_t atou(const char * str) {
		unsigned int val = 0;
		while (*str) {
			val = (val << 1) + (val << 3) + *(str++) - '0';
		}
		return val;
	}

	int atoi(const char *buff) {
		int c = 0, sign = 0, x = 0;
		const char *p = buff;

		/**
		 *	ASCII[45] = '-'
		 *	ASCII[47] = '/'
		 *  ASCII[48] = '0'
		 *  ASCII[57] = '9'
		 *  ASCII[58] = ':'
		 */
		for (c = *(p++); (c < 48 || c > 57); c = *(p++)) {
			if (c == 45) {
				sign = 1;
				c = *(p++);
				break;
			}
		}; // eat whitespaces and check sign
		for (; c > 47 && c < 58; c = *(p++))
			x = (x << 1) + (x << 3) + c - 48;

		return sign ? -x : x;
	}

}
