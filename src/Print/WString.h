/*
 *  WString.h
 *  字串排版器
 *  Created on: 2019年9月10日
 *  Author: happe
 *
 *  @brief:	it's a Arduino style String type. compared to std::string,
 *  		it's easily to use, and more efficiency.
 *
 *  		here defined some variable to strings function,
 *  		ex: itoa, utoa, dtostrf etc.
 *  		there have some optimized for 32-bit system, in here,
 *  		"int" is a 32-bit integer, "unsigned long long" is a 64-bit,
 *  		"float" is 32-bit floating point, "double" is a 64-bit.
 *
 *      Build Log:
 *
 *      2019/09/10
 *      	可用 += 延長字串，可加上整數、浮點等格式
 *      	you can use += operator to concat string, char, integer etc, even floating point.
 *
 *      2019/09/12
 *      	新增 String 型態
 *      	與 Arduino 的 String 功能接近
 *      	WString 接近完工
 *
 *      	new String types
 *      	it's a efficiency Arduino style char String Compared to std::string
 *
 *      2019/10/12
 *      	WString 暫時停用
 *      	WString is under construction
 *      2020/01/28
 *      	將 WString 搬離 Print.h
 *      	moved out WString from Print.h
 *      2020/03/02
 *      	itoa, dtostrf 等變數轉字串的程式完工
 *      	itoa, dtostrf etc that can convert variable to strings function Completed,
 */

#ifndef WSTRING_H_
#define WSTRING_H_

#ifdef __cplusplus

#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
#include "strconv.h"

namespace happe {

	/*void reverse(char str[], int length);
	char* itoa(int num, char* str, uint8_t base);
	char* utoa(unsigned int num, char* str, uint8_t base);
	char* ltoa(long long num, char* str, uint8_t base);
	char* ultoa(unsigned long long num, char* str, uint8_t base);
	char* ftostrf(float val, signed char width, unsigned char prec, char *sout);
	char* dtostrf(double val, signed char width, unsigned char prec, char *sout);

	uint atou(const char * str);*/

	class StringSumHelper;

	class String{
		private:
			typedef void (String::*StringIfHelperType)() const;
			void StringIfHelper() const {
			}
		public:
			String(const char *cstr = "");
			String(const String &str);

			// C++11 or later
			String(String &&rval);
			String(StringSumHelper &&rval);

			explicit String(char c);
			explicit String(unsigned char, unsigned char base = 10);
			explicit String(short, unsigned char base = 10);
			explicit String(unsigned short, unsigned char base = 10);
			explicit String(int, unsigned char base = 10);
			explicit String(unsigned int, unsigned char base = 10);
			explicit String(long, unsigned char base = 10);
			explicit String(unsigned long, unsigned char base = 10);
			explicit String(long long, unsigned char base = 10);
			explicit String(unsigned long long, unsigned char base = 10);
			explicit String(float, unsigned char decimalPlaces = 2);
			explicit String(double, unsigned char decimalPlaces = 2);
			~String(void);

			// memory management
			// return true on success, false on failure (in which case, the string
			// is left unchanged).  reserve(0), if successful, will validate an
			// invalid string (i.e., "if (s)" will be true afterwards)
			unsigned char reserve(unsigned int size);
			inline unsigned int length(void) const {
				return len;
			}

			// creates a copy of the assigned value.  if the value is null or
			// invalid, or if the memory allocation fails, the string will be
			// marked as invalid ("if (s)" will be false).
			String & operator =(const String &rhs);
			String & operator =(const char *cstr);

			// C++11 or later
			String & operator = (String &&rval);
			String & operator = (StringSumHelper &&rval);

			// concatenate (works w/ built-in types)

			// returns true on success, false on failure (in which case, the string
			// is left unchanged).  if the argument is null or invalid, the
			// concatenation is considered unsucessful.
			size_t concat(const String &str);
			size_t concat(const char *cstr);
			size_t concat(char c);
			size_t concat(unsigned char c);
			size_t concat(short num);
			size_t concat(unsigned short num);
			size_t concat(int num);
			size_t concat(unsigned int num);
			size_t concat(long num);
			size_t concat(unsigned long num);
			size_t concat(long long num);
			size_t concat(unsigned long long num);
			size_t concat(float num);
			size_t concat(double num);

			// if there's not enough memory for the concatenated value, the string
			// will be left unchanged (but this isn't signalled in any way)
			String & operator +=(const String &rhs) {
				concat(rhs);
				return (*this);
			}
			String & operator +=(const char *cstr) {
				concat(cstr);
				return (*this);
			}
			String & operator +=(char c) {
				concat(c);
				return (*this);
			}
			String & operator +=(unsigned char num) {
				concat(num);
				return (*this);
			}
			String & operator +=(short num) {
				concat(num);
				return (*this);
			}
			String & operator +=(unsigned short num) {
				concat(num);
				return (*this);
			}
			String & operator +=(int num) {
				concat(num);
				return (*this);
			}
			String & operator +=(unsigned int num) {
				concat(num);
				return (*this);
			}
			String & operator +=(long num) {
				concat(num);
				return (*this);
			}
			String & operator +=(unsigned long num) {
				concat(num);
				return (*this);
			}
			String & operator +=(long long num) {
				concat(num);
				return (*this);
			}
			String & operator +=(unsigned long long num) {
				concat(num);
				return (*this);
			}
			String & operator +=(float num) {
				concat(num);
				return (*this);
			}
			String & operator +=(double num) {
				concat(num);
				return (*this);
			}

