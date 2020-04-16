/**
 * @Name    : SPIFlashIO.cpp
 * @Date    : 2019年12月14日 下午4:58:03
 * @Author  : happe
 * @brief   :
 */

#include "SPIFlash.h"

#define Lo(param) ((char *)&param)[0] //0x000y
#define Hi(param) ((char *)&param)[1] //0x00y0
#define Higher(param) ((char *)&param)[2] //0x0y00
#define Highest(param) ((char *)&param)[3] //0xy000

namespace spiflash {
	//Troubleshooting function. Called when #ifdef RUNDIAGNOSTIC is uncommented at the top of this file.
	void SPIFlash::_troubleshoot(uint8_t _code, bool printoverride) {
		diagnostics.troubleshoot(_code, printoverride);
	}

	// Transfer Address.
	bool SPIFlash::_transferAddress(void) {
		if (address4ByteEnabled) {
			_spi.ReadWriteByte(Highest(_currentAddress));
		}
		_spi.ReadWriteByte(Higher(_currentAddress));
		_spi.ReadWriteByte(Hi(_currentAddress));
		_spi.ReadWriteByte(Lo(_currentAddress));
		return true;
	}

	//Initiates SPI operation - but data is not transferred yet. Always call _prep() before this function (especially when it involves writing or reading to/from an address)
	bool SPIFlash::_beginSPI(uint8_t opcode) {
		CHIP_SELECT
		switch (opcode) {
			case READDATA:
				_spi.ReadWriteByte(opcode);
				_transferAddress();
				break;

			case PAGEPROG:
				_spi.ReadWriteByte(opcode);
				_transferAddress();
				break;

			case FASTREAD:
				_spi.ReadWriteByte(opcode);
				_transferAddress();
				_spi.ReadWriteByte(DUMMYBYTE);
				break;

			case SECTORERASE:
				_spi.ReadWriteByte(opcode);
				_transferAddress();
				break;

			case BLOCK32ERASE:
				_spi.ReadWriteByte(opcode);
				_transferAddress();
				break;

			case BLOCK64ERASE:
				_spi.ReadWriteByte(opcode);
				_transferAddress();
				break;

			default:
				_spi.ReadWriteByte(opcode);
				break;
		}
		return true;
	}

//Stops all operations. Should be called after all the required data is read/written from repeated _nextByte() calls
	void SPIFlash::_endSPI(void) {
		CHIP_DESELECT
		if (address4ByteEnabled) {          // If the previous operation enabled 4-byte addressing, disable it
			_disable4ByteAddressing();
		}
		SPIBusState = false;
	}

	// Polls the status register 1 until busy flag is cleared or timeout
	bool SPIFlash::_notBusy(uint32_t timeout) {
		_delay_us(WINBOND_WRITE_DELAY);
		uint32_t _time = micros();

		do {
			_readStat1();
			if (!(stat1 & BUSY)) {
				return true;
			}

		} while ((micros() - _time) < timeout);
		if (timeout <= (micros() - _time)) {
			_troubleshoot(CHIPBUSY);
			return false;
		}
		return true;
	}

	//Enables writing to chip by setting the WRITEENABLE bit
	bool SPIFlash::_writeEnable(bool _troubleshootEnable) {
		_beginSPI(WRITEENABLE);
		CHIP_DESELECT
		if (!(_readStat1() & WRTEN)) {
			if (_troubleshootEnable) {
				_troubleshoot(CANTENWRITE);
			}
			return false;
		}
		return true;
	}

	//Disables writing to chip by setting the Write Enable Latch (WEL) bit in the Status Register to 0
	//_writeDisable() is not required under the following conditions because the Write Enable Latch (WEL) flag is cleared to 0
	// i.e. to write disable state:
	// Power-up, Write Disable, Page Program, Quad Page Program, Sector Erase, Block Erase, Chip Erase, Write Status Register,
	// Erase Security Register and Program Security register
	bool SPIFlash::_writeDisable(void) {
		_beginSPI(WRITEDISABLE);
		CHIP_DESELECT
		return true;
	}

