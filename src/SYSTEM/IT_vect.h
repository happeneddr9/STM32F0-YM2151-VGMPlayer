/**
 * @Name    : IT_vect.h
 * @Date    : 2019年12月18日 下午9:00:47
 * @Author  : happe
 * @brief   :
 */

#ifndef SYSTEM_IT_VECT_H_
#define SYSTEM_IT_VECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(STM32F0)
	/* STM32F0xx ALL interrupt Handler */
	/*extern void NMI_Handler(void);
	extern void HardFault_Handler(void);
	extern void SVC_Handler(void);
	extern void PendSV_Handler(void);
	extern void WWDG_IRQHandler(void);
	extern void PVD_IRQHandler(void);
	extern void RTC_IRQHandler(void);
	extern void FLASH_IRQHandler(void);
	extern void RCC_IRQHandler(void);

	extern void EXTI0_1_IRQHandler(void);
	extern void EXTI2_3_IRQHandler(void);
	extern void EXTI4_15_IRQHandler(void);

	extern void DMA1_CH1_IRQHandler(void);
	extern void DMA1_CH2_3_IRQHandler(void);
	extern void DMA1_CH4_5_IRQHandler(void);

	extern void ADC_IRQHandler(void);

	extern void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
	extern void TIM1_CC_IRQHandler(void);
	extern void TIM3_IRQHandler(void);
	extern void TIM6_IRQHandler(void);
	extern void TIM14_IRQHandler(void);
	extern void TIM15_IRQHandler(void);
	extern void TIM16_IRQHandler(void);
	extern void TIM17_IRQHandler(void);

	extern void I2C1_IRQHandler(void);
	extern void I2C2_IRQHandler(void);
	extern void SPI1_IRQHandler(void);
	extern void SPI2_IRQHandler(void);
	extern void USART1_IRQHandler(void);
	extern void USART2_IRQHandler(void);
	extern void USART3_4_5_6_IRQHandler(void);
	extern void USB_IRQHandler(void);*/
#endif // defined(STM32F0)
#ifdef __cplusplus
} // extern "C"{
#endif

#endif /* SYSTEM_IT_VECT_H_ */
