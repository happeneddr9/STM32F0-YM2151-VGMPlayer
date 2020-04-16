/**
 * @Name    : SPIFlash.h
 * @Date    : 2019年12月14日 下午3:24:07
 * @Author  : happe
 * @brief   : Flash Chips Main class
 * 			  Reference from Prajwal Bhattaram's Arduino SPIMemory Library
 */

#ifndef FLASH_SPI_SPIFLASH_H_
#define FLASH_SPI_SPIFLASH_H_

#include "main.h"
#include "Commands.h"
#include "GPIO/GPIO.h"
#include "SPI/SPI.hpp"

#define CHIP_SELECT   CS.setLow();
#define CHIP_DESELECT CS.setHigh();

namespace spiflash {

	class Diagnostics {
		public:
			//------------------------------------ Constructor ------------------------------------//

			Diagnostics(void) {
				errorcode = 0;
			}

			~Diagnostics(void) = default;
			//------------------------------- Public functions -----------------------------------//
			void troubleshoot(uint8_t _code, bool printoverride = false);

			uint8_t errorcode;
		private:

	};

	extern Diagnostics diagnostics; ///< default Diagnostics instance

	class SPIFlash {

		public:
			//------------------------------------ Constructor ------------------------------------//
			SPIFlash(GpioPin cs, Spi_lite& spiinterface);

			//----------------------------- Initial / Chip Functions ------------------------------//
			bool begin(uint32_t flashChipSize = 0);

			uint8_t error(bool verbosity = false);

			uint16_t getManID(void);
			uint32_t getJEDECID(void);
			uint64_t getUniqueID(void);
			uint32_t getAddress(uint16_t size);

			uint32_t getCapacity(void);
			uint32_t getMaxPage(void);

			//-------------------------------- Write / Read Bytes ---------------------------------//
			bool writeByte(uint32_t _addr, uint8_t data, bool errorCheck = true);
			uint8_t readByte(uint32_t _addr, bool fastRead = false);


			//-------------------------------- Write / Read Length of Bytes -----------------------//

			bool write_n(uint32_t _addr, uint8_t* data, size_t len, bool errorCheck = true);
			bool read_n(uint32_t _addr, uint8_t* data, size_t len, bool fastRead = false);

			//------------------------------- Write / Read Anything -------------------------------//

			template <class T> bool writeAnything(uint32_t _addr, const T& data, bool errorCheck = true);
			template <class T> bool readAnything(uint32_t _addr, T& data, bool fastRead = false);

			//-------------------------------- Erase functions ------------------------------------//
			bool eraseSection(uint32_t _addr, uint32_t _sz);
			bool eraseSector(uint32_t _addr);
			bool eraseBlock32K(uint32_t _addr);
			bool eraseBlock64K(uint32_t _addr);
			bool eraseChip(void);
			//-------------------------------- Power functions ------------------------------------//
			bool suspendProg(void);
			bool resumeProg(void);
			bool powerDown(void);
			bool powerUp(void);

		private:
			void _troubleshoot(uint8_t _code, bool printoverride = false);
			bool _beginSPI(uint8_t opcode);
			bool _transferAddress(void);
			bool _getJedecId(void);
			bool _chipID(uint32_t flashChipSize = 0);
			void _endSPI(void);
			bool _disableGlobalBlockProtect(void);
			bool _isChipPoweredDown(void);
			bool _prep(uint8_t opcode, uint32_t _addr, uint32_t size = 0);
			bool _startSPIBus(void);
			bool _noSuspend(void);
			bool _notBusy(uint32_t timeout = BUSY_TIMEOUT);
			bool _writeEnable(bool _troubleshootEnable = true);
			bool _writeDisable(void);

			uint8_t _readStat1(void);
			uint8_t _readStat2(void);
			uint8_t _readStat3(void);

			bool _addressCheck(uint32_t _addr, uint32_t size = 1);
			bool _notPrevWritten(uint32_t _addr, uint32_t size = 1);
			bool _enable4ByteAddressing(void);
			bool _disable4ByteAddressing(void);

			void _nextBuf(uint8_t opcode, uint8_t *data_buffer, uint32_t size);

