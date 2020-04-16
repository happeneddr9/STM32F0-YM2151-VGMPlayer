/**
 * @Name    : ColorConv.hpp
 * @Date    : 2019年11月24日 下午1:56:59
 * @Author  : happe
 * @brief   : structs, functions, variables which about RGB or HSV Colors.
 */

#ifndef PRINT_COLORCONV_HPP_
#define PRINT_COLORCONV_HPP_

#include "Arduino/Basic.h"
#include <initializer_list>

namespace Pixel {

#define USE_Dim_Curve 0
	// let colors saturation looks more natural and smooth but it'll less some brightness
	const uint8_t dim_curve[] = {
	    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
	    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
	    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
	    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
	    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
	    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
	    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
	    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
	    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
	    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
	    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
	    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
	    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
	    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
	    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
	    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
	};

	struct RGB;
	struct HSV;
	constexpr RGB HSV2RGB(uint8_t inHue, uint8_t inSaturation = 255, uint8_t inValue = 255);
	constexpr RGB HSV2RGB(const HSV& inHSV);

	constexpr HSV RGB2HSV(uint8_t inRed, uint8_t inGreen, uint8_t inBlue);
	constexpr HSV RGB2HSV(const RGB& inRGB);


	struct RGB {
			union {
					struct {
							union {
									uint8_t r;
									uint8_t red;
							};
							union {
									uint8_t g;
									uint8_t green;
							};
							union {
									uint8_t b;
									uint8_t blue;
							};
					};
					uint8_t raw[3];
			};

			constexpr inline RGB()
					: RGB(0) {

			}

			// allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
			constexpr inline RGB(uint32_t colorcode)
					: r((colorcode >> 16) & 0xFF),
					  g((colorcode >> 8) & 0xFF),
					  b((colorcode >> 0) & 0xFF) {

			}

			constexpr inline RGB(uint8_t Red, uint8_t Green, uint8_t Blue)
				: r(Red), g(Green), b(Blue) {

			}

			/* copy constructor */
			constexpr inline RGB(const RGB& rhs)
				: r(rhs.r), g(rhs.g), b(rhs.b) {

			}

			constexpr inline RGB(const HSV& rhs)
					: RGB(HSV2RGB(rhs)) {

			}

			constexpr inline RGB& operator = (const RGB& rhs) {
				r = rhs.r;
				g = rhs.g;
				b = rhs.b;
				return *this;
			}

			constexpr inline RGB& operator =(const HSV& rhs) {
				*this = HSV2RGB(rhs);
				return *this;
			}

			// allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
			constexpr inline RGB& operator =(uint32_t colorcode) {
				r = (colorcode >> 16) & 0xFF;
				g = (colorcode >> 8) & 0xFF;
				b = (colorcode >> 0) & 0xFF;
				return *this;
			}

			inline uint8_t& operator [](size_t n) {
				return raw[n];
			}

			constexpr inline const uint8_t& operator [](size_t n) const {
				return raw[n];
			}

			inline RGB& setRGB(uint8_t Red, uint8_t Green, uint8_t Blue) {
				r = Red;
				g = Green;
				b = Blue;
				return *this;
			}

			inline RGB& setHSV(uint8_t ih, uint8_t is, uint8_t iv) {
				*this = HSV2RGB(ih, is, iv);
				return *this;
			}

			inline RGB& setHue(uint8_t hue) {
				*this = HSV2RGB(hue);
				return *this;
			}

			constexpr operator uint16_t() const {					// 在可回傳 uint16_t 的情況下自動轉換
				uint16_t color_r = (r & 0xF8) << 8;					// format:  	BBBBBGGG GGGRRRRR	(ST7735)
				uint16_t color_g = (g & 0xFC) << 3;					// default:		RRRRRGGG GGGBBBBB	(ST7789)
				uint16_t color_b = (b & 0xF8) >> 3;
				return (color_b | color_g | color_r);
			}

			constexpr RGB operator !() const {					// RGB <-> BGR convert
				return RGB{b, g, r};
			}

			/* RGB value calculations */

