/**
 * @Name    : ST77xx_cmd.h
 * @Date    : 2020年2月18日 下午4:06:19
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_ST77XX_ST77XX_CMD_H_
#define DISPLAY_ST77XX_ST77XX_CMD_H_

namespace st77xx {
	enum Generic : uint8_t {
		ST_CMD_DELAY 	= 0x80, // special signifier for command lists

		ST77XX_NOP 		= 0x00,
		ST77XX_SWRESET 	= 0x01,
		ST77XX_RDDID 	= 0x04,
		ST77XX_RDDST 	= 0x09,

		ST77XX_SLPIN 	= 0x10,
		ST77XX_SLPOUT 	= 0x11,
		ST77XX_PTLON 	= 0x12,
		ST77XX_NORON 	= 0x13,

		ST77XX_INVOFF 	= 0x20,
		ST77XX_INVON 	= 0x21,
		ST77XX_DISPOFF 	= 0x28,
		ST77XX_DISPON	= 0x29,
		ST77XX_CASET 	= 0x2A,
		ST77XX_RASET 	= 0x2B,
		ST77XX_RAMWR 	= 0x2C,
		ST77XX_RAMRD 	= 0x2E,

		ST77XX_PTLAR 	= 0x30,
		ST77XX_TEOFF 	= 0x34,
		ST77XX_TEON 	= 0x35,
		ST77XX_MADCTL 	= 0x36,
		ST77XX_COLMOD 	= 0x3A,

		ST77XX_MADCTL_MY 	= 0x80,
		ST77XX_MADCTL_MX 	= 0x40,
		ST77XX_MADCTL_MV 	= 0x20,
		ST77XX_MADCTL_ML 	= 0x10,
		ST77XX_MADCTL_RGB 	= 0x00,

		ST77XX_RDID1 		= 0xDA,
		ST77XX_RDID2 		= 0xDB,
		ST77XX_RDID3 		= 0xDC,
		ST77XX_RDID4 		= 0xDD,
	};

}



#endif /* DISPLAY_ST77XX_ST77XX_CMD_H_ */
