/**
 * @Name    : Commands.h
 * @Date    : 2019年12月28日 下午1:31:14
 * @Author  : happe
 * @brief   : All ILI9341's Control command defines
 */

#ifndef DISPLAY_TFTLCD_ILI9341_COMMANDS_H_
#define DISPLAY_TFTLCD_ILI9341_COMMANDS_H_

#include "Arduino/Basic.h"

namespace ili9341 {
	// Delay between some initialisation commands
#define TFT_INIT_DELAY 0x80 // Not used unless commandlist invoked

	// Generic commands used by TFT_eSPI.cpp
	enum Generic : uint8_t{
		TFT_NOP 		= 0x00,
		TFT_SWRST 		= 0x01,

		TFT_CASET 		= 0x2A,
		TFT_PASET 		= 0x2B,
		TFT_RAMWR 		= 0x2C,

		TFT_RAMRD 		= 0x2E,
		TFT_IDXRD 		= 0xDD, 	// ILI9341 only, indexed control register read

		TFT_MADCTL 		= 0x36,
		TFT_MAD_MY 		= 0x80,
		TFT_MAD_MX 		= 0x40,
		TFT_MAD_MV 		= 0x20,
		TFT_MAD_ML 		= 0x10,
		TFT_MAD_BGR 	= 0x08,
		TFT_MAD_MH 		= 0x04,
		TFT_MAD_RGB 	= 0x00,

		TFT_INVOFF 		= 0x20,
		TFT_INVON 		= 0x21,
	};

	// All ILI9341 specific commands some are used by init()
	enum Specific : uint8_t {
		ILI9341_NOP 			= 0x00,
		ILI9341_SWRESET 		= 0x01,
		ILI9341_RDDID 			= 0x04,
		ILI9341_RDDST 			= 0x09,

		ILI9341_SLPIN 			= 0x10,
		ILI9341_SLPOUT 			= 0x11,
		ILI9341_PTLON 			= 0x12,
		ILI9341_NORON 			= 0x13,

		ILI9341_RDMODE 			= 0x0A,
		ILI9341_RDMADCTL 		= 0x0B,
		ILI9341_RDPIXFMT 		= 0x0C,
		ILI9341_RDIMGFMT 		= 0x0A,
		ILI9341_RDSELFDIAG 		= 0x0F,

		ILI9341_INVOFF 			= 0x20,
		ILI9341_INVON 			= 0x21,
		ILI9341_GAMMASET 		= 0x26,
		ILI9341_DISPOFF 		= 0x28,
		ILI9341_DISPON 			= 0x29,

		ILI9341_CASET 			= 0x2A,
		ILI9341_PASET 			= 0x2B,
		ILI9341_RAMWR 			= 0x2C,
		ILI9341_RAMRD 			= 0x2E,

		ILI9341_PTLAR 			= 0x30,
		ILI9341_VSCRDEF 		= 0x33,
		ILI9341_MADCTL 			= 0x36,
		ILI9341_VSCRSADD 		= 0x37,
		ILI9341_PIXFMT 			= 0x3A,

		ILI9341_WRDISBV 		= 0x51,
		ILI9341_RDDISBV 		= 0x52,
		ILI9341_WRCTRLD 		= 0x53,

		ILI9341_FRMCTR1 		= 0xB1,
		ILI9341_FRMCTR2 		= 0xB2,
		ILI9341_FRMCTR3 		= 0xB3,
		ILI9341_INVCTR 			= 0xB4,
		ILI9341_DFUNCTR 		= 0xB6,

		ILI9341_PWCTR1 			= 0xC0,
		ILI9341_PWCTR2 			= 0xC1,
		ILI9341_PWCTR3 			= 0xC2,
		ILI9341_PWCTR4 			= 0xC3,
		ILI9341_PWCTR5 			= 0xC4,
		ILI9341_VMCTR1 			= 0xC5,
		ILI9341_VMCTR2 			= 0xC7,

		ILI9341_RDID4 			= 0xD3,
		ILI9341_RDINDEX 		= 0xD9,
		ILI9341_RDID1 			= 0xDA,
		ILI9341_RDID2 			= 0xDB,
		ILI9341_RDID3 			= 0xDC,
		ILI9341_RDIDX 			= 0xDD, 	// TBC

		ILI9341_GMCTRP1 		= 0xE0,
		ILI9341_GMCTRN1 		= 0xE1,

		ILI9341_MADCTL_MY 		= 0x80,
		ILI9341_MADCTL_MX 		= 0x40,
		ILI9341_MADCTL_MV 		= 0x20,
		ILI9341_MADCTL_ML 		= 0x10,
		ILI9341_MADCTL_RGB 		= 0x00,
		ILI9341_MADCTL_BGR 		= 0x08,
		ILI9341_MADCTL_MH 		= 0x04,
	};

	//These enumerate the text plotting alignment (reference datum point)
	enum Datum : uint8_t {
		TL_DATUM 	= 0, 	// Top left (default)
		TC_DATUM 	= 1, 	// Top centre
		TR_DATUM 	= 2, 	// Top right
		ML_DATUM 	= 3, 	// Middle left
		CL_DATUM 	= 3, 	// Centre left, same as above
		MC_DATUM 	= 4, 	// Middle centre
		CC_DATUM 	= 4, 	// Centre centre, same as above
		MR_DATUM 	= 5, 	// Middle right
		CR_DATUM 	= 5, 	// Centre right, same as above
		BL_DATUM 	= 6, 	// Bottom left
		BC_DATUM 	= 7, 	// Bottom centre
		BR_DATUM 	= 8, 	// Bottom right
		L_BASELINE 	= 9, 	// Left character baseline (Line the 'A' character would sit on)
		C_BASELINE 	= 10, 	// Centre character baseline
		R_BASELINE 	= 11, 	// Right character baseline
	};

	enum Rotation{
		MADCTL_MY 	= 0x80,  ///< Bottom to top
		MADCTL_MX 	= 0x40,  ///< Right to left
		MADCTL_MV 	= 0x20,  ///< Reverse Mode
		MADCTL_ML 	= 0x10,  ///< LCD refresh Bottom to top
		MADCTL_RGB 	= 0x00,  ///< Red-Green-Blue pixel order
		MADCTL_BGR 	= 0x08,  ///< Blue-Green-Red pixel order
		MADCTL_MH 	= 0x04,  ///< LCD refresh right to left
	};

	// initialize step by bytes
	static const uint8_t PROGMEM initcmd[] = {
		0xEF, 3, 0x03, 0x80, 0x02,
		0xCF, 3, 0x00, 0xC1, 0x30,
		0xED, 4, 0x64, 0x03, 0x12, 0x81,
		0xE8, 3, 0x85, 0x00, 0x78,
		0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
		0xF7, 1, 0x20,
		0xEA, 2, 0x00, 0x00,
		ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
		ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
		ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
		ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
		ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
		ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
		ILI9341_PIXFMT  , 1, 0x55,
		ILI9341_FRMCTR1 , 2, 0x00, 0x18,
		ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
		0xF2, 1, 0x00,                         // 3Gamma Function Disable
		ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
		ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
		0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
		ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
		0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
		ILI9341_SLPOUT  , 0x80,                // Exit Sleep
		ILI9341_DISPON  , 0x80,                // Display on
		0x00                                   // End of list
	};

}

#endif /* DISPLAY_TFTLCD_ILI9341_COMMANDS_H_ */
