/*
 * I2C1PinMap.h
 *
 *  Created on: 2019年11月17日
 *      Author: happe
 */

#ifndef I2CMASTER_I2C1PINMAP_H_
#define I2CMASTER_I2C1PINMAP_H_

#include "main.h"
#include "HWI2C_IT.h"

namespace IIC1 {
	namespace PinMap {
		/*
		 * Default pin package:
		 * (SCL,SDA) = (PA9, PA10)
		 */
		constexpr auto Default = HWWire::I2C_PinMap { PA9, PA10 };

		/*
		 * Remap1 pin package:
		 * (SCL,SDA) = (PB6, PB7)
		 */
		constexpr auto Remap1 = HWWire::I2C_PinMap { PB6, PB7 };

		/*
		 * Remap2 pin package:
		 * (SCL,SDA) = (PB8, PB9)
		 */
		constexpr auto Remap2 = HWWire::I2C_PinMap { PB8, PB9 };

	}
}

#endif /* I2CMASTER_I2C1PINMAP_H_ */