			template <class T>
			bool _write(uint32_t _addr, const T& value, uint32_t _sz, bool errorCheck, uint8_t _dataType);
			template <class T>
			bool _read(uint32_t _addr, T& value, uint32_t _sz, bool fastRead = false,
					uint8_t _dataType = 0x00);

			template <class T>
			bool _writeErrorCheck(uint32_t _addr, const T& value, uint32_t _sz, uint8_t _dataType = 0x00);

			inline void _delay_us(uint32_t delayTime) {
				delayMicroseconds(delayTime);
			}

		protected:

			Spi_lite& _spi;
			Gpio CS;

			bool pageOverflow;
			bool SPIBusState = false;
			bool chipPoweredDown = false;
			bool address4ByteEnabled = false;
			bool _loopedOver = false;
			uint8_t cs_mask, errorcode, stat1, stat2, stat3, _SPCR, _SPSR, _a0, _a1, _a2;

			struct chipID {
					bool supported;
					bool supportedMan;
					bool sfdpAvailable;
					uint8_t manufacturerID;
					uint8_t memoryTypeID;
					uint8_t capacityID;
					uint32_t capacity;
					uint32_t eraseTime;
			};

			chipID _chip;
			struct eraseParam {
					bool supported;
					uint8_t opcode;
					uint32_t time;
			};

			eraseParam kb4Erase, kb32Erase, kb64Erase, kb256Erase, chipErase;
			uint8_t _noOfParamHeaders, _noOfBasicParamDwords;
			uint16_t _eraseTimeMultiplier, _prgmTimeMultiplier, _pageSize;
			uint32_t currentAddress, _currentAddress = 0;
			uint32_t _addressOverflow = false;
			uint32_t _BasicParamTableAddr, _SectorMapParamTableAddr, _byteFirstPrgmTime, _byteAddnlPrgmTime,
					_pagePrgmTime;
			uint8_t _uniqueID[8];
	};

	//--------------------------------- Public Templates ------------------------------------//

	// Writes any type of data to a specific location in the flash memory.
	// Takes three arguments -
	//  1. _addr --> Any address from 0 to maxAddress
	//  2. T& value --> Variable to write
	//  3. errorCheck --> Turned on by default. Checks for writing errors
	// WARNING: You can only write to previously erased memory locations (see datasheet).
	//      Use the eraseSector()/eraseBlock32K/eraseBlock64K commands to first clear memory (write 0xFFs)
	template <class T> bool SPIFlash::writeAnything(uint32_t _addr, const T& data, bool errorCheck) {
		return _write(_addr, data, sizeof(data), errorCheck, _STRUCT_);
	}

	// Reads any type of data from a specific location in the flash memory.
	// Takes three arguments -
	//  1. _addr --> Any address from 0 to maxAddress
	//  2. T& value --> Variable to return data into
	//  3. fastRead --> defaults to false - executes _beginFastRead() if set to true
	template <class T> bool SPIFlash::readAnything(uint32_t _addr, T& data, bool fastRead) {
		return _read(_addr, data, sizeof(data), fastRead);
	}

//---------------------------------- Private Templates ----------------------------------//

	template <class T> bool SPIFlash::_writeErrorCheck(uint32_t _addr, const T& value, uint32_t _sz,
			uint8_t _dataType) {
		if (_isChipPoweredDown() || !_addressCheck(_addr, _sz) || !_notBusy()) {
			return false;
		}
		const uint8_t* p = (const uint8_t*) (const void*) &value;

		CHIP_SELECT
		_spi.ReadWriteByte(READDATA);
		_transferAddress();
		for (uint16_t i = 0; i < _sz; i++) {
			if (*p++ != _spi.ReadWriteByte(0x00)) {
				_endSPI();
				return false;
			}
		}
		_endSPI();
		//}
		return true;
	}

// Writes any type of data to a specific location in the flash memory.
// Takes four arguments -
//  1. _addr --> Any address from 0 to maxAddress
//  2. T& value --> Variable to write
//  3. _sz --> Size of variable in bytes (1 byte = 8 bits)
//  4. errorCheck --> Turned on by default. Checks for writing errors
// WARNING: You can only write to previously erased memory locations (see datasheet).
//      Use the eraseSector()/eraseBlock32K/eraseBlock64K commands to first clear memory (write 0xFFs)

