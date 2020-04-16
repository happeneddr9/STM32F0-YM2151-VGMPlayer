/**
 * @Name    : GpioPin.h
 * @Date    : 2019年12月20日 下午9:06:02
 * @Author  : happe
 * @brief   : GpioPin and GpioPort struct implements in here, also define all the pins name.
 *
 * @log		:
 * 		(update)	2020/04/16
 * 			Define all GpioPin with macro package, replace the old way.
 */

#ifndef GPIO_GPIOPIN_H_
#define GPIO_GPIOPIN_H_

#include "main.h"
#include "Peripheral/Peripheral.h"
#include "Print/Print.h"

enum GPIO_Status : bool {
	LOW = false,
	HIGH = true
};

enum class Port : uint8_t {
	A = 0,
	B = 1,
	C = 2,
	D = 3,
	E = 4,
	F = 5,
	G = 6,
	H = 7,
	I = 8,
	X = 255
};

struct GpioPin {
	public:
		static constexpr uint8_t last_port = static_cast<uint8_t>(Port::F);


		Port port;
		uint8_t pin;

		/*constexpr GpioPin(Port po = Port::X, uint8_t pi = 255) :
			port(po), pin(pi) {

		}*/

		constexpr GpioPin(const GpioPin&) = default;
		constexpr GpioPin(GpioPin&&) = default;
		constexpr GpioPin& operator=(const GpioPin&) = default;
		constexpr GpioPin& operator=(GpioPin&&) = default;

		constexpr uint16_t getID(void) const {
			return ((static_cast<uint16_t>(port) * 16) + pin);
		}

		constexpr bool operator==(const GpioPin& n) const {
			return (port == n.port) && (pin == n.pin);
		}

};

/*
 *  : public Printable
		virtual size_t printTo(Print& p) const override {
			size_t len = 0;
			if (pin > 15 || static_cast<uint8_t>(port) > last_port)
				len += p.write("NoPin");
			else {
				len += p.write('P');
				len += p.write(static_cast<char>('A' + static_cast<uint8_t>(port)));
				len += p.print(pin);
			}
			return len;
		}
 *
 */

struct GpioPort {
		Port port;
		constexpr GpioPin operator[](uint8_t N) const {
			return GpioPin{port, N};
		}
};


#define __make_GpioPin_package(portx) \
constexpr const GpioPin P##portx##0 = { Port::portx, 0 };\
constexpr const GpioPin P##portx##1 = { Port::portx, 1 };\
constexpr const GpioPin P##portx##2 = { Port::portx, 2 };\
constexpr const GpioPin P##portx##3 = { Port::portx, 3 };\
constexpr const GpioPin P##portx##4 = { Port::portx, 4 };\
constexpr const GpioPin P##portx##5 = { Port::portx, 5 };\
constexpr const GpioPin P##portx##6 = { Port::portx, 6 };\
constexpr const GpioPin P##portx##7 = { Port::portx, 7 };\
\
constexpr const GpioPin P##portx##8 = { Port::portx, 8 };\
constexpr const GpioPin P##portx##9 = { Port::portx, 9 };\
constexpr const GpioPin P##portx##10 = { Port::portx, 10 };\
constexpr const GpioPin P##portx##11 = { Port::portx, 11 };\
constexpr const GpioPin P##portx##12 = { Port::portx, 12 };\
constexpr const GpioPin P##portx##13 = { Port::portx, 13 };\
constexpr const GpioPin P##portx##14 = { Port::portx, 14 };\
constexpr const GpioPin P##portx##15 = { Port::portx, 15 };

// All the Gpio
/* smarter way */
__make_GpioPin_package(A)
__make_GpioPin_package(B)
__make_GpioPin_package(C)
__make_GpioPin_package(D)
__make_GpioPin_package(E)
__make_GpioPin_package(F)


