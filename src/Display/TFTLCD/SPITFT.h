/**
 * @Name    : SPITFT.h
 * @Date    : 2020年2月18日 下午3:40:08
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_SPITFT_H_
#define DISPLAY_SPITFT_H_

#include "Display/Graphic.h"
#include "Print/ColorConv.hpp"
#include "SPI/SPI.hpp"

class SPITFT : public Adafruit_GFX {
	public:
		SPITFT(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin);
		virtual void begin(SPI_Prescaler sck_speed = SPI_Prescaler::fPCLK_4) = 0;


		void writePixels(const uint16_t *colors, uint32_t len, bool block = true);
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

		void invertDisplay(bool i) override;
		// Compatibility additions (non-essential)
		void startWrite(void) override;                     // Begin SPI transaction (not normally needed)
		void endWrite(void) override;                       // End SPI transaction


		using Adafruit_GFX::drawRGBBitmap; 					// Check base class first
		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t pcolors[], int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);


		inline void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);


		void writeCommand(uint8_t c) {
			//spi_begin(); // CS_L;
			DC.setLow();
			mSpi.write(c, false);
			DC.setHigh();
			//spi_end();  // CS_H;
		}

		void writeData(uint8_t d) {
			//spi_begin(); // CS_L;
			DC.setHigh();       		// Play safe, but should already be in data mode
			mSpi.write(d, false);
			//spi_end();   // CS_H;
		}

		void sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes);
		void sendCommand(uint8_t commandByte, const uint8_t *dataBytes = nullptr,
				uint8_t numDataBytes = 0);
		void sendCommand16(uint16_t commandWord, const uint8_t *dataBytes = nullptr,
				uint8_t numDataBytes = 0);

		virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
	private:

	protected:
		uint8_t invertOnCommand = 0; 	///< Command to enable invert mode
		uint8_t invertOffCommand = 0; 	///< Command to disable invert mode

		uint8_t _colstart = 0;   		///< Some displays need this changed to offset
		uint8_t _rowstart = 0;       	///< Some displays need this changed to offset



		Spi_dma& mSpi;
		GpioFast CS;
		GpioFast DC;
		GpioFast RST;
};

#endif /* DISPLAY_SPITFT_H_ */
