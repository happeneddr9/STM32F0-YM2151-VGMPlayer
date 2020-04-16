/*
 * Print.h
 *
 *  Created on: 2019年09月07日
 *  Author: happe
 *  Build Log:
 *      2019/11/10
 *   		修復了Print函式列印std::string時的小bug
 *
 *   		Fixed bug of Print function in printing std::string
 *
 *   	2019/11/25
 *   		Added print pointer
 *
 *   	2019/11/26
 *   		Added print Array package
 *   		Added print std::array package
 *
 *   	2020/04/15
 *   		Added Printable Class support, now you can use print() and println() for
 *   		your classes.
 *
 */

#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <type_traits>
#include <limits>
#include <cstddef>
#include <cstring>
#include <iterator>

#include <string>
#include <array>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <stdexcept>

#include "main.h"
#include "WString.h"
#include "strconv.h"
#include "Printable.h"


/*
replaced by constexpr constant, in strconv.h.
#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN
#undef BIN
#endif
#define BIN 2
*/



namespace cpp_io {



	struct _setprecision {
			uint8_t _digits;
			bool _sci;
	};

	struct _setBass {
			uint8_t _base;
	};

	/**
	 *  @brief  : 設定輸出流輸出小數點時的精度和格式
	 *  @param  : uint8_t __digits : precision
	 *  		  base = 2
	 *  @param  : bool __sci : format
	 *            false: Disable Scientific notation format
	 *            true:  Enable Scientific notation format
	 */
	inline _setprecision setprecision(uint8_t __digits = 2, bool __sci = false) {
		return {__digits, __sci};
	}

	/**
	 *  @brief  : 設定輸出流輸出整數時的進位格式
	 *  @param  : uint8_t base
	 *            can be BIN, OCT, DEC, HEX or Others Number
	 */
	inline _setBass setBase(uint8_t base) {
		return {base};
	}

}

class Print {

	public: /* printf */

		enum DS_status
			: uint8_t {
				DP_S_DEFAULT = 0,
			DP_S_FLAGS = 1,
			DP_S_MIN = 2,
			DP_S_DOT = 3,
			DP_S_MAX = 4,
			DP_S_MOD = 5,
			DP_S_CONV = 6,
			DP_S_DONE = 7
		};

		enum class Modifiers {
			MOD_NONE,
			MOD_CHAR,
			MOD_SHORT,
			MOD_LONG,
			MOD_LONG_LONG,
			MOD_LONG_DOUBLE,
			MOD_INTMAX_T,
			MOD_SIZE_T,
			MOD_PTRDIFF_T
		};

		struct Flags {
				uint8_t justify :1;
				uint8_t fsign :1;
				uint8_t space :1;
				uint8_t prefix :1;
				uint8_t padding :1;
				uint8_t issign :1;
				uint8_t reserved :2;
		};

		struct Arch {
				uint8_t base_num;
				uint8_t width;
				uint8_t Precision;
				const char * alphabet;
		};

		static_assert(sizeof(Flags) == sizeof(uint8_t), "");

	public:
		Print(void) {
		}
		~Print(void) {
		}
		virtual size_t write(uint8_t) = 0;
		size_t write(const char *str);
		virtual size_t write(const uint8_t *cstr, size_t size);
		//virtual int availableForWrite() { return 0; }

		size_t print(void *);
		size_t print(const void *);
		size_t print(bool);
		size_t print(char);
		size_t print(const char *);
		size_t print(short, int = DEC);
		size_t print(unsigned short, int = DEC);
		size_t print(int, int = DEC);
		size_t print(unsigned int, int = DEC);
		size_t print(long, int = DEC);
		size_t print(unsigned long, int = DEC);

		size_t print(long long, int = DEC);
		size_t print(unsigned long long, int = DEC);
		size_t print(float, int = 2, bool = false);
		size_t print(double, int = 2, bool = false);
		size_t print(const std::string& str);
		size_t print(const happe::String& str);

		size_t print(const Printable& x);

		/**
		 * @brief : print Any Array except char Array
		 * @param : Any Type Array
		 *
		 */
		template <typename T, size_t N>
		size_t print(const T (&Arr)[N]) {
			size_t counter = 0;
			size_t len = 0;
			len += write('{');
			for (; counter < N; counter++) {
				len += print(Arr[counter]);
				len += write((counter == (N - 1)) ? "}" : ", ");
			}
			return len;
		}

		template <size_t N>
		size_t print(const char (&Arr)[N]) {
			return print(reinterpret_cast<const char *>(&Arr[0]));
		}

