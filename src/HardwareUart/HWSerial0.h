/*
 * HWSerial0.h
 *
 *  Created on: 2019年9月7日
 *      Author: happe
 */

#ifndef HWSERIAL0_H_
#define HWSERIAL0_H_

#include "HWSerial.h"
#include "USART1PinMap.h"

#ifndef HAVE_Serial0
#define HAVE_Serial0
HWSerial Serial(USART1, USART1_IRQn, 3);
#endif // HAVE_Serial0


#ifdef HAVE_Serial0
#ifdef __cplusplus
extern "C" {
#endif
	void USART1_IRQHandler(void) {
		/*if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
			Serial.LoadDatain(uint8_t(USART_ReceiveData(USART1)));
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}

		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
			Serial.LoadStopbit();
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		}*/
		Serial.IRQ_Handler();
	}
#ifdef __cplusplus
}   //extern "C"
#endif
#endif // HAVE_Serial0



#endif /* HWSERIAL0_H_ */
