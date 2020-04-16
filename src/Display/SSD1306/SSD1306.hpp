/**
 * @Name    : SSD1306.hpp
 * @Date    : 2019年11月24日 下午8:00:22
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_SSD1306_HPP_
#define DISPLAY_SSD1306_HPP_

#include "Arduino/Basic.h"
#include "I2CMaster/HWI2C_IT.h"
#include "Display/Graphic.h"


#define BLACK                          0 ///< Draw 'off' pixels
#define WHITE                          1 ///< Draw 'on' pixels
#define INVERSE                        2 ///< Invert pixels

#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_EXTERNALVCC         0x01 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

template <typename T>
constexpr void ssd1306_swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}


// from Arduino Adafruit_SSD1306
// I2C pointer Replace to I2C reference
class SSD1306 : public Adafruit_GFX {

	public:
		// NEW CONSTRUCTORS -- recommended for new projects
		SSD1306(uint8_t w, uint8_t h, HWWire &twi, int8_t rst_pin = -1);

		~SSD1306(void);

		bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0, bool reset = true,
				bool periphBegin = true);
		size_t self_cost(void) const;
		void display(void);
		void clearDisplay(void);
		void invertDisplay(bool i) override;
		void dim(bool dim);
		void drawPixel(int16_t x, int16_t y, uint16_t color) override;
		virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
		virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
		void startscrollright(uint8_t start, uint8_t stop);
		void startscrollleft(uint8_t start, uint8_t stop);
		void startscrolldiagright(uint8_t start, uint8_t stop);
		void startscrolldiagleft(uint8_t start, uint8_t stop);
		void stopscroll(void);
		void ssd1306_command(uint8_t c);
		bool getPixel(int16_t x, int16_t y);
		uint8_t *getBuffer(void);

	private:
		void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
		void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);
		void ssd1306_command1(uint8_t c);
		void ssd1306_commandList(const uint8_t *c, uint8_t n);

		HWWire &wire;
		uint8_t *buffer;
		int8_t i2caddr, vccstate, page_end, rstPin;
};

/*!
 *	@brief  Constructor for I2C-interfaced SSD1306 displays.
 *	@param  w
 *	Display width in pixels
 *	@param  h
 *	Display height in pixels
 *	@param  twi
 *	Pointer to an existing TwoWire instance (e.g. &Wire, the
 *	microcontroller's primary I2C bus).
 */
SSD1306::SSD1306(uint8_t w, uint8_t h, HWWire &twi, int8_t rst_pin)
		: Adafruit_GFX(w, h), wire(twi), buffer(nullptr), rstPin(rst_pin) {
	i2caddr = 0;
	vccstate = 0;
	page_end = 0;
}

/*!
 *	@brief  Destructor for Adafruit_SSD1306 object.
 */
SSD1306::~SSD1306(void) {
	if (buffer) {
		free(buffer);
		buffer = nullptr;
	}
}

size_t SSD1306::self_cost(void) const{
	return ((WIDTH * HEIGHT) / 8) + sizeof(SSD1306);
}

// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void SSD1306::ssd1306_command1(uint8_t c) {
	wire.beginTransmission(i2caddr);
	wire.write((uint8_t) 0x00); // Co = 0, D/C = 0
	wire.write(c);
	wire.endTransmission();
}

void SSD1306::ssd1306_commandList(const uint8_t *c, uint8_t n) {
	wire.beginTransmission(i2caddr);
	wire.write((uint8_t) 0x00); // Co = 0, D/C = 0
	uint8_t bytesOut = 1;
	while (n--) {
		if (bytesOut >= wire.getWireTX_MAX()) {
			wire.endTransmission();
			wire.beginTransmission(i2caddr);
			wire.write((uint8_t) 0x00); // Co = 0, D/C = 0
			bytesOut = 1;
		}
		wire.write(pgm_read_byte(c++));
		bytesOut++;
	}
	wire.endTransmission();
}

void SSD1306::ssd1306_command(uint8_t c) {
	ssd1306_command1(c);
}