/* The original(stupid) way, it used a lot of lines */
/*
// GPIOA
constexpr const auto PA0 = GpioPin { Port::A, 0 };
constexpr const auto PA1 = GpioPin { Port::A, 1 };
constexpr const auto PA2 = GpioPin { Port::A, 2 };
constexpr const auto PA3 = GpioPin { Port::A, 3 };
constexpr const auto PA4 = GpioPin { Port::A, 4 };
constexpr const auto PA5 = GpioPin { Port::A, 5 };
constexpr const auto PA6 = GpioPin { Port::A, 6 };
constexpr const auto PA7 = GpioPin { Port::A, 7 };

constexpr const auto PA8 = GpioPin { Port::A, 8 };
constexpr const auto PA9 = GpioPin { Port::A, 9 };
constexpr const auto PA10 = GpioPin { Port::A, 10 };
constexpr const auto PA11 = GpioPin { Port::A, 11 };
constexpr const auto PA12 = GpioPin { Port::A, 12 };
constexpr const auto PA13 = GpioPin { Port::A, 13 };
constexpr const auto PA14 = GpioPin { Port::A, 14 };
constexpr const auto PA15 = GpioPin { Port::A, 15 };

// GPIOB
constexpr const auto PB0 = GpioPin { Port::B, 0 };
constexpr const auto PB1 = GpioPin { Port::B, 1 };
constexpr const auto PB2 = GpioPin { Port::B, 2 };
constexpr const auto PB3 = GpioPin { Port::B, 3 };
constexpr const auto PB4 = GpioPin { Port::B, 4 };
constexpr const auto PB5 = GpioPin { Port::B, 5 };
constexpr const auto PB6 = GpioPin { Port::B, 6 };
constexpr const auto PB7 = GpioPin { Port::B, 7 };

constexpr const auto PB8 = GpioPin { Port::B, 8 };
constexpr const auto PB9 = GpioPin { Port::B, 9 };
constexpr const auto PB10 = GpioPin { Port::B, 10 };
constexpr const auto PB11 = GpioPin { Port::B, 11 };
constexpr const auto PB12 = GpioPin { Port::B, 12 };
constexpr const auto PB13 = GpioPin { Port::B, 13 };
constexpr const auto PB14 = GpioPin { Port::B, 14 };
constexpr const auto PB15 = GpioPin { Port::B, 15 };

// GPIOC
constexpr const auto PC0 = GpioPin { Port::C, 0 };
constexpr const auto PC1 = GpioPin { Port::C, 1 };
constexpr const auto PC2 = GpioPin { Port::C, 2 };
constexpr const auto PC3 = GpioPin { Port::C, 3 };
constexpr const auto PC4 = GpioPin { Port::C, 4 };
constexpr const auto PC5 = GpioPin { Port::C, 5 };
constexpr const auto PC6 = GpioPin { Port::C, 6 };
constexpr const auto PC7 = GpioPin { Port::C, 7 };

constexpr const auto PC8 = GpioPin { Port::C, 8 };
constexpr const auto PC9 = GpioPin { Port::C, 9 };
constexpr const auto PC10 = GpioPin { Port::C, 10 };
constexpr const auto PC11 = GpioPin { Port::C, 11 };
constexpr const auto PC12 = GpioPin { Port::C, 12 };
constexpr const auto PC13 = GpioPin { Port::C, 13 };
constexpr const auto PC14 = GpioPin { Port::C, 14 };
constexpr const auto PC15 = GpioPin { Port::C, 15 };

// GPIOD
constexpr const auto PD0 = GpioPin { Port::D, 0 };
constexpr const auto PD1 = GpioPin { Port::D, 1 };
constexpr const auto PD2 = GpioPin { Port::D, 2 };
constexpr const auto PD3 = GpioPin { Port::D, 3 };
constexpr const auto PD4 = GpioPin { Port::D, 4 };
constexpr const auto PD5 = GpioPin { Port::D, 5 };
constexpr const auto PD6 = GpioPin { Port::D, 6 };
constexpr const auto PD7 = GpioPin { Port::D, 7 };

constexpr const auto PD8 = GpioPin { Port::D, 8 };
constexpr const auto PD9 = GpioPin { Port::D, 9 };
constexpr const auto PD10 = GpioPin { Port::D, 10 };
constexpr const auto PD11 = GpioPin { Port::D, 11 };
constexpr const auto PD12 = GpioPin { Port::D, 12 };
constexpr const auto PD13 = GpioPin { Port::D, 13 };
constexpr const auto PD14 = GpioPin { Port::D, 14 };
constexpr const auto PD15 = GpioPin { Port::D, 15 };

// GPIOE
constexpr const auto PE0 = GpioPin { Port::E, 0 };
constexpr const auto PE1 = GpioPin { Port::E, 1 };
constexpr const auto PE2 = GpioPin { Port::E, 2 };
constexpr const auto PE3 = GpioPin { Port::E, 3 };
constexpr const auto PE4 = GpioPin { Port::E, 4 };
constexpr const auto PE5 = GpioPin { Port::E, 5 };
constexpr const auto PE6 = GpioPin { Port::E, 6 };
constexpr const auto PE7 = GpioPin { Port::E, 7 };

constexpr const auto PE8 = GpioPin { Port::E, 8 };
constexpr const auto PE9 = GpioPin { Port::E, 9 };
constexpr const auto PE10 = GpioPin { Port::E, 10 };
constexpr const auto PE11 = GpioPin { Port::E, 11 };
constexpr const auto PE12 = GpioPin { Port::E, 12 };
constexpr const auto PE13 = GpioPin { Port::E, 13 };
constexpr const auto PE14 = GpioPin { Port::E, 14 };
constexpr const auto PE15 = GpioPin { Port::E, 15 };

// GPIOF
constexpr const auto PF0 = GpioPin { Port::F, 0 };
constexpr const auto PF1 = GpioPin { Port::F, 1 };
constexpr const auto PF2 = GpioPin { Port::F, 2 };
constexpr const auto PF3 = GpioPin { Port::F, 3 };
constexpr const auto PF4 = GpioPin { Port::F, 4 };
constexpr const auto PF5 = GpioPin { Port::F, 5 };
constexpr const auto PF6 = GpioPin { Port::F, 6 };
constexpr const auto PF7 = GpioPin { Port::F, 7 };

constexpr const auto PF8 = GpioPin { Port::F, 8 };
constexpr const auto PF9 = GpioPin { Port::F, 9 };
constexpr const auto PF10 = GpioPin { Port::F, 10 };
constexpr const auto PF11 = GpioPin { Port::F, 11 };
constexpr const auto PF12 = GpioPin { Port::F, 12 };
constexpr const auto PF13 = GpioPin { Port::F, 13 };
constexpr const auto PF14 = GpioPin { Port::F, 14 };
constexpr const auto PF15 = GpioPin { Port::F, 15 };
*/
/* Not a Port */
constexpr auto NoPin = GpioPin { Port::X, 255 };

constexpr const GpioPort PA = { Port::A };
constexpr const GpioPort PB = { Port::B };
constexpr const GpioPort PC = { Port::C };
constexpr const GpioPort PD = { Port::D };
constexpr const GpioPort PE = { Port::E };
constexpr const GpioPort PF = { Port::F };

constexpr uint16_t getPinID(GpioPin Pinx) {
	uint8_t Port_ID = static_cast<uint8_t>(Pinx.port);
	if (Port_ID < 16 && Pinx.pin < 16) {
		return ((static_cast<uint16_t>(Pinx.port) * 16) + Pinx.pin);
	} else {
		return -1;	// 65535
	}
}

#ifdef __make_GpioPin_package
// undefine it because I don't wanna see that in other files.
#undef __make_GpioPin_package
#endif

#endif /* GPIO_GPIOPIN_H_ */