			inline RGB& operator +=(const RGB& rhs) {
				r += rhs.r;
				g += rhs.g;
				b += rhs.b;
				return *this;
			}

			inline RGB& operator -=(const RGB& rhs) {
				r -= rhs.r;
				g -= rhs.g;
				b -= rhs.b;
				return *this;
			}

			/// divide each of the channels by a constant
			inline RGB& operator /=(uint8_t d) {
				r /= d;
				g /= d;
				b /= d;
				return *this;
			}

			/// right shift each of the channels by a constant
			inline RGB& operator>>=(uint8_t d) {
				r >>= d;
				g >>= d;
				b >>= d;
				return *this;
			}

			/// multiply each of the channels by a constant,
			/// saturating each channel at 0xFF
			inline RGB& operator*=(uint8_t d) {
				r *= d;
				g *= d;
				b *= d;
				return *this;
			}

		    /// subtract a constant of '1' from each channel, saturating at 0x00
			inline RGB& operator--() {
				*this -= 1;
				return *this;
			}

			/// subtract a constant of '1' from each channel, saturating at 0x00
			inline RGB operator--(int) {
				RGB retval(*this);
				--(*this);
				return retval;
			}

			/// add a constant of '1' from each channel, saturating at 0xFF
			inline RGB& operator++() {
				*this += 1;
				return *this;
			}

			/// add a constant of '1' from each channel, saturating at 0xFF
			inline RGB operator++(int) {
				RGB retval(*this);
				++(*this);
				return retval;
			}

			/// "or" operator brings each channel up to the higher of the two values
			inline RGB& operator|=(const RGB& rhs) {
				if (rhs.r > r)
					r = rhs.r;
				if (rhs.g > g)
					g = rhs.g;
				if (rhs.b > b)
					b = rhs.b;
				return *this;
			}

			/// "or" operator brings each channel up to the higher of the two values
			inline RGB& operator|=(uint8_t d) {
				if (d > r)
					r = d;
				if (d > g)
					g = d;
				if (d > b)
					b = d;
				return *this;
			}

			/// "and" operator brings each channel down to the lower of the two values
			inline RGB& operator&=(const RGB& rhs) {
				if (rhs.r < r)
					r = rhs.r;
				if (rhs.g < g)
					g = rhs.g;
				if (rhs.b < b)
					b = rhs.b;
				return *this;
			}

			/// "and" operator brings each channel down to the lower of the two values
			inline RGB& operator&=(uint8_t d) {
				if (d < r)
					r = d;
				if (d < g)
					g = d;
				if (d < b)
					b = d;
				return *this;
			}


			/* RGB logic calculations */

			inline bool operator==(const RGB& rhs) {
				return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
			}

			inline bool operator!=(const RGB& rhs) {
				return !(*this == rhs);
			}

			// is it is a black
			explicit inline operator bool() const {
				return (r || g || b);
			}

			// RGB color invert
			inline RGB operator-() {
				RGB retval;
				retval.r = 255 - r;
				retval.g = 255 - g;
				retval.b = 255 - b;
				return retval;
			}



	};

	struct HSV {
			union {
					struct {
							union {
									uint8_t hue;
									uint8_t h;
							};
							union {
									uint8_t saturation;
									uint8_t sat;
									uint8_t s;
							};
							union {
									uint8_t value;
									uint8_t val;
									uint8_t v;
							};
					};
					uint8_t raw[3];
			};

			constexpr inline HSV()
					: HSV(0, 0, 0) {

			}


			constexpr inline HSV(uint8_t hue, uint8_t sat, uint8_t val)
					: h(hue), s(sat), v(val) {

			}

			constexpr inline HSV(const HSV& rhs)
					: h(rhs.hue), s(rhs.sat), v(rhs.val) {

			}

			constexpr inline HSV& operator =(const HSV& rhs) {
				h = rhs.h;
				s = rhs.s;
				v = rhs.v;
				return *this;
			}

			inline uint8_t& operator [](size_t n) {
				return raw[n];
			}

			constexpr inline const uint8_t& operator [](size_t n) const {
				return raw[n];
			}

			inline HSV& setHSV(uint8_t ih, uint8_t is, uint8_t iv) {
				h = ih;
				s = is;
				v = iv;
				return *this;
			}

	};

