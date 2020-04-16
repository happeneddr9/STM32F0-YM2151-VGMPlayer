/*
 * USART1PinMap.h
 *
 *  Created on: 2019年11月16日
 *      Author: happe
 */

#ifndef HARDWAREUART_USART1PINMAP_H_
#define HARDWAREUART_USART1PINMAP_H_

#include "main.h"
#include "HWSerial_Config.h"
#include "GPIO/GPIO.h"

namespace Usart1 {

	using HWSerial_PinMap::USART_PinMap_TXRX;
	using HWSerial_PinMap::USART_PinMap_ALL;

	/**
	 * Pin Map for (TX,RX,RTS,CTS,CK) Full USART
	 */
	namespace PinMap_ALL {
		/**
		 * Default pin package:
		 * (TX,RX,RTS,CTS,CK) = (PA9,PA10,PA12,PA11,PA8)
		 */
		constexpr const USART_PinMap_ALL DefaultMap = { PA9, PA10, PA12, PA11, PA8 , GpioAF::AF1};
		/**
		 * Remap #1:
		 * (TX,RX,RTS,CTS,CK) = (PB6,PB7,PA12,PA11,PA8)
		 */
		constexpr const USART_PinMap_ALL Remap1 = { PB6, PB7, PA12, PA11, PA8 , GpioAF::AF1};
		/**
		 * Remap #2:
		 * (TX,RX,RTS,CTS,CK) = (PA2,PA3,PA1,PA0,PA4)
		 */
		constexpr const USART_PinMap_ALL Remap2 = { PA2, PA3, PA1, PA0, PA4 , GpioAF::AF1};
		/**
		 * Remap #3:
		 * (TX,RX,RTS,CTS,CK) = (PA14,PA15,PA1,PA0,PA4)
		 */
		constexpr const USART_PinMap_ALL Remap3 = { PA14, PA15, PA1, PA0, PA4 , GpioAF::AF1};
	}

	/**
	 * Pin Map for (TX,RX) Simple UART
	 */
	namespace PinMap_TXRX {
		/**
		 * Default pin package:
		 * (TX,RX) = (PA9,PA10)
		 */
		constexpr const USART_PinMap_TXRX DefaultMap = { PA9, PA10 , GpioAF::AF1 };
		/**
		 * Remap #1:
		 * (TX,RX) = (PB6,PB7)
		 */
		constexpr const USART_PinMap_TXRX Remap1 = { PB6, PB7 , GpioAF::AF0 };
		/**
		 * Remap #2:
		 * (TX,RX) = (PA2,PA3)
		 */
		constexpr const USART_PinMap_TXRX Remap2 = { PA2, PA3 , GpioAF::AF1 };
		/**
		 * Remap #3:
		 * (TX,RX) = (PA14,PA15)
		 */
		constexpr const USART_PinMap_TXRX Remap3 = { PA14, PA15 , GpioAF::AF1 };
	}
}

#endif /* HARDWAREUART_USART1PINMAP_H_ */
