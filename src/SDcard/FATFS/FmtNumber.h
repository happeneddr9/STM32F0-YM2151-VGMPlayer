/**
 * @Name    : FmtNumber.h
 * @Date    : 2020年1月1日 下午10:29:16
 * @Author  : happe
 * @brief   :
 */

/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef FmtNumber_h
#define FmtNumber_h
//  #include <ctype.h>
inline bool isDigit(char c) {
	return '0' <= c && c <= '9';
}
inline bool isSpace(char c) {
	return c == ' ' || (0X9 <= c && c <= 0XD);
}
#include <math.h>
#include <stdint.h>
char* fmtDec(uint16_t n, char* p);
char* fmtDec(uint32_t n, char* p);
char* fmtFloat(float value, char* p, uint8_t prec);
char* fmtFloat(float value, char* ptr, uint8_t prec, char expChar);
char* fmtHex(uint32_t n, char* p);
float scale10(float v, int8_t n);
float scanFloat(const char* str, char** ptr);
#endif  // FmtNumber_h

