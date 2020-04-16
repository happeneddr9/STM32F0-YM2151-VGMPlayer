/*
 * GPIO.h
 *
 *  Created on: 2019年11月11日
 *      Author: happe
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#include "main.h"
#include "GPIO_namespace.h"
#include <array>
#include <initializer_list>
#include <type_traits>

enum pinMode_Status
	: uint8_t {
		INPUT = 0x00,
	OUTPUT = 0x01,
	AFunction = 0x02,
	ANALOG = 0x03,
	OUTPUT_PP = OUTPUT,
	OUTPUT_OD = 0x05,
	INPUT_PULLUP = 0x10,
	INPUT_PULLDOWN = 0x20
};

constexpr GPIO_TypeDef * const GPIO_Port_Defines[] = {
GPIOA,
GPIOB,
GPIOC,
GPIOD,
#if !defined(STM32F051)
		GPIOE,
#else
		GPIOA,
#endif
		GPIOF };

constexpr const uint32_t GPIO_RCC_Defines[] = {
RCC_AHBPeriph_GPIOA,
RCC_AHBPeriph_GPIOB,
RCC_AHBPeriph_GPIOC,
RCC_AHBPeriph_GPIOD,
#if !defined(STM32F051)
		RCC_AHBPeriph_GPIOE,
#else
		0x00,
#endif
		RCC_AHBPeriph_GPIOF };

// get the GPIO Port Peripheral Address
// and also setup the RCC Clock
// if none, return GPIOA. TODO: looking for a better way
inline constexpr GPIO_TypeDef* getPortBase(Port port) {
	/*if (static_cast<uint8_t>(port) > GpioPin::last_port) {
	 return GPIO_Port_Defines[0];
	 } else {
	 uint8_t port_n = static_cast<uint8_t>(port);
	 //RCC->AHBENR |= GPIO_RCC_Defines[0] << port_n;
	 return GPIO_Port_Defines[port_n];
	 }*/

	uint8_t port_n =
			(static_cast<uint8_t>(port) > GpioPin::last_port) ?
					0 : static_cast<uint8_t>(port);

	return GPIO_Port_Defines[port_n];

	/*switch (port) {
	 case Port::A:
	 RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
	 return GPIOA;
	 break;
	 case Port::B:
	 RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
	 return GPIOB;
	 break;
	 case Port::C:
	 RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
	 return GPIOC;
	 break;
	 case Port::D:
	 RCC->AHBENR |= RCC_AHBPeriph_GPIOD;
	 return GPIOD;
	 break;
	 #if !defined(STM32F051)
	 case Port::E:
	 RCC->AHBENR |= RCC_AHBPeriph_GPIOE;
	 return GPIOE;
	 break;
	 #endif
	 case Port::F:
	 RCC->AHBENR |= RCC_AHBPeriph_GPIOF;
	 return GPIOF;
	 break;
	 default:
	 break;
	 }
	 return GPIOA;*/
}

