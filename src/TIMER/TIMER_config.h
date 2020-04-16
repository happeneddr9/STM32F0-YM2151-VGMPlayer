/**
 * @Name    : TIMER_config.h
 * @Date    : 2019年11月20日 下午9:36:34
 * @Author  : happe
 * @brief   :
 */

#ifndef TIMER_TIMER_CONFIG_H_
#define TIMER_TIMER_CONFIG_H_

#include "main.h"
#include "GPIO/GPIO.h"

enum class TIM
	: uint8_t {
		Timer1 = 1,
	Timer2 = 2,
	Timer3 = 3,
	Timer6 = 6,
	Timer7 = 7,
	Timer14 = 14,
	Timer15 = 15,
	Timer16 = 16,
	Timer17 = 17
};

enum class PWM
	: uint8_t {
		Channel1 = 0,
	Channel2 = 1,
	Channel3 = 2,
	Channel4 = 3,
	NoChannel = 255
};

constexpr uint16_t getPWM_ID(TIM TIMx, PWM pwmx) {
	return ((static_cast<uint16_t>(TIMx)) * 4 + static_cast<uint16_t>(pwmx));
}

constexpr uint16_t getPWM_AF_ID(TIM TIMx, GpioPin Pinx) {
	return ((static_cast<uint16_t>(TIMx)) * 256 + Pinx.getID());
}

namespace TIMER {

	struct PWM_AdvancedPinMap {
			GpioPin TIM_CH1;
			GpioPin TIM_CH2;
			GpioPin TIM_CH3;
			GpioPin TIM_CH4;

			GpioPin TIM_CH1N;
			GpioPin TIM_CH2N;
			GpioPin TIM_CH3N;
			GpioPin TIM_CH4N;

			GpioPin TIM_BKIN;
	};

	struct PWM_GeneralPinMap {
			GpioPin TIM_CH1;
			GpioPin TIM_CH2;
			GpioPin TIM_CH3;
			GpioPin TIM_CH4;
	};

	namespace T3 {
		//constexpr const GpioPin CH1_Remap[4] = {PA0, PA1, PA2, PA3}; TODO:getMapping
		constexpr const GpioPin CH1_Pin[4] = { PA6, NoPin, PB4, PC6 };
		constexpr const GpioPin CH2_Pin[4] = { PA7, NoPin, PB5, PC7 };
		constexpr const GpioPin CH3_Pin[4] = { PB0, NoPin, PB0, PC8 };
		constexpr const GpioPin CH4_Pin[4] = { PB1, NoPin, PB1, PC9 };

	}

	struct AF_Channel{
			GpioAF Pin_AFx;
			PWM Channelx;
	};
}

//

#endif /* TIMER_TIMER_CONFIG_H_ */
