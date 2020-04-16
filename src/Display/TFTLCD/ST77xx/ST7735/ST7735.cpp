/**
 * @Name    : ST7735.cpp
 * @Date    : 2020年1月29日 下午9:16:35
 * @Author  : happe
 * @brief   :
 */

#include "ST7735.h"

namespace st7735 {
	ST7735::ST7735(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin)
			: ST77xx(Spix, W, H, CSPin, DCPin, ResetPin) {

	}

	/**************************************************************************/
	/*!
	 @brief  Initialize ST77xx chip. Connects to the ST77XX over SPI and
	 sends initialization procedure commands
	 @param  freq  Desired SPI clock frequency
	 */
	/**************************************************************************/
	void ST7735::begin(SPI_Prescaler sck_speed, uint8_t options) {
		mSpi.setClockPrescaler(sck_speed);
		initR(options);
	}

	/**************************************************************************/
	/*!
	 @brief  Set origin of (0,0) and orientation of TFT display
	 @param  m  The index for rotation, from 0-3 inclusive
	 */
	/**************************************************************************/
	void ST7735::setRotation(uint8_t m) {
		uint8_t madctl = 0;

		rotation = m & 3; // can't be higher than 3

		// For ST7735 with GREEN TAB (including HalloWing)...
		if ((tabcolor == INITR_144GREENTAB) || (tabcolor == INITR_HALLOWING)) {
			// ..._rowstart is 3 for rotations 0&1, 1 for rotations 2&3
			_rowstart = (rotation < 2) ? 3 : 1;
		}

		switch (rotation) {
			case 0:
				if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
					madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
				} else {
					madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR;
				}

				if (tabcolor == INITR_144GREENTAB) {
					_height = 128;
					_width = 128;
				} else if (tabcolor == INITR_MINI160x80) {
					_height = 160;
					_width = 80;
				} else {
					_height = 160;
					_width = 128;
				}
				_xstart = _colstart;
				_ystart = _rowstart;
				break;
			case 1:
				if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
					madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
				} else {
					madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
				}

				if (tabcolor == INITR_144GREENTAB) {
					_width = 128;
					_height = 128;
				} else if (tabcolor == INITR_MINI160x80) {
					_width = 160;
					_height = 80;
				} else {
					_width = 160;
					_height = 128;
				}
				_ystart = _colstart;
				_xstart = _rowstart;
				break;
			case 2:
				if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
					madctl = ST77XX_MADCTL_RGB;
				} else {
					madctl = ST7735_MADCTL_BGR;
				}

				if (tabcolor == INITR_144GREENTAB) {
					_height = 128;
					_width = 128;
				} else if (tabcolor == INITR_MINI160x80) {
					_height = 160;
					_width = 80;
				} else {
					_height = 160;
					_width = 128;
				}
				_xstart = _colstart;
				_ystart = _rowstart;
				break;
			case 3:
				if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
					madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
				} else {
					madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
				}

				if (tabcolor == INITR_144GREENTAB) {
					_width = 128;
					_height = 128;
				} else if (tabcolor == INITR_MINI160x80) {
					_width = 160;
					_height = 80;
				} else {
					_width = 160;
					_height = 128;
				}
				_ystart = _colstart;
				_xstart = _rowstart;
				break;
		}

		sendCommand(ST77XX_MADCTL, &madctl, 1);
	}

	/**************************************************************************/
	/*!
	 @brief  Initialization code common to all ST7735B displays
	 */
	/**************************************************************************/
	void ST7735::initB(void) {
		commonInit(Bcmd);
		setRotation(0);
	}

	/**************************************************************************/
	/*!
	 @brief  Initialization code common to all ST7735R displays
	 @param  options  Tab color from adafruit purchase
	 */
	/**************************************************************************/
	void ST7735::initR(uint8_t options) {
		commonInit(Rcmd1);
		if (options == INITR_GREENTAB) {
			displayInit(Rcmd2green);
			_colstart = 2;
			_rowstart = 1;
		} else if ((options == INITR_144GREENTAB) || (options == INITR_HALLOWING)) {
			_height = 128;
			_width = 128;
			displayInit(Rcmd2green144);
			_colstart = 2;
			_rowstart = 3; // For default rotation 0
		} else if (options == INITR_MINI160x80) {
			_height = 80;
			_width = 160;
			displayInit(Rcmd2green160x80);
			_colstart = 24;
			_rowstart = 0;
		} else {
			// colstart, rowstart left at default '0' values
			displayInit(Rcmd2red);
		}
		displayInit(Rcmd3);

		// Black tab, change MADCTL color filter
		if ((options == INITR_BLACKTAB) || (options == INITR_MINI160x80)) {
			uint8_t data = 0xC0;
			sendCommand(ST77XX_MADCTL, &data, 1);
		}

		if (options == INITR_HALLOWING) {
			// Hallowing is simply a 1.44" green tab upside-down:
			tabcolor = INITR_144GREENTAB;
			setRotation(2);
		} else {
			tabcolor = options;
			setRotation(0);
		}
	}

}

