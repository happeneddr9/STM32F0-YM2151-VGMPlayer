/**
 * @Name    : SPIFlash.cpp
 * @Date    : 2019年12月14日 下午3:42:13
 * @Author  : happe
 * @brief   :
 */

#include "SPIFlash.h"

namespace spiflash {
//Troubleshooting function. Called when #ifdef RUNDIAGNOSTIC is uncommented at the top of SPIMemory.h.
	void Diagnostics::troubleshoot(uint8_t _code, bool printoverride) {
		bool _printoverride;
		errorcode = _code;
		_printoverride = printoverride;
		if (_printoverride) {
			switch (_code) {
				case SUCCESS:
					//Serial.println(F("Function executed successfully"));
					break;

				case NORESPONSE:
					//Serial.println(F("Check your wiring. Flash chip is non-responsive."));
					break;

				case CALLBEGIN:
					//Serial.println(F("*constructor_of_choice*.begin() was not called in void setup()"));
					break;

				case UNKNOWNCHIP:
					//Serial.println(F("Unable to identify chip. Are you sure this chip is supported?"));
					//Serial.println(F("Chip details:"));
					//TODO: Insert a diagnostics subroutine here.
					break;

				case UNKNOWNCAP:
					//Serial.println(F("Unable to identify capacity. Is this chip officially supported? If not, please define a `CAPACITY` constant and include it in flash.begin(CAPACITY)."));
					break;

				case CHIPBUSY:
					//Serial.println(F("Chip is busy."));
					//Serial.println(F("Make sure all pins have been connected properly"));
					break;

				case OUTOFBOUNDS:
					//Serial.println(F("Page overflow has been disabled and the address called exceeds the memory"));
					break;

				case CANTENWRITE:
					//Serial.println(F("Unable to Enable Writing to chip."));
					//Serial.println(F("Please make sure the HOLD & WRITEPROTECT pins are pulled up to VCC"));
					break;

				case PREVWRITTEN:
					//Serial.println(F("This sector already contains data."));
					//Serial.println(F("Please make sure the sectors being written to are erased."));
					break;

				case LOWRAM:
					//Serial.println(F("You are running low on SRAM. Please optimise your program for better RAM usage"));
					/*#if defined (ARDUINO_ARCH_SAM)
					 Serial.print(F("Current Free SRAM: "));
					 Serial.println(freeRAM());
					 #endif*/
					break;

				case UNSUPPORTEDFUNC:
					//Serial.println(F("This function is not supported by the flash memory hardware."));
					break;

				case SYSSUSPEND:
					//Serial.println(F("Unable to suspend/resume operation."));
					break;

				case ERRORCHKFAIL:
					//Serial.println(F("Write Function has failed errorcheck."));
					break;

				case UNABLETO4BYTE:
					//Serial.println(F("Unable to enable 4-byte addressing."));
					break;

				case UNABLETO3BYTE:
					//Serial.println(F("Unable to disable 4-byte addressing."));
					break;

				case CHIPISPOWEREDDOWN:
					//Serial.println(F("The chip is currently powered down."));
					break;

				case NOSFDP:
					//Serial.println(F("The Flash chip does not support SFDP."));
					break;

				case NOSFDPERASEPARAM:
					//Serial.println(F("Unable to read Erase Parameters from chip. Reverting to library defaults."));
					break;

				case NOSFDPERASETIME:
					//Serial.println(F("Unable to read erase times from flash memory. Reverting to library defaults."));
					break;

				case NOSFDPPROGRAMTIMEPARAM:
					//Serial.println(F("Unable to read program times from flash memory. Reverting to library defaults."));
					break;

				case NOCHIPSELECTDECLARED:
					//Serial.println(F("No Chip Select pin defined in the custom SPI Array."));
					break;

				default:
					//Serial.println(F("Unknown error"));
					break;
			}
			if (_code == ERRORCHKFAIL || _code == CANTENWRITE || _code == UNKNOWNCHIP || _code == NORESPONSE) {
				//_printSupportLink();
			}
		}
	}