bool SSD1306::begin(uint8_t vcs, uint8_t addr, bool reset, bool periphBegin) {

	if ((!buffer) && !(buffer = (uint8_t *) malloc(WIDTH * ((HEIGHT + 7) / 8))))
		return false;

	clearDisplay();
	/*if (HEIGHT > 32) {
	 drawBitmap((WIDTH - splash1_width) / 2, (HEIGHT - splash1_height) / 2, splash1_data, splash1_width,
	 splash1_height, 1);
	 } else {
	 drawBitmap((WIDTH - splash2_width) / 2, (HEIGHT - splash2_height) / 2, splash2_data, splash2_width,
	 splash2_height, 1);
	 }*/

	vccstate = vcs;

	i2caddr = addr ? addr : ((HEIGHT == 32) ? 0x3C : 0x3D);
	// Init sequence
	static const uint8_t PROGMEM init1[] = {
			SSD1306_DISPLAYOFF,                   // 0xAE
			SSD1306_SETDISPLAYCLOCKDIV,                   // 0xD5
			0x80,                   // the suggested ratio 0x80
			SSD1306_SETMULTIPLEX };               // 0xA8

	ssd1306_commandList(init1, sizeof(init1));
	ssd1306_command1(HEIGHT - 1);

	static const uint8_t PROGMEM init2[] = {
			SSD1306_SETDISPLAYOFFSET,             // 0xD3
			0x0,                                  // no offset
			SSD1306_SETSTARTLINE | 0x0,           // line #0
			SSD1306_CHARGEPUMP };                 // 0x8D

	ssd1306_commandList(init2, sizeof(init2));

	ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

	static const uint8_t PROGMEM init3[] = {
			SSD1306_MEMORYMODE,                   // 0x20
			0x00,                                 // 0x0 act like ks0108
			SSD1306_SEGREMAP | 0x1,
			SSD1306_COMSCANDEC };
	ssd1306_commandList(init3, sizeof(init3));

	if ((WIDTH == 128) && (HEIGHT == 32)) {
		static const uint8_t PROGMEM init4a[] = {
				SSD1306_SETCOMPINS,                 // 0xDA
				0x02,
				SSD1306_SETCONTRAST,                // 0x81
				0x8F };
		ssd1306_commandList(init4a, sizeof(init4a));
	} else if ((WIDTH == 128) && (HEIGHT == 64)) {
		static const uint8_t PROGMEM init4b[] = {
				SSD1306_SETCOMPINS,                 // 0xDA
				0x12,
				SSD1306_SETCONTRAST };              // 0x81
		ssd1306_commandList(init4b, sizeof(init4b));
		ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
	} else if ((WIDTH == 96) && (HEIGHT == 16)) {
		static const uint8_t PROGMEM init4c[] = {
				SSD1306_SETCOMPINS,                 // 0xDA
				0x2,    // ada x12
				SSD1306_SETCONTRAST };              // 0x81
		ssd1306_commandList(init4c, sizeof(init4c));
		ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF);
	} else {
		// Other screen varieties -- TBD
	}

	ssd1306_command1(SSD1306_SETPRECHARGE); // 0xd9
	ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
	static const uint8_t PROGMEM init5[] = {
			SSD1306_SETVCOMDETECT,               // 0xDB
			0x40,
			SSD1306_DISPLAYALLON_RESUME,         // 0xA4
			SSD1306_NORMALDISPLAY,               // 0xA6
			SSD1306_DEACTIVATE_SCROLL,
			SSD1306_DISPLAYON };                 // Main screen turn on
	ssd1306_commandList(init5, sizeof(init5));

	return true; // Success
}

/*!
 @brief  Set/clear/invert a single pixel. This is also invoked by the
 Adafruit_GFX library in generating many higher-level graphics
 primitives.
 @param  x
 Column of display -- 0 at left to (screen width - 1) at right.
 @param  y
 Row of display -- 0 at top to (screen height -1) at bottom.
 @param  color
 Pixel color, one of: BLACK, WHITE or INVERT.
 @return None (void).
 @note   Changes buffer contents only, no immediate effect on display.
 Follow up with a call to display(), or with other graphics
 commands as needed by one's own application.
 */
void SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
		// Pixel is in-bounds. Rotate coordinates if needed.
		switch (getRotation()) {
			case 1:
				ssd1306_swap(x, y);
				x = WIDTH - x - 1;
				break;
			case 2:
				x = WIDTH - x - 1;
				y = HEIGHT - y - 1;
				break;
			case 3:
				ssd1306_swap(x, y);
				y = HEIGHT - y - 1;
				break;
		}
		switch (color) {
			case WHITE:
				buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
				break;
			case BLACK:
				buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
				break;
			case INVERSE:
				buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
				break;
		}
	}
}

