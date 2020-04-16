/**
 * @Name    : ST77xx.cpp
 * @Date    : 2020年2月18日 下午4:05:32
 * @Author  : happe
 * @brief   :
 */

#include "ST77xx.h"

namespace st77xx {
	ST77xx::ST77xx(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin)
		: SPITFT(Spix, W, H, CSPin, DCPin, ResetPin){

	}


	/**************************************************************************/
	/*!
	 @brief  SPI displays set an address window rectangle for blitting pixels
	 @param  x  Top left corner x coordinate
	 @param  y  Top left corner x coordinate
	 @param  w  Width of window
	 @param  h  Height of window
	 */
	/**************************************************************************/
	void ST77xx::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
		x += _xstart;
		y += _ystart;
		uint16_t xa = x + w - 1;
		uint16_t ya = y + h - 1;
		static uint8_t fastDrawbuf[8];

		fastDrawbuf[0] = static_cast<uint8_t>(x >> 8);
		fastDrawbuf[1] = static_cast<uint8_t>(x);
		fastDrawbuf[2] = static_cast<uint8_t>(xa >> 8);
		fastDrawbuf[3] = static_cast<uint8_t>(xa);

		fastDrawbuf[4] = static_cast<uint8_t>(y >> 8);
		fastDrawbuf[5] = static_cast<uint8_t>(y);
		fastDrawbuf[6] = static_cast<uint8_t>(ya >> 8);
		fastDrawbuf[7] = static_cast<uint8_t>(ya);

		writeCommand(ST77XX_CASET); // Column addr set
		/*mSpi.write(static_cast<uint8_t>(xa >> 24), false);
		 mSpi.write(static_cast<uint8_t>(xa >> 16), false);
		 mSpi.write(static_cast<uint8_t>(xa >> 8), false);
		 mSpi.write(static_cast<uint8_t>(xa), false);*/
		mSpi.Write_n(fastDrawbuf, 4, false);
		while (mSpi.isBusy());
		writeCommand(ST77XX_RASET); // Row addr set
		/*mSpi.write(static_cast<uint8_t>(ya >> 24), false);
		 mSpi.write(static_cast<uint8_t>(ya >> 16), false);
		 mSpi.write(static_cast<uint8_t>(ya >> 8), false);
		 mSpi.write(static_cast<uint8_t>(ya), false);*/
		mSpi.Write_n(fastDrawbuf + 4, 4, false);
		while (mSpi.isBusy());
		writeCommand(ST77XX_RAMWR); // write to RAM
	}

	/**************************************************************************/
	/*!
	 @brief  Companion code to the initiliazation tables. Reads and issues
	 a series of LCD commands stored in PROGMEM byte array.
	 @param  addr  Flash memory array with commands and data to send
	 */
	/**************************************************************************/
	void ST77xx::displayInit(const uint8_t *addr) {

		uint8_t numCommands, cmd, numArgs;
		uint16_t ms;

		numCommands = pgm_read_byte(addr++); // Number of commands to follow
		while (numCommands--) {              // For each command...
			cmd = pgm_read_byte(addr++);       // Read command
			numArgs = pgm_read_byte(addr++);   // Number of args to follow
			ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
			numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
			sendCommand(cmd, addr, numArgs);
			addr += numArgs;

			if (ms) {
				ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
				if (ms == 255)
					ms = 500; // If 255, delay for 500 ms
				delay(ms);
			}
		}
	}

	/**************************************************************************/
	/*!
	    @brief  Initialize ST77xx chip. Connects to the ST77XX over SPI and
	            sends initialization procedure commands
	    @param  freq  Desired SPI clock frequency
	*/
	/**************************************************************************/
	void ST77xx::begin(SPI_Prescaler sck_speed){
		mSpi.setClockPrescaler(sck_speed);
		invertOnCommand = ST77XX_INVON;
		invertOffCommand = ST77XX_INVOFF;
	}

	/**************************************************************************/
	/*!
	 @brief  Initialization code common to all ST77XX displays
	 @param  cmdList  Flash memory array with commands and data to send
	 */
	/**************************************************************************/
	void ST77xx::commonInit(const uint8_t *cmdList) {
		begin();

		if (cmdList) {
			displayInit(cmdList);
		}
	}

	/**************************************************************************/
	/*!
	 @brief  Set origin of (0,0) and orientation of TFT display
	 @param  m  The index for rotation, from 0-3 inclusive
	 */
	/**************************************************************************/
	void ST77xx::setRotation(uint8_t m) {
		uint8_t madctl = 0;

		rotation = m % 4; // can't be higher than 3

		switch (rotation) {
			case 0:
				madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
				_xstart = _colstart;
				_ystart = _rowstart;
				break;
			case 1:
				madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
				_ystart = _colstart;
				_xstart = _rowstart;
				break;
			case 2:
				madctl = ST77XX_MADCTL_RGB;
				_xstart = _colstart;
				_ystart = _rowstart;
				break;
			case 3:
				madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
				_ystart = _colstart;
				_xstart = _rowstart;
				break;
		}

		sendCommand(ST77XX_MADCTL, &madctl, 1);
	}

	/**************************************************************************/
	/*!
	 @brief  Set origin of (0,0) of display with offsets
	 @param  col  The offset from 0 for the column address
	 @param  row  The offset from 0 for the row address
	 */
	/**************************************************************************/
	void ST77xx::setColRowStart(int8_t col, int8_t row) {
		_colstart = col;
		_rowstart = row;
	}

	/**************************************************************************/
	/*!
	 @brief  Change whether display is on or off
	 @param  enable True if you want the display ON, false OFF
	 */
	/**************************************************************************/
	void ST77xx::enableDisplay(boolean enable) {
		writeCommand(enable ? ST77XX_DISPON : ST77XX_DISPOFF);
	}

	/**************************************************************************/
	/*!
	 @brief  Change whether TE pin output is on or off
	 @param  enable True if you want the TE pin ON, false OFF
	 */
	/**************************************************************************/
	void ST77xx::enableTearing(boolean enable) {
		writeCommand(enable ? ST77XX_TEON : ST77XX_TEOFF);
	}
}