///**
// * @brief : common Gpio Pin
// * @param : GpioPin or Port number + pin number
// *
// *
// */
//class Gpio {
//	public:
//		enum types : uint8_t {
//			NotGpio = 255,
//		};
//		// using GpioPin to initialize
//		constexpr Gpio(GpioPin GP = NoPin)
//				: mPort(getPortBase(GP.port)), mPin(GP.pin) {
//			RCC->AHBENR |= RCC_AHBPeriph_GPIOA << static_cast<uint8_t>(GP.port);
//		}
//
//		// using Port number and pin number to initialize
//		constexpr Gpio(Port port, uint8_t pin)
//				: Gpio(GpioPin{port, pin}) {
//
//		}
//
//		//  default none
//		~Gpio() = default;
//
//		/**
//		 *  @brief : Higher Abstract Gpio Mode initialize
//		 *  @param[in]: pinMode_Status enum
//		 *  @param[in]:	Gpio Speed Setting, 2MHz, 25MHz or 50MHz		# default use Highest Speed
//		 */
//		void begin(pinMode_Status mode_in, GpioSpeed GpioSP = GpioSpeed::s50MHz) {
//			uint8_t modex = static_cast<uint8_t>(mode_in);
//			GpioMode mode = static_cast<GpioMode>(modex & 0x03);
//			GpioOutType GpioOT = static_cast<GpioOutType>((modex & 0x0C) >> 2);
//			GpioPuPd GpioPP = static_cast<GpioPuPd>((modex & 0x30) >> 4);
//
//			init(mode, GpioOT, GpioSP, GpioPP);
//		}
//
//		/**
//		 *  @brief : Gpio Mode initialize
//		 *  @param[in]:	Input Pin, Output Pin, Analog Pin or AF Pin		# default use input
//		 *  @param[in]:	Push-Pull or Open Drain (for Output)			# default use Push-Pull
//		 *  @param[in]:	Gpio Speed Setting, 2MHz, 25MHz or 50MHz		# default use Highest Speed
//		 *  @param[in]: Gpio's Pull-up or Pull-down						# default is floatting
//		 */
//		void init(GpioMode mode = GpioMode::In, GpioOutType GpioOutType = GpioOutType::PP,
//				GpioSpeed GpioSpeed = GpioSpeed::s50MHz, GpioPuPd GpioPuPd = GpioPuPd::NoPull) const {
//			if (mPort != nullptr) {
//				uint8_t dmPin = mPin * 2;
//				mPort->MODER |= static_cast<uint32_t>(mode) << dmPin;
//				mPort->OSPEEDR |= static_cast<uint32_t>(GpioSpeed) << dmPin;
//				mPort->OTYPER |= static_cast<uint16_t>(GpioOutType) << mPin;
//				mPort->PUPDR |= static_cast<uint32_t>(GpioPuPd) << dmPin;
//			}
//		}
//
//		void setAF(GpioAF af) const {
//			const uint32_t MskOft = static_cast<uint32_t>(mPin & static_cast<uint32_t>(0x07)) * 4;
//			const uint32_t AfMask = ~(static_cast<uint32_t>(0xf) << MskOft);
//			const uint32_t AfValue = (static_cast<uint32_t>(af) << MskOft);
//			const uint32_t AfrIdx = mPin >> 3;
//
//			mPort->AFR[AfrIdx] &= AfMask;
//			mPort->AFR[AfrIdx] |= AfValue;
//		}
//
//		inline GPIO_TypeDef* getGPIOPort(void) const {
//			return mPort;
//		}
//
//		inline uint8_t getPinNum(void) const {
//			return mPin;
//		}
//
//		inline uint16_t getPinNum16(void) const {
//			return 1 << mPin;
//		}
//
//		inline void setState(bool state) const {
//			state ? setHigh() : setLow();
//		}
//
//		finline void setHigh() const {
//			mPort->BSRR = 1 << mPin;
//		}
//
//		finline void setLow() const {
//			mPort->BRR = 1 << mPin;
//		}
//
//		finline bool read() const {
//			return mPort->IDR & 1 << mPin;
//		}
//
//		finline void toggle() const {
//			mPort->ODR ^= 1 << mPin;
//		}
//
//		finline void operator =(GPIO_Status s) const {
//			setState((bool) s);
//		}
//
//		finline void operator =(int status) const {
//			setState(static_cast<bool>(status));
//		}
//
//	private:
//
//		bool isNotExti(GpioMode mode) const {
//			return !(mode == GpioMode::Exti);
//		}
//		GPIO_TypeDef* mPort;
//		uint8_t mPin;
//
//	protected:
//
//};

/**
 * @brief : GpioPin Fast, fast than Gpio
 * @param : GpioPin or Port number + pin number
 *
 *
 */
class GpioFast {
	public:
		enum types
			: uint8_t {
				NotGpio = 255,
		};
		// using GpioPin to initialize
		constexpr GpioFast(const GpioPin GP = NoPin) :
				mPort(getPortBase(GP.port)), mPin(GP.pin), mPin16(1 << GP.pin) {
			RCC->AHBENR |= RCC_AHBPeriph_GPIOA << static_cast<uint8_t>(GP.port);
		}

		// using Port number and pin number to initialize
		constexpr GpioFast(Port port, uint8_t pin) :
				GpioFast(GpioPin { port, pin }) {

		}

		constexpr GpioFast(const GpioFast& gf) = default;	// copy-constructor
		constexpr GpioFast(GpioFast&& gf) = default;		// move-constructor

		void swap(GpioFast& __x) {
			std::swap(mPort, __x.mPort);
			std::swap(mPin, __x.mPin);
			std::swap(mPin16, __x.mPin16);
		}

		constexpr GpioFast& operator=(const GpioFast&) = default;
		constexpr GpioFast& operator=(GpioFast&& __x) = default;

		//  default none
		~GpioFast() = default;

		/**
		 *  @brief : Higher Abstract Gpio Mode initialize
		 *  @param[in]: pinMode_Status enum
		 *  @param[in]:	Gpio Speed Setting, 2MHz, 25MHz or 50MHz		# default use Highest Speed
		 */
		void begin(pinMode_Status mode_in = INPUT, GpioSpeed GpioSP =
				GpioSpeed::s50MHz) const {
			uint8_t modex = static_cast<uint8_t>(mode_in);
			GpioMode mode = static_cast<GpioMode>(modex & 0x03);
			GpioOutType GpioOT = static_cast<GpioOutType>((modex & 0x0C) >> 2);
			GpioPuPd GpioPP = static_cast<GpioPuPd>((modex & 0x30) >> 4);

			init(mode, GpioOT, GpioSP, GpioPP);
		}

