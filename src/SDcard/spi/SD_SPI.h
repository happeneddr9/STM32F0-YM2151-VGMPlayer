/**
 * @Name    : SD_SPI.h
 * @Date    : 2019年12月29日 下午3:31:55
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_SPI_SD_SPI_H_
#define SDCARD_SPI_SD_SPI_H_

#include "Arduino/Basic.h"
#include "SPI/SPI.hpp"
#include "SDInfo.h"

#include "SDcard/FATFS/BaseBlockDriver.h"
#include "SDcard/SysCall.h"

namespace SDCard{
	class SDCard_Spi : public BaseBlockDriver {
		public:
			SDCard_Spi(GpioPin mcs, Spi_dma& Spix);

			bool begin(void);
			uint8_t init(void);

			uint8_t getCID(uint8_t* CID_Data);		//16-bytes CID
			uint8_t getCSD(uint8_t* CSD_Data);		//16-bytes CSD
			uint32_t getCapacity_kB(void);

			/**
			 * Determine the size of an SD flash memory card.
			 *
			 * \return The number of 512 byte data blocks in the card
			 *         or zero if an error occurs.
			 */
			uint32_t cardSize();
			/** Clear debug stats. */
			void dbgClearStats();
			/** Print debug stats. */
			void dbgPrintStats();
			/** Erase a range of blocks.
			 *
			 * \param[in] firstBlock The address of the first block in the range.
			 * \param[in] lastBlock The address of the last block in the range.
			 *
			 * \note This function requests the SD card to do a flash erase for a
			 * range of blocks.  The data on the card after an erase operation is
			 * either 0 or 1, depends on the card vendor.  The card must support
			 * single block erase.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool erase(uint32_t beginblock, uint32_t endblock);
			/** Determine if card supports single block erase.
			 *
			 * \return true is returned if single block erase is supported.
			 * false is returned if single block erase is not supported.
			 */
			bool eraseSingleBlockEnable() {
				csd_t csd;
				return getCSD(reinterpret_cast<uint8_t*>(&csd)) ? csd.v1.erase_blk_en : false;
			}

			bool readStatus(uint8_t* status);

			/** Read one data block in a multiple block read sequence
			 *
			 * \param[out] dst Pointer to the location for the data to be read.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool readData(uint8_t *dst);
			/** Read OCR register.
			 *
			 * \param[out] ocr Value of OCR register.
			 * \return true for success else false.
			 */
			bool readOCR(uint32_t* ocr);
			/** Start a read multiple blocks sequence.
			 *
			 * \param[in] blockNumber Address of first block in sequence.
			 *
			 * \note This function is used with readData() and readStop() for optimized
			 * multiple block reads.  SPI chipSelect must be low for the entire sequence.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool readStart(uint32_t blockNumber);
			/** End a read multiple blocks sequence.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool readStop();

			bool syncBlocks() override {
				return true;
			}

			bool readBlock(uint32_t block, uint8_t* dst) override;

			bool writeBlock(uint32_t block, const uint8_t* src) override;

			bool readBlocks(uint32_t block, uint8_t* dst, size_t nb) override;

			bool writeBlocks(uint32_t block, const uint8_t* src, size_t nb) override;


			uint8_t writeOneBlock(uint32_t sector, const uint8_t * buffer);
			uint8_t readOneBlock(uint32_t sector, uint8_t * buffer);

			uint8_t writeMultiBlock(uint32_t sector, const uint8_t * buffer, uint8_t counts, bool pre_erase = false);
			uint8_t readMultiBlock(uint32_t sector, uint8_t * buffer, uint8_t counts);

			/**
			 *  Set SD error code.
			 *  \param[in] code value for error code.
			 */
			void error(uint8_t code) {
				m_errorCode = code;
			}
			/**
			 * \return code for the last error. See SdInfo.h for a list of error codes.
			 */
			int errorCode() const {
				return m_errorCode;
			}
			/** \return error data for last error. */
			int errorData() const {
				return m_status;
			}
			/**
			 * Check for busy.  MISO low indicates the card is busy.
			 *
			 * \return true if busy else false.
			 */
			bool isBusy();
			/**
			 * Read a card's CID register. The CID contains card identification
			 * information such as Manufacturer ID, Product name, Product serial
			 * number and Manufacturing date.
			 *
			 * \param[out] cid pointer to area for returned data.
			 *
			 * \return true for success or false for failure.
			 */
			bool readCID(cid_t* cid) {
				return readRegister(CMD10, cid);
			}
			/**
			 * Read a card's CSD register. The CSD contains Card-Specific Data that
			 * provides information regarding access to the card's contents.
			 *
			 * \param[out] csd pointer to area for returned data.
			 *
			 * \return true for success or false for failure.
			 */
			bool readCSD(csd_t* csd) {
				return readRegister(CMD9, csd);
			}

			inline SDCard_Type card_type(void){
				return SD_Type;
			}

			inline SDCard_Type type(void) {
				return SD_Type;
			}

			/** Write one data block in a multiple block write sequence.
			 * \param[in] src Pointer to the location of the data to be written.
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool writeData(const uint8_t* src);
			/** Start a write multiple blocks sequence.
			 *
			 * \param[in] blockNumber Address of first block in sequence.
			 *
			 * \note This function is used with writeData() and writeStop()
			 * for optimized multiple block writes.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool writeStart(uint32_t blockNumber);

			/** Start a write multiple blocks sequence with pre-erase.
			 *
			 * \param[in] blockNumber Address of first block in sequence.
			 * \param[in] eraseCount The number of blocks to be pre-erased.
			 *
			 * \note This function is used with writeData() and writeStop()
			 * for optimized multiple block writes.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
			/** End a write multiple blocks sequence.
			 *
			 * \return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			bool writeStop();
			void spiStart() {
				if (!m_spiActive) {
					spiActivate();
					spiSelect();
					m_spiActive = true;
				}
			}
			//-----------------------------------------------------------------------------
			void spiStop() {
				if (m_spiActive) {
					spiUnselect();
					spiSend(0XFF);
					spiDeactivate();
					m_spiActive = false;
				}
			}
		private:
			// private functions
			uint8_t cardAcmd(uint8_t cmd, uint32_t arg) {
				cardCommand(CMD55, 0);
				return cardCommand(cmd, arg);
			}

			uint8_t cardCommand(uint8_t cmd, uint32_t arg);
			//------------------------------------------------------------------------------
			bool isTimedOut(SystemTicker_t startMS, uint16_t timeoutMS) {
#if WDT_YIELD_TIME_MICROS
				static uint32_t last;
				if ((micros() - last) > WDT_YIELD_TIME_MICROS) {
					SysCall::yield();
					last = micros();
				}
#endif  // WDT_YIELD_TIME_MICROS
				return ((curTimeMS() - startMS) > timeoutMS);
			}
			//------------------------------------------------------------------------------
			bool readData(uint8_t* dst, size_t count);
			bool readRegister(uint8_t cmd, void* buf);


			bool waitNotBusy(SystemTicker_t timeoutMS);
			bool writeData(uint8_t token, const uint8_t* src);

			uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc);
			uint8_t SD_SendCommand_NoDeassert(uint8_t cmd, uint32_t arg, uint8_t crc);

			uint8_t SD_ReceiveData(uint8_t *data, uint32_t len, bool release = true);

			uint8_t SD_WaitReady(void);

			//---------------------------------------------------------------------------
			// functions defined in SdSpiDriver.h
			inline void spiActivate() {
				//mSpi.setCSPin(mCS);
			}

			inline void spiDeactivate() {
				//mSpi.setCSPin(NoPin);
			}

			inline uint8_t spiReceive() {
				return mSpi.read(0xFF, false);
			}

			inline uint8_t spiReceive(uint8_t* buf, size_t n) {
				mSpi.Read_n(buf, n, false, 0xFF);
				while (mSpi.isBusy());
				return 0;
			}

			inline void spiSend(uint8_t data) {
				mSpi.write(data, false);
			}

			inline void spiSend(const uint8_t* buf, size_t n) {
				mSpi.Write_n(buf, n, false);
				while (mSpi.isBusy());
			}

			inline void spiSelect() {
				mCS.setLow();
			}

			inline void spiUnselect() {
				mCS.setHigh();
			}

			inline void spiDummy(uint16_t n){
				mSpi.Dummy_n(n, false, 0xFF);
				while (mSpi.isBusy());
			}

		protected:
			Spi_dma& mSpi;
			GpioFast mCS;
			SDCard_Type SD_Type;
			uint8_t m_errorCode;
			uint8_t m_status;
			bool m_spiActive;
	};
}

using SDCard::SDCard_Spi;




#endif /* SDCARD_SPI_SD_SPI_H_ */