	constexpr uint16_t RGB565(uint8_t red, uint8_t green, uint8_t blue) {
		return RGB { red, green, blue };		// using automatic uint16_t convert member
	}

	constexpr uint16_t RGB565(const RGB& rgb) {
		return rgb;
	}

	constexpr uint32_t RGB16to24(uint16_t val) {
		uint8_t r = ((((val >> 11) & 0x1F) * 527) + 23) >> 6;
		uint8_t g = ((((val >> 5) & 0x3F) * 259) + 33) >> 6;
		uint8_t b = (((val & 0x1F) * 527) + 23) >> 6;
		return (r << 16 | g << 8 | b);
	}

	constexpr RGB HSV2RGB(uint8_t inHue, uint8_t inSaturation, uint8_t inValue) {

		RGB RGBPixel = { 0, 0, 0 };

		if (inSaturation == 0) {
			// achromatic (grey)
			RGBPixel.r = RGBPixel.g = RGBPixel.b = inValue;
		} else {

#if USE_Dim_Curve
			inValue = dim_curve[inValue];
			inSaturation = 255 - dim_curve[255 - inSaturation];
#endif

			uint16_t scaledHue = inHue * 6;							// 0 ... 1530
			uint8_t sector = scaledHue >> 8; 						// 0 ... 5 (5.9765625)
			uint8_t offsetInSector = scaledHue - (sector << 8); 	// 0 ... 250 	position within the sector
			uint8_t p = (inValue * (255 - inSaturation)) >> 8;  	//				V(1 - S)
			uint8_t q = (inValue * (255 - ((inSaturation * offsetInSector) >> 8))) >> 8; 			//下降斜率
			uint8_t t = (inValue * (255 - ((inSaturation * (255 - offsetInSector)) >> 8))) >> 8; 	//上升斜率

			switch (sector) {
				case 0:
					RGBPixel.r = inValue;
					RGBPixel.g = t;
					RGBPixel.b = p;
					break;
				case 1:
					RGBPixel.r = q;
					RGBPixel.g = inValue;
					RGBPixel.b = p;
					break;
				case 2:
					RGBPixel.r = p;
					RGBPixel.g = inValue;
					RGBPixel.b = t;
					break;
				case 3:
					RGBPixel.r = p;
					RGBPixel.g = q;
					RGBPixel.b = inValue;
					break;
				case 4:
					RGBPixel.r = t;
					RGBPixel.g = p;
					RGBPixel.b = inValue;
					break;
				default:    // case 5:
					RGBPixel.r = inValue;
					RGBPixel.g = p;
					RGBPixel.b = q;
					break;
			}

		}
		return RGBPixel;
	}

	constexpr RGB HSV2RGB(const HSV& inHSV) {
		return HSV2RGB(inHSV.h, inHSV.s, inHSV.v);
	}

	/**
	 * @brief:	convert RGB color range to HSV color range
	 *
	 */

	constexpr HSV RGB2HSV(uint8_t inRed, uint8_t inGreen, uint8_t inBlue){
		HSV HSVPixel = { 0, 0, 0 };

		uint8_t maximum = max(inRed, inGreen, inBlue);
		uint8_t minimum = min(inRed, inGreen, inBlue);

		uint8_t delta = maximum - minimum;

		HSVPixel.v = maximum;

		if (maximum != 0)
			HSVPixel.s = static_cast<uint16_t>(delta) * 255 / maximum;	// s, 0..255
		else {
			// r = g = b = 0		// s = 0, v is undefined
			HSVPixel.s = 0;
			HSVPixel.h = 0;
			return HSVPixel;
		}

		 if (inRed == maximum)
			HSVPixel.h = 0 + ((inGreen - inBlue) * 43 / delta);			// between yellow & magenta
		else if (inGreen == maximum)
			HSVPixel.h = 85 + ((inBlue - inRed) * 43 / delta);			// between cyan & yellow
		else
			HSVPixel.h = 170 + ((inRed - inGreen) * 43 / delta);		// between magenta & cyan

		return HSVPixel;
	}

