/**
 * @Name    : Commands.h
 * @Date    : 2020年2月18日 下午4:44:44
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_ST77XX_ST7789_COMMANDS_H_
#define DISPLAY_ST77XX_ST7789_COMMANDS_H_

#include "Display/TFTLCD/ST77xx/ST77xx.h"

namespace st7789 {
	using namespace st77xx;

	// SCREEN INITIALIZATION ***************************************************

	// Rather than a bazillion writecommand() and writedata() calls, screen
	// initialization commands and arguments are organized in these tables
	// stored in PROGMEM.  The table may look bulky, but that's mostly the
	// formatting -- storage-wise this is hundreds of bytes more compact
	// than the equivalent code.  Companion function follows.

	// clang-format off
	// taken from Adafruit
	static const uint8_t PROGMEM
	generic_st7789[] = {						// Init commands for 7789 screens
			9,									//  9 commands in list:
			ST77XX_SWRESET, ST_CMD_DELAY,		//  1: Software reset, no args, w/delay
			150,								//     ~150 ms delay
			ST77XX_SLPOUT, ST_CMD_DELAY,		//  2: Out of sleep mode, no args, w/delay
			10,									//      10 ms delay
			ST77XX_COLMOD, 1 + ST_CMD_DELAY,	//  3: Set color mode, 1 arg + delay:
			0x55,								//     16-bit color
			10,									//     10 ms delay
			ST77XX_MADCTL, 1,					//  4: Mem access ctrl (directions), 1 arg:
			0x08,								//     Row/col addr, bottom-top refresh
			ST77XX_CASET, 4,					//  5: Column addr set, 4 args, no delay:
			0x00, 0,							//     XSTART = 0
			0, 240,								//     XEND = 240
			ST77XX_RASET, 4,					//  6: Row addr set, 4 args, no delay:
			0x00, 0,							//     YSTART = 0
			320 >> 8, 320 & 0xFF,				//     YEND = 320
			ST77XX_INVON, ST_CMD_DELAY,			//  7: hack
			10, ST77XX_NORON, ST_CMD_DELAY,		//  8: Normal display on, no args, w/delay
			10,									//     10 ms delay
			ST77XX_DISPON, ST_CMD_DELAY,		//  9: Main screen turn on, no args, delay
			10									//    10 ms delay
	};     	// clang-format on




}



#endif /* DISPLAY_ST77XX_ST7789_COMMANDS_H_ */
