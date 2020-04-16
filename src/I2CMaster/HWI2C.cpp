/*
 * HWI2C.cpp
 *
 *  Created on: 2019年9月26日
 *      Author: happe
 */

#include "HWI2C.h"
/*
 uint32_t I2C_Timeout;

 uint8_t HWWire::transmitting = 0;

 uint8_t HWWire::rx_buffer[RX_BUFFER_LENGTH];
 uint8_t HWWire::rx_index = 0;
 uint8_t HWWire::rx_length = 0;

 uint8_t HWWire::txAddress = 0;
 uint8_t HWWire::tx_buffer[TX_BUFFER_LENGTH];
 uint8_t HWWire::tx_index = 0;
 uint8_t HWWire::tx_length = 0;



 HWWire::HWWire(I2C_TypeDef * I2Cx, IRQn vect) {
 this->I2Cx = I2Cx;
 this->txAddress = 0x00;
 //ITvect = vect;

 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_InitStructure.NVIC_IRQChannel = vect;
 NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
 NVIC_EnableIRQ (vect);

 RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
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
 I2C_InitStructure.I2C_Timing = 0x2000090E; //Fast Mode: 100KHz

 //GPIO_PinAFConfig(Portx, SCL_pin, GPIO_AF_4);
 //GPIO_PinAFConfig(Portx, SDA_pin, GPIO_AF_4);

 Gpio SCL_pin(PinMap.Port_SCL, PinMap.Pin_SCL);
 Gpio SDA_pin(PinMap.Port_SDA, PinMap.Pin_SDA);

 SCL_pin.init(GpioMode::AlternateFunction, GpioOutType::OD, GpioSpeed::s50MHz, GpioPuPd::PullUp);
 SDA_pin.init(GpioMode::AlternateFunction, GpioOutType::OD, GpioSpeed::s50MHz, GpioPuPd::PullUp);

 SCL_pin.setAlternateFunction(GpioAlternateFunction::AF4);  //覆用
 SDA_pin.setAlternateFunction(GpioAlternateFunction::AF4);
 */
/*
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = (1 << SCL_pin) | (1 << SDA_pin);
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(Portx, &GPIO_InitStructure);
 */
/*NVIC_InitStructure.NVIC_IRQChannel = ITvect;
 NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
 NVIC_EnableIRQ (ITvect);*/

/* Enable I2C2 */
/*I2C_Init(I2Cx, &I2C_InitStructure);
 I2C_ITConfig(I2Cx, I2C_IT_RXI | I2C_IT_TXI | I2C_IT_TCI, ENABLE);
 I2C_Cmd(I2Cx, ENABLE);*/

/*
 }
 */
 /*
 uint8_t HWWire::writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop) {
 uint8_t been_write = 0;
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET) {
 if ((I2C_Timeout--) == 0) {
 return 255;
 }
 }
 if (length > 0) {
 I2C_TransferHandling(I2Cx, address, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 254;
 }
 }

 I2C_SendData(I2C1, data[0]);

 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 253;
 }
 }
 been_write++;

 uint32_t send_stop = I2C_SoftEnd_Mode;
 if (sendStop == true)
 send_stop = I2C_AutoEnd_Mode;

 I2C_TransferHandling(I2Cx, address, length - 1, send_stop, I2C_No_StartStop);

 for (uint8_t write_Num = 1; write_Num < length; write_Num++) {
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 252;
 }
 }

 I2C_SendData(I2Cx, data[write_Num]);
 been_write++;
 }

 } else {
 I2C_TransferHandling(I2Cx, address, 0, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 254;
 }
 }
 }
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 251;
 }
 }

 return been_write;
 }*/
/*
 uint8_t HWWire::readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop, uint8_t* txdata,
 uint8_t txlength) {
 uint8_t been_read = 0;
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET) {
 //while (I2C_Timeout > 50) {
 if ((I2C_Timeout--) == 0) {
 return 0;
 }
 }
 //I2C_TransferHandling(I2Cx, address, txlength, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
 //I2C_TransferHandling(I2Cx, address, txlength, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);
*//*
 I2C_Timeout = I2C_TIMEOUT;

 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 254;
 }
 }*/