	//Checks for presence of chip by requesting JEDEC ID
	bool SPIFlash::_getJedecId(void) {
		if (!_notBusy()) {
			return false;
		}
		_beginSPI(JEDECID);
		_chip.manufacturerID = _spi.ReadWriteByte(0x00);		// manufacturer id
		_chip.memoryTypeID = _spi.ReadWriteByte(0x00);		// memory type
		_chip.capacityID = _spi.ReadWriteByte(0x00);		// capacity
		CHIP_DESELECT
		if (!_chip.manufacturerID) {
			_troubleshoot(NORESPONSE);
			return false;
		} else {
			return true;
		}
	}

//Identifies the chip
	bool SPIFlash::_chipID(uint32_t flashChipSize) {
		//set some default values
		kb4Erase.supported = kb32Erase.supported = kb64Erase.supported = chipErase.supported = true;
		kb4Erase.opcode = SECTORERASE;
		kb32Erase.opcode = BLOCK32ERASE;
		kb64Erase.opcode = BLOCK64ERASE;
		kb4Erase.time = BUSY_TIMEOUT;
		kb32Erase.time = kb4Erase.time * 8;
		kb64Erase.time = kb32Erase.time * 4;
		kb256Erase.supported = false;
		chipErase.opcode = CHIPERASE;
		chipErase.time = kb64Erase.time * 100L;
		_pageSize = SPI_PAGESIZE;

		_getJedecId();

		for (uint8_t i = 0; i < sizeof(_supportedManID); i++) {
			if (_chip.manufacturerID == _supportedManID[i]) {
				_chip.supportedMan = true;
				break;
			}
		}

		for (uint8_t i = 0; i < sizeof(_altChipEraseReq); i++) {
			if (_chip.memoryTypeID == _altChipEraseReq[i]) {
				chipErase.opcode = ALT_CHIPERASE;
				break;
			}
		}

		if (_chip.supportedMan) {
			//Identify capacity
			if (_chip.manufacturerID == MACRONIX_MANID) {
				switch (_chip.capacityID) {
					case MX25L4005:
						_chip.capacity = bytes_of_MB(4);
						break;

					case MX25L8005:
						_chip.capacity = bytes_of_MB(8);
						break;

					default:
						break;
				} //TODO - Set up other manufaturerIDs in a similar pattern.
			}
			for (uint8_t j = 0; j < sizeof(_capID); j++) {
				if (_chip.capacityID == _capID[j]) {
					_chip.capacity = (_memSize[j]);
					_chip.supported = true;
					return true;
				}
			}
		} else {
			if (_chip.sfdpAvailable) {
				return true;
			} else {
				return false;
			}

		}

		if (!_chip.capacity) {
			if (flashChipSize) {
				_chip.capacity = flashChipSize;
				_chip.supported = false;
				return true;
			} else {
				return false;
			}

		}
		return true;
	}

//Checks to see if page overflow is permitted and assists with determining next address to read/write.
//Sets the global address variable
	bool SPIFlash::_addressCheck(uint32_t _addr, uint32_t size) {
		uint32_t _submittedAddress = _addr;
		uint8_t _errorcode = error();
		if (_errorcode == UNKNOWNCAP || _errorcode == NORESPONSE) {
			return false;
		}
		if (!_chip.capacity) {
			return false;
		}

		if (_submittedAddress + size >= _chip.capacity) {
			_addressOverflow = ((_submittedAddress + size) - _chip.capacity);
			_currentAddress = _addr;
			//Serial.print(F("_addressOverflow: "));
			//Serial.println(_addressOverflow, HEX);
			return true;					// At end of memory - (pageOverflow)
		} else {
			_addressOverflow = false;
			_currentAddress = _addr;
			return true;				// Not at end of memory if (address < _chip.capacity)
		}
	}

// Checks to see if the block of memory has been previously written to
	bool SPIFlash::_notPrevWritten(uint32_t _addr, uint32_t size) {
		//uint8_t _dat;
		_beginSPI(READDATA);
		for (uint32_t i = 0; i < size; i++) {
			if (_spi.ReadWriteByte(0x00) != 0xFF) {
				CHIP_DESELECT
				return false;
			}
		}
		CHIP_DESELECT
		return true;
	}

