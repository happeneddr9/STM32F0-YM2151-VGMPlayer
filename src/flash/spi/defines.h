/**
 * @Name    : defines.h
 * @Date    : 2019年12月14日 下午2:38:31
 * @Author  : happe
 * @brief   :
 */

#ifndef FLASH_SPI_DEFINES_H_
#define FLASH_SPI_DEFINES_H_

#include "Arduino/Basic.h"

namespace spiflash {

	constexpr uint32_t BUSY_TIMEOUT = 1000000000L;

	/* Bytes Symbol Prefix Convert */

	constexpr uint32_t kiB = 1024UL;
	// kilo Bytes = 1024 B

	constexpr uint32_t MiB = kiB * kiB;
	// Mega Bytes = 1024 kiB
	constexpr uint64_t GiB = static_cast<uint64_t>(MiB) * kiB;
	// Giga Bytes = 1024 MiB ** Do you really have a chance to use it? **

	constexpr uint64_t TiB = static_cast<uint64_t>(GiB) * kiB;
	// Tera Bytes = 1024 GiB ** for Fun **

	inline constexpr uint32_t bytes(uint32_t x) {
		return x * 1L;
	}

	inline constexpr uint32_t bytes_of_kB(uint32_t x) {
		return x * kiB;
	}

	inline constexpr uint32_t bytes_of_MB(uint32_t x) {
		return x * MiB;
	}

	inline constexpr uint64_t bytes_of_GB(uint32_t x) {
		return (static_cast<uint64_t>(x) * GiB);
	}

	inline constexpr uint64_t bytes_of_TB(uint32_t x) {
		return (static_cast<uint64_t>(x) * TiB);
	}

	enum data_types {
		_BYTE_ = 0x01,
		_CHAR_ = 0x02,
		_WORD_ = 0x03,
		_SHORT_ = 0x04,
		_ULONG_ = 0x05,
		_LONG_ = 0x06,
		_FLOAT_ = 0x07,
		_STRING_ = 0x08,
		_BYTEARRAY_ = 0x09,
		_CHARARRAY_ = 0x0A,
		_STRUCT_ = 0x0B
	};

}

#endif /* FLASH_SPI_DEFINES_H_ */
