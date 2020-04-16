/**
 * @Name    : SdFat.h
 * @Date    : 2020年1月1日 下午2:47:51
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_SDFAT_H_
#define SDCARD_SDFAT_H_

#include "SysCall.h"
#include "BlockDriver.h"
#include "FATFS/FatLib.h"

namespace SDCard {
	//==============================================================================
	/**
	 * \class SdBaseFile
	 * \brief Class for backward compatibility.
	 */
	class SdBaseFile : public FatFile {
		public:
			SdBaseFile() {

			}
			/**  Create a file object and open it in the current working directory.
			 *
			 * \param[in] path A path for a file to be opened.
			 *
			 * \param[in] oflag Values for \a oflag are constructed by a
			 * bitwise-inclusive OR of open flags. see
			 * FatFile::open(FatFile*, const char*, oflag_t).
			 */
			SdBaseFile(const char* path, oflag_t oflag)
					: FatFile(path, oflag) {
			}
	};

	/**
	 * \class SdFile
	 * \brief Class for backward compatibility.
	 */
	class SdFile : public PrintFile {
		public:
			SdFile() {
			}
			/**  Create a file object and open it in the current working directory.
			 *
			 * \param[in] path A path for a file to be opened.
			 *
			 * \param[in] oflag Values for \a oflag are constructed by a
			 * bitwise-inclusive OR of open flags. see
			 * FatFile::open(FatFile*, const char*, oflag_t).
			 */
			SdFile(const char* path, oflag_t oflag)
					: PrintFile(path, oflag) {
			}
	};

	//-----------------------------------------------------------------------------
	/**
	 * \class SdFileSystem
	 * \brief Virtual base class for %SdFat library.
	 */
	template <class SdDriverClass>
	class SdFileSystem : public FatFileSystem {
		public:

			SdFileSystem(SdDriverClass& sddriver)
					: FatFileSystem(),  m_card(sddriver) {

			}

			/** Initialize file system.
			 * \return true for success else false.
			 */
			bool begin() {
				return FatFileSystem::begin(&m_card);
			}
			/** \return Pointer to SD card object */
			SdDriverClass *card() {
				m_card.syncBlocks();
				return &m_card;
			}
			/** %Print any SD error code to Serial and halt. */
			void errorHalt(HWSerial& Serial) {
				errorHalt(&Serial);
			}
			/** %Print any SD error code and halt.
			 *
			 * \param[in] pr Print destination.
			 */
			void errorHalt(Print* pr) {
				errorPrint(pr);
				SysCall::halt();
			}
			/** %Print msg, any SD error code and halt.
			 *
			 * \param[in] msg Message to print.
			 */
			void errorHalt(HWSerial& Serial, char const* msg) {
				errorHalt(&Serial, msg);
			}
			/** %Print msg, any SD error code, and halt.
			 *
			 * \param[in] pr Print destination.
			 * \param[in] msg Message to print.
			 */
			void errorHalt(Print* pr, char const* msg) {
				errorPrint(pr, msg);
				SysCall::halt();
			}
			/** %Print any SD error code to Serial */
			void errorPrint(HWSerial& Serial) {
				errorPrint(&Serial);
			}
			/** %Print any SD error code.
			 * \param[in] pr Print device.
			 */
			void errorPrint(Print* pr) {
				if (!cardErrorCode()) {
					return;
				}
				pr->print(F("SD errCode: 0X"));
				pr->print(cardErrorCode(), HEX);
				pr->print(F(",0X"));
				pr->println(cardErrorData(), HEX);
			}
			/** %Print msg, any SD error code.
			 *
			 * \param[in] msg Message to print.
			 */
			void errorPrint(HWSerial& Serial, const char* msg) {
				errorPrint(&Serial, msg);
			}
			/** %Print msg, any SD error code.
			 *
			 * \param[in] pr Print destination.
			 * \param[in] msg Message to print.
			 */
			void errorPrint(Print* pr, char const* msg) {
				pr->print(F("err: "));
				pr->println(msg);
				errorPrint(pr);
			}
			/** %Print any SD error code and halt. */
			void initErrorHalt(HWSerial& Serial) {
				initErrorHalt(&Serial);
			}
			/** %Print error details and halt after begin fails.
			 *
			 * \param[in] pr Print destination.
			 */
			void initErrorHalt(Print* pr) {
				initErrorPrint(pr);
				SysCall::halt();
			}
			/**Print message, error details, and halt after begin() fails.
			 *
			 * \param[in] msg Message to print.
			 */
			void initErrorHalt(HWSerial& Serial, char const *msg) {
				initErrorHalt(&Serial, msg);
			}
			/**Print message, error details, and halt after begin() fails.
			 * \param[in] pr Print device.
			 * \param[in] msg Message to print.
			 */
			void initErrorHalt(Print* pr, char const *msg) {
				pr->println(msg);
				initErrorHalt(pr);
			}

			/** Print error details after begin() fails. */
			void initErrorPrint(HWSerial& Serial) {
				initErrorPrint(&Serial);
			}
			/** Print error details after begin() fails.
			 *
			 * \param[in] pr Print destination.
			 */
			void initErrorPrint(Print* pr) {
				if (cardErrorCode()) {
					pr->println(F("Can't access SD card. Do not reformat."));
					if (cardErrorCode() == SD_CARD_ERROR_CMD0) {
						pr->println(F("No card, wrong chip select pin, or SPI problem?"));
					}
					errorPrint(pr);
				} else if (vol()->fatType() == 0) {
					pr->println(F("Invalid format, reformat SD."));
				} else if (!vwd()->isOpen()) {
					pr->println(F("Can't open root directory."));
				} else {
					pr->println(F("No error found."));
				}
			}
			/**Print message and error details and halt after begin() fails.
			 *
			 * \param[in] msg Message to print.
			 */
			void initErrorPrint(HWSerial& Serial, char const *msg) {
				initErrorPrint(&Serial, msg);
			}
			/**Print message and error details and halt after begin() fails.
			 *
			 * \param[in] pr Print destination.
			 * \param[in] msg Message to print.
			 */
			void initErrorPrint(Print* pr, char const *msg) {
				pr->println(msg);
				initErrorPrint(pr);
			}
			/** \return The card error code */
			uint8_t cardErrorCode() {
				return m_card.errorCode();
			}
			/** \return the card error data */
			uint32_t cardErrorData() {
				return m_card.errorData();
			}

		protected:
			SdDriverClass& m_card;
	};

	//==============================================================================
	/**
	 * \class SdFat
	 * \brief Main file system class for %SdFat library.
	 */
	class SdFat : public SdFileSystem<SDCard_Spi> {
		public:
			/** SdFat constructor
			 *  @param[in]:		which pin you want to use for SD Card chip select
			 *  @param[in]:		which SPI Group use for SD Card
			 */
			SdFat(GpioPin mCS, Spi_dma& Spix)
					: SdFileSystem(Card_Driver), m_spi(Spix), Card_Driver(mCS, m_spi) {

			}

			/** Initialize SD card and file system.
			 *
			 * \param[in] csPin SD card chip select pin.				# moved to constructor
			 * \param[in] spiSettings SPI speed, mode, and bit order.	# used default speed
			 * \return true for success else false.
			 */
			bool begin(void) {
				return m_card.begin() && SdFileSystem::begin();
			}
			/** Initialize SD card for diagnostic use only.
			 *
			 * \param[in] csPin SD card chip select pin.
			 * \param[in] settings SPI speed, mode, and bit order.
			 * \return true for success else false.
			 */
			bool cardBegin() {
				return m_card.begin();
			}
			/** Initialize file system for diagnostic use only.
			 * \return true for success else false.
			 */
			bool fsBegin() {
				return FatFileSystem::begin(card());
			}

		private:
			Spi_dma& m_spi;
			SDCard_Spi Card_Driver;
	};

	//=============================================================================
	/**
	 * \class Sd2Card
	 * \brief Raw access to SD and SDHC card using default SPI library.
	 */
	class Sd2Card : public SDCard_Spi {
		public:

			Sd2Card(GpioPin mCS, Spi_dma& Spix)
					: SDCard_Spi(mCS, Spix), m_spi(Spix) {

			}
			/** Initialize the SD card.
			 * \param[in] csPin SD chip select pin.
			 * \param[in] settings SPI speed, mode, and bit order.
			 * \return true for success else false.
			 */
			bool begin() {
				return SDCard_Spi::begin();
			}
		private:
			Spi_dma m_spi;
	};
}

using SDCard::SdFat;
using SDCard::SdBaseFile;
using SDCard::SdFile;

#endif /* SDCARD_SDFAT_H_ */
