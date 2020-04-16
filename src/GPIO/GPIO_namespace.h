/*
 *  GPIO_namespace.h
 *
 *  Created on: 2019年11月11日
 *      Author: happe
 *
 *  Build Log:
 *  	2019/11/11
 *  		定義各GPIO的屬性
 *  		added GpioPin Structure
 *
 *  		ex: PA0  = { GPIOA, GPIO_Pin_0 }
 *  		ex: PB15 = { GPIOB, GPIO_Pin_15 }
 *
 *  	2019/12/14
 *  		根據型號調整ADC腳位
 *
 *  		added Another name for Analog Pin, ex: A0 = PA6, like Arduino's style
 *  		# It will adjust according to MCU belong series
 *  		# Currently only supported STM32F030F4xx series
 *
 *  	2019/12/20
 *  		Gpio Pin and Analog Pin Defines have been moved to external header file
 */

#ifndef GPIO_GPIO_NAMESPACE_H_
#define GPIO_GPIO_NAMESPACE_H_

#include "GpioPin.h"
#include "AnalogPin.h"

enum class GpioMode
	: uint8_t {
		In = 0x00,
	Out = 0x01,
	AF = 0x02,
	Analog = 0x03,
	Exti = 0xFF,

	in = In,
	out = Out
};

enum class GpioOutType
	: uint8_t {
		PP = 0x00,
	OD = 0x01
};

enum class GpioSpeed
	: uint8_t {
		s2MHz = 0x00,
	s25MHz = 0x01,
	s50MHz = 0x02,

	level_1 = s2MHz,
	level_2 = s25MHz,
	level_3 = s50MHz,

	Speed_low = s2MHz,
	Speed_medium = s25MHz,
	Speed_high = s50MHz,
};

enum class GpioPuPd
	: uint8_t {
		NoPull = 0x00,
	PullUp = 0x01,
	PullDown = 0x02
};

enum class GpioAF
	: uint8_t {
		AF0 = 0x00,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7
};

#endif /* GPIO_GPIO_NAMESPACE_H_ */
