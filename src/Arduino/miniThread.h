/*
 * miniThread.h
 *
 *  Created on: 2020年3月20日
 *      Author: user
 */

#ifndef ARDUINO_MINITHREAD_H_
#define ARDUINO_MINITHREAD_H_

#include "Basic.h"

template<volatile SystemTicker_t& ClockSource>
class miniThread_Base {
	public:

		miniThread_Base()
			: miniThread_Base(1000) {

		}

		miniThread_Base(SystemTicker_t base) {
			_tickTime = base;
			reset();
		}

		inline void reset() {
			_lastTime = ClockSource;
		}

		inline void enable() {
			_lastTime = ClockSource + _tickTime;
		}

		inline void setTick(SystemTicker_t newTime) {
			_tickTime = newTime;
		}

		inline SystemTicker_t getTick(void) {
			return _tickTime;
		}

		inline SystemTicker_t getLast(void) {
			return _lastTime;
		}

		inline bool isTick(void) {
			if (ClockSource - _lastTime >= _tickTime) {
				reset();
				return true;
			} else {
				return false;
			}
		}

		inline operator bool() {
			return isTick();
		}


	private:

	protected:
		SystemTicker_t _tickTime;
		SystemTicker_t _lastTime;
};


using miniThread = miniThread_Base<System_Clock>;



#endif /* ARDUINO_MINITHREAD_H_ */
