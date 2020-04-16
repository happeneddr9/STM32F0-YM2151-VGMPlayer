/**
 * @Name    : Peripheral.h
 * @Date    : 2019年11月23日 下午8:08:21
 * @Author  : happe
 * @brief   : 周邊配置
 */

#ifndef PERIPHERAL_PERIPHERAL_H_
#define PERIPHERAL_PERIPHERAL_H_

#include "main.h"

namespace PeripheralMap {

#if defined(STM32F030) || defined(STM32F070)
	enum PeripheralName {
		PERIPHERAL_GPIOA,		// GPIO Ports
		PERIPHERAL_GPIOB,
		PERIPHERAL_GPIOC,
		PERIPHERAL_GPIOD,
		PERIPHERAL_GPIOE,
		PERIPHERAL_GPIOF,

		PERIPHERAL_USART1,		// USART
		PERIPHERAL_USART2,
		PERIPHERAL_USART3,
		PERIPHERAL_USART4,
		PERIPHERAL_USART5,
		PERIPHERAL_USART6,
		PERIPHERAL_USART7,

		PERIPHERAL_I2C1,		// I2C
		PERIPHERAL_I2C2,

		PERIPHERAL_SPI1,		// SPI
		PERIPHERAL_SPI2,

		PERIPHERAL_TIMER1,		// Advanced-Control-Timer

		PERIPHERAL_TIMER3,		// General-Purpose-Timer

		PERIPHERAL_TIMER6,		// Basic-Timer
		PERIPHERAL_TIMER7,

		PERIPHERAL_TIMER14,		// General-Purpose-Timer

		PERIPHERAL_TIMER15,		// General-Purpose-Timer
		PERIPHERAL_TIMER16,
		PERIPHERAL_TIMER17,

		PERIPHERAL_DMA1,		// Direct Memory Access controller

		PERIPHERAL_ADC1,		// Analog-to-Digital Converter

		PERIPHERAL_RTC,			// Real-Time-Clock

		PERIPHERAL_WWDG,		// System window watchdog
		PERIPHERAL_IWDG			// Independent watchdog
	// ...
	};

	constexpr auto DMA1_channels = 5;


#endif	// if defined(STM32F030) || defined(STM32F070)


}

#endif /* PERIPHERAL_PERIPHERAL_H_ */
