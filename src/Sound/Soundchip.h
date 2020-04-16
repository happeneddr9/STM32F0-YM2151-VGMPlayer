/*
 * Soundchip.h
 *
 *  Created on: 2020年2月28日
 *      Author: user
 */

#ifndef SOUND_SOUNDCHIP_H_
#define SOUND_SOUNDCHIP_H_

#include "Arduino/Basic.h"
#include "GPIO/GPIO.h"

class SoundChip {
	public:
		using DataBusType = const GpioFastParallel<GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin>;
		// 											LSB

		/* Sound chip constructor */
		SoundChip(DataBusType& pins)
			: DataBus(pins) {

		}

		virtual void reset() const = 0;
		virtual void sendData(uint8_t addr, uint8_t data) const = 0;

		static void tinyDelay(uint32_t us) {
			uint32_t count = 48 * us;
			while(count--){
				yield();
			}
		}

	protected:
		DataBusType& DataBus;
};



#endif /* SOUND_SOUNDCHIP_H_ */
