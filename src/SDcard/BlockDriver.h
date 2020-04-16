/**
 * @Name    : BlockDriver.h
 * @Date    : 2020年1月1日 下午2:07:16
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_BLOCKDRIVER_H_
#define SDCARD_BLOCKDRIVER_H_

#include "FATFS/BaseBlockDriver.h"
#include "spi/SD_SPI.h"

namespace SDCard {
	typedef BaseBlockDriver BlockDriver;
}



#endif /* SDCARD_BLOCKDRIVER_H_ */