			friend StringSumHelper & operator +(const StringSumHelper &lhs, const String &rhs);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, const char *cstr);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, char c);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned char num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, short num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned short num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, int num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned int num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, long num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned long num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, long long num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned long long num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, float num);
			friend StringSumHelper & operator +(const StringSumHelper &lhs, double num);

			operator StringIfHelperType() const {
				return buffer ? &String::StringIfHelper : 0;
			}

			operator char*(){
				return buffer;
			}

			int compareTo(const String &s) const;
			unsigned char equals(const String &s) const;
			unsigned char equals(const char *cstr) const;
			unsigned char operator ==(const String &rhs) const {
				return equals(rhs);
			}
			unsigned char operator ==(const char *cstr) const {
				return equals(cstr);
			}
			unsigned char operator !=(const String &rhs) const {
				return !equals(rhs);
			}
			unsigned char operator !=(const char *cstr) const {
				return !equals(cstr);
			}
			unsigned char operator <(const String &rhs) const;
			unsigned char operator >(const String &rhs) const;
			unsigned char operator <=(const String &rhs) const;
			unsigned char operator >=(const String &rhs) const;
			unsigned char equalsIgnoreCase(const String &s) const;
			unsigned char startsWith(const String &prefix) const;
			unsigned char startsWith(const String &prefix, unsigned int offset) const;
			unsigned char endsWith(const String &suffix) const;

			// character acccess
			char charAt(unsigned int index) const;
			void setCharAt(unsigned int index, char c);
			char operator [](unsigned int index) const;
			char& operator [](unsigned int index);
			void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const;
			void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const {
				getBytes((unsigned char *) buf, bufsize, index);
			}
			const char* c_str() const {
				return buffer;
			}
			char* begin() {
				return buffer;
			}
			char* end() {
				return buffer + length();
			}
			const char* begin() const {
				return c_str();
			}
			const char* end() const {
				return c_str() + length();
			}

			// search
			int indexOf(char ch) const;
			int indexOf(char ch, unsigned int fromIndex) const;
			int indexOf(const String &str) const;
			int indexOf(const String &str, unsigned int fromIndex) const;
			int lastIndexOf(char ch) const;
			int lastIndexOf(char ch, unsigned int fromIndex) const;
			int lastIndexOf(const String &str) const;
			int lastIndexOf(const String &str, unsigned int fromIndex) const;
			String substring(unsigned int beginIndex) const {
				return substring(beginIndex, len);
			}
			;
			String substring(unsigned int beginIndex, unsigned int endIndex) const;

			// modification
			void replace(char find, char replace);
			void replace(const String& find, const String& replace);
			void remove(unsigned int index);
			void remove(unsigned int index, unsigned int count);
			inline void erase(unsigned int index){
				remove(index);
			}

			inline void erase(unsigned int index, unsigned int count){
				remove(index, count);
			}

			void toLowerCase(void);
			void toUpperCase(void);
			void trim(void);

			// parsing/conversion
			long toInt(void) const;
			float toFloat(void) const;
			double toDouble(void) const;



			inline const char * c_str(void) {
				return buffer;
			}
			inline size_t String_Capacity(void) const {
				return capacity;
			}

		protected:
			char *buffer;	  // the actual char array
			size_t capacity;  // the array length minus one (for the '\0')
			size_t len;       // the String length (not counting the '\0')
		protected:
			void init(void);
			void invalidate(void);
			unsigned char changeBuffer(unsigned int maxStrLen);
			size_t concat(const char *cstr, unsigned int length);

			// copy and move
			String & copy(const char *cstr, unsigned int length);

			// C++11 or later
			void move(String &rhs);

	};

	class StringSumHelper : public String {
		public:
			StringSumHelper(const String &s)
					: String(s) {
			}
			StringSumHelper(const char *p)
					: String(p) {
			}
			StringSumHelper(char c)
					: String(c) {
			}
			StringSumHelper(unsigned char num)
					: String(num) {
			}
			StringSumHelper(short num)
					: String(num) {
			}
			StringSumHelper(unsigned short num)
					: String(num) {
			}
			StringSumHelper(int num)
					: String(num) {
			}
			StringSumHelper(unsigned int num)
					: String(num) {
			}
			StringSumHelper(long num)
					: String(num) {
			}
			StringSumHelper(unsigned long num)
					: String(num) {
			}
			StringSumHelper(long long num)
					: String(num) {
			}
			StringSumHelper(unsigned long long num)
					: String(num) {
			}
			StringSumHelper(float num)
					: String(num) {
			}
			StringSumHelper(double num)
					: String(num) {
			}

	};

}

using String = happe::String;

/*String operator+(const char * lhs, const String& rhs) {
	String tmp = lhs;
	return (lhs + rhs);
}*/

#endif // __cplusplus

#endif /* WSTRING_H_ */
