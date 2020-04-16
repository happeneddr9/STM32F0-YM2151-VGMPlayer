/**
 * @Name    : EXTI_handler.h
 * @Date    : 2019年11月24日 下午4:37:09
 * @Author  : happe
 * @brief   :
 */

#ifndef GPIO_EXTI_HANDLER_H_
#define GPIO_EXTI_HANDLER_H_

#include "main.h"

extern "C" void EXTI0_1_IRQHandler();
extern "C" void EXTI2_3_IRQHandler();
extern "C" void EXTI4_15_IRQHandler();
extern "C" void EXTI15_10_IRQHandler();
#if defined(STM32F051)
extern "C" void PVD_IRQHandler();
#endif
extern "C" void RTC_IRQHandler();
#if defined(STM32F051)
extern "C" void TS_IRQHandler();
#endif




#endif /* GPIO_EXTI_HANDLER_H_ */
