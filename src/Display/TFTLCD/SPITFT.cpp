/**
 * @Name    : SPITFT.cpp
 * @Date    : 2020年2月18日 下午3:44:43
 * @Author  : happe
 * @brief   :
 */

#include "SPITFT.h"

SPITFT::SPITFT(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin)
		: Adafruit_GFX(W, H), mSpi(Spix), CS(CSPin), DC(DCPin), RST(ResetPin) {
	CS.init(GpioMode::out);
	DC.init(GpioMode::out);
	RST.init(GpioMode::out);

	CS.setHigh();
	DC.setHigh();
	RST.setHigh();
}

void SPITFT::startWrite(void) {
	CS.setLow();
}

void SPITFT::endWrite(void) {
	CS.setHigh();
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
void SPITFT::writePixel(int16_t x, int16_t y, uint16_t color) {
	static uint8_t pixelbuf[2];
	if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
		setAddrWindow(x, y, 1, 1);
		pixelbuf[0] = static_cast<uint8_t>(color >> 8);
		pixelbuf[1] = static_cast<uint8_t>(color);
		mSpi.Write_n(pixelbuf, 2, false);
		while(mSpi.isBusy());
	}
}

/*!
 *   @brief  Issue a series of pixels from memory to the display. Not self-
 *           contained; should follow startWrite() and setAddrWindow() calls.
 *           It will be very fast in STM32 system!
 *
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
void SPITFT::writePixels(const uint16_t *colors, uint32_t len, bool block) {
	mSpi.setDataWidth(SPI_DataSize::DataSize_16b);	// enable the 16-bit transmission mode
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

		while (mSpi.isBusy());
		mSpi.Write_n(colors, count, false);

		len -= count;
		colors += count;
	}
	mSpi.setDataWidth(SPI_DataSize::DataSize_8b);	// back the 8-bit transmission mode
}

/*!
 *   @brief  Issue a series of pixels, all the same color. Not self-
 *           contained; should follow startWrite() and setAddrWindow() calls.
 *   @param  color  16-bit pixel color in '565' RGB format.
 *   @param  len    Number of pixels to draw.
 */
