/**
 * @Name    : YM2151.h
 * @Date    : 2020年1月14日 下午7:48:35
 * @Author  : happe
 * @brief   :
 */

#ifndef SOUND_YM2151_H_
#define SOUND_YM2151_H_

#include "Arduino/Basic.h"
#include "GPIO/GPIO.h"
#include "Sound/Soundchip.h"

class YM2151 : public SoundChip {
	public:
		YM2151(DataBusType& pins, GpioPin cs, GpioPin rd, GpioPin wr, GpioPin a0, GpioPin iqr, GpioPin ic);

		void reset() const override;
		void sendData(uint8_t addr, uint8_t data) const override;

	private:

	protected:
		GpioFast _CS;
		GpioFast _RD;
		GpioFast _WR;
		GpioFast _A0;
		GpioFast _IRQ;
		GpioFast _IC;
};

#endif /* SOUND_YM2151_H_ */
