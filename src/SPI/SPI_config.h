/**
 * @Name    : SPI_config.h
 * @Date    : 2019年12月10日 下午6:48:36
 * @Author  : happe
 * @brief   :
 */

#ifndef SPI_SPI_CONFIG_H_
#define SPI_SPI_CONFIG_H_

#include "main.h"


namespace SPI_Defines{
	enum CR1_offset{
		CR1_CPHA 		= 0,
		CR1_CPOL		= 1,
		CR1_MSTR		= 2,
		CR1_BR			= 3,	// Buad Rate
		CR1_SPE			= 6,
		CR1_LSBFIRST	= 7,
		CR1_SSI			= 8,
		CR1_SSM			= 9,
		CR1_RXONLY		= 10,
		CR1_CRCL		= 11,
		CR1_CRCNEXT		= 12,
		CR1_CRCEN		= 13,
		CR1_BIDIOE		= 14,
		CR1_BIDIMODE 	= 15
	};

	enum CR2_offset{
		CR2_RXDMAEN		= 0,
		CR2_TXDMAEN		= 1,
		CR2_SSOE		= 2,
		CR2_NSSP		= 3,
		CR2_FRF			= 4,
		CR2_ERRIE		= 5,
		CR2_RXNEIE		= 6,
		CR2_TXEIE		= 7,
		CR2_DS			= 8,	// Data size
		CR2_FRXTH		= 12,
		CR2_LDMA_RX		= 13,
		CR2_LDMA_TX		= 14
	};



}

enum class SPI_Prescaler {
	fPCLK_2 		= (0x00 << SPI_Defines::CR1_BR),
	fPCLK_4 		= (0x01 << SPI_Defines::CR1_BR),
	fPCLK_8 		= (0x02 << SPI_Defines::CR1_BR),
	fPCLK_16 		= (0x03 << SPI_Defines::CR1_BR),
	fPCLK_32 		= (0x04 << SPI_Defines::CR1_BR),
	fPCLK_64 		= (0x05 << SPI_Defines::CR1_BR),
	fPCLK_128 		= (0x06 << SPI_Defines::CR1_BR),
	fPCLK_256 		= (0x07 << SPI_Defines::CR1_BR),
};

enum class SPI_DataSize {
	DataSize_4b 		= (0x03 << SPI_Defines::CR2_DS),
	DataSize_5b 		= (0x04 << SPI_Defines::CR2_DS),
	DataSize_6b 		= (0x05 << SPI_Defines::CR2_DS),
	DataSize_7b 		= (0x06 << SPI_Defines::CR2_DS),
	DataSize_8b 		= (0x07 << SPI_Defines::CR2_DS),
	DataSize_9b 		= (0x08 << SPI_Defines::CR2_DS),
	DataSize_10b 		= (0x09 << SPI_Defines::CR2_DS),
	DataSize_11b 		= (0x0A << SPI_Defines::CR2_DS),
	DataSize_12b 		= (0x0B << SPI_Defines::CR2_DS),
	DataSize_13b 		= (0x0C << SPI_Defines::CR2_DS),
	DataSize_14b 		= (0x0D << SPI_Defines::CR2_DS),
	DataSize_15b 		= (0x0E << SPI_Defines::CR2_DS),
	DataSize_16b 		= (0x0F << SPI_Defines::CR2_DS),
};



namespace SPI_PinMap {

	struct FullDuplex {
			GpioPin SCLK;
			GpioPin MISO;
			GpioPin MOSI;
			GpioAF PinAF;
	};

	constexpr auto DefaultMap = FullDuplex { PA5, PA6, PA7 , GpioAF::AF0};
}

#endif /* SPI_SPI_CONFIG_H_ */