	template <class T> bool SPIFlash::_write(uint32_t _addr, const T& value, uint32_t _sz, bool errorCheck,
			uint8_t _dataType) {
		bool _retVal;

		uint32_t _addrIn = _addr;
		if (!_prep(PAGEPROG, _addrIn, _sz)) {
			return false;
		}
		_addrIn = _currentAddress;
		//Serial.print(F("_addrIn: "));
		//Serial.println(_addrIn, HEX);
		const uint8_t* p = ((const uint8_t*) (const void*) &value);
		//Serial.print(F("Address being written to: "));
		//Serial.println(_addr);
		uint32_t length = _sz;
		uint16_t maxBytes = SPI_PAGESIZE - (_addrIn % SPI_PAGESIZE); // Force the first set of bytes to stay within the first page

		if (!SPIBusState) {
			_startSPIBus();
		}
		CHIP_SELECT
		_spi.ReadWriteByte(PAGEPROG);
		_transferAddress();

		if (maxBytes > length) {
			for (uint16_t i = 0; i < length; ++i) {
				_spi.ReadWriteByte(*p++);
			}
			CHIP_DESELECT
		} else {
			uint32_t writeBufSz;
			uint16_t data_offset = 0;

			do {
				writeBufSz = (length <= maxBytes) ? length : maxBytes;
				if (_currentAddress % SPI_PAGESIZE == 0) {
					CHIP_SELECT
					_spi.ReadWriteByte(PAGEPROG);
					_transferAddress();
				}
				for (uint16_t i = 0; i < writeBufSz; ++i) {
					_spi.ReadWriteByte(*p++);
				}
				CHIP_DESELECT
				if (!_addressOverflow) {
					_currentAddress += writeBufSz;
				} else {
					if (data_offset >= _addressOverflow) {
						_currentAddress = 0x00;
						_addressOverflow = false;
					}
				}
				data_offset += writeBufSz;
				length -= writeBufSz;
				maxBytes = SPI_PAGESIZE;   // Now we can do up to 256 bytes per loop
				if (!_notBusy() || !_writeEnable()) {
					return false;
				}
			} while (length > 0);
		}

		if (!errorCheck) {
			_endSPI();
			return true;
		} else {
			//Serial.print(F("Address sent to error check: "));
			//Serial.println(_addr);
			_retVal = _writeErrorCheck(_addr, value, _sz, _dataType);
		}
		return _retVal;
	}

// Reads any type of data from a specific location in the flash memory.
// Takes four arguments -
//  1. _addr --> Any address from 0 to maxAddress
//  2. T& value --> Variable to return data into
//  3. _sz --> Size of the variable in bytes (1 byte = 8 bits)
//  4. fastRead --> defaults to false - executes _beginFastRead() if set to true
	template <class T> bool SPIFlash::_read(uint32_t _addr, T& value, uint32_t _sz, bool fastRead,
			uint8_t _dataType) {
		if (!_prep(READDATA, _addr, _sz)) {
			return false;
		} else {
			uint8_t* p = (uint8_t*) (void*) &value;

			if (_dataType == _STRING_) {
				_sz++;
				char _inChar[_sz];
				_beginSPI(READDATA);
				_nextBuf(READDATA, (uint8_t*) &(*_inChar), _sz);
				_endSPI();
				for (uint16_t i = 0; i < _sz; i++) {
					*p++ = _inChar[i];
				}
			} else {
				CHIP_SELECT
				if (fastRead) {
					_beginSPI(FASTREAD);
				} else {
					_beginSPI(READDATA);
				}
				for (uint16_t i = 0; i < _sz; i++) {
					*p++ = _spi.ReadWriteByte(0x00);
				}
				_endSPI();
			}
		}
		return true;
	}
}

using spiflash::SPIFlash;

#endif /* FLASH_SPI_SPIFLASH_H_ */
