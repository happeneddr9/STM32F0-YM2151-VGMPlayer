/**
 * @Name    : TIMER3.hpp
 * @Date    : 2019年11月21日 下午9:19:36
 * @Author  : happe
 * @brief   :
 */

#ifndef TIMER_TIMER3_HPP_
#define TIMER_TIMER3_HPP_

#include "TIMER.h"

Timer Timer3(TIM::Timer3, TIM3_IRQn);



extern "C"{
	void TIM3_IRQHandler(void) {
		/*if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
			if (Timer3.inInterrupt != nullptr)
				Timer3.inInterrupt();
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		}*/
		Timer3.IRQ_handler();
	}
}


#endif /* TIMER_TIMER3_HPP_ */
