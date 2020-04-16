/**
 * @Name    : ILI9341.cpp
 * @Date    : 2019年12月28日 下午1:06:12
 * @Author  : happe
 * @brief   :
 */

#include "ILI9341.h"

namespace ili9341 {

	inline void ILI9341::spi_begin(void) {
		mSpi.setCSLow();
	}

	inline void ILI9341::spi_end(void) {
		mSpi.setCSHigh();
	}

	inline void ILI9341::spi_begin_read(void) {
		mSpi.setCSLow();
	}

	inline void ILI9341::spi_end_read(void) {
		mSpi.setCSHigh();
	}

	ILI9341::ILI9341(Spi_dma& Spix, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin, int16_t W, int16_t H)
			: Adafruit_GFX(W, H), mSpi(Spix), TFT_DC(DCPin), TFT_RST(ResetPin) {
		_init_width = _width = W; // Set by specific xxxxx_Defines.h file or by users sketch
		_init_height = _height = H; // Set by specific xxxxx_Defines.h file or by users sketch
		rotation = 0;
		cursor_y = cursor_x = 0;
		textfont = 1;
		textcolor = bitmap_fg = 0xFFFF; // White
		textbgcolor = bitmap_bg = 0x0000; // Black
		textdatum = TL_DATUM; // Top Left text alignment is default
		fontsloaded = 0;

		addr_row = 0xFFFF;
		addr_col = 0xFFFF;

		_xpivot = 0;
		_ypivot = 0;
		decoderBuffer = 0;

		mSpi.setCSPin(CSPin);
		TFT_DC.init(GpioMode::out);
		TFT_RST.init(GpioMode::out);
	}

	void ILI9341::begin(uint8_t tc) {
		init(tc);
	}

	/**
	 * @brief	: SPI TFT init
	 *
	 */
	void ILI9341::init(uint8_t tc) {

		if (F_CPU <= 72000000)
			mSpi.setClockPrescaler(SPI_Prescaler::fPCLK_4);
		else
			mSpi.setClockPrescaler(SPI_Prescaler::fPCLK_8);
		// Toggle RST low to reset
		spi_begin();

		digitalWrite(TFT_RST, HIGH);
		delay(100);
		digitalWrite(TFT_RST, LOW);
		delay(100);
		digitalWrite(TFT_RST, HIGH);
		delay(200);

		spi_end();

		delay(150); 	// Wait for reset to complete

		startWrite();

		uint8_t cmd, x, numArgs;
		const uint8_t *addr = initcmd;
		while ((cmd = pgm_read_byte(addr++)) > 0) {
			writecommand(cmd);
			x = pgm_read_byte(addr++);
			numArgs = x & 0x7F;
			while (numArgs--)
				writedata(pgm_read_byte(addr++));
			if (x & 0x80)
				delay(150);
		}

		endWrite();

		_width = _init_width;
		_height = _init_height;

		setRotation(rotation);

	}

	/**************************************************************************/
	/*!
	 @brief   Set origin of (0,0) and orientation of TFT display
	 @param   m  The index for rotation, from 0-3 inclusive
	 */
	/**************************************************************************/
	void ILI9341::setRotation(uint8_t m) {
		rotation = m % 4; // can't be higher than 3
		switch (rotation) {
			case 0:
				m = (MADCTL_MX | MADCTL_BGR);
				_width = _init_width;
				_height = _init_height;
				break;
			case 1:
				m = (MADCTL_MV | MADCTL_BGR);
				_width = _init_height;
				_height = _init_width;
				break;
			case 2:
				m = (MADCTL_MY | MADCTL_BGR);
				_width = _init_width;
				_height = _init_height;
				break;
			case 3:
				m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
				_width = _init_height;
				_height = _init_width;
				break;
		}

		startWrite();
		writecommand(ILI9341_MADCTL);
		writedata(m);
		endWrite();
	}

	void ILI9341::writecommand(uint8_t c) {
		//spi_begin(); // CS_L;
		digitalWrite(TFT_DC, LOW);
		mSpi.Write_n(&c, 1, false);
		digitalWrite(TFT_DC, HIGH);
		//spi_end();  // CS_H;
	}

	void ILI9341::writedata(uint8_t d) {
		//spi_begin(); // CS_L;
		digitalWrite(TFT_DC, HIGH);       // Play safe, but should already be in data mode
		mSpi.Write_n(&d, 1, false);
		//spi_end();   // CS_H;
	}

