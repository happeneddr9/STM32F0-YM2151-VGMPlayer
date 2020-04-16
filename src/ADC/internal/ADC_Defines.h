/*
 * ADC_Defines.h
 *
 *  Created on: 2020年3月31日
 *      Author: user
 */

#ifndef ADC_INTERNAL_ADC_DEFINES_H_
#define ADC_INTERNAL_ADC_DEFINES_H_

#include "GPIO/GPIO.h"

namespace ADC_Defines {
	struct CFGR1 {
			union {
					struct {
							/* Index: 0 & 1 Bytes */
							unsigned DMAEN 			:1;
							unsigned DMACFG 		:1;
							unsigned SCANDIR 		:1;
							unsigned RES 			:2;		// [1:0]
							unsigned ALIGN 			:1;
							unsigned EXTSEL 		:3;		// [2:0]

							unsigned reserved0 		:1;
							unsigned EXTEN 			:2;
							unsigned OVRMOD 		:1;
							unsigned CONT 			:1;
							unsigned WAIT 			:1;
							unsigned AUTOFF 		:1;

							/* Index: 2 Bytes */
							unsigned DISCEN 		:1;
							unsigned reserved1 		:5;
							unsigned AWDSGL 		:1;
							unsigned AWDEN 			:1;

							/* Index: 3 Bytes */
							unsigned reserved2 		:2;
							unsigned reserved3 		:1;
							unsigned AWDCH 			:5;
					};

					uint32_t reg;							// use it as 32-bits register
			};

	} __attribute__((packed));
}



#endif /* ADC_INTERNAL_ADC_DEFINES_H_ */
