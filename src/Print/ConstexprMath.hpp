/**
 * @Name    : ConstexprMath.hpp
 * @Date    : 2019年11月21日 下午10:57:46
 * @Author  : happe
 * @brief   :
 */

#ifndef PRINT_CONSTEXPRMATH_HPP_
#define PRINT_CONSTEXPRMATH_HPP_

#include "main.h"
#include "Basic.h"
#include <cmath>
#include <type_traits>
#include <limits>

namespace Mathexpr {
	template <typename T, size_t len>
	struct ArrayStruct {
			T val[len];
			T operator [](size_t index) const {
				return val[index];
			}
	};

	// 1/4 sine table
	template <typename T, size_t len>
	constexpr ArrayStruct<T, len> Sine_Quarter_generator(int range) {
		ArrayStruct<T, len> tmp { };
		long double angle { };
		constexpr auto T_max = std::numeric_limits<T>::max();
		constexpr auto T_sign = std::is_signed<T>::value;

		if (range > T_max) {
			range = T_max;
		}

		for (size_t i = 0; i < len; i++) {
			angle = i * PI_base<long double> / static_cast<long double>(len * 2.0);
			if (T_sign) {
				tmp.val[i] = (range * std::sin(angle));
			} else {
				tmp.val[i] = ((T_max / 2.0) + (range * __builtin_sin(angle)));
			}
		}
		return tmp;
	}

	// 1/2 sine table
	template <typename T, size_t len>
	constexpr ArrayStruct<T, len> Sine_Half_generator(int range) {
		ArrayStruct<T, len> tmp { };
		long double angle { };
		constexpr auto T_max = std::numeric_limits<T>::max();
		constexpr auto T_sign = std::is_signed<T>::value;

		if (range > T_max) {
			range = T_max;
		}

		for (size_t i = 0; i < len; i++) {
			angle = i * PI_base<long double> / static_cast<long double>(len);
			if (T_sign) {
				tmp.val[i] = (range * std::sin(angle));
			} else {
				tmp.val[i] = ((T_max / 2.0) + (range * std::sin(angle)));
			}
		}
		return tmp;
	}

	// Full sine table
	template <typename T, size_t len>
	constexpr ArrayStruct<T, len> Sine_Full_generator(int range, bool start_form_zero = true) {
		ArrayStruct<T, len> tmp { };
		long double angle = 0;
		constexpr auto T_max = std::numeric_limits<T>::max();
		constexpr auto T_sign = std::is_signed<T>::value;
		int T_minus = start_form_zero ? ((T_max / 2.0) - range) : 0;

		constexpr long double len_base = (len / 2.0L);

		for (size_t i = 0; i < len; i++) {
			angle = i * PI_base<long double> / len_base;
			if (T_sign) {
				tmp.val[i] = (range * std::sin(angle));
			} else {
				tmp.val[i] = ((T_max / 2.0) + (range * std::sin(angle))) - T_minus;
			}
		}
		return tmp;
	}

}

#endif /* PRINT_CONSTEXPRMATH_HPP_ */
