/**
 * @Name    : EXTI.hpp
 * @Date    : 2019年11月24日 下午4:27:47
 * @Author  : happe
 * @brief   : reference from Andy Brown's STM32Plus EXTI
 */

#ifndef GPIO_EXTI_HPP_
#define GPIO_EXTI_HPP_

#include "main.h"
#include "EXTI_handler.h"
#include "GPIO.h"
#include "SYSTEM/Nvic.hpp"

template <uint8_t TExtiNumber>
class ExtiInterruptEnabler {

	private:
		typedef void (*FPTR)();         // this trick will force the linker to include the ISR
		static FPTR _forceLinkage;

	public:
		static void enable();
		static void disable();
};

template <uint8_t TExtiNumber>
typename ExtiInterruptEnabler<TExtiNumber>::FPTR ExtiInterruptEnabler<TExtiNumber>::_forceLinkage = nullptr;

/**
 * Enabler specialisations for lines 0 to 3
 */

template <>
inline void ExtiInterruptEnabler<0>::enable() {
	_forceLinkage = &EXTI0_1_IRQHandler;
	Nvic::configureIrq(EXTI0_1_IRQn);
}

template <>
inline void ExtiInterruptEnabler<0>::disable() {
	Nvic::configureIrq(EXTI0_1_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<1>::enable() {
	_forceLinkage = &EXTI0_1_IRQHandler;
	Nvic::configureIrq(EXTI0_1_IRQn);
}

template <>
inline void ExtiInterruptEnabler<1>::disable() {
	Nvic::configureIrq(EXTI0_1_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<2>::enable() {
	_forceLinkage = &EXTI2_3_IRQHandler;
	Nvic::configureIrq(EXTI2_3_IRQn);
}

template <>
inline void ExtiInterruptEnabler<2>::disable() {
	Nvic::configureIrq(EXTI2_3_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<3>::enable() {
	_forceLinkage = &EXTI2_3_IRQHandler;
	Nvic::configureIrq(EXTI2_3_IRQn);
}

template <>
inline void ExtiInterruptEnabler<3>::disable() {
	Nvic::configureIrq(EXTI2_3_IRQn, DISABLE);
}

/**
 * 4 through 15 are on a shared IRQ
 */

template <>
inline void ExtiInterruptEnabler<4>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<4>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<5>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<5>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<6>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<6>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<7>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<7>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<8>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<8>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<9>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<9>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<10>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<10>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<11>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<11>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<12>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<12>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<13>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<13>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<14>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<14>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

template <>
inline void ExtiInterruptEnabler<15>::enable() {
	_forceLinkage = &EXTI4_15_IRQHandler;
	Nvic::configureIrq(EXTI4_15_IRQn);
}

template <>
inline void ExtiInterruptEnabler<15>::disable() {
	Nvic::configureIrq(EXTI4_15_IRQn, DISABLE);
}

/**
 * Non-GPIO EXTI lines
 */

#if defined(STM32F051)
template<>
inline void ExtiInterruptEnabler<16>::enable() {
	_forceLinkage=&PVD_IRQHandler;
	Nvic::configureIrq(PVD_IRQn);
}

template<>
inline void ExtiInterruptEnabler<16>::disable() {
	Nvic::configureIrq(PVD_IRQn,DISABLE);
}

#endif

template <>
inline void ExtiInterruptEnabler<17>::enable() {
	_forceLinkage = &RTC_IRQHandler;
	Nvic::configureIrq(RTC_IRQn);
}

template <>
inline void ExtiInterruptEnabler<17>::disable() {
	Nvic::configureIrq(RTC_IRQn, DISABLE);
}

#if defined(STM32F051)

template<>
inline void ExtiInterruptEnabler<19>::enable() {
	_forceLinkage=&TS_IRQHandler;
	Nvic::configureIrq(TS_IRQn);
}

template<>
inline void ExtiInterruptEnabler<19>::disable() {
	Nvic::configureIrq(TS_IRQn,DISABLE);
}
#endif

enum class EXTI_Trigger
	: uint8_t {
		Rising = 0x08,
	Falling = 0x0C,
	Changing = 0x10
};

enum class EXTI_Mode
	: uint8_t {
		EXTI_Interrupt = 0x00,
	EXTI_Event = 0x04
};

template <uint8_t EXTI_Source>
class EXTI_Gpio {
	public:

		EXTI_Gpio(GpioPin Pinx, GpioPuPd pupd = GpioPuPd::PullDown)
				: mPin(Pinx) {
			Gpio EXT_Gpio(Pinx);
			EXT_Gpio.init(GpioMode::In, GpioOutType::PP, GpioSpeed::s50MHz, pupd);
			RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
			SYSCFG_EXTILineConfig(static_cast<uint8_t>(Pinx.port), Pinx.pin);


		}

		void setTrigger(EXTI_Trigger Trigger, EXTI_Mode mode = EXTI_Mode::EXTI_Interrupt) const {

			EXTI_InitTypeDef EXTI_InitStructure;
			EXTI_InitStructure.EXTI_Line = (1 << mPin.pin);                     //對應pin值
			EXTI_InitStructure.EXTI_Mode = static_cast<EXTIMode_TypeDef>(mode);
			EXTI_InitStructure.EXTI_Trigger = static_cast<EXTITrigger_TypeDef>(Trigger);         //正緣觸發
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);

		}

		void EXTI_Enable(void) const {
			Exti_IT_Ena.enable();
		}

		void EXTI_Disable(void) const {
			Exti_IT_Ena.disable();
		}

	private:

	protected:
		GpioPin mPin;
		ExtiInterruptEnabler<EXTI_Source> Exti_IT_Ena;
};

#endif /* GPIO_EXTI_HPP_ */
