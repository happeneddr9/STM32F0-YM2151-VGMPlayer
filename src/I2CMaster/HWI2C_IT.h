/*
 * HWI2C_IT.h
 *
 *  Created on: 2019年11月17日
 *      Author: happe
 *
 *  Build Log:
 *  	2019/11/17
 *  		I2C 接收&發送 完成
 *  		使用中斷的方式
 *  	2019/11/18
 *   	    新增Gpio Port 和 Gpio Pin映射
 *
 *   	    Added Gpio Port and Pin Mapping Struct
 *
 *   	2019/12/12
 *   		Gpio映射改用GpioPin結構宣告
 *
 *   		used GpioPin Struct replace Gpio Port and Pin (Increase Readability)
 *
 *
 */

#ifndef I2CMASTER_HWI2C_IT_H_
#define I2CMASTER_HWI2C_IT_H_

#include "main.h"
#include "GPIO/GPIO.h"
#include "FIFObuffer/FIFObuffer.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TX_BUFFER_LENGTH 192
#define RX_BUFFER_LENGTH 32

#if TX_BUFFER_LENGTH > 255
typedef uint16_t TX_header_size;
#else
typedef uint8_t TX_header_size;
#endif

#if RX_BUFFER_LENGTH > 255
typedef uint16_t RX_header_size;
#else
typedef uint8_t RX_header_size;
#endif

//extern "C" void I2C1_IRQHandler();

class HWWire {
	public:
		struct I2C_PinMap {
				GpioPin Pin_SCL;
				GpioPin Pin_SDA;
		};

		enum WireClock {
			StandardSpeed = 0x2000090E,		// 100kHz
			FastSpeed = 0x0000020B,			// 400kHz
			HyperSpeed = 0x00000001			// 1000kHz
		};

		HWWire(I2C_TypeDef * I2Cx, IRQn vect);
		~HWWire(void);
		void begin(I2C_PinMap PinMap, uint8_t ownaddr = 0x00);
		void setClock(WireClock clk = StandardSpeed);

		inline void reset() const {
			I2C_SoftwareResetCmd(I2Cx);
		}

		inline I2C_TypeDef * getI2CStruct() {
			return I2Cx;
		}

		bool beginTransmission(uint8_t);
		bool beginTransmission(int);


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
		uint8_t requestFrom(int16_t, int16_t, int16_t);

		inline TX_header_size getWireTX_MAX(void){
			return TX_BUFFER_LENGTH;
		}

		inline void __IT_pushData_in(uint8_t n) {
			RX_Buff.push(n);
			TR_length--;
			if (TR_length == 0)
				send_stop = false;
		}

		inline void __IT_pushData_out(void) {
			//I2C_SendData(I2Cx, TX_Buff.get());
			if (TX_Buff.size() > 0) {
				I2C_SendData(I2Cx, TX_Buff.get());
				if (TX_Buff.size() == 0 && RX_Ready == true) {
					uint32_t to_send_stop = I2C_SoftEnd_Mode;
					if (send_stop == true)
						to_send_stop = I2C_AutoEnd_Mode;
					I2C_TransferHandling(I2Cx, txAddress, TR_length, to_send_stop, I2C_Generate_Start_Read);
				}
			} else {

			}
		}

		inline void __IT_AddressMatch(void) {

		}

		inline void __IT_getStopbit(void) {
			readyTransmit = true; //reTransmit
		}

		inline void __IT_getNACK(void) {
			readyTransmit = true;
		}

		inline void __IT_getERROR(void) {
			readyTransmit = true;
		}

	private:
		I2C_TypeDef * I2Cx;
		bool readyTransmit;
		uint8_t TR_length;
		uint8_t txAddress;
		bool send_stop;

		bool RX_Ready;

		FIFO<uint8_t, TX_BUFFER_LENGTH, TX_header_size> TX_Buff;
		FIFO<uint8_t, RX_BUFFER_LENGTH, RX_header_size> RX_Buff;

	protected:
		void writeTo(uint8_t address, uint8_t length, uint8_t sendStop);
		void readFrom(uint8_t address, uint8_t length, uint8_t sendStop);
};

#endif /* I2CMASTER_HWI2C_IT_H_ */