	Diagnostics diagnostics; // default instantiation of Diagnostics object

	SPIFlash::SPIFlash(GpioPin cs, Spi_lite& spiinterface)
			: _spi(spiinterface), CS(cs) {
		CS.init(GpioMode::Out);
		CS.setHigh();
	}

	bool SPIFlash::begin(uint32_t flashChipSize) {
		//_spi.setClockPrescaler(SPI_BaudRatePrescaler_4);
		bool retVal = _chipID(flashChipSize);
		CS.setHigh();
		return retVal;
	}

	uint8_t SPIFlash::error(bool _verbosity) {
		if (!_verbosity) {
			return diagnostics.errorcode;
		} else {
			_troubleshoot(diagnostics.errorcode, PRINTOVERRIDE);
			return diagnostics.errorcode;
		}
	}

//Checks for and initiates the chip by requesting the Manufacturer ID which is returned as a 16 bit int
	uint16_t SPIFlash::getManID(void) {
		uint8_t b1, b2;
		_beginSPI(MANID);
		_spi.ReadWriteByte(0x00);
		_spi.ReadWriteByte(0x00);
		_spi.ReadWriteByte(0x00);
		b1 = _spi.ReadWriteByte(0x00);
		b2 = _spi.ReadWriteByte(0x00);
		CHIP_DESELECT
		uint32_t id = b1;
		id = (id << 8) | (b2 << 0);
		return id;
	}

//Returns JEDEC ID which is returned as a 32 bit int
	uint32_t SPIFlash::getJEDECID(void) {
		uint32_t id = 0;
		id = _chip.manufacturerID;
		id = (id << 8) | (_chip.memoryTypeID << 0);
		id = (id << 8) | (_chip.capacityID << 0);
		return id;
	}

// Returns a 64-bit Unique ID that is unique to each flash memory chip
	uint64_t SPIFlash::getUniqueID(void) {
		_beginSPI(UNIQUEID);
		for (uint8_t i = 0; i < 4; i++) {
			_spi.ReadWriteByte(DUMMYBYTE);
		}
		if (address4ByteEnabled) {
			_spi.ReadWriteByte(DUMMYBYTE);
		}

		for (uint8_t i = 0; i < 8; i++) {
			_uniqueID[i] = _spi.ReadWriteByte(0x00);
		}
		CHIP_DESELECT

		long long _uid = 0;
		for (uint8_t i = 0; i < 8; i++) {
			_uid += _uniqueID[i];
			_uid = _uid << 8;
		}
		return _uid;
	}

//Gets the next available address for use.
// Takes the size of the data as an argument and returns a 32-bit address
// This function can be called anytime - even if there is preexisting data on the flash chip. It will simply find the next empty address block for the data.
	uint32_t SPIFlash::getAddress(uint16_t size) {
		if (!_addressCheck(currentAddress, size)) {
			return false;
		}
		while (!_notPrevWritten(currentAddress, size)) {
			currentAddress += size;
			_currentAddress = currentAddress;
			if (_currentAddress >= _chip.capacity) {
				if (_loopedOver) {
					return false;
				}
				currentAddress = 0x00;					// At end of memory - (pageOverflow)
				_loopedOver = true;
			}
		}
		uint32_t _addr = currentAddress;
		currentAddress += size;
		return _addr;
	}

//Returns capacity of chip
	uint32_t SPIFlash::getCapacity(void) {
		return _chip.capacity;
	}

//Returns maximum number of pages
	uint32_t SPIFlash::getMaxPage(void) {
		return (_chip.capacity / _pageSize);
	}

// Reads a byte of data from a specific location in a page.
//  Takes two arguments -
//    1. _addr --> Any address from 0 to capacity
//    2. fastRead --> defaults to false - executes _beginFastRead() if set to true
	uint8_t SPIFlash::readByte(uint32_t _addr, bool fastRead) {
		uint8_t data = 0;
		_read(_addr, data, sizeof(data), fastRead);
		return data;
	}

// Writes a byte of data to a specific location in a page.
//  Takes three arguments -
//    1. _addr --> Any address - from 0 to capacity
//    2. data --> One byte to be written to a particular location on a page
//    3. errorCheck --> Turned on by default. Checks for writing errors
// WARNING: You can only write to previously erased memory locations (see datasheet).
// Use the eraseSector()/eraseBlock32K/eraseBlock64K commands to first clear memory (write 0xFFs)
	bool SPIFlash::writeByte(uint32_t _addr, uint8_t data, bool errorCheck) {
		//return _write(_addr, data, sizeof(data), errorCheck, _BYTE_);
		if (!_prep(PAGEPROG, _addr, sizeof(data))) {
			return false;
		}

		_beginSPI(PAGEPROG);
		_spi.ReadWriteByte(data);
		CHIP_DESELECT

		if (!errorCheck) {
			_endSPI();
			return true;
		} else {
			if (!_notBusy()) {
				return false;
			}
			_currentAddress = _addr;
			CHIP_SELECT
			_spi.ReadWriteByte(READDATA);
			_transferAddress();
			if (data != _spi.ReadWriteByte(0x00)) {
				_endSPI();
				return false;
			} else {
				_endSPI();
				return true;
			}
		}
		return true;
	}