	uint8_t ILI9341::readcommand8(uint8_t cmd_function, uint8_t index) {
		uint8_t reg = 0;

		// for ILI9341 Interface II i.e. IM [3:0] = "1101"
		spi_begin_read();
		index = 0x10 + (index & 0x0F);

		digitalWrite(TFT_DC, LOW);
		mSpi.write(0xD9);
		digitalWrite(TFT_DC, HIGH);
		mSpi.write(index);

		mSpi.setCSHigh(); 	// Some displays seem to need CS to be pulsed here, or is just a delay needed?
		mSpi.setCSLow();

		digitalWrite(TFT_DC, LOW);
		mSpi.write(cmd_function);
		digitalWrite(TFT_DC, HIGH);
		reg = mSpi.read();

		spi_end_read();
		return reg;
	}

	uint16_t ILI9341::readcommand16(uint8_t cmd_function, uint8_t index) {
		uint32_t reg;

		reg = (readcommand8(cmd_function, index + 0) << 8);
		reg |= (readcommand8(cmd_function, index + 1) << 0);

		return reg;
	}

	uint32_t ILI9341::readcommand32(uint8_t cmd_function, uint8_t index) {
		uint32_t reg;

		reg = (readcommand8(cmd_function, index + 0) << 24);
		reg |= (readcommand8(cmd_function, index + 1) << 16);
		reg |= (readcommand8(cmd_function, index + 2) << 8);
		reg |= (readcommand8(cmd_function, index + 3) << 0);

		return reg;
	}

	uint16_t ILI9341::readPixel(int32_t x0, int32_t y0) {

		spi_begin_read();

		readAddrWindow(x0, y0, 1, 1); // Sets CS low

		// Dummy read to throw away don't care value
		mSpi.read(0xFF, false);
		// Read the 3 RGB bytes, colour is actually only in the top 6 bits of each byte
		// as the TFT stores colours as 18 bits
		uint8_t r = mSpi.read(0xFF, false);
		uint8_t g = mSpi.read(0xFF, false);
		uint8_t b = mSpi.read(0xFF, false);

		mSpi.setCSHigh();

		spi_end_read();

		return Pixel::RGB565(r, g, b);
	}

	void ILI9341::setPivot(int16_t x, int16_t y) {
		_xpivot = x;
		_ypivot = y;
	}

	int16_t ILI9341::getPivotX(void) {
		return _xpivot;
	}

	int16_t ILI9341::getPivotY(void) {
		return _ypivot;
	}

	uint16_t ILI9341::fontsLoaded(void) {
		return fontsloaded;
	}

	void ILI9341::setAddrWindow(int32_t x0, int32_t y0, int32_t w, int32_t h) {
		uint16_t x2 = (x0 + w - 1), y2 = (y0 + h - 1);
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
		writecommand(ILI9341_CASET); // Column address set
		mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
		//mSpi.write(high_byte(xs));
		//mSpi.write(low_byte(xs));
		//mSpi.write(high_byte(x2));
		//mSpi.write(low_byte(x2));
		mSpi.write(x0, false);
		mSpi.write(x2, false);
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
		writecommand(ILI9341_PASET); // Row address set
		mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
		//mSpi.write(high_byte(ys));
		//mSpi.write(low_byte(ys));
		//mSpi.write(high_byte(y2));
		//mSpi.write(low_byte(y2));
		mSpi.write(y0, false);
		mSpi.write(y2, false);
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
		writecommand(ILI9341_RAMWR); // Write to RAM
	}

