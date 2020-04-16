/*
 * Print.cpp
 *
 *  Created on: 2019年9月7日
 *      Author: happe
 *
 *      從HWSerial搬移過來
 *
 */

#include "Print.h"
#include <assert.h>




cpp_io::_setprecision Print::_M_precision = { 2, false };
cpp_io::_setBass Print::_M_Base = { DEC };





/*Print::Print(void){

 }*/

/**
 *  @name   : write
 *  @brief  : write char or char array               //default implementation: may be overriden
 *            write(char c) should be overriden
 *
 *  @param  : char string or unsigned char string
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::write(const char *str) {
	size_t n = 0;
	while (*str != '\0') {
		if (write(*str++))
			n++;
		else
			break;
	}
	return n;
}

size_t Print::write(const uint8_t *cstr, size_t size) {
	size_t n = 0;
	while (size--) {
		if (write(*cstr++))
			n++;
		else
			break;
	}
	return n;
}

/**
 *  @name   : print
 *  @brief  : print string, integer or floating point
 *  @param  : integer, floating point, char string or std::string(c++ std)
 *  @param  : int base :change the positional system,
 *                      may be Hexadecimal, Decimal, Octal ,Binary or others
 *  @return : size_t, how many character have been Print out
 *
 */
size_t Print::print(void * p) {
	return printPointer(p);
}

size_t Print::print(const void * p) {
	return print((void *) p);
}

size_t Print::print(bool boo) {
	return print(boo ? "true" : "false");
}

size_t Print::print(char c) {
	return write(c);
}

size_t Print::print(const char * str) {
	return write(str);
}

size_t Print::print(short n, int base) {
	return print((int) n, base);
}

size_t Print::print(unsigned short n, int base) {
	return print((unsigned int) n, base);
}

size_t Print::print(int n, int base) {
	if (base == 0) {
		return print('X');
	} else if (n < 0 && base == DEC) {
		size_t len = print('-');
		n = -n;
		return printShort((unsigned long) n, base) + len;
	}
	return printShort((unsigned long) n, base);
}

size_t Print::print(unsigned int n, int base) {
	if (base == 0)
		return print('X');
	else
		return printShort(n, base);
}

size_t Print::print(long n, int base) {
	return print((int) n, base);
}

size_t Print::print(unsigned long n, int base) {
	return print((unsigned int) n, base);
}

size_t Print::print(long long n, int base) {
	//size_t n = 0;
	if (base == 0) {
		return print('X');
	} else if (base == 10) {
		if (n < 0) {
			size_t len = print('-');
			n = -n;
			return printNumber(n, base) + len;
		}
		return printNumber(n, base);
	} else {
		return printNumber(n, base);
	}
	//return n;
}

size_t Print::print(unsigned long long n, int base) {
	if (base == 0)
		return print('X');
	else
		return printNumber(n, base);
}

size_t Print::print(float n, int digits, bool Sci) {
	if (Sci == true)
		return printSciFloat(n, digits);
	else
		return printFloat(n, digits);
}

size_t Print::print(double n, int digits, bool Sci) {
	if (Sci == true)
		return printSciDouble(n, digits);
	else
		return printDouble(n, digits);
}

size_t Print::print(const std::string& str) {
	return print(str.c_str());
}

size_t Print::print(const happe::String& str) {
	return print(str.c_str());
}

size_t Print::print(const Printable& x) {
	return x.printTo(*this);
}

