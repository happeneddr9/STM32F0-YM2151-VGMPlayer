/**
 * @Name    : TIMER.cpp
 * @Date    : 2019年11月20日 下午9:31:03
 * @Author  : happe
 * @brief   :
 */

#include "TIMER.h"
#include "SYSTEM/Nvic.hpp"

void Timer::Clock_Config(uint32_t CLKDiv, uint32_t CLKcounter, uint16_t prediv) const {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_InternalClockConfig(mTIM);
	TIM_TimeBaseStructure.TIM_Prescaler = CLKDiv;
	TIM_TimeBaseStructure.TIM_ClockDivision = prediv;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = CLKcounter;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(mTIM, &TIM_TimeBaseStructure);
}

void Timer::IT_Config(uint32_t CLKDiv, uint32_t CLKcounter) const {

	Nvic::configureIrq(mIRQ, ENABLE, 1);

	this->Clock_Config(CLKDiv, CLKcounter);
	/* Clear the flags */
	mTIM->SR = static_cast<uint16_t>(~TIM_FLAG_Update);
	//TIM_ARRPreloadConfig(mTIM, DISABLE);
	TIM_ITConfig(mTIM, TIM_IT_Update, ENABLE);
}


/**
 * @brief:	configrate GPIO for PWM output
 *
 * 	TODO:	looking for a better ways.
 */
void Timer::PWM_Config(GpioPin Pinx, PWM CHx, GpioAF AFn) const {
	Gpio PWM_IO(Pinx);
	PWM_IO.init(GpioMode::AF);
	PWM_IO.setAF(AFn);

	TIM_OCInitTypeDef TIM_OC;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	TIM_OCStructInit(&TIM_OC);
	TIM_OC.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC.TIM_Pulse = 0; // null duty

	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	//TIM_BDTRConfig(mTIM, &TIM_BDTRInitStructure);
	mTIM->BDTR = static_cast<uint16_t>(TIM_BDTRInitStructure.TIM_OSSRState)
			| TIM_BDTRInitStructure.TIM_OSSIState
			| TIM_BDTRInitStructure.TIM_LOCKLevel
			| TIM_BDTRInitStructure.TIM_DeadTime
			| TIM_BDTRInitStructure.TIM_Break
			| TIM_BDTRInitStructure.TIM_BreakPolarity
			| TIM_BDTRInitStructure.TIM_AutomaticOutput;

	switch (CHx) {
		case PWM::Channel1:
			TIM_OC1Init(mTIM, &TIM_OC);
			TIM_OC1PolarityConfig(mTIM, TIM_OCPreload_Enable);

			break;
		case PWM::Channel2:
			TIM_OC2Init(mTIM, &TIM_OC);
			TIM_OC2PolarityConfig(mTIM, TIM_OCPreload_Enable);

			break;
		case PWM::Channel3:
			TIM_OC3Init(mTIM, &TIM_OC);
			TIM_OC3PolarityConfig(mTIM, TIM_OCPreload_Enable);

			break;
		case PWM::Channel4:
			TIM_OC4Init(mTIM, &TIM_OC);
			TIM_OC4PolarityConfig(mTIM, TIM_OCPreload_Enable);

			break;
		default:
			break;
	}
}

void Timer::outputTrigger(uint16_t TIM_TRGOSource) const {
	/* Reset the MMS Bits */
	mTIM->CR2 &= static_cast<uint16_t>(~(static_cast<uint16_t>(TIM_CR2_MMS)));
	/* Select the TRGO source */
	mTIM->CR2 |= TIM_TRGOSource;
}



void Timer::PWM_Compare(uint32_t cp, PWM CHx) const {
	switch (CHx) {
		case PWM::Channel1:
			PWM_Compare_CH1(cp);
			break;
		case PWM::Channel2:
			PWM_Compare_CH2(cp);
			break;
		case PWM::Channel3:
			PWM_Compare_CH3(cp);
			break;
		case PWM::Channel4:
			PWM_Compare_CH4(cp);
			break;
		default:
			break;
	}
}


/*
 #define EnableTim(n)               (n.TIMx->CR1 |= TIM_CR1_CEN)
 #define DisableTim(n)              (n.TIMx->CR1 &= (~((U16)TIM_CR1_CEN)))

 #define ClearTimCount(n)           (n.TIMx->CNT = 0)
 #define ClearTimeFlag(n)           (n.TIMx->SR = (U16)~TIM_FLAG_Update)
 */

void Timer::start(void) const{
	mTIM->CR1 |= TIM_CR1_CEN;
	mTIM->CNT = 0;
	mTIM->SR = static_cast<uint16_t>(~TIM_FLAG_Update);

}

void Timer::stop(void) const{
	mTIM->CR1 &= ~(static_cast<uint16_t>(TIM_CR1_CEN));
}

