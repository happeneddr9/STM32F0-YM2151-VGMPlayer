/**
 * @Name    : HWSerial_Config.h
 * @Date    : 2020年1月9日 下午6:59:08
 * @Author  : happe
 * @brief   :
 */

#ifndef HARDWAREUART_HWSERIAL_CONFIG_H_
#define HARDWAREUART_HWSERIAL_CONFIG_H_

#include "GPIO/GPIO.h"

namespace HWSerial_PinMap {

	struct USART_PinMap_ALL {
			GpioPin Pin_TX;
			GpioPin Pin_RX;
			GpioPin Pin_RTS;
			GpioPin Pin_CTS;
			GpioPin Pin_CK;
			GpioAF	Pin_AF;
	};

	struct USART_PinMap_TXRX {
			GpioPin Pin_TX;
			GpioPin Pin_RX;
			GpioAF	Pin_AF;
	};
}


#endif /* HARDWAREUART_HWSERIAL_CONFIG_H_ */
