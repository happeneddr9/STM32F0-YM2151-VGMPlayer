/**
 * @Name    : YM2151.cpp
 * @Date    : 2020年1月14日 下午7:48:46
 * @Author  : happe
 * @brief   :
 */

#include "YM2151.h"

YM2151::YM2151(DataBusType& pins, GpioPin cs, GpioPin rd, GpioPin wr, GpioPin a0, GpioPin iqr, GpioPin ic)
	: SoundChip(pins), _CS(cs), _RD(rd), _WR(wr), _A0(a0), _IRQ(iqr), _IC(ic) {


	DataBus.init(GpioMode::out);
	DataBus.setValue(0);
	_CS.init(GpioMode::out);
	_RD.init(GpioMode::out);
	_WR.init(GpioMode::out);
	_A0.init(GpioMode::out);
	_IRQ.init(GpioMode::out);
	_IC.init(GpioMode::out);

	_CS.setHigh();
	_RD.setHigh();
	_WR.setHigh();
	_A0.setHigh();
	if (_IRQ.getPinNum() != GpioFast::NotGpio)
		_IRQ.setLow();
	_IC.setHigh();
}

void YM2151::reset() const {
	_IC.setLow();
	tinyDelay(35);
	_IC.setHigh();
}

void YM2151::sendData(uint8_t addr, uint8_t data) const {
	_WR.setLow();				// Start Write to YM2151
	_A0.setLow();				// Enable Address Write
	DataBus.setValue(addr);		// Write Register Address
	_CS.setLow();				// Chip Select

	//delayMicroseconds(1);		// wait a same time
	tinyDelay(1);

	_CS.setHigh();				// Chip Unselect
	//_WR.setLow();
	_A0.setHigh();				// Disable Address Write
	DataBus.setValue(data);
	_CS.setLow();				// Chip Select

	//delayMicroseconds(1);		// wait a same time
	tinyDelay(1);

	_CS.setHigh();				// Chip Unselect
	_WR.setHigh();				// End Write to YM2151

}










