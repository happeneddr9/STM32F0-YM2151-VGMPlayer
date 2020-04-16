/*
 * Complex_GFX.h
 *
 *  Created on: 2020年3月24日
 *      Author: happe
 *
 *      It's a Graphics Library for complex graphics
 *      added Quad Bézier curve algorithm 			(二次貝茲曲線)
 *      added Cubic Bézier curve algorithm 			(三次貝茲曲線)
 *      added Rotated Ellipse algorithm 			(旋轉橢圓)
 *
 */

#ifndef DISPLAY_COMPLEX_GFX_H_
#define DISPLAY_COMPLEX_GFX_H_

#include "Graphic.h"
#include "Arduino/Basic.h"

class Complex_GFX {
	public:
		typedef float FPoint;
		typedef float FPoint_convtype;

		Complex_GFX(Adafruit_GFX * display);
		Complex_GFX(Adafruit_GFX& display);

		// Draw a bezier curve of a defined colour between specified points
		void drawBezier(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
						int32_t x2, int32_t y2, uint16_t color);
		void drawBezierSegment(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
							   int32_t x2, int32_t y2, uint16_t color);


		void drawCubicBezierSeg(int32_t x0, int32_t y0, FPoint x1, FPoint y1, FPoint x2, FPoint y2,
				int32_t x3, int32_t y3, uint16_t color);
		void drawCubicBezier(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3,
				int32_t y3, uint16_t color);


		void drawRationalBezierSeg(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2,
				int32_t y2, FPoint w, uint16_t color);
		void drawRationalBezier(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
				FPoint w, uint16_t color);
		void drawEllipseRect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color);
		void drawRotatedEllipse(int32_t x, int32_t y, int32_t a, int32_t b, FPoint angle, uint16_t color);
		void drawRotatedEllipseRect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, long zd, uint16_t color);

	private:

	protected:

		int32_t abs(int32_t x) {
			return std::abs(x);
		}

		long long abs(long long x) {
			return std::abs(x);
		}

		FPoint abs(FPoint x) {
			return std::abs(static_cast<FPoint_convtype>(x));
		}

		FPoint fabs(FPoint x) {
			return abs(x);
		}

		FPoint floor(FPoint x){
			return std::floor(x);
		}

		FPoint ceil(FPoint x) {
			return std::ceil(x);
		}

		FPoint sqrt(FPoint x) {
			return std::sqrt(static_cast<FPoint_convtype>(x));
		}

		FPoint sin(FPoint x) {
			return std::sin(static_cast<FPoint_convtype>(x));
		}

		FPoint cos(FPoint x) {
			return std::cos(static_cast<FPoint_convtype>(x));
		}


		Adafruit_GFX * display;
};



#endif /* DISPLAY_COMPLEX_GFX_H_ */
