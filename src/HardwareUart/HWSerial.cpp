/*
 * HWSerial.cpp
 *
 *  Created on: 2019年8月31日
 *      Author: happe
 */

#include "HWSerial.h"
#include "SYSTEM/Nvic.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//#define BV(b) (1 << b)

//extern uint8_t Serial_setup(HWSerial * Seri);

HWSerial::HWSerial(USART_TypeDef * UARTx, IRQn vect, uint8_t IRQ_Priority) {

	//UART01 = UARTx;
	this->UARTx = UARTx;
	RX_Done = false;

	Nvic::configureIrq(vect, ENABLE, IRQ_Priority);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // TODO: conv to function
	RCC->APB2ENR |= RCC_APB2Periph_USART1;
	/*******************************************************/
	//BaudRate = 0;
}

HWSerial::~HWSerial(void) {
	end();
}

uint8_t HWSerial::begin(uint32_t baud, const HWSerial_PinMap::USART_PinMap_TXRX& PinMap) {
	USART_InitTypeDef USART_InitStructure;

	//BaudRate = baud;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	Gpio Rx_Pinx(PinMap.Pin_RX);
	Gpio Tx_Pinx(PinMap.Pin_TX);

	Rx_Pinx.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);
	Tx_Pinx.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);

	Rx_Pinx.setAF(PinMap.Pin_AF); 	// will add a AF Map for this # Done
	Tx_Pinx.setAF(PinMap.Pin_AF);

	USART_Init(UARTx, &USART_InitStructure);
	cmd(ENABLE);
	return 1;
}

uint8_t HWSerial::begin(uint32_t baud, const HWSerial_PinMap::USART_PinMap_ALL& PinMap) {   // TODO: ALL pin Mode
	USART_InitTypeDef USART_InitStructure;

	//BaudRate = baud;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	Gpio Rx_Pinx(PinMap.Pin_RX);
	Gpio Tx_Pinx(PinMap.Pin_TX);

	Rx_Pinx.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);
	Tx_Pinx.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);

	Rx_Pinx.setAF(PinMap.Pin_AF);
	Tx_Pinx.setAF(PinMap.Pin_AF);


	USART_Init(UARTx, &USART_InitStructure);
	cmd(ENABLE);
	return 1;
}

inline void HWSerial::end(void){
	cmd(DISABLE);
}

size_t HWSerial::available(void) {
	return buff_rx.available();
}

uint8_t HWSerial::peek(void) {
	return buff_rx.peek();
}

uint8_t HWSerial::read(void) {
	return buff_rx.pop_front();
}

void HWSerial::clearBuffer(void) {
	buff_rx.clear();
}

bool HWSerial::find(const char * str) {  //建議收完完整字串後再使用
	return false;
}

int32_t HWSerial::parseInt(void) {

	SystemTicker_t _startMillis = millis();

	do {
		if (RX_Done) {
			RX_Done = false;
			bool isNegative = false;
			int32_t value = 0;
			uint32_t c = peek();

			if (c == 255)   // not a number
				return 0;

			do {
				if (c == '-')
					isNegative = true;
				else if (c >= '0' && c <= '9')        // is c a digit?
					value = value * 10 + c - '0';
				read();  // consume the character we got with peek
				c = peek();
			} while (c >= '0' && c <= '9');

			if (isNegative)
				value = -value;
			return value;
		}
	} while (millis() - _startMillis < 100);
	return 0;
}

happe::String HWSerial::readString(void){
	happe::String ret;
	SystemTicker_t _startMillis = millis();
	do {
		if (RX_Done) {
			RX_Done = false;
			uint8_t c = read();
			while(c > 0 && available()){
				ret += static_cast<char>(c);
				c = read();
			}

		}
	} while (millis() - _startMillis < 100);
	return ret;
}

size_t HWSerial::write(uint8_t c) {
	//USART_SendData(this->UARTx, (uint8_t) c);
	UARTx->TDR = (c & (uint16_t) 0x01FF);
	while (USART_GetFlagStatus(this->UARTx, USART_FLAG_TC) == RESET)
		;
	return 1;
}

bool HWSerial::ready2read(void) {
	bool tmp = RX_Done;
	if(tmp)
		RX_Done = false;
	return tmp;
}

int HWSerial::getch(void) {
	return buff_rx.pop_front();
}

int HWSerial::peekch(void) {
	return buff_rx.peek();
}


/*
 size_t len = 0;
 while (*str != '\0') {
 USART_SendData(this->UARTx, (uint8_t) (*str));
 while (USART_GetFlagStatus(this->UARTx, USART_FLAG_TC) == RESET)
 ;
 str++;
 len++;
 }
 return len;
 */
