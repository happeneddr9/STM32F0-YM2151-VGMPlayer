/*
 * usingHWWire0.h
 *
 *  Created on: 2019年10月1日
 *      Author: happe
 */

#ifndef I2CMASTER_USINGHWI2C1_H_
#define I2CMASTER_USINGHWI2C1_H_

#include "HWI2C_IT.h"
#include "I2C1PinMap.h"

HWWire Wire(I2C1, I2C1_IRQn);
#define HAVE_Wire

extern "C" {
	void I2C1_IRQHandler(void) {
		if (I2C_GetITStatus(I2C1, I2C_IT_RXNE) != RESET) {
			//Receive
			Wire.__IT_pushData_in(I2C_ReceiveData(I2C1));
			I2C_ClearITPendingBit(I2C1, I2C_IT_RXNE);
		} else if (I2C_GetITStatus(I2C1, I2C_IT_TXIS) != RESET) {
			//Ready to Transmit
			Wire.__IT_pushData_out();
			I2C_ClearITPendingBit(I2C1, I2C_IT_TXIS);
		} else if (I2C_GetITStatus(I2C1, I2C_IT_ADDR) != RESET) {
			//Address Match
			Wire.__IT_AddressMatch();
			I2C_ClearITPendingBit(I2C1, I2C_IT_ADDR);
		} else if (I2C_GetITStatus(I2C1, I2C_IT_NACKF) != RESET) {
			//Not Acknowledge
			Wire.__IT_getNACK();
			I2C_ClearITPendingBit(I2C1, I2C_IT_NACKF);
		} else if (I2C_GetITStatus(I2C1, I2C_IT_STOPF) != RESET) {
			//Stop bit Received
			Wire.__IT_getStopbit();
			I2C_ClearITPendingBit(I2C1, I2C_IT_STOPF);
		} else if (I2C_GetITStatus(I2C1, I2C_IT_TC | I2C_IT_TCR) != RESET) {
			//Transfer Complete
			I2C_ClearITPendingBit(I2C1, I2C_IT_TC | I2C_IT_TCR);
		} else if (I2C_GetITStatus(I2C1, I2C_IT_BERR | I2C_IT_ARLO | I2C_IT_OVR | I2C_IT_PECERR | I2C_IT_TIMEOUT | I2C_IT_ALERT) != RESET) {
			//ERROR
			Wire.__IT_getERROR();
			I2C_ClearITPendingBit(I2C1, I2C_IT_BERR | I2C_IT_ARLO | I2C_IT_OVR | I2C_IT_PECERR | I2C_IT_TIMEOUT | I2C_IT_ALERT);
		}
	}
}

#endif /* I2CMASTER_USINGHWI2C1_H_ */
