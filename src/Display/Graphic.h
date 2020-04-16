/**
 * @Name    : 	Graphic.h
 * @Date    : 	2019年11月24日 下午8:04:17
 * @Author  : 	happe
 * @brief   : 	Codes from Adafruit GFX
 * 				Added draw Ellipse function
 */

#ifndef DISPLAY_GRAPHIC_H_
#define DISPLAY_GRAPHIC_H_

#include "main.h"
#include "Print/Print.h"
#include "Print/ColorConv.hpp"
#include "GFX_Font.h"




/// A generic graphics superclass that can handle all sorts of drawing. At a minimum you can subclass and provide drawPixel(). At a maximum you can do a ton of overriding to optimize. Used for any/all Adafruit displays!
class Adafruit_GFX : public Print {

	public:

		struct TextBounds {
			int16_t x, y;
			uint16_t w, h;
		};

		Adafruit_GFX(int16_t w, int16_t h); // Constructor
		~Adafruit_GFX() = default;

		/**
		 * 	here is a ton of virtual function members,
		 * 	override them for get the maximum efficiency in different display!
		 */

		// This MUST be defined by the subclass:
		virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
		/// < Virtual drawPixel() function to draw to the screen/framebuffer/etc,
		/// must be overridden in subclass. @param x X coordinate.
		/// @param y Y coordinate.
		/// @param color 16-bit pixel color.

		// TRANSACTION API / CORE DRAW API
		// These MAY be overridden by the subclass to provide device-specific
		// optimized code.  Otherwise 'generic' versions are used.
		virtual void startWrite(void);
		virtual void writePixel(int16_t x, int16_t y, uint16_t color);
		virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		virtual void endWrite(void);

		// CONTROL API
		// These MAY be overridden by the subclass to provide device-specific
		// optimized code.  Otherwise 'generic' versions are used.
		virtual void setRotation(uint8_t r);
		virtual void invertDisplay(bool i);

		// BASIC DRAW API
		// These MAY be overridden by the subclass to provide device-specific
		// optimized code.  Otherwise 'generic' versions are used.

		// It's good to implement those, even if using transaction API
		virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		virtual void fillScreen(uint16_t color);
		// Optional and probably not necessary to change
		virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

		/**
		 * 	here is base GFX algorithm
		 */

		// These exist only with Adafruit_GFX (no subclass overrides)
		void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
		void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
		void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
		void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,
				uint16_t color);
		void drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color);		// New
		void fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color);		// New

		void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
				uint16_t color);
		void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
				uint16_t color);
		void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
		void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

		/**
		 * 	draw images, Mono, Grayscale or full-color.
		 */


		void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color,
				uint16_t bg);
		void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color,
				uint16_t bg);
		void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
		void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[],
				int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
		/* with image magnification level */
		void drawSRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h, size_t size);
		void drawSRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h, size_t size);


		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);

		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w,
				int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);

		/**
		 * 	draw Text
		 */

		void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
		void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x,
					  uint8_t size_y);
		void setCursor(int16_t x, int16_t y);
		void setTextColor(uint16_t c);
		void setTextColor(uint16_t c, uint16_t bg);
		void setTextSize(uint8_t s);
		void setTextSize(uint8_t s_x, uint8_t s_y);
		void setTextWrap(bool w);
		void cp437(bool x = true);
		void setFont(const GFXfont *f = nullptr);
		void getTextBounds(const char *string, int16_t x, int16_t y,
						   int16_t *x1, int16_t *y1,
						   uint16_t *w, uint16_t *h);

		TextBounds getTextBounds(const char *string, int16_t x, int16_t y) {
			TextBounds tmp;
			getTextBounds(string, x, y, &tmp.x, &tmp.y, &tmp.w, &tmp.h);
			return tmp;
		}

		virtual size_t write(uint8_t) override;		// inherited from Print, and implement

		/**************************************************************************/
		/*!
		 @brief      Get width of the display, accounting for the current rotation
		 @returns    Width in pixels
		 */
		/**************************************************************************/
		inline int16_t width(void) const {
			return _width;
		}

		/**************************************************************************/
		/*!
		 @brief      Get height of the display, accounting for the current rotation
		 @returns    Height in pixels
		 */
		/**************************************************************************/
		inline int16_t height(void) const {
			return _height;
		}

		/**************************************************************************/
		/*!
		 @brief      Get rotation setting for display
		 @returns    0 thru 3 corresponding to 4 cardinal rotations
		 */
		/**************************************************************************/
		inline uint8_t getRotation(void) const {
			return rotation;
		}

		// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
		/**************************************************************************/
		/*!
		 @brief  Get text cursor X location
		 @returns    X coordinate in pixels
		 */
		/**************************************************************************/
		inline int16_t getCursorX(void) const {
			return cursor_x;
		}

		/**************************************************************************/
		/*!
		 @brief      Get text cursor Y location
		 @returns    Y coordinate in pixels
		 */
		/**************************************************************************/
		inline int16_t getCursorY(void) const {
			return cursor_y;
		}

	protected:
		void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx,
						int16_t *maxy);
		const int16_t WIDTH,    ///< This is the 'raw' display width - never changes
				HEIGHT;         ///< This is the 'raw' display height - never changes
		int16_t _width,         ///< Display width as modified by current rotation
				_height,        ///< Display height as modified by current rotation
				cursor_x,       ///< x location to start print()ing text
				cursor_y;       ///< y location to start print()ing text
		uint16_t textcolor,     ///< 16-bit background color for print()
				textbgcolor;    ///< 16-bit text color for print()
		uint8_t textsize_x, 	///< Desired magnification in X-axis of text to print()
				textsize_y;     ///< Desired magnification in Y-axis of text to print()
		uint8_t rotation;       ///< Display rotation (0 thru 3)
		bool wrap,           	///< If set, 'wrap' text at right edge of display
				_cp437;         ///< If set, use correct CP437 charset (default is off)
		GFXfont *gfxFont;       ///< Pointer to special font
	private:
		inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
#ifdef __AVR__
			return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
			// expression in __AVR__ section may generate "dereferencing type-punned
			// pointer will break strict-aliasing rules" warning In fact, on other
			// platforms (such as STM32) there is no need to do this pointer magic as
			// program memory may be read in a usual way So expression may be simplified
			return gfxFont->glyph + c;
#endif //__AVR__
		}

		inline uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont) {
#ifdef __AVR__
			return (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
#else
			// expression in __AVR__ section generates "dereferencing type-punned pointer
			// will break strict-aliasing rules" warning In fact, on other platforms (such
			// as STM32) there is no need to do this pointer magic as program memory may
			// be read in a usual way So expression may be simplified
			return gfxFont->bitmap;
#endif //__AVR__
		}
};

#endif /* DISPLAY_GRAPHIC_H_ */