		/**
		 *  @brief : Gpio Mode initialization.
		 *  @param[in]:	Input Pin, Output Pin, Analog Pin or AF Pin		# default use input
		 *  @param[in]:	Push-Pull or Open Drain (for Output)			# default use Push-Pull
		 *  @param[in]:	Gpio Speed Setting, 2MHz, 25MHz or 50MHz		# default use Highest Speed
		 *  @param[in]: Gpio's Pull-up or Pull-down						# default is floatting
		 */
		void init(GpioMode mode = GpioMode::In, GpioOutType GpioOutType =
				GpioOutType::PP, GpioSpeed GpioSpeed = GpioSpeed::s50MHz,
				GpioPuPd GpioPuPd = GpioPuPd::NoPull) const {
			if (mPin < 16) {
				uint8_t dmPin = mPin * 2;

				mPort->MODER &= ~(GPIO_MODER_MODER0 << dmPin);
				mPort->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << dmPin);
				mPort->OTYPER &= ~((GPIO_OTYPER_OT_0) << mPin);
				mPort->PUPDR &= ~(GPIO_PUPDR_PUPDR0
						<< (static_cast<uint16_t>(dmPin)));

				mPort->MODER |= static_cast<uint32_t>(mode) << dmPin;
				mPort->OSPEEDR |= static_cast<uint32_t>(GpioSpeed) << dmPin;
				mPort->OTYPER |= static_cast<uint16_t>(GpioOutType) << mPin;
				mPort->PUPDR |= static_cast<uint32_t>(GpioPuPd) << dmPin;
			}
		}

		/**
		 * 	@brief:	Gpio Mode initialization, unordered version.
		 */
		void init_unordered(GpioMode mode) const {
			if (mPin < 16) {
				uint8_t dmPin = mPin * 2;
				mPort->MODER &= ~(GPIO_MODER_MODER0 << dmPin);
				mPort->MODER |= static_cast<uint32_t>(mode) << dmPin;
			}
		}

		void init_unordered(GpioOutType GpioOutType) const {
			if (mPin < 16) {
				mPort->OTYPER &= ~((GPIO_OTYPER_OT_0) << mPin);
				mPort->OTYPER |= static_cast<uint16_t>(GpioOutType) << mPin;
			}
		}

		void init_unordered(GpioSpeed GpioSpeed) const {
			if (mPin < 16) {
				uint8_t dmPin = mPin * 2;
				mPort->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << dmPin);
				mPort->OSPEEDR |= static_cast<uint32_t>(GpioSpeed) << dmPin;
			}
		}

		void init_unordered(GpioPuPd GpioPuPd) const {
			if (mPin < 16) {
				uint8_t dmPin = mPin * 2;
				mPort->PUPDR &= ~(GPIO_PUPDR_PUPDR0
						<< (static_cast<uint16_t>(dmPin)));
				mPort->PUPDR |= static_cast<uint32_t>(GpioPuPd) << dmPin;
			}
		}

		/*template<class unknow>
		 void init_unordered(unknow xx) const {
		 static_assert(false, "unknown initialize parameter!");
		 }*/

		template<class T, class ...Args>
		void init_unordered(T first, Args ... others) const {
			init_unordered(first);
			return init_unordered(others...);
		}

		void setAF(GpioAF af) const {
			if (mPin < 16) {
				const uint32_t MskOft = static_cast<uint32_t>(mPin
						& static_cast<uint32_t>(0x07)) * 4;
				const uint32_t AfMask = ~(static_cast<uint32_t>(0xf) << MskOft);
				const uint32_t AfValue = (static_cast<uint32_t>(af) << MskOft);
				const uint32_t AfrIdx = mPin >> 3;

				mPort->AFR[AfrIdx] &= AfMask;
				mPort->AFR[AfrIdx] |= AfValue;
			}
		}

		finline GPIO_TypeDef* getGPIOPort(void) const {
			return mPort;
		}

		finline uint8_t getPinNum(void) const {
			return mPin;
		}

		finline uint16_t getPinNum16(void) const {
			return mPin16;
		}

		finline void setState(bool state) const {
			state ? setHigh() : setLow();
		}

		finline void setHigh() const {
			mPort->BSRR = mPin16;
		}

		finline void setLow() const {
			mPort->BRR = mPin16;
		}

		finline bool read() const {
			return mPort->IDR & mPin16;
		}

		finline void toggle() const {
			mPort->ODR ^= mPin16;
		}

		finline GpioFast& operator =(GPIO_Status s) {
			setState(static_cast<bool>(s));
			return *this;
		}

		finline GpioFast& operator =(bool status) {
			setState(status);
			return *this;
		}

		finline const GpioFast& operator =(GPIO_Status s) const {
			setState(static_cast<bool>(s));
			return *this;
		}

		finline const GpioFast& operator =(bool status) const {
			setState(status);
			return *this;
		}

		finline operator bool() const {
			return read();
		}

	private:

		bool isNotExti(GpioMode mode) const {
			return !(mode == GpioMode::Exti);
		}
		GPIO_TypeDef* mPort;
		uint8_t mPin;
		uint16_t mPin16;

	protected:
};