	constexpr HSV RGB2HSV(const RGB& inRGB){
		return RGB2HSV(inRGB.r, inRGB.g, inRGB.b);
	}

	namespace RGB_888{
		//main color zone
		constexpr const RGB Maroon			= { 128,   0,   0 };
		constexpr const RGB Dark_Green		= { 0,   128,   0 };
		constexpr const RGB Navy			= { 0,     0, 128 };

		constexpr const RGB Olive			= { 128, 128,   0 };
		constexpr const RGB Dark_Cyan		= { 0,   128, 128 };
		constexpr const RGB Purple			= { 128,   0, 128 };

		constexpr const RGB Red				= { 255,   0,   0 };
		constexpr const RGB Green			= { 0,   255,   0 };
		constexpr const RGB Blue			= { 0,     0, 255 };

		constexpr const RGB Yellow			= { 255, 255,   0 };
		constexpr const RGB Cyan			= { 0,   255, 255 };
		constexpr const RGB Magenta			= { 255,   0, 255 };

		constexpr const RGB Black			= {   0,   0,   0 };
		constexpr const RGB Dark_Grey		= {  92,  92,  92 };
		constexpr const RGB Grey			= { 128, 128, 128 };
		constexpr const RGB Light_Grey		= { 192, 192, 192 };
		constexpr const RGB White			= { 255, 255, 255 };

		constexpr const RGB Orange			= { 255, 128,   0 };
		constexpr const RGB GreenYellow		= { 180, 255,   0 };
		constexpr const RGB Pink			= { 255, 120, 255 };
	}


	// RGB Type Added auto Convert operator, so didn't needed "RGB_888" namespace
	using namespace RGB_888;
}

inline bool operator==(const Pixel::RGB& lhs, const Pixel::RGB& rhs) {
	return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}

inline bool operator!=(const Pixel::RGB& lhs, const Pixel::RGB& rhs) {
	return !(lhs == rhs);
}

inline bool operator<(const Pixel::RGB& lhs, const Pixel::RGB& rhs) {
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl < sr;
}

inline bool operator>(const Pixel::RGB& lhs, const Pixel::RGB& rhs) {
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl > sr;
}

inline bool operator>=(const Pixel::RGB& lhs, const Pixel::RGB& rhs) {
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl >= sr;
}

inline bool operator<=(const Pixel::RGB& lhs, const Pixel::RGB& rhs) {
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl <= sr;
}

inline Pixel::RGB operator+(const Pixel::RGB& p1, const Pixel::RGB& p2) {
	return Pixel::RGB(p1.r + p2.r, p1.g + p2.g, p1.b + p2.b);
}

inline Pixel::RGB operator+(const Pixel::RGB& p1, int d) {
	return Pixel::RGB(p1.r + d, p1.g + d, p1.b + d);
}

inline Pixel::RGB operator-(const Pixel::RGB& p1, const Pixel::RGB& p2) {
	return Pixel::RGB(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b);
}

inline Pixel::RGB operator-(const Pixel::RGB& p1, int d) {
	return Pixel::RGB(p1.r - d, p1.g - d, p1.b - d);
}

inline Pixel::RGB operator*(const Pixel::RGB& p1, int d) {
	return Pixel::RGB(p1.r * d, p1.g * d, p1.b * d);
}

inline Pixel::RGB operator/(const Pixel::RGB& p1, int d) {
	return Pixel::RGB(p1.r / d, p1.g / d, p1.b / d);
}

inline Pixel::RGB operator&(const Pixel::RGB& p1, const Pixel::RGB& p2) {
	return Pixel::RGB(p1.r < p2.r ? p1.r : p2.r, p1.g < p2.g ? p1.g : p2.g,
			p1.b < p2.b ? p1.b : p2.b);
}

inline Pixel::RGB operator|(const Pixel::RGB& p1, const Pixel::RGB& p2) {
	return Pixel::RGB(p1.r > p2.r ? p1.r : p2.r, p1.g > p2.g ? p1.g : p2.g,
			p1.b > p2.b ? p1.b : p2.b);
}

#endif /* PRINT_COLORCONV_HPP_ */
