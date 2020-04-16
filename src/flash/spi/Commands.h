/**
 * @Name    : Commands.h
 * @Date    : 2019年12月14日 下午2:35:37
 * @Author  : happe
 * @brief   : Flash Chips commands define
 * 			  Reference from Prajwal Bhattaram's Arduino SPIMemory Library
 *            Replace #define to enum
 */

#ifndef FLASH_SPI_COMMANDS_H_
#define FLASH_SPI_COMMANDS_H_

#include "defines.h"

namespace spiflash {

	/* List of Error codes */
	enum error_codes {
	    SUCCESS                 = 0x00,
	    CALLBEGIN               = 0x01,
	    UNKNOWNCHIP             = 0x02,
	    UNKNOWNCAP              = 0x03,
	    CHIPBUSY                = 0x04,
	    OUTOFBOUNDS             = 0x05,
	    CANTENWRITE             = 0x06,
	    PREVWRITTEN             = 0x07,
	    LOWRAM                  = 0x08,
	    SYSSUSPEND              = 0x09,
	    ERRORCHKFAIL            = 0x0A,
	    NORESPONSE              = 0x0B,
	    UNSUPPORTEDFUNC         = 0x0C,
	    UNABLETO4BYTE           = 0x0D,
	    UNABLETO3BYTE           = 0x0E,
	    CHIPISPOWEREDDOWN       = 0x0F,
	    NOSFDP                  = 0x10,
	    NOSFDPERASEPARAM        = 0x11,
	    NOSFDPERASETIME         = 0x12,
	    NOSFDPPROGRAMTIMEPARAM  = 0x13,
	    NOCHIPSELECTDECLARED    = 0x14,
	    UNKNOWNERROR            = 0xFE
	};


	/* "Standard" command list */
	enum Generic : uint8_t {
		MANID 			= 0x90,
		PAGEPROG 		= 0x02,
		READDATA 		= 0x03,
		FASTREAD 		= 0x0B,
		WRITEDISABLE 	= 0x04,
		READSTAT1 		= 0x05,
		READSTAT2 		= 0x35,
		READSTAT3 		= 0x15,
		WRITESTATEN 	= 0x50,
		WRITESTAT1 		= 0x01,
		WRITESTAT2 		= 0x31,
		WRITESTAT3 		= 0x11,
		WRITEENABLE 	= 0x06,
		ADDR4BYTE_EN 	= 0xB7,
		ADDR4BYTE_DIS 	= 0xE9,
		SECTORERASE 	= 0x20,
		BLOCK32ERASE 	= 0x52,
		BLOCK64ERASE 	= 0xD8,
		CHIPERASE 		= 0x60,
		ALT_CHIPERASE 	= 0xC7,    // Some flash chips use a different chip erase command
		SUSPEND 		= 0x75,
		ID 				= 0x90,
		RESUME 			= 0x7A,
		JEDECID 		= 0x9F,
		POWERDOWN 		= 0xB9,
		RELEASE 		= 0xAB,
		READSFDP 		= 0x5A,
		UNIQUEID 		= 0x4B,
		FRAMSERNO 		= 0xC3
	};

	enum {
		SPI_PAGESIZE 	= 0x100
	};


	/* Chip specific instructions */
	enum Winbond : uint8_t {
		WINBOND_MANID 			= 0xEF,
		WINBOND_WRITE_DELAY 	= 0x02,
		WINBOND_WREN_TIMEOUT 	= 10L
	};

	enum Microchip : uint8_t {
		MICROCHIP_MANID = 0xBF,
		SST25 			= 0x25,
		SST26 			= 0x26,
		ULBPR 			= 0x98    //Global Block Protection Unlock (Ref sections 4.1.1 & 5.37 of datasheet)
	};

	enum Cypress : uint8_t {
		CYPRESS_MANID 		= 0x01,
		RAMTRON_FRAM_MANID 	= 0xC2
	};

	enum Adesto : uint8_t {
		ADESTO_MANID 		= 0x1F
	};

	enum Micron : uint8_t {
		MICRON_MANID 		= 0x20,
		M25P40 				= 0x20
	};

	enum ON_com : uint8_t {
		ON_MANID 			= 0x62
	};

	enum Giga_com : uint8_t {
		GIGA_MANID 			= 0xC8
	};

	enum AMIC_A : uint8_t {
		AMIC_MANID 			= 0x37,
		A25L512 			= 0x30
	};

	enum AMIC_MX : uint8_t {
		MACRONIX_MANID 		= 0xC2,
		MX25L4005 			= 0x13,
		MX25L8005 			= 0x14
	};

	enum Definitions : uint8_t {
		BUSY        =  0x01,
		STDSPI      =  0x0A,
		ALTSPI      =  0x0B,

		ENFASTREAD  =  0x01,
		WRTEN       =  0x02,
		SUS         =  0x80,

		WSE         =  0x04,
		WSP         =  0x08,
		ADS         =  0x01,            // Current Address mode in Status register 3
		DUMMYBYTE   =  0xEE,

		NO_CONTINUE =  0x00,
		NOVERBOSE   =  0x00,
		VERBOSE     =  0x01,
		PASS        =  0x01,
		FAIL        =  0x00,
		NOOVERFLOW  =  false,
		NOERRCHK    =  false,
		PRINTOVERRIDE = true,
		ERASEFUNC   =  0xEF
	};


	constexpr uint8_t _capID[18] = {
			0x10, 0x11, 0x12, 0x13,
			0x14, 0x15, 0x16, 0x17,
			0x18, 0x19, 0x41, 0x42,
			0x43, 0x4B, 0x00, 0x01,
			0x13, 0x37
	};

	constexpr uint32_t _memSize[18] = {
			bytes_of_kB(64),
			bytes_of_kB(128),
			bytes_of_kB(256),
			bytes_of_kB(512),
			bytes_of_MB(1),
			bytes_of_MB(2),
			bytes_of_MB(4),
			bytes_of_MB(8),
			bytes_of_MB(16),
			bytes_of_MB(32),
			bytes_of_MB(2),
			bytes_of_MB(4),
			bytes_of_MB(8),
			bytes_of_MB(8),
			bytes_of_kB(256),
			bytes_of_kB(512),
			bytes_of_MB(4),
			bytes_of_kB(512)
	};
	// To understand the _memSize definitions check defines.h

	constexpr uint8_t _supportedManID[9] = {
			WINBOND_MANID,    	// It's Winbond's Flash
			MICROCHIP_MANID,	// It's Microchip's Flash
			CYPRESS_MANID,	  	// ...same
			ADESTO_MANID,
			MICRON_MANID,
			ON_MANID,
			GIGA_MANID,
			AMIC_MANID,
			MACRONIX_MANID
	};

	constexpr uint8_t _altChipEraseReq[3] = { A25L512, M25P40, SST26 };

}

#endif /* FLASH_SPI_COMMANDS_H_ */
