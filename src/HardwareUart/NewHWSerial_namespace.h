/*
 * NewHWSerial_namespace.h
 *
 *  Created on: 2019年11月12日
 *      Author: happe
 */

#ifndef HARDWAREUART_NEWHWSERIAL_NAMESPACE_H_
#define HARDWAREUART_NEWHWSERIAL_NAMESPACE_H_

#include "main.h"

enum class UARTx
	: unsigned char {
		HW_UART1 = 1,
	HW_UART2,
	HW_UART3,
	HW_UART4,
	HW_UART5,
	HW_UART6,
	HW_UART7,
	HW_UART8
};

constexpr USART_TypeDef * getUARTStruct(UARTx uartx) {
	switch (uartx) {
		case UARTx::HW_UART1:
			return USART1;
			break;
		case UARTx::HW_UART2:
			return USART2;
			break;
#if !defined(STM32F051)
		case UARTx::HW_UART3:
			return USART3;
			break;
		case UARTx::HW_UART4:
			return USART4;
			break;
		case UARTx::HW_UART5:
			return USART5;
			break;
		case UARTx::HW_UART6:
			return USART6;
			break;
		case UARTx::HW_UART7:
			return USART7;
			break;
		case UARTx::HW_UART8:
			return USART8;
			break;
#endif
		default:
			return USART1;
			break;
	}
}

void setUART_RCC(UARTx RCC_UARTx) {
#if defined(STM32F0)
	switch (RCC_UARTx) {
		case UARTx::HW_UART1:
			RCC->APB2ENR |= RCC_APB2Periph_USART1;
			break;
		case UARTx::HW_UART2:
			RCC->APB1ENR |= RCC_APB1Periph_USART2;  //APB1
			break;
#if !defined(STM32F051)
		case UARTx::HW_UART3:
			RCC->APB1ENR |= RCC_APB1Periph_USART3;  //APB1
			break;
		case UARTx::HW_UART4:
			RCC->APB1ENR |= RCC_APB1Periph_USART4;  //APB1
			break;
		case UARTx::HW_UART5:
			RCC->APB1ENR |= RCC_APB1Periph_USART5;  //APB1
			break;
		case UARTx::HW_UART6:
			RCC->APB2ENR |= RCC_APB2Periph_USART6;
			break;
		case UARTx::HW_UART7:
			RCC->APB2ENR |= RCC_APB2Periph_USART7;
			break;
		case UARTx::HW_UART8:
			RCC->APB2ENR |= RCC_APB2Periph_USART8;
			break;
#endif
	}
#elif defined(STM32F1)
	switch (RCC_UARTx) {

	}
#elif defined(STM32F4)
	switch (RCC_UARTx) {

	}
#elif defined(STM32F7)
	switch (RCC_UARTx) {

	}
#else

#endif
}

#endif /* HARDWAREUART_NEWHWSERIAL_NAMESPACE_H_ */