	void ILI9341::setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye) {
		uint16_t x2 = (xs + xe - 1), y2 = (ys + ye - 1);
		writecommand(ILI9341_CASET); // Column address set
		mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
		//mSpi.write(high_byte(xs));
		//mSpi.write(low_byte(xs));
		//mSpi.write(high_byte(x2));
		//mSpi.write(low_byte(x2));
		mSpi.write(xs, false);
		mSpi.write(x2, false);
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
		writecommand(ILI9341_PASET); // Row address set
		mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
		//mSpi.write(high_byte(ys));
		//mSpi.write(low_byte(ys));
		//mSpi.write(high_byte(y2));
		//mSpi.write(low_byte(y2));
		mSpi.write(ys, false);
		mSpi.write(y2, false);
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
		writecommand(ILI9341_RAMWR); // Write to RAM
	}

	void ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color) {
		startWrite();
		writePixel(x, y, color);
		endWrite();
	}

	/*!
	 *   @brief  Draw a filled rectangle to the display. Not self-contained;
	 *           should follow startWrite(). Typically used by higher-level
	 *           graphics primitives; user code shouldn't need to call this and
	 *           is likely to use the self-contained fillRect() instead.
	 *           writeFillRect() performs its own edge clipping and rejection;
	 *           see writeFillRectPreclipped() for a more 'raw' implementation.
	 *   @param  x      Horizontal position of first corner.
	 *   @param  y      Vertical position of first corner.
	 *   @param  w      Rectangle width in pixels (positive = right of first
	 *                  corner, negative = left of first corner).
	 *   @param  h      Rectangle height in pixels (positive = below first
	 *                  corner, negative = above first corner).
	 *   @param  color  16-bit fill color in '565' RGB format.
	 *   @note   Written in this deep-nested way because C by definition will
	 *           optimize for the 'if' case, not the 'else' -- avoids branches
	 *           and rejects clipped rectangles at the least-work possibility.
	 */
	void ILI9341::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
		if (w && h) {                            // Nonzero width and height?
			if (w < 0) {                         // If negative width...
				x += w + 1;                    //   Move X to left edge
				w = -w;                        //   Use positive width
			}
			if (x < _width) {                    // Not off right
				if (h < 0) {                     // If negative height...
					y += h + 1;                //   Move Y to top edge
					h = -h;                    //   Use positive height
				}
				if (y < _height) {               // Not off bottom
					int16_t x2 = x + w - 1;
					if (x2 >= 0) {               // Not off left
						int16_t y2 = y + h - 1;
						if (y2 >= 0) {           // Not off top
							// Rectangle partly or fully overlaps screen
							if (x < 0) {
								x = 0;
								w = x2 + 1;
							} // Clip left
							if (y < 0) {
								y = 0;
								h = y2 + 1;
							} // Clip top
							if (x2 >= _width) {
								w = _width - x;
							} // Clip right
							if (y2 >= _height) {
								h = _height - y;
							} // Clip bottom
							writeFillRectPreclipped(x, y, w, h, color);
						}
					}
				}
			}
		}
	}

	/*!
	 *	@brief  A lower-level version of writeFillRect(). This version requires
	 *			all inputs are in-bounds, that width and height are positive,
	 *			and no part extends offscreen. NO EDGE CLIPPING OR REJECTION IS
	 *			PERFORMED. If higher-level graphics primitives are written to
	 *			handle their own clipping earlier in the drawing process, this
	 *			can avoid unnecessary function calls and repeated clipping
	 *			operations in the lower-level functions.
	 *	@param  x      Horizontal position of first corner. MUST BE WITHIN
	 *				   SCREEN BOUNDS.
	 *	@param  y      Vertical position of first corner. MUST BE WITHIN SCREEN
	 *				   BOUNDS.
	 *	@param  w      Rectangle width in pixels. MUST BE POSITIVE AND NOT
	 *				   EXTEND OFF SCREEN.
	 *	@param  h      Rectangle height in pixels. MUST BE POSITIVE AND NOT
	 *				   EXTEND OFF SCREEN.
	 *	@param  color  16-bit fill color in '565' RGB format.
	 *	@note   This is a new function, no graphics primitives besides rects
	 *			and horizontal/vertical lines are written to best use this yet.
	 */
	inline void ILI9341::writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
		setAddrWindow(x, y, w, h);
		writeColor(color, (static_cast<uint32_t>(w) * h));
	}

	/*!
	 *	@brief  Draw a horizontal line on the display. Performs edge clipping
	 *			and rejection. Not self-contained; should follow startWrite().
	 *			Typically used by higher-level graphics primitives; user code
	 *			shouldn't need to call this and is likely to use the self-
	 *			contained drawFastHLine() instead.
	 *	@param  x      	Horizontal position of first point.
	 *	@param  y      	Vertical position of first point.
	 *	@param  w      	Line width in pixels (positive = right of first point,
	 *					negative = point of first corner).
	 *	@param  color  	16-bit line color in '565' RGB format.
	 */
	void inline ILI9341::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
		if ((y >= 0) && (y < _height) && w) { // Y on screen, nonzero width
			if (w < 0) {                      // If negative width...
				x += w + 1;                 //   Move X to left edge
				w = -w;                     //   Use positive width
			}
			if (x < _width) {                 // Not off right
				int16_t x2 = x + w - 1;
				if (x2 >= 0) {                // Not off left
					// Line partly or fully overlaps screen
					if (x < 0) {
						x = 0;
						w = x2 + 1;
					} // Clip left
					if (x2 >= _width) {
						w = _width - x;
					} // Clip right
					writeFillRectPreclipped(x, y, w, 1, color);
				}
			}
		}
	}

	/*!
	 *	@brief  		Draw a vertical line on the display. Performs edge clipping and
	 *					rejection. Not self-contained; should follow startWrite().
	 *					Typically used by higher-level graphics primitives; user code
	 *					shouldn't need to call this and is likely to use the self-
	 *					contained drawFastVLine() instead.
	 *	@param  x      	Horizontal position of first point.
	 *	@param  y      	Vertical position of first point.
	 *	@param  h      	Line height in pixels (positive = below first point,
	 *					negative = above first point).
	 *	@param  color  	16-bit line color in '565' RGB format.
	 */
	void inline ILI9341::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
		if ((x >= 0) && (x < _width) && h) { // X on screen, nonzero height
			if (h < 0) {                     // If negative height...
				y += h + 1;                //   Move Y to top edge
				h = -h;                    //   Use positive height
			}
			if (y < _height) {               // Not off bottom
				int16_t y2 = y + h - 1;
				if (y2 >= 0) {               // Not off top
					// Line partly or fully overlaps screen
					if (y < 0) {
						y = 0;
						h = y2 + 1;
					} // Clip top
					if (y2 >= _height) {
						h = _height - y;
					} // Clip bottom
					writeFillRectPreclipped(x, y, 1, h, color);
				}
			}
		}
	}

	/*!
	 *   @brief  Draw a filled rectangle to the display. Self-contained and
	 *           provides its own transaction as needed (see writeFillRect() or
	 *           writeFillRectPreclipped() for lower-level variants). Edge
	 *           clipping and rejection is performed here.
	 *   @param  x      Horizontal position of first corner.
	 *   @param  y      Vertical position of first corner.
	 *   @param  w      Rectangle width in pixels (positive = right of first
	 *                  corner, negative = left of first corner).
	 *   @param  h      Rectangle height in pixels (positive = below first
	 *                  corner, negative = above first corner).
	 *   @param  color  16-bit fill color in '565' RGB format.
	 *   @note   This repeats the writeFillRect() function almost in its entirety,
	 *           with the addition of a transaction start/end. It's done this way
	 *           (rather than starting the transaction and calling writeFillRect()
	 *           to handle clipping and so forth) so that the transaction isn't
	 *           performed at all if the rectangle is rejected. It's really not
	 *           that much code.
	 */
	void ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
		if (w && h) {                            // Nonzero width and height?
			if (w < 0) {                         // If negative width...
				x += w + 1;                    //   Move X to left edge
				w = -w;                        //   Use positive width
			}
			if (x < _width) {                    // Not off right
				if (h < 0) {                     // If negative height...
					y += h + 1;                //   Move Y to top edge
					h = -h;                    //   Use positive height
				}
				if (y < _height) {               // Not off bottom
					int16_t x2 = x + w - 1;
					if (x2 >= 0) {               // Not off left
						int16_t y2 = y + h - 1;
						if (y2 >= 0) {           // Not off top
							// Rectangle partly or fully overlaps screen
							if (x < 0) {
								x = 0;
								w = x2 + 1;
							} // Clip left
							if (y < 0) {
								y = 0;
								h = y2 + 1;
							} // Clip top
							if (x2 >= _width) {
								w = _width - x;
							} // Clip right
							if (y2 >= _height) {
								h = _height - y;
							} // Clip bottom
							startWrite();
							writeFillRectPreclipped(x, y, w, h, color);
							endWrite();
						}
					}
				}
			}
		}
	}

	/*!
	 *   @brief  Draw a horizontal line on the display. Self-contained and
	 *           provides its own transaction as needed (see writeFastHLine() for
	 *           a lower-level variant). Edge clipping and rejection is performed
	 *           here.
	 *   @param  x      Horizontal position of first point.
	 *   @param  y      Vertical position of first point.
	 *   @param  w      Line width in pixels (positive = right of first point,
	 *                  negative = point of first corner).
	 *   @param  color  16-bit line color in '565' RGB format.
	 *   @note   This repeats the writeFastHLine() function almost in its
	 *           entirety, with the addition of a transaction start/end. It's
	 *           done this way (rather than starting the transaction and calling
	 *           writeFastHLine() to handle clipping and so forth) so that the
	 *           transaction isn't performed at all if the line is rejected.
	 */
	void ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
		if ((y >= 0) && (y < _height) && w) { // Y on screen, nonzero width
			if (w < 0) {                      // If negative width...
				x += w + 1;                 //   Move X to left edge
				w = -w;                     //   Use positive width
			}
			if (x < _width) {                 // Not off right
				int16_t x2 = x + w - 1;
				if (x2 >= 0) {                // Not off left
					// Line partly or fully overlaps screen
					if (x < 0) {
						x = 0;
						w = x2 + 1;
					} // Clip left
					if (x2 >= _width) {
						w = _width - x;
					} // Clip right
					startWrite();
					writeFillRectPreclipped(x, y, w, 1, color);
					endWrite();
				}
			}
		}
	}

	/*!
	 *   @brief  Draw a vertical line on the display. Self-contained and provides
	 *           its own transaction as needed (see writeFastHLine() for a lower-
	 *           level variant). Edge clipping and rejection is performed here.
	 *   @param  x      Horizontal position of first point.
	 *   @param  y      Vertical position of first point.
	 *   @param  h      Line height in pixels (positive = below first point,
	 *                  negative = above first point).
	 *   @param  color  16-bit line color in '565' RGB format.
	 *   @note   This repeats the writeFastVLine() function almost in its
	 *           entirety, with the addition of a transaction start/end. It's
	 *           done this way (rather than starting the transaction and calling
	 *           writeFastVLine() to handle clipping and so forth) so that the
	 *           transaction isn't performed at all if the line is rejected.
	 */
	void ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
		if ((x >= 0) && (x < _width) && h) { // X on screen, nonzero height
			if (h < 0) {                     // If negative height...
				y += h + 1;                //   Move Y to top edge
				h = -h;                    //   Use positive height
			}
			if (y < _height) {               // Not off bottom
				int16_t y2 = y + h - 1;
				if (y2 >= 0) {               // Not off top
					// Line partly or fully overlaps screen
					if (y < 0) {
						y = 0;
						h = y2 + 1;
					} // Clip top
					if (y2 >= _height) {
						h = _height - y;
					} // Clip bottom
					startWrite();
					writeFillRectPreclipped(x, y, 1, h, color);
					endWrite();
				}
			}
		}
	}

	/*!
	 *   @brief  Draw a 16-bit image (565 RGB) at the specified (x,y) position.
	 *           For 16-bit display devices; no color reduction performed.
	 *           Adapted from https://github.com/PaulStoffregen/ILI9341_t3
	 *           by Marc MERLIN. See examples/pictureEmbed to use this.
	 *           5/6/2017: function name and arguments have changed for
	 *           compatibility with current GFX library and to avoid naming
	 *           problems in prior implementation.  Formerly drawBitmap() with
	 *           arguments in different order. Handles its own transaction and
	 *           edge clipping/rejection.
	 *   @param  x        Top left corner horizontal coordinate.
	 *   @param  y        Top left corner vertical coordinate.
	 *   @param  pcolors  Pointer to 16-bit array of pixel values.
	 *   @param  w        Width of bitmap in pixels.
	 *   @param  h        Height of bitmap in pixels.
	 */
	void ILI9341::drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h) {

		int16_t x2, y2; // Lower-right coord
		if ((x >= _width) ||      // Off-edge right
		(y >= _height) ||      // " top
		((x2 = (x + w - 1)) < 0) ||      // " left
		((y2 = (y + h - 1)) < 0))
			return; // " bottom

		int16_t bx1 = 0, by1 = 0, // Clipped top-left within bitmap
				saveW = w;      // Save original bitmap width value
		if (x < 0) { // Clip left
			w += x;
			bx1 = -x;
			x = 0;
		}
		if (y < 0) { // Clip top
			h += y;
			by1 = -y;
			y = 0;
		}
		if (x2 >= _width)
			w = _width - x; // Clip right
		if (y2 >= _height)
			h = _height - y; // Clip bottom

		pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
		startWrite();
		setAddrWindow(x, y, w, h); // Clipped area
		while (h--) { // For each (clipped) scanline...
			writePixels(pcolors, w); // Push one (clipped) row
			pcolors += saveW; // Advance pointer by one full (unclipped) line
		}
		endWrite();
	}

	// -------------------------------------------------------------------------
	// Miscellaneous class member functions that don't draw anything.

	/*!
	 @brief  Invert the colors of the display (if supported by hardware).
	 Self-contained, no transaction setup required.
	 @param  i  true = inverted display, false = normal display.
	 */
	void ILI9341::invertDisplay(bool i) {
		startWrite();
		writecommand(i ? invertOnCommand : invertOffCommand);
		endWrite();
	}

	void ILI9341::startWrite(void) {
		mSpi.setCSLow();
	}

	void ILI9341::endWrite(void) {
		mSpi.setCSHigh();
	}

	// -------------------------------------------------------------------------
	// Lower-level graphics operations. These functions require a chip-select
	// and/or SPI transaction around them (via startWrite(), endWrite() above).
	// Higher-level graphics primitives might start a single transaction and
	// then make multiple calls to these functions (e.g. circle or text
	// rendering might make repeated lines or rects) before ending the
	// transaction. It's more efficient than starting a transaction every time.

	/*!
	 *   @brief  Draw a single pixel to the display at requested coordinates.
	 *           Not self-contained; should follow a startWrite() call.
	 *   @param  x      Horizontal position (0 = left).
	 *   @param  y      Vertical position   (0 = top).
	 *   @param  color  16-bit pixel color in '565' RGB format.
	 */
	void ILI9341::writePixel(int16_t x, int16_t y, uint16_t color) {
		if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
			setAddrWindow(x, y, 1, 1);
			mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
			mSpi.write(color, false);
			mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
		}
	}

	/*!
	 *   @brief  Issue a series of pixels from memory to the display. Not self-
	 *           contained; should follow startWrite() and setAddrWindow() calls.
	 *   @param  colors  Pointer to array of 16-bit pixel values in '565' RGB
	 *                   format.
	 *   @param  len     Number of elements in 'colors' array.
	 *   @param  block   If true (default case if unspecified), function blocks
	 *                   until DMA transfer is complete. This is simply IGNORED
	 *                   if DMA is not enabled. If false, the function returns
	 *                   immediately after the last DMA transfer is started,
	 *                   and one should use the dmaWait() function before
	 *                   doing ANY other display-related activities (or even any
	 *                   SPI-related activities, if using an SPI display that
	 *                   shares the bus with other devices).
	 */
	void ILI9341::writePixels(uint16_t *colors, uint32_t len, bool block) {
		mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
		if (!len)
			return; // Avoid 0-byte transfers
		// All other cases (bitbang SPI or non-DMA hard SPI or parallel),
		// use a loop with the normal 16-bit data write function:
		// each transferring a max of 65535 pixel(DMA counter's limit)
		// This won't run off the end of the allocated descriptor list,
		// since we're using much larger chunks per descriptor here.
		uint16_t numDescriptors = (len + 65534) / 65535;

		for (uint16_t i = 0; i < numDescriptors; i++) {
			uint16_t count = (len < 65535) ? len : 65535;

			mSpi.Write_n(colors, count, false);

			while (mSpi.isBusy()) {

			}
			len -= count;
			colors += count;
		}
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
	}

	/*!
	 *   @brief  Issue a series of pixels, all the same color. Not self-
	 *           contained; should follow startWrite() and setAddrWindow() calls.
	 *   @param  color  16-bit pixel color in '565' RGB format.
	 *   @param  len    Number of pixels to draw.
	 */
	void ILI9341::writeColor(uint16_t color, uint32_t len) {
		mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
		if (!len)
			return; // Avoid 0-byte transfers
		uint16_t dummy;



		// each transferring a max of 65535 pixel(DMA counter's limit)
		// This won't run off the end of the allocated descriptor list,
		// since we're using much larger chunks per descriptor here.
		uint16_t numDescriptors = (len + 65534) / 65535;

		for (uint16_t i = 0; i < numDescriptors; i++){
			uint16_t count = (len < 65535) ? len : 65535;

			mSpi.WriteRead(&color, &dummy, count, false, false, false);

			while (mSpi.isBusy()) {

			}
			len -= count;
		}
		mSpi.setDataWidth(SPI_DataSize::DataSize_8b);

	}
}
