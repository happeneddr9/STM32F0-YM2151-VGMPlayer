// RRE Font support library
// (c) 2019 by Pawel A. Hernik

#ifndef _RRE_FONT_H
#define _RRE_FONT_H 

#include "Arduino/Basic.h"

// -------  CONFIG ---------
// saves 400 bytes
#define CONVERT_PL_CHARS 0
// saves 720 bytes
#define ENABLE_NOSORT    0
// disable unused types to save memory (up to 4100 bytes)
#define ENABLE_RRE_16B   1
#define ENABLE_RRE_24B   1
#define ENABLE_RRE_32B   1
#define ENABLE_RRE_V16B  1
#define ENABLE_RRE_H16B  1
#define ENABLE_RRE_V24B  1
#define ENABLE_RRE_H24B  1
// --------------------------

#define RRE_16B     0    // 16x16 pixels, rects (X4Y4W4H4)
#define RRE_V16B    1    // 64x32 pixels, lines (X6Y5W0H5)
#define RRE_H16B    2    // 32x64 pixels, lines (X5Y6W5H0)
#define RRE_24B     3    // 64x64 pixels, rects (X6Y6W6H6)
#define RRE_V24B    4    // 256x256 pixels, vertical lines (X8Y8W0H8)
#define RRE_H24B    5    // 256x256 pixels, horizontal lines (X8Y8W8H0)
#define RRE_32B     6    // 256x256 pixels, rects (X8Y8W8H8)
#define RRE_NO_SORT 0x80 // old fonts not optimized for fast width calculation

#define ALIGN_LEFT    0
#define ALIGN_RIGHT  -1 
#define ALIGN_CENTER -2

struct RRE_Font {
		uint8_t type;
		uint8_t wd;
		uint8_t ht;
		uint8_t firstCh;
		uint8_t lastCh;
		const uint8_t *rects;
		const uint16_t *offs;
};
// ---------------------------------
class RREFont {
	public:

		typedef void (*FillRectFunPtr)(int16_t x, int16_t y, int16_t w, int16_t h, int16_t c);

		RREFont();
		void init(FillRectFunPtr rectFun, int16_t swd, int16_t sht);
		void setFont(RRE_Font *f);
		void setCR(uint8_t _cr) {
			cr = _cr;
		}
		void setFg(int16_t _fg) {
			fg = _fg;
		}
		void setBg(int16_t _bg) {
			bg = _bg;
		}
		void setColor(int16_t c) {
			fg = bg = c;
		}
		void setColor(int16_t c, int16_t _bg) {
			fg = c;
			bg = _bg;
		}
		void setBold(uint8_t _bold) {
			bold = _bold;
		}
		void setSpacing(uint8_t sp) {
			spacing = sp;
		}
		void setSpacingY(uint8_t sp) {
			spacingY = sp;
		}
		void setScale(uint8_t _sx, uint8_t _sy) {
			sx = _sx;
			sy = _sy;
		}
		void setScale(uint8_t s) {
			sx = sy = s;
		}
		void setFontMinWd(uint8_t wd) {
			minCharWd = wd;
		}
		void setCharMinWd(uint8_t wd) {
			minCharWd = wd;
		}
		void setDigitMinWd(uint8_t wd) {
			minDigitWd = wd;
		}
		unsigned char convertPolish(unsigned char _c);
		int16_t getWidth() {
			return rFont ? rFont->wd : 0;
		}
		int16_t getHeight() {
			return rFont ? rFont->ht : 0;
		}
		int16_t charWidthNoSort(uint8_t c, int16_t *_xmin);
		int16_t charWidthOptim(uint8_t c, int16_t *_xmin = 0);
		int16_t charWidth(uint8_t c, int16_t *_xmin = 0);
		int16_t drawChar(int16_t x, int16_t y, unsigned char c);
		int16_t strWidth(const char *str);
		int16_t printStr(int16_t xpos, int16_t ypos, const char *str);
		void setIsNumberFun(bool (*fun)(uint8_t)) {
			isNumberFun = fun;
		}
		void setFillRectFun(void (*fun)(int16_t x, int16_t y, int16_t w, int16_t h, int16_t c)) {
			fillRectFun = fun;
		}

		static bool isNumber(uint8_t ch);
		static bool isNumberExt(uint8_t ch);

	public:
		bool (*isNumberFun)(uint8_t ch);
		FillRectFunPtr fillRectFun;
		RRE_Font *rFont;
		int16_t xf, yf, wf, hf;
		int16_t scrWd, scrHt;

		int16_t spacing = 1;
		int16_t spacingY = 0;
		int16_t bold = 0;
		int16_t sx = 1;
		int16_t sy = 1;
		int16_t cr = 0;
		int16_t fg = 1;
		int16_t bg = 1;
		uint8_t minCharWd;
		uint8_t minDigitWd;
		uint8_t dualChar;
};
#endif

