/*
 * PushButton.h
 *
 *  Created on: 2020年3月17日
 *      Author: user
 */

#ifndef GPIO_BUTTON_PUSHBUTTON_H_
#define GPIO_BUTTON_PUSHBUTTON_H_

#include "Arduino/miniThread.h"

class PushButton {
	public:
		enum internal_state : uint8_t {
			idle,
			inDebouncing
		};

		PushButton(const PushButton&) = default;
		PushButton(PushButton&&) = default;

		PushButton(GpioFast&& io, GPIO_Status _ps = HIGH, GpioPuPd pupd = GpioPuPd::NoPull)
			: pin(io) {
			init(_ps, pupd);
		}

		PushButton(const GpioFast& io, GPIO_Status _ps = HIGH, GpioPuPd pupd = GpioPuPd::NoPull)
			: pin(io) {
			init(_ps, pupd);
		}

		PushButton(const GpioPin io = NoPin, GPIO_Status _ps = HIGH, GpioPuPd pupd = GpioPuPd::NoPull)
			: pin(io) {
			init(_ps, pupd);
		}


		~PushButton() = default;

		void init(GPIO_Status _ps = HIGH, GpioPuPd pupd = GpioPuPd::NoPull) {
			_internal = idle;
			_pressedState = _ps;
			pin.init(GpioMode::In, GpioOutType::PP, GpioSpeed::s50MHz, pupd);
			_lastTime = 0;
		}

		bool getState() {
			SystemTicker_t newTime;
			bool state;

			state = read();

			if (!state) {
				_internal = idle;
				return false;
			}

			newTime = millis();

			if (_internal == idle) {
				_internal = inDebouncing;
				_lastTime = newTime;
			} else if (_internal == inDebouncing) {
				if (newTime - _lastTime >= DEBOUNCE_DELAY_MILLIS) {
					return true;
				}
			}

			return false;
		}

		bool pressed() {
			if (getState() && state_wait == false) {
				state_wait = true;
				return true;
			} else if (!read() && state_wait == true) {
				state_wait = false;
			}
			return false;
		}

		bool read() {
			return (!static_cast<bool>(_pressedState)) ^ pin.read();
		}

		operator bool() {
			return getState();
		}

	private:

	protected:
		static const SystemTicker_t DEBOUNCE_DELAY_MILLIS = 20;
		GpioFast pin;
		SystemTicker_t _lastTime;
		GPIO_Status _pressedState;
		internal_state _internal;
		bool state_wait = false;
};

#endif /* GPIO_BUTTON_PUSHBUTTON_H_ */