/**
 *  @name   : println
 *  @brief  : print() function add "\r\n" in tail, 加上換行符號的print
 *  @param  : integer, floating point, char string or std::string(c++ std)
 *  @param  : int base :change the positional system,
 *                      may be Hexadecimal, Decimal, Octal ,Binary or others
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::println(void) {
	return write("\r\n");
}

size_t Print::println(void * p) {
	size_t len = print(p);
	len += println();
	return len;
}

size_t Print::println(const void * p) {
	size_t len = print(p);
	len += println();
	return len;
}

size_t Print::println(bool boo) {
	size_t len = print(boo);
	len += println();
	return len;
}

size_t Print::println(char c) {
	size_t len = print(c);
	len += println();
	return len;
}

size_t Print::println(const char * str) {
	size_t len = print(str);
	len += println();
	return len;
}

size_t Print::println(short n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(unsigned short n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(int n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(unsigned int n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(long n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(unsigned long n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(long long n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(unsigned long long n, int base) {
	size_t len = print(n, base);
	len += println();
	return len;
}

size_t Print::println(float n, int digits, bool Sci) {
	size_t len = print(n, digits, Sci);
	len += println();
	return len;
}

size_t Print::println(double n, int digits, bool Sci) {
	size_t len = print(n, digits, Sci);
	len += println();
	return len;
}

size_t Print::println(const std::string& str) {
	size_t len = print(str);
	len += println();
	return len;
}

size_t Print::println(const happe::String& str){
	size_t len = print(str);
	len += println();
	return len;
}

size_t Print::println(const Printable& x) {
	size_t len = print(x);
	len += println();
	return len;
}

/*
 template<class First, class ...Args>
 size_t Print::printf(const char * format, First type, Args... arg) {
 size_t len = 0;
 len += print(type);
 len += printf(format, arg...);
 return len;
 }*/

size_t Print::printPointer(void * ptr, bool print0x) {

	size_t len = 0;
	if (print0x)
		len += print("0x");
	uintptr_t ptr_val = reinterpret_cast<uintptr_t>(ptr);

	uintptr_t conv_val = conv_val_max;
	do {
		if (ptr_val & conv_val) {
			len += print(ptr_val, HEX);
			return len;
		} else {
			len += print('0');
		}
		conv_val >>= 4;
	} while (conv_val != 0);

	return len;
}

/**
 *  @name   : printShort
 *  @brief  : convert integer is under 32bit to string, and print it out
 *  @param  : unsigned long n : integer less than 32bit
 *  @param  : uint8_t base    : change the positional system,
 *                              may be Hexadecimal, Decimal, Octal ,Binary or others
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::printShort(unsigned long n, uint8_t base) {
	char buf[35]; 							// Assumes 8-bit chars plus zero byte.
	/*char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	if (base < 2)
		base = 10;

	do {
		char c = n % base;
		n /= base;

		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while (n);

	return write(str);*/
	happe::utoa(static_cast<unsigned int>(n), buf, base);
	return write(buf);
}

/**
 *  @name   : printNumber
 *  @brief  : convert integer is over 32bit to string, and print it out
 *  @param  : unsigned long n : integer larger than 32bit
 *  @param  : uint8_t base    : change the positional system,
 *                              may be Hexadecimal, Decimal, Octal ,Binary or others
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::printNumber(unsigned long long n, uint8_t base) {
	char buf[65]; // Assumes 8-bit chars plus zero byte.
	/*char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	if (base < 2)
		base = 10;

	do {
		char c = n % base;
		n /= base;

		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while (n);

	return write(str);*/
	happe::ultoa(n, buf, base);
	return write(buf);
}

/**
 *  @name   : printFloat
 *  @brief  : convert Single-precision floating-point format to String, and print it out
 *  @param  : float number   : Single-precision floating-point
 *  @param  : uint8_t digits : precision of Decimal point
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::printFloat(float number, uint8_t digits) {
	/*size_t n = 0;

	if (isnan(number))
		return print(FloatLabel[0]);
	if (isinf(number))
		return print(FloatLabel[1]);
	if (number > 4294967040.0f) 		// If use MCU System,
		return print(FloatLabel[2]);	// float to unsigned long max =  4294967040.0
										// constant determined empirically
	if (number < -4294967040.0f)
		return print(FloatLabel[2]);	// constant determined empirically

	// Handle negative numbers
	if (number < 0.0) {
		n += write('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	float rounding = 0.5;
	for (uint8_t i = 0; i < digits; ++i)
		rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long) number;
	float remainder = number - (float) int_part;
	n += print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		n += write('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0) {
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int) (remainder);
		n += print(toPrint);
		remainder -= toPrint;
	}

	return n;*/

	char buf[32];
	happe::ftostrf(number, 4, digits, buf);
	return write(buf);
}