// kill them all!!
void SSD1306::clearDisplay(void) {
	memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}

/*!
 @brief  Draw a horizontal line. This is also invoked by the Adafruit_GFX
 library in generating many higher-level graphics primitives.
 @param  x
 Leftmost column -- 0 at left to (screen width - 1) at right.
 @param  y
 Row of display -- 0 at top to (screen height -1) at bottom.
 @param  w
 Width of line, in pixels.
 @param  color
 Line color, one of: BLACK, WHITE or INVERT.
 @return None (void).
 @note   Changes buffer contents only, no immediate effect on display.
 Follow up with a call to display(), or with other graphics
 commands as needed by one's own application.
 */
void SSD1306::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	bool bSwap = false;
	switch (rotation) {
		case 1:
			// 90 degree rotation, swap x & y for rotation, then invert x
			bSwap = true;
			ssd1306_swap(x, y);
			x = WIDTH - x - 1;
			break;
		case 2:
			// 180 degree rotation, invert x and y, then shift y around for height.
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;
			x -= (w - 1);
			break;
		case 3:
			// 270 degree rotation, swap x & y for rotation,
			// then invert y and adjust y for w (not to become h)
			bSwap = true;
			ssd1306_swap(x, y);
			y = HEIGHT - y - 1;
			y -= (w - 1);
			break;
	}

	if (bSwap)
		drawFastVLineInternal(x, y, w, color);
	else
		drawFastHLineInternal(x, y, w, color);
}

void SSD1306::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {

	if ((y >= 0) && (y < HEIGHT)) { // Y coord in bounds?
		if (x < 0) { // Clip left
			w += x;
			x = 0;
		}
		if ((x + w) > WIDTH) { // Clip right
			w = (WIDTH - x);
		}
		if (w > 0) { // Proceed only if width is positive
			uint8_t *pBuf = &buffer[(y / 8) * WIDTH + x], mask = 1 << (y & 7);
			switch (color) {
				case WHITE:
					while (w--) {
						*pBuf++ |= mask;
					}
					;
					break;
				case BLACK:
					mask = ~mask;
					while (w--) {
						*pBuf++ &= mask;
					}
					;
					break;
				case INVERSE:
					while (w--) {
						*pBuf++ ^= mask;
					}
					;
					break;
			}
		}
	}
}

/*!
 @brief  Draw a vertical line. This is also invoked by the Adafruit_GFX
 library in generating many higher-level graphics primitives.
 @param  x
 Column of display -- 0 at left to (screen width -1) at right.
 @param  y
 Topmost row -- 0 at top to (screen height - 1) at bottom.
 @param  h
 Height of line, in pixels.
 @param  color
 Line color, one of: BLACK, WHITE or INVERT.
 @return None (void).
 @note   Changes buffer contents only, no immediate effect on display.
 Follow up with a call to display(), or with other graphics
 commands as needed by one's own application.
 */
void SSD1306::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	bool bSwap = false;
	switch (rotation) {
		case 1:
			// 90 degree rotation, swap x & y for rotation,
			// then invert x and adjust x for h (now to become w)
			bSwap = true;
			ssd1306_swap(x, y);
			x = WIDTH - x - 1;
			x -= (h - 1);
			break;
		case 2:
			// 180 degree rotation, invert x and y, then shift y around for height.
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;
			y -= (h - 1);
			break;
		case 3:
			// 270 degree rotation, swap x & y for rotation, then invert y
			bSwap = true;
			ssd1306_swap(x, y);
			y = HEIGHT - y - 1;
			break;
	}

	if (bSwap)
		drawFastHLineInternal(x, y, h, color);
	else
		drawFastVLineInternal(x, y, h, color);
}

