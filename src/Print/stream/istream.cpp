/**
 * @Name    : istream.cpp
 * @Date    : 2020年1月6日 下午8:42:07
 * @Author  : happe
 * @brief   : like C++ STD istream
 */


#include "istream.h"

namespace cpp_io {

	size_t istream::readChar(char& c){
		int tmp = readSkip();
		if(tmp < 0)
			return 0;
		else
			c = tmp;
		return 1;
	}

	size_t istream::readInt(int& n){
		while (!ready2read()) {

		}

		size_t len = 0;
		bool isNegative = false;
		int32_t value = 0;
		uint32_t c = peekch();

		if (c == 255)   // not a number
			return 0;

		do {
			if (c == '-')
				isNegative = true;
			else if (c >= '0' && c <= '9')        // is c a digit?
				value = value * 10 + c - '0';
			getch();  // consume the character we got with peek
			c = peekch();
			len++;
		} while (c >= '0' && c <= '9');

		if (isNegative)
			value = -value;
		n = value;
		return len;
	}

	size_t istream::readFloat(float& n){
		return 0;
	}

	size_t istream::readDouble(double& n){
		return 0;
	}
}


