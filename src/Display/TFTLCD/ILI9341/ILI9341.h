/**
 * @Name    : 	ILI9341.h
 * @Date    : 	2019年12月28日 下午1:06:02
 * @Author  : 	happe
 * @brief   :	ILI9341 Library
 * 				Codes from Adafruit ILI9341 and TFT_eSPI
 * 				using SPI with DMA
 * 				Fast in Transmitting, but you still wait for each command transmitted done
 * 				wait for DMA Transmit Complete Interrupt, still better than interrupt every bytes (SPI TXE)
 */

#ifndef DISPLAY_TFTLCD_ILI9341_ILI9341_H_
#define DISPLAY_TFTLCD_ILI9341_ILI9341_H_

/**
 * @Pin Defines:
 * 	1.8'	128x160
 * 		GND		GND
 * 		VCC		VCC
 * 		SCL		SCK
 * 		SDA		MOSI
 * 		RES		RESET
 * 		DC		Data/Command
 * 		CS		Chip Select
 * 		BL		Backlight
 * 	2.2'	240x320
 * 		VCC		VCC
 * 		GND		GND
 * 		CS		Chip Select
 * 		RESET	RESET
 * 		DC		Data/Command
 * 		SDI		MOSI
 * 		SCK		SCK
 * 		LED		Backlight
 * 		SDO		MISO
 */

#include "Arduino/Basic.h"
#include "SPI/SPI.hpp"
#include "Display/Graphic.h"
#include "Commands.h"
#include "Print/ColorConv.hpp"

namespace ili9341 {
    // #define DC_C digitalWrite(TFT_DC, LOW)
    // #define DC_D digitalWrite(TFT_DC, HIGH)

	class ILI9341 : public Adafruit_GFX {
		public:
			ILI9341(Spi_dma& Spix, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin, int16_t W, int16_t H);
			~ILI9341() = default;
			void init(uint8_t tc = 0);
			void begin(uint8_t tc = 0); // Same - begin included for backwards compatibility


			void writePixels(uint16_t *colors, uint32_t len, bool block = true);
			void writeColor(uint16_t color, uint32_t len);

			// override member
			void writePixel(int16_t x, int16_t y, uint16_t color) override;
			void drawPixel(int16_t x, int16_t y, uint16_t color) override;
			void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
			void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
			void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
			void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
			void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
			void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
			void setRotation(uint8_t r) override;
			void invertDisplay(bool i) override;
			// Compatibility additions (non-essential)
			void startWrite(void) override;                     // Begin SPI transaction (not normally needed)
			void endWrite(void) override;                       // End SPI transaction

			using Adafruit_GFX::drawRGBBitmap; // Check base class first
			void drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);

			inline void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

			// The TFT_eSprite class inherits the following functions
			void setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);
			void pushColor(uint16_t color);
			void pushColor(uint16_t color, uint32_t len);
			void pushColors(uint16_t *data, uint32_t len, bool swap = true); // With byte swap option
			void pushColors(uint8_t *data, uint32_t len);

			void spiwrite(uint8_t n);
			void writecommand(uint8_t c);
			void writedata(uint8_t d);

			uint8_t readcommand8(uint8_t cmd_function, uint8_t index = 0);
			uint16_t readcommand16(uint8_t cmd_function, uint8_t index = 0);
			uint32_t readcommand32(uint8_t cmd_function, uint8_t index = 0);

			// Read the colour of a pixel at x,y and return value in 565 format
			uint16_t readPixel(int32_t x0, int32_t y0);

			// The next functions can be used as a pair to copy screen blocks (or horizontal/vertical lines) to another location
			// Read a block of pixels to a data buffer, buffer is 16 bit and the array size must be at least w * h
			void readRect(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data);
			// Write a block of pixels to the screen
			void pushRect(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data);

			// These are used to render images or sprites stored in RAM arrays
			void pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data);
			void pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data,
					uint16_t transparent);

			// These are used to render images stored in FLASH (PROGMEM)
			void pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, const uint16_t *data,
					uint16_t transparent);
			void pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, const uint16_t *data);

			// These are used by pushSprite for 1 and 8 bit colours
			void pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint8_t *data, bool bpp8 = true);
			void pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint8_t *data, uint8_t transparent,
					bool bpp8 = true);

			// Swap the byte order for pushImage() - corrects endianness
			void setSwapBytes(bool swap);
			bool getSwapBytes(void);

			// This next function has been used successfully to dump the TFT screen to a PC for documentation purposes
			// It reads a screen area and returns the RGB 8 bit colour values of each pixel
			// Set w and h to 1 to read 1 pixel's colour. The data buffer must be at least w * h * 3 bytes
			void readRectRGB(int32_t x0, int32_t y0, int32_t w, int32_t h, uint8_t *data);

			uint8_t getTextDatum(void);
			uint8_t color16to8(uint16_t color565); // Convert 16 bit colour to 8 bits

			void setPivot(int16_t x, int16_t y);
			int16_t getPivotX(void);
			int16_t getPivotY(void);

			uint16_t fontsLoaded(void);

			int16_t textWidth(const char *string, uint8_t font);
			int16_t textWidth(const char *string);
			int16_t fontHeight(int16_t font);
			int16_t fontHeight(void);

			void setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);

			uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining);
			uint16_t decodeUTF8(uint8_t c);
			//size_t write(uint8_t) override;
		private:
			int32_t _init_width, _init_height; // Display w/h as input, used by setRotation()
			uint32_t bitmap_fg, bitmap_bg;

			uint8_t textfont,  // Current selected font
					textdatum; // Text reference datum
			int16_t _xpivot;   // x pivot point coordinate
			int16_t _ypivot;   // x pivot point coordinate


			uint8_t decoderState = 0;   // UTF8 decoder state
			uint16_t decoderBuffer;      // Unicode code-point buffer

			inline void spi_begin() __attribute__((always_inline));
			inline void spi_end() __attribute__((always_inline));

			inline void spi_begin_read() __attribute__((always_inline));
			inline void spi_end_read() __attribute__((always_inline));

			void readAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);

		protected:
			int32_t addr_row, addr_col;

			uint32_t fontsloaded;

			Spi_dma& mSpi;
			Gpio TFT_DC;
			Gpio TFT_RST;

			uint8_t invertOnCommand = 0; ///< Command to enable invert mode
			uint8_t invertOffCommand = 0; ///< Command to disable invert mode
	};

}

using ili9341::ILI9341;

#endif /* DISPLAY_TFTLCD_ILI9341_ILI9341_H_ */
