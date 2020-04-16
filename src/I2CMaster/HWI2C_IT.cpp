/*
 * HWI2C_IT.cpp
 *
 *  Created on: 2019年11月17日
 *      Author: happe
 */

#include "HWI2C_IT.h"
#include "SYSTEM/Nvic.hpp"



HWWire::HWWire(I2C_TypeDef * I2Cx, IRQn vect) {
	this->I2Cx = I2Cx;
	//ITvect = vect;

	readyTransmit = true;
	TR_length = 0;
	txAddress = 0;
	send_stop = false;

	RX_Ready = false;

	Nvic::configureIrq(vect, ENABLE);

	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);  //TODO: Function
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
}

HWWire::~HWWire(void) {
	I2C_Cmd(I2Cx, DISABLE);
}

void HWWire::begin(I2C_PinMap PinMap, uint8_t ownaddr) {
	I2C_InitTypeDef I2C_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	I2C_DeInit(I2Cx);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = ownaddr;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = 0x2000090E; //Fast Mode: 100KHz 0x2000090E

	//GPIO_PinAFConfig(Portx, SCL_pin, GPIO_AF_4);
	//GPIO_PinAFConfig(Portx, SDA_pin, GPIO_AF_4);

	Gpio SCL_pin(PinMap.Pin_SCL);
	Gpio SDA_pin(PinMap.Pin_SDA);

	SCL_pin.init(GpioMode::AF, GpioOutType::OD, GpioSpeed::s50MHz, GpioPuPd::PullUp);
	SDA_pin.init(GpioMode::AF, GpioOutType::OD, GpioSpeed::s50MHz, GpioPuPd::PullUp);

	SCL_pin.setAF(GpioAF::AF4);  //覆用
	SDA_pin.setAF(GpioAF::AF4);

	/* Enable I2C2 */
	I2C_Init(I2Cx, &I2C_InitStructure);
	I2C_ITConfig(I2Cx, I2C_IT_RXI | I2C_IT_TXI | I2C_IT_TCI |
			     I2C_IT_ERRI | I2C_IT_STOPI | I2C_IT_ADDRI, ENABLE);
	I2C_Cmd(I2Cx, ENABLE);

}

void HWWire::setClock(WireClock clk){
	I2Cx->TIMINGR = clk & ((uint32_t)0xF0FFFFFF);
}

void HWWire::writeTo(uint8_t address, uint8_t length, uint8_t sendStop) {
	uint32_t to_send_stop = I2C_SoftEnd_Mode;
	if (sendStop == true)
		to_send_stop = I2C_AutoEnd_Mode;
	I2C_TransferHandling(I2Cx, address, length, to_send_stop, I2C_Generate_Start_Write);
	readyTransmit = false;
}

void HWWire::readFrom(uint8_t address, uint8_t length, uint8_t sendStop) {
	txAddress = address << 1;
	TR_length = length;
	send_stop = sendStop;

}

bool HWWire::beginTransmission(uint8_t address) {
	// set address of targeted slave
	txAddress = address << 1;
	while(!readyTransmit){
		__asm("nop");
	}
	return true;
	// reset tx buffer iterator vars
	//TX_Buff.clear();
}

/**
 * @param  : 7bit I2C Address
 *
 */
bool HWWire::beginTransmission(int address) {
	return beginTransmission(static_cast<uint8_t>(address));
}

uint8_t HWWire::endTransmission(void) {
	return endTransmission(true);
}

uint8_t HWWire::endTransmission(uint8_t sendStop) {
	writeTo(txAddress, TX_Buff.size(), sendStop);
	return 1;
}

uint8_t HWWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize,
		uint8_t sendStop) {
	// clamp to buffer length
	if (quantity > RX_BUFFER_LENGTH) {
		quantity = RX_BUFFER_LENGTH;
	}
	RX_Ready = true;
	readFrom(address, quantity, sendStop);
	return 1;
}

uint8_t HWWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint32_t) 0, (uint8_t) 0, (uint8_t) sendStop);
}

uint8_t HWWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t HWWire::requestFrom(int16_t address, int16_t quantity, int16_t sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}

size_t HWWire::write(uint8_t data) {
	TX_Buff.push(data);
	return 1;
}

/*size_t HWWire::write(const char * str) {
 return write((uint8_t *) str, strlen(str));
 }*/

size_t HWWire::write(const uint8_t *data, size_t quantity) {
	size_t n = 0;
	for (size_t i = 0; i < quantity; ++i) {
		n += write(data[i]);
	}
	return n;
}

uint16_t HWWire::available(void) {
	return RX_Buff.size();
}

int16_t HWWire::read(void) {
	return RX_Buff.get();
}

int16_t HWWire::peek(void) {
	return RX_Buff.peek();
}

void HWWire::flush(void) {
	//TODO:to be continue.
}

