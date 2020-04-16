/**
 * @Name    : ST7789.cpp
 * @Date    : 2020年2月18日 下午4:44:34
 * @Author  : happe
 * @brief   :
 */

#include "ST7789.h"

namespace st7789 {
	using namespace st77xx;

	ST7789::ST7789(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin)
		: ST77xx(Spix, W, H, CSPin, DCPin, ResetPin) {

	}

	/**************************************************************************/
	/*!
	 @brief  Initialization code common to all ST7789 displays
	 @param  width  Display width
	 @param  height Display height
	 @param  mode   SPI data mode; one of SPI_MODE0, SPI_MODE1, SPI_MODE2
	 or SPI_MODE3 (do NOT pass the numbers 0,1,2 or 3 -- use
	 the defines only, the values are NOT the same!)
	 */
	/**************************************************************************/
	void ST7789::init(uint16_t width, uint16_t height) {
		// Save SPI data mode. commonInit() calls begin() (in Adafruit_ST77xx.cpp),
		// which in turn calls initSPI() (in Adafruit_SPITFT.cpp), passing it the
		// value of spiMode. It's done this way because begin() really should not
		// be modified at this point to accept an SPI mode -- it's a virtual
		// function required in every Adafruit_SPITFT subclass and would require
		// updating EVERY such library...whereas, at the moment, we know that
		// certain ST7789 displays are the only thing that may need a non-default
		// SPI mode, hence this roundabout approach...
		// (Might get added similarly to other display types as needed on a
		// case-by-case basis.)

		commonInit(nullptr);

		if ((width == 240) && (height == 240)) {			// 1.3" and 1.54" displays
			_colstart = 0;
			_rowstart = 80;
		} else if ((width == 135) && (height == 240)) {		// 1.13" display
			_colstart = 53;
			_rowstart = 40;
		} else {											// Iduno wat is this?
			_colstart = 0;
			_rowstart = 0;
		}
		_width = width;
		_height = height;

		displayInit(generic_st7789);

		if ((width == 135) && (height == 240)) {
			setRotation(0);
		} else {
			setRotation(0);
		}
	}

	/**************************************************************************/
	/*!
	 @brief  Set origin of (0,0) and orientation of TFT display
	 @param  m  The index for rotation, from 0-3 inclusive
	 */
	/**************************************************************************/
	void ST7789::setRotation(uint8_t m) {
		uint8_t madctl = 0;

		rotation = m & 3; // can't be higher than 3

		switch (rotation) {
			case 0:
				madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
				_xstart = _colstart;
				_ystart = _rowstart;
				_width = WIDTH;
				_height = HEIGHT;
				break;
			case 1:
				madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
				_xstart = _rowstart;
				_ystart = _colstart;
				_height = WIDTH;
				_width = HEIGHT;
				break;
			case 2:
				madctl = ST77XX_MADCTL_RGB;
				if ((WIDTH == 135) && (HEIGHT == 240)) {
					_xstart = _colstart - 1;
					_ystart = _rowstart;
				} else {
					_xstart = 0;
					_ystart = 0;
				}
				_width = WIDTH;
				_height = HEIGHT;
				break;
			case 3:
				madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
				if ((WIDTH == 135) && (HEIGHT == 240)) {
					_xstart = _rowstart;
					_ystart = _colstart;
				} else {
					_xstart = 0;
					_ystart = 0;
				}
				_height = WIDTH;
				_width = HEIGHT;
				break;
		}

		sendCommand(ST77XX_MADCTL, &madctl, 1);
	}

}