void SPITFT::writeColor(uint16_t color, uint32_t len) {
	mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
	if (!len)
		return; // Avoid 0-byte transfers
	uint16_t dummy;

	// each transferring a max of 65535 pixel(DMA counter's limit)
	// This won't run off the end of the allocated descriptor list,
	// since we're using much larger chunks per descriptor here.
	uint16_t numDescriptors = (len + 65534) / 65535;

	for (uint16_t i = 0; i < numDescriptors; i++) {
		uint16_t count = (len < 65535) ? len : 65535;
		while (mSpi.isBusy());
		mSpi.WriteRead(&color, &dummy, count, false, false, false);
		len -= count;
	}
	mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
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
void SPITFT::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
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

/*	@brief  Draw a horizontal line on the display. Performs edge clipping
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
void inline SPITFT::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	if ((y >= 0) && (y < _height) && w) { // Y on screen, nonzero width
		if (w < 0) {                      // If negative width...
			x += w + 1;                      //   Move X to left edge
			w = -w;                      //   Use positive width
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
void inline SPITFT::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
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
inline void SPITFT::writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	setAddrWindow(x, y, w, h);
	writeColor(color, (static_cast<uint32_t>(w) * h));
}

void SPITFT::drawPixel(int16_t x, int16_t y, uint16_t color) {
	startWrite();
	writePixel(x, y, color);
	endWrite();
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
void SPITFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	if (w && h) {								// Nonzero width and height?
		if (w < 0) {							// If negative width...
			x += w + 1;							// Move X to left edge
			w = -w;								// Use positive width
		}
		if (x < _width) {						// Not off right
			if (h < 0) {						// If negative height...
				y += h + 1;						// Move Y to top edge
				h = -h;							// Use positive height
			}
			if (y < _height) {					// Not off bottom
				int16_t x2 = x + w - 1;
				if (x2 >= 0) {					// Not off left
					int16_t y2 = y + h - 1;
					if (y2 >= 0) {				// Not off top
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

/**
 *	@brief  Draw a horizontal line on the display. Self-contained and
 *			provides its own transaction as needed (see writeFastHLine() for
 *			a lower-level variant). Edge clipping and rejection is performed
 *			here.
 *	@param  x		Horizontal position of first point.
 *	@param  y		Vertical position of first point.
 *	@param  w		Line width in pixels (positive = right of first point,
 *					negative = point of first corner).
 *	@param  color  16-bit line color in '565' RGB format.
 *	@note   This repeats the writeFastHLine() function almost in its
 *			entirety, with the addition of a transaction start/end. It's
 *			done this way (rather than starting the transaction and calling
 *			writeFastHLine() to handle clipping and so forth) so that the
 *			transaction isn't performed at all if the line is rejected.
 */
void SPITFT::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
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

/**
 *	@brief  Draw a vertical line on the display. Self-contained and provides
 *			its own transaction as needed (see writeFastHLine() for a lower-
 *			level variant). Edge clipping and rejection is performed here.
 *	@param  x		Horizontal position of first point.
 *	@param  y		Vertical position of first point.
 *	@param  h		Line height in pixels (positive = below first point,
 *					negative = above first point).
 *	@param  color  16-bit line color in '565' RGB format.
 *	@note   This repeats the writeFastVLine() function almost in its
 *			entirety, with the addition of a transaction start/end. It's
 *			done this way (rather than starting the transaction and calling
 *			writeFastVLine() to handle clipping and so forth) so that the
 *			transaction isn't performed at all if the line is rejected.
 */
void SPITFT::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
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

void SPITFT::drawRGBBitmap(int16_t x, int16_t y, const uint16_t pcolors[], int16_t w, int16_t h) {

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

/**
 *	@brief  Draw a 16-bit image (565 RGB) at the specified (x,y) position.
 *			For 16-bit display devices; no color reduction performed.
 *			Adapted from https://github.com/PaulStoffregen/ILI9341_t3
 *			by Marc MERLIN. See examples/pictureEmbed to use this.
 *			5/6/2017: function name and arguments have changed for
 *			compatibility with current GFX library and to avoid naming
 *			problems in prior implementation.  Formerly drawBitmap() with
 *			arguments in different order. Handles its own transaction and
 *			edge clipping/rejection.
 *	@param  x        Top left corner horizontal coordinate.
 *	@param  y        Top left corner vertical coordinate.
 *	@param  pcolors  Pointer to 16-bit array of pixel values.
 *	@param  w        Width of bitmap in pixels.
 *	@param  h        Height of bitmap in pixels.
 */
void SPITFT::drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h) {

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
void SPITFT::invertDisplay(bool i) {
	startWrite();
	writeCommand(i ? invertOnCommand : invertOffCommand);
	endWrite();
}

/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and
 data
 @param   commandByte       The Command Byte
 @param   dataBytes         A pointer to the Data bytes to send
 @param   numDataBytes      The number of bytes we should send
 */
void SPITFT::sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes) {
	startWrite();
	DC.setLow();          // Command mode
	mSpi.write(commandByte, false); // Send the command byte

	DC.setHigh();
	for (int i = 0; i < numDataBytes; i++) {
		mSpi.write(*dataBytes, false); // Send the data bytes
		dataBytes++;
	}

	endWrite();
}

/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and
 data
 @param   commandByte       The Command Byte
 @param   dataBytes         A pointer to the Data bytes to send
 @param   numDataBytes      The number of bytes we should send
 */
void SPITFT::sendCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes) {
	startWrite();
	DC.setLow();          // Command mode
	mSpi.write(commandByte, false); // Send the command byte

	DC.setHigh();
	for (int i = 0; i < numDataBytes; i++) {
		mSpi.write(pgm_read_byte(dataBytes++), false);
	}

	endWrite();
}

/*!
 @brief  Adafruit_SPITFT sendCommand16 handles complete sending of
 commands and data for 16-bit parallel displays. Currently somewhat
 rigged for the NT35510, which has the odd behavior of wanting
 commands 16-bit, but subsequent data as 8-bit values, despite
 the 16-bit bus (high byte is always 0). Also seems to require
 issuing and incrementing address with each transfer.
 @param  commandWord   The command word (16 bits)
 @param  dataBytes     A pointer to the data bytes to send
 @param  numDataBytes  The number of bytes we should send
 */
void SPITFT::sendCommand16(uint16_t commandWord, const uint8_t *dataBytes, uint8_t numDataBytes) {
	startWrite();
	mSpi.setDataWidth(SPI_DataSize::DataSize_16b);
	if (numDataBytes == 0) {
		DC.setLow();					// Command mode
		mSpi.write(commandWord, false);	// Send the command word
		DC.setHigh();					// Data mode
	}
	for (int i = 0; i < numDataBytes; i++) {
		DC.setLow();					// Command mode
		mSpi.write(commandWord, false);	// Send the command word
		DC.setHigh();					// Data mode
		commandWord++;
		mSpi.write(static_cast<uint16_t>(pgm_read_byte(dataBytes++)), false);
	}
	mSpi.setDataWidth(SPI_DataSize::DataSize_8b);
	endWrite();
}