	// Checks to see if chip is powered down. If it is, retrns true. If not, returns false.
	bool SPIFlash::_isChipPoweredDown(void) {
		if (chipPoweredDown) {
			_troubleshoot(CHIPISPOWEREDDOWN);
			return true;
		} else {
			return false;
		}
	}

	//Double checks all parameters before calling a read or write. Comes in two variants
	//Takes address and returns the address if true, else returns false. Throws an error if there is a problem.
	bool SPIFlash::_prep(uint8_t opcode, uint32_t _addr, uint32_t size) {
		// If the flash memory is >= 256 MB enable 4-byte addressing
		if (_chip.manufacturerID == WINBOND_MANID && _addr >= bytes_of_MB(16)) {
			if (!_enable4ByteAddressing()) {    // If unable to enable 4-byte addressing
				return false;
			}          // TODO: Add SFDP compatibility here
		}
		switch (opcode) {
			case PAGEPROG:
				if (_isChipPoweredDown() || !_addressCheck(_addr, size) || !_notPrevWritten(_addr, size) || !_notBusy() || !_writeEnable()) {
					return false;
				}
				return true;
				break;

			case ERASEFUNC:
				if (_isChipPoweredDown() || !_addressCheck(_addr, size) || !_notBusy() || !_writeEnable()) {
					return false;
				}
				return true;
				break;

			default:
				if (_isChipPoweredDown() || !_addressCheck(_addr, size) || !_notBusy()) {
					return false;
				}
				return true;
				break;
		}
	}

	bool SPIFlash::_startSPIBus(void) {
		// SPI.setClockDivider(_clockdiv);
		// SPI.setDataMode(SPI_MODE0);
		// SPI.setBitOrder(MSBFIRST);
		//SPIBusState = true;
		return true;
	}

	// Checks if status register 1 can be accessed - used to check chip status, during powerdown and power up and for debugging
	uint8_t SPIFlash::_readStat1(void) {
		_beginSPI(READSTAT1);
		stat1 = _spi.ReadWriteByte(0x00);
		CHIP_DESELECT
		return stat1;
	}

	// Checks if status register 2 can be accessed, if yes, reads and returns it
	uint8_t SPIFlash::_readStat2(void) {
		_beginSPI(READSTAT2);
		stat2 = _spi.ReadWriteByte(0x00);
		//stat2 = _nextByte(READ);
		CHIP_DESELECT
		return stat2;
	}

	// Checks if status register 3 can be accessed, if yes, reads and returns it
	uint8_t SPIFlash::_readStat3(void) {
		_beginSPI(READSTAT3);
		stat3 = _spi.ReadWriteByte(0x00);
		//stat2 = _nextByte(READ);
		CHIP_DESELECT
		return stat3;
	}

// Checks to see if 4-byte addressing is already enabled and if not, enables it
	bool SPIFlash::_enable4ByteAddressing(void) {
		if (_readStat3() & ADS) {
			return true;
		}
		_beginSPI(ADDR4BYTE_EN);
		CHIP_DESELECT
		if (_readStat3() & ADS) {
			address4ByteEnabled = true;
			return true;
		} else {
			return false;
		}
	}

// Checks to see if 4-byte addressing is already disabled and if not, disables it
	bool SPIFlash::_disable4ByteAddressing(void) {
		if (!(_readStat3() & ADS)) {      // If 4 byte addressing is disabled (default state)
			return true;
		}
		_beginSPI(ADDR4BYTE_DIS);
		CHIP_DESELECT
		if (_readStat3() & ADS) {
			return false;
		}
		address4ByteEnabled = false;
		return true;
	}

//Reads/Writes next data buffer. Should be called after _beginSPI()
	void SPIFlash::_nextBuf(uint8_t opcode, uint8_t *data_buffer, uint32_t size) {
		uint8_t *_dataAddr = &(*data_buffer);

		switch (opcode) {
			case READDATA:
				for (uint16_t i = 0; i < size; i++) {
					*_dataAddr = _spi.ReadWriteByte(0x00);
					_dataAddr++;
				}
				break;

			case PAGEPROG:
				for (uint16_t i = 0; i < size; i++) {
					_spi.ReadWriteByte(*_dataAddr);
					_dataAddr++;
				}
				break;
		}
	}

} // namespace spiflash

