/**
 * @Name    : ArduinoLib.h
 * @Date    : 2019年11月29日 下午7:36:28
 * @Author  : happe
 * @brief   : Arduino style supports library
 */

#ifndef ARDUINO_ARDUINOLIB_H_
#define ARDUINO_ARDUINOLIB_H_

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

//for Arduino Binary Code Support
#include "binary.h"

#define finline __attribute__((always_inline))
#define ARDUINO 189
#define PROGMEM // didn't do anything on ARM series

#define F(x) x  // didn't do anything on ARM series

typedef unsigned char byte;  	// Arduino's byte type
typedef bool boolean;  			// Arduino's boolean type

// Many (but maybe not all) non-AVR board installs define macros
// for compatibility with existing PROGMEM-reading AVR code.
// Do our own checks and defines here for good measure...
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(reinterpret_cast<const unsigned char *>(addr)))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(reinterpret_cast<const unsigned short *>(addr)))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(reinterpret_cast<const unsigned long *>(addr)))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.  Try to accommodate both...

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) (reinterpret_cast<void *>(addr))
#else
#define pgm_read_pointer(addr) (reinterpret_cast<void *>(pgm_read_word(addr)))
#endif





#endif /* ARDUINO_ARDUINOLIB_H_ */