		template <typename T, size_t N>
		size_t print(const std::array<T, N>& arr) {
			size_t counter = 0;
			size_t len = 0;
			len += write('{');
			for (const auto& tmp : arr) {
				len += print(tmp);
				counter++;
				len += write(counter == N ? "}" : ", ");
			}
			return len;
		}

		size_t println(void);
		size_t println(void *);
		size_t println(const void *);
		size_t println(bool);
		size_t println(char);
		size_t println(const char *);
		size_t println(short, int = DEC);
		size_t println(unsigned short, int = DEC);
		size_t println(int, int = DEC);
		size_t println(unsigned int, int = DEC);
		size_t println(long, int = DEC);
		size_t println(unsigned long, int = DEC);
		size_t println(long long, int = DEC);
		size_t println(unsigned long long, int = DEC);
		size_t println(float, int = 2, bool = false);
		size_t println(double, int = 2, bool = false);
		size_t println(const std::string& str);
		size_t println(const happe::String& str);

		size_t println(const Printable& x);

		/**
		 * @brief : print Any Array except char Array
		 * @param : Any Type Array
		 *
		 */
		template <typename T, size_t N>
		size_t println(const T (&Arr)[N]) {
			size_t len = print(Arr);
			len += println();
			return len;
		}

		template <size_t N>
		size_t println(const char (&Arr)[N]) {
			return println(reinterpret_cast<const char *>(&Arr[0]));
		}

		template <typename T, size_t N>
		size_t println(const std::array<T, N>& arr) {
			size_t len = print(arr);
			len += println();
			return len;
		}

		template <typename T>   // 輸出流
		Print& operator <<(const T& stream_data) {
			print(stream_data);
			return *this;
		}

		Print& operator <<(const Print& os) {
			return *this;
		}

		//例外情況 數字轉字串
		Print& operator <<(const uint8_t& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const short& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const unsigned short& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const int& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const unsigned int& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const long& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const unsigned long& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const long long& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}
		Print& operator <<(const unsigned long long& stream_data) {
			print(stream_data, _M_Base._base);
			return *this;
		}

		//例外情況 浮點數轉字串
		Print& operator <<(const float& stream_data) {
			print(stream_data, _M_precision._digits, _M_precision._sci);
			return *this;
		}

		Print& operator <<(const double& stream_data) {
			print(stream_data, _M_precision._digits, _M_precision._sci);
			return *this;
		}

		/**
		 *  @brief  : set the print precision and format for floating point
		 *  @param  : happe::_setprecision : a struct of floating point precision and
		 *                                   Enable or Disable Scientific notation format print
		 *  @return : It self
		 */

		//例外情況 格式設定
		Print& operator <<(cpp_io::_setprecision _f) {
			_M_precision = _f;
			return *this;
		}

		Print& operator <<(cpp_io::_setBass _b) {
			_M_Base = _b;
			return *this;
		}

		/**
		 * @brief : 可變參數式格式化輸出
		 *
		 *
		 */

		size_t printf(const char *s) {
			size_t len = 0;
			while (*s) {
				if (*s == '%') {
					if (*(s + 1) == '%') {
						++s;
					} else {
						/* No Parameter?, ignore */
					}
				}
				len += write(*s++);
			}
			return len;
		}

		/* Under construction */
		template <typename T, typename ... Args>
		size_t printf(const char *s, T value, Args ... args) {
			size_t len = 0;
			while (*s) {
				if (*s == '%') {
					if (*(s + 1) == '%') {
						++s;
					} else {
						len += print(value);
						len += this->printf(s + 1, args...); // call even when *s == 0 to detect extra arguments
						return len;
					}
				}
				len += write(*s++);
			}
			return len;
		}

		size_t printDelimiter(char symbol = '-', size_t len = 20){
			size_t n = len;
			while (len--) {
				write(static_cast<uint8_t>(symbol));
			}
			return n;
		}

		size_t printlnDelimiter(char symbol = '-', size_t len = 20) {
			size_t n = printDelimiter(symbol, len);
			n += println();
			return n;
		}


	protected:
		//void setWriteError(int err = 1) { write_error = err; }
		static cpp_io::_setprecision _M_precision;
		static cpp_io::_setBass _M_Base;

		//int16_t write_error;

		static constexpr uintptr_t conv_val_max = (static_cast<uintptr_t>(
				sizeof(uintptr_t) > 4 ? 0xF000000000000000ULL : 0xF0000000UL));


		size_t printPointer(void *, bool = true);
		size_t printShort(unsigned long, uint8_t);
		size_t printNumber(unsigned long long, uint8_t);
		size_t printFloat(float, uint8_t);
		size_t printSciFloat(float, uint8_t);
		size_t printDouble(double, uint8_t);
		size_t printSciDouble(double, uint8_t);
	private:
};
