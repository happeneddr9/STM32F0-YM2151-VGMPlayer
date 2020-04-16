/*
 * HWI2C.h
 *
 *  Created on: 2019年9月26日
 *      Author: happe
 */

#ifndef I2CMASTER_HWI2C_H_
#define I2CMASTER_HWI2C_H_
/*
#include "main.h"
#include "GPIO.h"
#include "FIFObuffer.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sBoard_I2C                       I2C1
#define I_I2C                            I2C1
#define I2C_TIMEOUT                      ((uint32_t)0x1000)
#define I2C_LONG_TIMEOUT                 ((uint32_t)(3 * I2C_TIMEOUT))

#define TX_BUFFER_LENGTH 128
#define RX_BUFFER_LENGTH 32

//extern "C" void I2C1_IRQHandler();

class HWWire {
	public:
		struct I2C_PinMap {
				Port Port_SCL;
				Port Port_SDA;

				uint8_t Pin_SCL;
				uint8_t Pin_SDA;
		};

		HWWire(I2C_TypeDef * I2Cx, IRQn vect);
		~HWWire(void);
		void begin(I2C_PinMap PinMap, uint8_t ownaddr = 0x00);

		inline void reset() const {
			I2C_SoftwareResetCmd(I2Cx);
		}

		inline I2C_TypeDef * getI2CStruct(){
			return I2Cx;
		}

		void beginTransmission(uint8_t);
		void beginTransmission(int);
		uint8_t endTransmission(void);
		uint8_t endTransmission(uint8_t);

		size_t write(uint8_t);
		size_t write(const uint8_t *, size_t);
		//size_t write(const char *);
		uint16_t available(void);
		int16_t read(void);
		int16_t peek(void);
		void flush(void);

		uint8_t requestFrom(uint8_t, uint8_t);
		uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
		uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
		uint8_t requestFrom(int16_t, int16_t);
		uint8_t requestFrom(int16_t, int16_t, int16_t);
	private:
		I2C_TypeDef * I2Cx;
		//IRQn ITvect;

		static uint8_t transmitting;

		static uint8_t txAddress;
		static uint8_t tx_buffer[];
		static uint8_t tx_index;
		static uint8_t tx_length;

		static uint8_t rx_buffer[];
		static uint8_t rx_index;
		static uint8_t rx_length;



	protected:
		uint8_t writeTo(uint8_t, uint8_t *, uint8_t, uint8_t);
		uint8_t readFrom(uint8_t, uint8_t *, uint8_t, uint8_t, uint8_t *, uint8_t);
};
*/
#endif /* I2CMASTER_HWI2C_H_ */