/**
 *  @name   : printSciFloat
 *  @brief  : convert Single-precision floating-point with Scientific notation format to String, and print it out
 *  @param  : float number   : Single-precision floating-point
 *  @param  : uint8_t digits : precision of Decimal point
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::printSciFloat(float number, uint8_t digits) {
	size_t n = 0;

	if (isnan(number))
		return print(happe::FloatLabel[0]);
	if (isinf(number))
		return print(happe::FloatLabel[1]);

	// Handle negative numbers
	if (number < 0.0) {
		n += write('-');
		number = -number;
	}

	int16_t n_digits = 0;
	if (number >= 10) {
		while (number >= 10) {
			number /= 10.0;
			n_digits++;
		}
	} else if (number <= 1.0 && number != 0) {
		while (number <= 1.0) {
			number *= 10.0;
			n_digits--;
		}
	}

	float rounding = 0.5;
	for (uint8_t i = 0; i < digits; ++i)
		rounding /= 10.0;
	number += rounding;

	uint8_t first_digits = (uint8_t) number;
	float remainder = number - (float) first_digits;
	n += print(first_digits);
	n += write('.');

	while (digits-- > 0) {
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int) (remainder);
		n += print(toPrint);
		remainder -= toPrint;
	}
	n += write('E');
	if (n_digits >= 0)
		n += write('+');
	n += print(n_digits);
	return n;
}

/**
 *  @name   : printDouble
 *  @brief  : convert Double-precision floating-point format to String, and print it out
 *  @param  : double number  : Double-precision floating-point
 *  @param  : uint8_t digits : precision of Decimal point
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::printDouble(double number, uint8_t digits) {
	/*size_t n = 0;

	if (isnan(number))
		return print(FloatLabel[0]);
	if (isinf(number))
		return print(FloatLabel[1]);
	if (number > 18446744073709551615.0) 	// If use MCU System,
		return print(FloatLabel[2]);		// double to unsigned long max =  18446744073709551615.0
											// constant determined empirically
	if (number < -18446744073709551615.0)
		return print(FloatLabel[2]);		// constant determined empirically

	// Handle negative numbers
	if (number < 0.0) {
		n += write('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i = 0; i < digits; ++i)
		rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long long int_part = (unsigned long long) number;
	double remainder = number - (double) int_part;
	n += print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		n += write('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0) {
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int) (remainder);
		n += print(toPrint);
		remainder -= toPrint;
	}

	return n;*/
	char buf[48];
	happe::dtostrf(number, 4, digits, buf);
	return write(buf);
}

/**
 *  @name   : printSciDouble
 *  @brief  : convert Double-precision floating-point with Scientific notation format to String,
 *            and print it out
 *  @param  : double number  : Double-precision floating-point
 *  @param  : uint8_t digits : precision of Decimal point
 *  @return : size_t, how many character have been Print out
 *
 */

size_t Print::printSciDouble(double number, uint8_t digits) {
	size_t n = 0;

	if (isnan(number))
		return print(happe::FloatLabel[0]);
	if (isinf(number))
		return print(happe::FloatLabel[1]);

	// Handle negative numbers
	if (number < 0.0) {
		n += write('-');
		number = -number;
	}

	int16_t n_digits = 0;
	if (number >= 10) {
		while (number >= 10) {
			number /= 10.0;
			n_digits++;
		}
	} else if (number <= 1.0 && number != 0) {
		while (number <= 1.0) {
			number *= 10.0;
			n_digits--;
		}
	}

	double rounding = 0.5;
	for (uint8_t i = 0; i < digits; ++i)
		rounding /= 10.0;
	number += rounding;

	uint8_t first_digits = (uint8_t) number;
	double remainder = number - static_cast<double>(first_digits);
	n += print(first_digits);
	n += write('.');

	while (digits-- > 0) {
		remainder *= 10.0;
		unsigned int toPrint = static_cast<unsigned int>(remainder);
		n += print(toPrint);
		remainder -= toPrint;
	}
	n += write('E');
	if (n_digits >= 0)
		n += write('+');
	n += print(n_digits);
	return n;
}


