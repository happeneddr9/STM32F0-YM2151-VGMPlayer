/*
 *	Basic.h		// not for BASIC Programming Language
 *
 *  Created on: 2019年10月1日
 *      Author: happe
 *
 *  with some Arduino's Style code
 *  have been Configured for STM32
 *
 */

#ifndef BASIC_H_
#define BASIC_H_


#include "main.h"
//#include "SYSTEM/IT_vect.h"
#include "GPIO/GPIO.h"
#include "FastBits.h"
#include "function_view.h"
#include "Print/WString.h"


#ifdef Use64bitSystemTicker
typedef uint64_t SystemTicker_t;
#else
typedef uint32_t SystemTicker_t;
#endif



extern volatile SystemTicker_t System_Clock;

//for Arduino Code Support!
template<class T>
constexpr auto PI_base = T(3.1415926535897932384626433832795L);

template<class T>
constexpr auto HALF_PI_base = T(1.5707963267948966192313216916398L);

template<class T>
constexpr auto TWO_PI_base = T(6.283185307179586476925286766559L);

template<class T>
constexpr auto DEG2RAD_base = T(0.017453292519943295769236907684886L);

template<class T>
constexpr auto RAD2DEG_base = T(57.295779513082320876798154814105L);

template<class T>
constexpr auto EULER_base = T(2.718281828459045235360287471352L);

constexpr auto PI = PI_base<float>;
constexpr auto HALF_PI = HALF_PI_base<float>;
constexpr auto TWO_PI = TWO_PI_base<float>;
constexpr auto DEG2RAD = DEG2RAD_base<float>;
constexpr auto RAD2DEG = RAD2DEG_base<float>;
constexpr auto EULER = EULER_base<float>;

constexpr inline uint8_t high_byte(uint16_t n){
	return static_cast<uint8_t>((n & 0xFF00) >> 8);
}

constexpr inline uint8_t low_byte(uint16_t n){
	return static_cast<uint8_t>(n & 0x00FF);
}

void randomSeed(unsigned long seed);
int random(void);
int random(int howbig);
int random(int howsmall, int howbig);

void pinMode(GpioPin pinx, pinMode_Status mode_in);
void analogPin(GpioPin pinx);
void digitalWrite(GpioPin pinx, GPIO_Status HL);
void digitalWrite(GpioPin pinx, bool HL);

/*inline void digitalWrite(const Gpio& pinx, bool HL){
	pinx.setState(HL);
}*/
inline void digitalWrite(const GpioFast& pinx, bool HL){
	pinx.setState(HL);
}



bool digitalRead(GpioPin pinx);
uint16_t analogRead(GpioPin pinx);

template <typename T>
constexpr const T radians(T deg) {
	return static_cast<T>(deg * DEG2RAD_base<T>);
}

template <typename T>
constexpr const T degree(T rad) {
	return static_cast<T>(rad * RAD2DEG_base<T>);
}

template <typename T1, typename T2 = int>
constexpr const T1 map(T1 x, T2 in_min, T2 in_max, T2 out_min, T2 out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T>
constexpr const T max(const T& a, const T& b) {
	return ((a > b) ? a : b);
}

template <typename T>
constexpr const T min(const T& a, const T& b) {
	return ((a < b) ? a : b);
}

template <typename T>
constexpr const T max(const T& a, const T& b, const T& c) {
	return max(max(a, b), c);
}

template <typename T>
constexpr const T min(const T& a, const T& b, const T& c) {
	return min(min(a, b), c);
}

/*template <typename T>
constexpr void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}*/
using happe::swap;

constexpr void _swap_int16_t(int16_t& a, int16_t& b) {
	swap(a, b);
}

template <class T1, class T2>
bool Buffercmp(const T1* pBuffer1, const T2* pBuffer2, uint32_t BufferLength) {
	while (BufferLength--) {
		if (*pBuffer1 != *pBuffer2) {
			return false;
		}
		pBuffer1++;
		pBuffer2++;
	}

	return true;
}

// Not Tested
int FreeStack(void);

void yield(void);
void delay(uint32_t);

/**
 * @brief:	handle some function when it trapped in delay, function return true can escape delay
 * @param:	delayTime 	as its name.
 * @param:	_F 			can be any function that return boolean value. usually put a lambda
 */
template<typename Func_>
bool delay(uint32_t delayTime, Func_&& _F) {
	uint32_t delaymsNow = System_Clock;
	while (System_Clock - delaymsNow < delayTime) {
		yield();
		if(_F())
			return true;
	}
	return false;
}
void delayMicroseconds(uint32_t);

inline SystemTicker_t millis(void) {
	return System_Clock;
}

inline SystemTicker_t micros(void) {
	uint32_t tmp = SysTick->VAL;
	return ((System_Clock * 1000) + (1000 - ((tmp) / (F_CPU / 1000000))));
}

void * operator new(size_t size);
void * operator new[](size_t size);
void operator delete(void * ptr);
void operator delete[](void * ptr);

/**
 * @brief 	:	setup the System Tick Frequency
 * 				It'll affect millis() delay() micros() delayMicroseconds()
 * 				Recommended setting for 1000Hz
 *
 */
inline static void SysTickInit(uint32_t frequency = 1000) {
	SysTick->CTRL |= SysTick_CLKSource_HCLK;						// SysTick's source is F_CPU clock
	uint32_t ticks = F_CPU / frequency;
	SysTick->LOAD = (ticks & SysTick_LOAD_RELOAD_Msk) - 1; 			/* set reload register */

	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); 	/* set Priority for Cortex-M0 System Interrupts */
	SysTick->VAL = 0; 												/* Load the SysTick Counter Value */
	/* Enable SysTick IRQ and SysTick Timer */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk |
					SysTick_CTRL_ENABLE_Msk;
	RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
}

extern "C" {
	void SysTick_Handler(void);
}

#endif /* BASIC_H_ */