	bool SPIFlash::write_n(uint32_t _addr, uint8_t* data, size_t len, bool errorCheck) {
		if (!_prep(PAGEPROG, _addr, len)) {
			return false;
		}
		uint16_t maxBytes = SPI_PAGESIZE - (_addr % SPI_PAGESIZE); // Force the first set of bytes to stay within the first page

		if (len <= maxBytes) {
			CHIP_SELECT
			_spi.ReadWriteByte(PAGEPROG);
			_transferAddress();
			//_nextBuf(PAGEPROG, &data_buffer[0], bufferSize);
			for (uint16_t i = 0; i < len; ++i) {
				_spi.ReadWriteByte(data[i]);
			}
			CHIP_DESELECT
		} else {
			uint16_t length = len;
			uint16_t writeBufSz;
			uint16_t data_offset = 0;

			do {
				writeBufSz = (length <= maxBytes) ? length : maxBytes;

				CHIP_SELECT
				_spi.ReadWriteByte(PAGEPROG);
				_transferAddress();
				//_nextBuf(PAGEPROG, &data_buffer[data_offset], writeBufSz);
				for (uint16_t i = 0; i < writeBufSz; ++i) {
					_spi.ReadWriteByte(data[data_offset + i]);
				}
				CHIP_DESELECT

				_currentAddress += writeBufSz;
				data_offset += writeBufSz;
				length -= writeBufSz;
				maxBytes = 256;   // Now we can do up to 256 bytes per loop

				if (!_notBusy() || !_writeEnable()) {
					return false;
				}

			} while (length > 0);
		}

		if (!errorCheck) {
			_endSPI();
			return true;
		} else {
			if (!_notBusy()) {
				return false;
			}
			_currentAddress = _addr;
			CHIP_SELECT
			_spi.ReadWriteByte(READDATA);
			_transferAddress();
			for (uint16_t j = 0; j < len; j++) {
				if (_spi.ReadWriteByte(0x00) != data[j]) {
					return false;
				}
			}
			_endSPI();
			return true;
		}
	}

	bool SPIFlash::read_n(uint32_t _addr, uint8_t* data, size_t len, bool fastRead) {
		if (!_prep(READDATA, _addr, len)) {
			return false;
		}
		if (fastRead) {
			_beginSPI(FASTREAD);
		} else {
			_beginSPI(READDATA);
		}
		_nextBuf(READDATA, data, len);
		_endSPI();
		return true;
	}