/*if (txlength > 0) {
 //uint8_t first_data = txlength - 1;
 for (uint8_t write_Num = 0; write_Num < txlength; write_Num++) {
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 0;
 }
 }
 I2C_SendData(I2Cx, txdata[write_Num]);
 }

 //I2C_SendData(I2Cx, txdata[first_data]);
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TC) == RESET) {
 if ((I2C_Timeout--) == 0) {
 return 0;
 }
 }
 }*/
/*
 uint32_t send_stop = I2C_SoftEnd_Mode;
 if (sendStop == true)
 send_stop = I2C_AutoEnd_Mode;

 I2C_TransferHandling(I2Cx, address, length, send_stop, I2C_Generate_Start_Read);

 for (uint8_t read_Num = 0; read_Num < length; read_Num++) {
 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET) {
 //while (I2C_Timeout > 50) {
 if ((I2C_Timeout--) == 0) {
 return 0;
 }
 }
 data[read_Num] = I2C_ReceiveData(I2Cx);
 been_read++;
 }

 I2C_Timeout = I2C_TIMEOUT;
 while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET) {
 //while (I2C_Timeout > 50) {
 if ((I2C_Timeout--) == 0) {
 return 0;
 }
 }

 return been_read;
 }

 void HWWire::beginTransmission(uint8_t address) {
 // indicate that we are transmitting
 transmitting = 1;
 // set address of targeted slave
 txAddress = address;
 // reset tx buffer iterator vars
 tx_index = 0;
 tx_length = 0;
 }

 void HWWire::beginTransmission(int address) {
 beginTransmission(static_cast<uint8_t>(address));
 }

 uint8_t HWWire::endTransmission(void) {
 return endTransmission(true);
 }

 uint8_t HWWire::endTransmission(uint8_t sendStop) {
 uint8_t ret = writeTo(txAddress, tx_buffer, tx_length, sendStop);
 // reset tx buffer iterator vars
 tx_index = 0;
 tx_length = 0;
 // indicate that we are done transmitting
 transmitting = 0;
 return ret;
 }

 uint8_t HWWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize,
 uint8_t sendStop) {
 // clamp to buffer length
 if (quantity > RX_BUFFER_LENGTH) {
 quantity = RX_BUFFER_LENGTH;
 }
 // perform blocking read into buffer
 uint8_t read = readFrom(address, rx_buffer, quantity, sendStop, tx_buffer, tx_length);
 // set rx buffer iterator vars
 tx_index = 0;
 tx_length = 0;
 transmitting = 0;

 rx_index = 0;
 rx_length = read;

 return read;
 }

 uint8_t HWWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
 return requestFrom((uint8_t) address, (uint8_t) quantity, (uint32_t) 0, (uint8_t) 0, (uint8_t) sendStop);
 }

 uint8_t HWWire::requestFrom(uint8_t address, uint8_t quantity) {
 return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
 }

 uint8_t HWWire::requestFrom(int16_t address, int16_t quantity) {
 return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
 }

 uint8_t HWWire::requestFrom(int16_t address, int16_t quantity, int16_t sendStop) {
 return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
 }

 size_t HWWire::write(uint8_t data) {
 if (tx_length >= TX_BUFFER_LENGTH) {
 return 0;
 }
 // put byte in tx buffer
 tx_buffer[tx_index] = data;
 tx_index++;
 // update amount in buffer
 tx_length = tx_index;
 return 1;
 }
 */
/*size_t HWWire::write(const char * str) {
 return write((uint8_t *) str, strlen(str));
 }*/
/*
 size_t HWWire::write(const uint8_t *data, size_t quantity) {
 size_t n = 0;
 for (size_t i = 0; i < quantity; ++i) {
 n += write(data[i]);
 }
 return n;
 }

 uint16_t HWWire::available(void) {
 return rx_length - rx_index;
 }

 int16_t HWWire::read(void) {
 int16_t value = -1;
 if (rx_index < rx_length) {
 value = rx_buffer[rx_index];
 rx_index++;
 }
 return value;
 }

 int16_t HWWire::peek(void) {
 int16_t value = -1;
 if (rx_index < rx_length) {
 value = rx_buffer[rx_index];
 }
 return value;
 }

 void HWWire::flush(void) {
 //TODO:to be continue.
 }
 */
