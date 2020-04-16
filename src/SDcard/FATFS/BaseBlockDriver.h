/**
 * @Name    : BaseBlockDriver.h
 * @Date    : 2020年1月1日 下午1:58:53
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_FATFS_BASEBLOCKDRIVER_H_
#define SDCARD_FATFS_BASEBLOCKDRIVER_H_

#include "FatLibConfig.h"

namespace SDCard {
	/**
	 * \class BaseBlockDriver
	 * \brief Base block driver.
	 */
	class BaseBlockDriver {
		public:
			/**
			 * Read a 512 byte block from an SD card.
			 *
			 * @param[in] block Logical block to be read.
			 * @param[out] dst Pointer to the location that will receive the data.
			 * @return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			virtual bool readBlock(uint32_t block, uint8_t* dst) = 0;
			/** End multi-block transfer and go to idle state.
			 * @return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			virtual bool syncBlocks() = 0;
			/**
			 * Writes a 512 byte block to an SD card.
			 *
			 * @param[in] block Logical block to be written.
			 * @param[in] src Pointer to the location of the data to be written.
			 * @return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			virtual bool writeBlock(uint32_t block, const uint8_t* src) = 0;
			/**
			 * Read multiple 512 byte blocks from an SD card.
			 *
			 * @param[in] block Logical block to be read.
			 * @param[in] nb Number of blocks to be read.
			 * @param[out] dst Pointer to the location that will receive the data.
			 * @return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			virtual bool readBlocks(uint32_t block, uint8_t* dst, size_t nb) = 0;
			/**
			 * Write multiple 512 byte blocks to an SD card.
			 *
			 * @param[in] block Logical block to be written.
			 * @param[in] nb Number of blocks to be written.
			 * @param[in] src Pointer to the location of the data to be written.
			 * @return The value true is returned for success and
			 * the value false is returned for failure.
			 */
			virtual bool writeBlocks(uint32_t block, const uint8_t* src, size_t nb) = 0;
	};
}

#endif /* SDCARD_FATFS_BASEBLOCKDRIVER_H_ */