	// Erases a number of sectors or blocks as needed by the data being input.
	//  Takes an address and the size of the data being input as the arguments and erases the block/s of memory containing the address.
	bool SPIFlash::eraseSection(uint32_t _addr, uint32_t _sz) {

		if (!_prep(ERASEFUNC, _addr, _sz)) {
			return false;
		}

		// If size of data is > 4KB more than one sector needs to be erased. So the number of erase sessions is determined by the quotient of _sz/KB(4). If the _sz is not perfectly divisible by KB(4), then an additional sector has to be erased.
		uint32_t noOfEraseRunsB4Boundary = 0;
		uint32_t noOf4KBEraseRuns, KB64Blocks, KB32Blocks, KB4Blocks, totalBlocks;

		if (_sz / bytes_of_kB(4)) {
			noOf4KBEraseRuns = _sz / bytes_of_kB(4);
		} else {
			noOf4KBEraseRuns = 1;
		}
		KB64Blocks = noOf4KBEraseRuns / 16;
		KB32Blocks = (noOf4KBEraseRuns % 16) / 8;
		KB4Blocks = (noOf4KBEraseRuns % 8);
		totalBlocks = KB64Blocks + KB32Blocks + KB4Blocks;
		//Serial.print(F("noOf4KBEraseRuns: "));
		//Serial.println(noOf4KBEraseRuns);
		//Serial.print(F("totalBlocks: "));
		//Serial.println(totalBlocks);

		uint16_t _eraseFuncOrder[totalBlocks];

		if (KB64Blocks) {
			for (uint32_t i = 0; i < KB64Blocks; i++) {
				_eraseFuncOrder[i] = kb64Erase.opcode;
			}
		}
		if (KB32Blocks) {
			for (uint32_t i = KB64Blocks; i < (KB64Blocks + KB32Blocks); i++) {
				_eraseFuncOrder[i] = kb32Erase.opcode;
			}
		}
		if (KB4Blocks) {
			for (uint32_t i = (KB64Blocks + KB32Blocks); i < totalBlocks; i++) {
				_eraseFuncOrder[i] = kb4Erase.opcode;
			}
		}

		// Now that the number of blocks to be erased have been calculated and the information saved, the erase function is carried out.
		if (_addressOverflow) {
			noOfEraseRunsB4Boundary = (_sz - _addressOverflow) / 16;
			noOfEraseRunsB4Boundary += ((_sz - _addressOverflow) % 16) / 8;
			noOfEraseRunsB4Boundary += ((_sz - _addressOverflow) % 8);
			//Serial.print(F("noOfEraseRunsB4Boundary: "));
			//Serial.println(noOfEraseRunsB4Boundary);
		}
		if (!_addressOverflow) {
			for (uint32_t j = 0; j < totalBlocks; j++) {
				_beginSPI(_eraseFuncOrder[j]);   //The address is transferred as a part of this function
				_endSPI();

				//Serial.printF("_eraseFuncOrder: 0x"));
				//Serial.println(_eraseFuncOrder[j], HEX);

				uint16_t _timeFactor = 0;
				if (_eraseFuncOrder[j] == kb64Erase.opcode) {
					_timeFactor = kb64Erase.time;
				}
				if (_eraseFuncOrder[j] == kb32Erase.opcode) {
					_timeFactor = kb32Erase.time;
				}
				if (_eraseFuncOrder[j] == kb4Erase.opcode) {
					_timeFactor = kb4Erase.time;
				}
				if (!_notBusy(_timeFactor * 1000L)) {
					return false;
				}
				if (j == noOfEraseRunsB4Boundary) {
					if (!_prep(ERASEFUNC, (_addr + (_sz - _addressOverflow)), _sz)) {
						return false;
					}
					//Serial.print(F("Overflow triggered"));
				}
			}
		}

		return true;
	}

