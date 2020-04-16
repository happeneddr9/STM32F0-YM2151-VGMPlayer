/**
 * @Name    : TIMER.h
 * @Date    : 2019年11月20日 下午9:30:55
 * @Author  : happe
 * @brief   :
 *
 *
 * 	Log:
 * 		2020/03/11
 * 			std::function update [cost Too expensive]
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include "main.h"
#include "TIMER_config.h"
#include "Arduino/function_view.h"
#include <functional>

class Timer {
	public:

		struct TIM_init {
			TIM_TypeDef * TIM_vect;
			uint32_t TIM_RCC;
		};


		Timer(TIM TIMx, IRQn vect)
				:  mTIM(getTIMBase(TIMx).TIM_vect), mIRQ(vect), mTimer(TIMx) {


			if (static_cast<uint8_t>(mTimer) >= 15 || static_cast<uint8_t>(mTimer) == 1)
				RCC->APB2ENR |= getTIMBase(mTimer).TIM_RCC;
			else
				RCC->APB1ENR |= getTIMBase(mTimer).TIM_RCC;
			inInterrupt = nullptr;
		}

		~Timer() = default;

		void Clock_Config(uint32_t CLKDiv, uint32_t CLKcounter, uint16_t prediv = TIM_CKD_DIV1) const;
		void IT_Config(uint32_t CLKDiv, uint32_t CLKcounter) const;
		void PWM_Config(GpioPin Pinx, PWM CHx, GpioAF AFn) const;
		void GenerateClock(uint32_t freq) const {
			freq = (freq > 0) ? freq : 1;
			Clock_Config((F_CPU / (freq * 2)) - 1, 1);
		}

		void outputTrigger(uint16_t TIM_TRGOSource) const;

		void PWM_Compare(uint32_t cp, PWM CHx) const;

		inline void PWM_Compare_CH1(uint32_t cp) const{
			mTIM->CCR1 = cp;
		}

		inline void PWM_Compare_CH2(uint32_t cp) const{
			mTIM->CCR2 = cp;
		}

		inline void PWM_Compare_CH3(uint32_t cp) const{
			mTIM->CCR3 = cp;
		}

		inline void PWM_Compare_CH4(uint32_t cp) const{
			mTIM->CCR4 = cp;
		}

		finline void onInterrupt(void (*func)(void)){
			inInterrupt = func;
		}

		/*inline void onInterrupt(function_view<void(void)>& func) {
			inInterrupt = func;
		}

		inline void onInterrupt(function_view<void(void)> && func) {
			inInterrupt = func;
		}*/

		inline void IRQ_handler(void) {
			if (getIT(TIM_IT_Update)) {
				if (inInterrupt != nullptr)
					inInterrupt();
				clearIT(TIM_IT_Update);
			}
		}


		inline bool getIT(uint16_t TIM_IT) {
			uint16_t itstatus = 0x0, itenable = 0x0;

			itstatus = mTIM->SR & TIM_IT;
			itenable = mTIM->DIER & TIM_IT;

			return (itstatus && itenable);
		}

		inline void clearIT(uint16_t TIM_IT) {
			/* Clear the IT pending Bit */
			mTIM->SR = static_cast<uint16_t>(~TIM_IT);
		}




		void start(void) const;
		void stop(void) const;

		inline uint32_t getTimerCount(void) const {
			return mTIM->CNT;
		}

		void (*inInterrupt)(void);
		//std::function<void(void)> inInterrupt;
		//function_view<void(void)> inInterrupt;

	private:
		constexpr TIM_init getTIMBase(TIM timerx) const {
			switch (timerx) {
				case TIM::Timer1:
					//RCC->APB2ENR |= RCC_APB2Periph_TIM1;
					return {TIM1, RCC_APB2Periph_TIM1};
					break;
				case TIM::Timer2:
					//RCC->APB1ENR |= RCC_APB1Periph_TIM2;
					return {TIM2, RCC_APB1Periph_TIM2};
					break;
				case TIM::Timer3:
					//RCC->APB1ENR |= RCC_APB1Periph_TIM3;
					return {TIM3, RCC_APB1Periph_TIM3};
					break;
				case TIM::Timer6:
					//RCC->APB1ENR |= RCC_APB1Periph_TIM6;
					return {TIM6, RCC_APB1Periph_TIM6};
					break;
#if !defined(STM32F051)
				case TIM::Timer7:
					//RCC->APB1ENR |= RCC_APB1Periph_TIM7;
					return {TIM7, RCC_APB1Periph_TIM7};
					break;
#endif
				case TIM::Timer14:
					//RCC->APB1ENR |= RCC_APB1Periph_TIM14;
					return {TIM14, RCC_APB1Periph_TIM14};
					break;
				case TIM::Timer15:
					//RCC->APB2ENR |= RCC_APB2Periph_TIM15;
					return {TIM15, RCC_APB2Periph_TIM15};
					break;
				case TIM::Timer16:
					//RCC->APB2ENR |= RCC_APB2Periph_TIM16;
					return {TIM16, RCC_APB2Periph_TIM16};
					break;
				case TIM::Timer17:
					//RCC->APB2ENR |= RCC_APB2Periph_TIM17;
					return {TIM17, RCC_APB2Periph_TIM17};
					break;
				default:
					break;
			}
			return {TIM1, 0};
		}



		TIM_TypeDef * mTIM;
		IRQn mIRQ;
		TIM mTimer;
	protected:

};

#endif /* TIMER_TIMER_H_ */