void SSD1306::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {

	if ((x >= 0) && (x < WIDTH)) { // X coord in bounds?
		if (__y < 0) { // Clip top
			__h += __y;
			__y = 0;
		}
		if ((__y + __h) > HEIGHT) { // Clip bottom
			__h = (HEIGHT - __y);
		}
		if (__h > 0) { // Proceed only if height is now positive
			// this display doesn't need ints for coordinates,
			// use local byte registers for faster juggling
			uint8_t y = __y, h = __h;
			uint8_t *pBuf = &buffer[(y / 8) * WIDTH + x];

			// do the first partial byte, if necessary - this requires some masking
			uint8_t mod = (y & 7);
			if (mod) {
				// mask off the high n bits we want to set
				mod = 8 - mod;
				// note - lookup table results in a nearly 10% performance
				// improvement in fill* functions
				// uint8_t mask = ~(0xFF >> mod);
				static const uint8_t PROGMEM premask[8] = {
						0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
				uint8_t mask = pgm_read_byte(&premask[mod]);
				// adjust the mask if we're not going to reach the end of this byte
				if (h < mod)
					mask &= (0XFF >> (mod - h));

				switch (color) {
					case WHITE:
						*pBuf |= mask;
						break;
					case BLACK:
						*pBuf &= ~mask;
						break;
					case INVERSE:
						*pBuf ^= mask;
						break;
				}
				pBuf += WIDTH;
			}

			if (h >= mod) { // More to go?
				h -= mod;
				// Write solid bytes while we can - effectively 8 rows at a time
				if (h >= 8) {
					if (color == INVERSE) {
						// separate copy of the code so we don't impact performance of
						// black/white write version with an extra comparison per loop
						do {
							*pBuf ^= 0xFF;  // Invert byte
							pBuf += WIDTH; // Advance pointer 8 rows
							h -= 8;     // Subtract 8 rows from height
						} while (h >= 8);
					} else {
						// store a local value to work with
						uint8_t val = (color != BLACK) ? 255 : 0;
						do {
							*pBuf = val;    // Set byte
							pBuf += WIDTH;  // Advance pointer 8 rows
							h -= 8;      // Subtract 8 rows from height
						} while (h >= 8);
					}
				}

				if (h) { // Do the final partial byte, if necessary
					mod = h & 7;
					// this time we want to mask the low bits of the byte,
					// vs the high bits we did above
					// uint8_t mask = (1 << mod) - 1;
					// note - lookup table results in a nearly 10% performance
					// improvement in fill* functions
					static const uint8_t PROGMEM postmask[8] = {
							0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
					uint8_t mask = pgm_read_byte(&postmask[mod]);
					switch (color) {
						case WHITE:
							*pBuf |= mask;
							break;
						case BLACK:
							*pBuf &= ~mask;
							break;
						case INVERSE:
							*pBuf ^= mask;
							break;
					}
				}
			}
		} // endif positive height
	} // endif x in bounds
}

/*!
 @brief  Return color of a single pixel in display buffer.
 @param  x
 Column of display -- 0 at left to (screen width - 1) at right.
 @param  y
 Row of display -- 0 at top to (screen height -1) at bottom.
 @return true if pixel is set (usually WHITE, unless display invert mode
 is enabled), false if clear (BLACK).
 @note   Reads from buffer contents; may not reflect current contents of
 screen if display() has not been called.
 */
bool SSD1306::getPixel(int16_t x, int16_t y) {
	if ((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
		// Pixel is in-bounds. Rotate coordinates if needed.
		switch (getRotation()) {
			case 1:
				ssd1306_swap(x, y);
				x = WIDTH - x - 1;
				break;
			case 2:
				x = WIDTH - x - 1;
				y = HEIGHT - y - 1;
				break;
			case 3:
				ssd1306_swap(x, y);
				y = HEIGHT - y - 1;
				break;
		}
		return (buffer[x + (y / 8) * WIDTH] & (1 << (y & 7)));
	}
	return false; // Pixel out of bounds
}

/*!
 @brief  Get base address of display buffer for direct reading or writing.
 @return Pointer to an unsigned 8-bit array, column-major, columns padded
 to full byte boundary if needed.
 */
uint8_t *SSD1306::getBuffer(void) {
	return buffer;
}

// REFRESH DISPLAY ---------------------------------------------------------

/*!
 @brief  Push data currently in RAM to SSD1306 display.
 @return None (void).
 @note   Drawing operations are not visible until this function is
 called. Call after each graphics command, or after a whole set
 of graphics commands, as best needed by one's own application.
 */
void SSD1306::display(void) {
	static const uint8_t dlist1[] = {
			SSD1306_PAGEADDR, 0,                            // Page start address
			0xFF,                         // Page end (not really, but works here)
			SSD1306_COLUMNADDR, 0 };      // Column start address
	ssd1306_commandList(dlist1, sizeof(dlist1));
	ssd1306_command1(WIDTH - 1); // Column end address

	uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
	uint8_t *ptr = buffer;
	wire.beginTransmission(i2caddr);
	wire.write((uint8_t) 0x40);
	uint8_t bytesOut = 1;
	while (count--) {
		if (bytesOut >= wire.getWireTX_MAX() - 1) {
			wire.endTransmission();
			wire.beginTransmission(i2caddr);
			wire.write((uint8_t) 0x40);
			bytesOut = 1;
		}
		wire.write(*ptr++);
		bytesOut++;
	}
	wire.endTransmission();
}

// SCROLLING FUNCTIONS -----------------------------------------------------

/*!
 @brief  Activate a right-handed scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// To scroll the whole display, run: display.startscrollright(0x00, 0x0F)
void SSD1306::startscrollright(uint8_t start, uint8_t stop) {
	static const uint8_t PROGMEM scrollList1a[] = {
			SSD1306_RIGHT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList1a, sizeof(scrollList1a));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static const uint8_t PROGMEM scrollList1b[] = {
			0X00, 0XFF,
			SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList1b, sizeof(scrollList1b));
}

/*!
 @brief  Activate a left-handed scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// To scroll the whole display, run: display.startscrollleft(0x00, 0x0F)
void SSD1306::startscrollleft(uint8_t start, uint8_t stop) {
	static const uint8_t PROGMEM scrollList2a[] = {
			SSD1306_LEFT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList2a, sizeof(scrollList2a));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static const uint8_t PROGMEM scrollList2b[] = {
			0X00, 0XFF,
			SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList2b, sizeof(scrollList2b));
}

/*!
 @brief  Activate a diagonal scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// display.startscrolldiagright(0x00, 0x0F)
void SSD1306::startscrolldiagright(uint8_t start, uint8_t stop) {
	static const uint8_t PROGMEM scrollList3a[] = {
			SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00 };
	ssd1306_commandList(scrollList3a, sizeof(scrollList3a));
	ssd1306_command1(HEIGHT);
	static const uint8_t PROGMEM scrollList3b[] = {
			SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList3b, sizeof(scrollList3b));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static const uint8_t PROGMEM scrollList3c[] = {
			0X01,
			SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList3c, sizeof(scrollList3c));
}

/*!
 @brief  Activate alternate diagonal scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// To scroll the whole display, run: display.startscrolldiagleft(0x00, 0x0F)
void SSD1306::startscrolldiagleft(uint8_t start, uint8_t stop) {
	static const uint8_t PROGMEM scrollList4a[] = {
			SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00 };
	ssd1306_commandList(scrollList4a, sizeof(scrollList4a));
	ssd1306_command1(HEIGHT);
	static const uint8_t PROGMEM scrollList4b[] = {
			SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList4b, sizeof(scrollList4b));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static const uint8_t PROGMEM scrollList4c[] = {
			0X01,
			SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList4c, sizeof(scrollList4c));
}

/*!
 @brief  Cease a previously-begun scrolling action.
 @return None (void).
 */
void SSD1306::stopscroll(void) {
	ssd1306_command1(SSD1306_DEACTIVATE_SCROLL);
}

// OTHER HARDWARE SETTINGS -------------------------------------------------

/*!
 @brief  Enable or disable display invert mode (white-on-black vs
 black-on-white).
 @param  i
 If true, switch to invert mode (black-on-white), else normal
 mode (white-on-black).
 @return None (void).
 @note   This has an immediate effect on the display, no need to call the
 display() function -- buffer contents are not changed, rather a
 different pixel mode of the display hardware is used. When
 enabled, drawing BLACK (value 0) pixels will actually draw white,
 WHITE (value 1) will draw black.
 */
void SSD1306::invertDisplay(bool i) {
	ssd1306_command1(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);

}

/*!
 @brief  Dim the display.
 @param  dim
 true to enable lower brightness mode, false for full brightness.
 @return None (void).
 @note   This has an immediate effect on the display, no need to call the
 display() function -- buffer contents are not changed.
 */
void SSD1306::dim(bool dim) {
	uint8_t contrast;

	if (dim) {
		contrast = 0; // Dimmed display
	} else {
		contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
	}
	// the range of contrast to too small to be really useful
	// it is useful to dim the display
	ssd1306_command1(SSD1306_SETCONTRAST);
	ssd1306_command1(contrast);
}

#endif /* DISPLAY_SSD1306_HPP_ */