using Gpio = GpioFast;


/**
 * @brief : Parallel Gpio, used like Port
 * @param : Variable Length Gpio Pin list
 *
 *
 */
template<typename ...Items>
class GpioParallel {

	public:

		void init(GpioMode mode = GpioMode::In, GpioOutType GpioOutType =
				GpioOutType::PP, GpioSpeed GpioSpeed = GpioSpeed::s50MHz,
				GpioPuPd GpioPuPd = GpioPuPd::NoPull) const {
			for (auto& pinx : IO_Array) {
				pinx.init(mode, GpioOutType, GpioSpeed, GpioPuPd);
			}
		}

		void setValue(uint32_t val) const {
			val &= mask;

			for (uint32_t i = 0; i < sizeof...(Items); i++) {
				bool tmp = val & (1 << i);
				IO_Array.at(i).setState(tmp);
			}
		}

		uint32_t readValue(void) const {
			uint32_t val = 0;

			for (uint32_t i = 0; i < sizeof...(Items); i++) {
				bool tmp = IO_Array.at(i).read();
				if (tmp) {
					val += 1 << i;
				}
			}
			return val;
		}

		inline const Gpio& at(size_t index) {
			return IO_Array.at(index);
		}

		inline const Gpio& getPin(uint8_t pinx) const {
			return IO_Array.at(pinx);
		}

		//operator

		inline Gpio& operator[](size_t index) {
			return IO_Array[index];
		}

		inline const Gpio& operator[](size_t index) const {
			return IO_Array[index];
		}

		inline GpioParallel& operator=(uint32_t val) {
			setValue(val);
			return *this;
		}

		inline GpioParallel& operator+=(uint32_t val) {
			setValue(readValue() + val);
			return *this;
		}

		inline GpioParallel& operator-=(uint32_t val) {
			setValue(readValue() - val);
			return *this;
		}

		inline GpioParallel& operator*=(uint32_t val) {
			setValue(readValue() * val);
			return *this;
		}

		inline GpioParallel& operator/=(uint32_t val) {
			setValue(readValue() / val);
			return *this;
		}

		inline GpioParallel& operator%=(uint32_t val) {
			setValue(readValue() % val);
			return *this;
		}

		inline GpioParallel& operator&=(uint32_t val) {
			setValue(readValue() & val);
			return *this;
		}

		inline GpioParallel& operator|=(uint32_t val) {
			setValue(readValue() | val);
			return *this;
		}

		inline GpioParallel& operator^=(uint32_t val) {
			setValue(readValue() ^ val);
			return *this;
		}

		inline GpioParallel& operator<<=(uint32_t val) {
			setValue(readValue() << val);
			return *this;
		}

		inline GpioParallel& operator>>=(uint32_t val) {
			setValue(readValue() >> val);
			return *this;
		}

		inline bool operator==(uint32_t val) const {
			return (readValue() == val);
		}

		inline bool operator!=(uint32_t val) const {
			return (readValue() != val);
		}

		inline bool operator<=(uint32_t val) const {
			return (readValue() <= val);
		}

		inline bool operator>=(uint32_t val) const {
			return (readValue() >= val);
		}

		inline bool operator<(uint32_t val) const {
			return (readValue() < val);
		}

		inline bool operator>(uint32_t val) const {
			return (readValue() > val);
		}

		private:

		protected:
		std::array<Gpio, sizeof...(Items)> IO_Array;
		const uint32_t mask;

		public:
		GpioParallel(Items ... Gpios)
		: IO_Array( {Gpios...}), mask((1 << (sizeof...(Items) + 1)) - 1) {

		}