	// Erases one 4k sector.
	//  Takes an address as the argument and erases the block containing the address.
	bool SPIFlash::eraseSector(uint32_t _addr) {
		if (!kb4Erase.supported) {
			_troubleshoot(UNSUPPORTEDFUNC);
			return false;
		}

		if (!_prep(ERASEFUNC, _addr, bytes_of_kB(4))) {
			return false;
		}
		_beginSPI(kb4Erase.opcode);   //The address is transferred as a part of this function
		_endSPI();

		if (!_notBusy(kb4Erase.time)) {
			return false;	//Datasheet says erasing a sector takes 400ms max
		}

		return true;
	}

	// Erases one 32k block.
	//  Takes an address as the argument and erases the block containing the address.
	bool SPIFlash::eraseBlock32K(uint32_t _addr) {
		if (!kb32Erase.supported) {
			_troubleshoot(UNSUPPORTEDFUNC);
			return false;
		}

		if (!_prep(ERASEFUNC, _addr, bytes_of_kB(32))) {
			return false;
		}
		_beginSPI(kb32Erase.opcode);
		_endSPI();

		if (!_notBusy(kb32Erase.time)) {
			return false;	//Datasheet says erasing a sector takes 400ms max
		}
		_writeDisable();

		return true;
	}

	// Erases one 64k block.
	//  Takes an address as the argument and erases the block containing the address.
	bool SPIFlash::eraseBlock64K(uint32_t _addr) {
		if (!kb64Erase.supported) {
			_troubleshoot(UNSUPPORTEDFUNC);
			return false;
		}

		if (!_prep(ERASEFUNC, _addr, bytes_of_kB(64))) {
			return false;
		}

		_beginSPI(kb64Erase.opcode);
		_endSPI();

		if (!_notBusy(kb64Erase.time)) {
			return false;	//Datasheet says erasing a sector takes 400ms max
		}
		return true;
	}

	//Erases whole chip. Think twice before using.
	bool SPIFlash::eraseChip(void) {
		if (_isChipPoweredDown() || !_notBusy() || !_writeEnable()) {
			return false;
		}

		_beginSPI(chipErase.opcode);
		_endSPI();

		while (_readStat1() & BUSY) {
			//_delay_us(30000L);
		}
		_endSPI();
		return true;

	}

	/* Power functions */

	//Suspends current Block Erase/Sector Erase/Page Program. Does not suspend chipErase().
	//Page Program, Write Status Register, Erase instructions are not allowed.
	//Erase suspend is only allowed during Block/Sector erase.
	//Program suspend is only allowed during Page/Quad Page Program
	bool SPIFlash::suspendProg(void) {
		if (_isChipPoweredDown() || _notBusy()) {
			return false;
		}

		if (!_noSuspend()) {
			return true;
		}

		_beginSPI(SUSPEND);
		_endSPI();
		_delay_us(20);
		if (!_notBusy(50) || _noSuspend()) {
			return false;
		}
		return true;
	}

	//Resumes previously suspended Block Erase/Sector Erase/Page Program.
	bool SPIFlash::resumeProg(void) {
		if (_isChipPoweredDown() || !_notBusy() || _noSuspend()) {
			return false;
		}

		_beginSPI(RESUME);
		_endSPI();

		_delay_us(20);

		if (_notBusy(10) || !_noSuspend()) {
			return false;
		}
		return true;

	}

	//Puts device in low power state. Good for battery powered operations.
	//In powerDown() the chip will only respond to powerUp()
	bool SPIFlash::powerDown(void) {
		if (_chip.manufacturerID != MICROCHIP_MANID) {
			if (!_notBusy(20))
				return false;

			_beginSPI(POWERDOWN);
			_endSPI();

			_delay_us(5);
			chipPoweredDown = true;
			return !_writeEnable(false);
		} else {
			_troubleshoot(UNSUPPORTEDFUNC);
			return false;
		}
	}

	//Wakes chip from low power state.
	bool SPIFlash::powerUp(void) {
		_beginSPI(RELEASE);
		_endSPI();
		_delay_us(3);						    //Max release enable time according to the Datasheet

		if (_writeEnable(false)) {
			_writeDisable();
			chipPoweredDown = false;
			return true;
		}
		return false;
	}

} //namespace spiflash
