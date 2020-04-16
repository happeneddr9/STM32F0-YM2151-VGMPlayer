/**
 * @Name    : AnalogPin.h
 * @Date    : 2019年12月20日 下午9:07:49
 * @Author  : happe
 * @brief   :
 */

#ifndef GPIO_ANALOGPIN_H_
#define GPIO_ANALOGPIN_H_

#include "GpioPin.h"


#if defined(STM32F030) || defined(STM32F051)  // only ADC1

/* ADC1 index Channels */
constexpr const GpioPin ADC1_AnalogPin[16] = {
		PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
		PB0, PB1,
		PC0, PC1, PC2, PC3, PC4, PC5
};

constexpr const size_t __ADC1_CH_number__ = 12;

/*      Analog Pin      */
/* For Arduino Supports */

constexpr auto A0 = PA0;
constexpr auto A1 = PA1;
constexpr auto A2 = PA2;
constexpr auto A3 = PA3;
constexpr auto A4 = PA4;
constexpr auto A5 = PA5;
constexpr auto A6 = PA6;
constexpr auto A7 = PA7;
// No A8
constexpr auto A9 = PB1;


constexpr const ADC_TypeDef * ADC_Group[] = { ADC1 };
constexpr ADC_TypeDef * isAnalogPin(uint16_t PinID) {
	if (PinID <= getPinID(PA7) || PinID == getPinID(PB1)) {
		RCC->APB2ENR |= RCC_APB2Periph_ADC1;
		return ADC1;
	}
	return nullptr;
}

// PA0 ... PA7, PB1 as Analog Pin
constexpr uint32_t getAnalogChannel(uint16_t PinID) {
	uint32_t ADC_CH = 0;
	switch (PinID) {
		case getPinID(PA0) ... getPinID(PA7):
			ADC_CH = 1 << (PinID & 0x0F);
			break;
		case getPinID(PB1):
			ADC_CH = 0x100;
			break;
		default:
			ADC_CH = 0;
			break;
	}
	return ADC_CH;
}
#endif



#endif /* GPIO_ANALOGPIN_H_ */