		static constexpr uint32_t width = sizeof...(Items);

	};

	/**
	 * @brief : Parallel Gpio Fast, used like Port,
	 * 			40% Faster than GpioParallel in toggle speed (0.75us -> 0.525us),
	 * 			Even smaller than GpioParallel in memory usage, don't know why
	 * @param : Variable Length Gpio Pin list
	 *
	 *
	 */
template<typename ...Items>
class GpioFastParallel {

	public:

		void init(GpioMode mode = GpioMode::In, GpioOutType GpioOutType =
				GpioOutType::PP, GpioSpeed GpioSpeed = GpioSpeed::s50MHz,
				GpioPuPd GpioPuPd = GpioPuPd::NoPull) const {
			for (auto& pinx : IO_Array) {
				pinx.init(mode, GpioOutType, GpioSpeed, GpioPuPd);
			}
		}

		/**
		 *  @brief : Higher Abstract Gpio Mode initialize
		 *  @param[in]: pinMode_Status enum
		 *  @param[in]:	Gpio Speed Setting, 2MHz, 25MHz or 50MHz		# default use Highest Speed
		 */
		void begin(pinMode_Status mode_in = INPUT, GpioSpeed GpioSP =
				GpioSpeed::s50MHz) const {
			for (auto& pinx : IO_Array) {
				pinx.begin(mode_in, GpioSP);
			}
		}

		void setValue(uint32_t val) const {
			val &= mask;

			for (uint32_t i = 0; i < width; i++) {
				bool tmp = val & (1 << i);
				IO_Array[i].setState(tmp);
			}
		}

		uint32_t readValue(void) const {
			uint32_t val = 0;

			for (uint32_t i = 0; i < width; i++) {
				bool tmp = IO_Array[i].read();
				if (tmp) {
					val += 1 << i;
				}
			}
			return val;
		}

		inline const GpioFast& at(size_t index) const {
			return IO_Array[index];
		}

		inline const GpioFast& getPin(uint8_t pinx) const {
			return IO_Array[pinx];
		}

		//operator

		inline GpioFast& operator[](size_t index) const {
			return IO_Array[index];
		}

		inline GpioFastParallel& operator=(uint32_t val) {
			setValue(val);
			return *this;
		}

		inline GpioFastParallel& operator+=(uint32_t val) {
			setValue(readValue() + val);
			return *this;
		}

		inline GpioFastParallel& operator-=(uint32_t val) {
			setValue(readValue() - val);
			return *this;
		}

		inline GpioFastParallel& operator*=(uint32_t val) {
			setValue(readValue() * val);
			return *this;
		}

		inline GpioFastParallel& operator/=(uint32_t val) {
			setValue(readValue() / val);
			return *this;
		}

		inline GpioFastParallel& operator%=(uint32_t val) {
			setValue(readValue() % val);
			return *this;
		}

		inline GpioFastParallel& operator&=(uint32_t val) {
			setValue(readValue() & val);
			return *this;
		}

		inline GpioFastParallel& operator|=(uint32_t val) {
			setValue(readValue() | val);
			return *this;
		}

		inline GpioFastParallel& operator^=(uint32_t val) {
			setValue(readValue() ^ val);
			return *this;
		}

		inline GpioFastParallel& operator<<=(uint32_t val) {
			setValue(readValue() << val);
			return *this;
		}

		inline GpioFastParallel& operator>>=(uint32_t val) {
			setValue(readValue() >> val);
			return *this;
		}

		inline bool operator==(uint32_t val) const {
			return (readValue() == val);
		}

		inline bool operator!=(uint32_t val) const {
			return (readValue() != val);
		}

		inline bool operator<=(uint32_t val) const {
			return (readValue() <= val);
		}

		inline bool operator>=(uint32_t val) const {
			return (readValue() >= val);
		}

		inline bool operator<(uint32_t val) const {
			return (readValue() < val);
		}

		inline bool operator>(uint32_t val) const {
			return (readValue() > val);
		}

	private:

	protected:
		GpioFast IO_Array[sizeof...(Items)];
		const uint32_t mask;

		public:
		// First one is LSB
		GpioFastParallel(Items ... Gpios)
		: IO_Array( {Gpios...}), mask((1 << (sizeof...(Items) + 1)) - 1) {

		}

		static constexpr uint32_t width = sizeof...(Items);
	};

template<typename ...Items>
constexpr GpioFastParallel<Items...> make_GpioFastParallel(Items ... Gpios) {
	return GpioFastParallel<Items...>(Gpios...);
}

template<typename ...Items>
constexpr GpioParallel<Items...> make_GpioParallel(Items ... Gpios) {
	return GpioParallel<Items...>(Gpios...);
}

#endif /* GPIO_GPIO_H_ */
