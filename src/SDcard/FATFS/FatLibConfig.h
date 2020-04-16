/**
 * @Name    : FatLibConfig.h
 * @Date    : 2020年1月1日 下午1:51:08
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_FATFS_FATLIBCONFIG_H_
#define SDCARD_FATFS_FATLIBCONFIG_H_

#include "SDcard/SdFatConfig.h"

//------------------------------------------------------------------------------
/**
 * Set USE_LONG_FILE_NAMES nonzero to use long file names (LFN).
 * Long File Name are limited to a maximum length of 255 characters.
 *
 * This implementation allows 7-bit characters in the range
 * 0X20 to 0X7E.  The following characters are not allowed:
 *
 *  < (less than)
 *  > (greater than)
 *  : (colon)
 *  " (double quote)
 *  / (forward slash)
 *  \ (backslash)
 *  | (vertical bar or pipe)
 *  ? (question mark)
 *  * (asterisk)
 *
 */
#ifndef USE_LONG_FILE_NAMES
#define USE_LONG_FILE_NAMES 1
#endif  // USE_LONG_FILE_NAMES
//------------------------------------------------------------------------------
/**
 * Set USE_MULTI_BLOCK_IO non-zero to use multi-block SD read/write.
 *
 * Don't use mult-block read/write on small AVR boards.
 */
#ifndef USE_MULTI_BLOCK_IO
#if defined(RAMEND) && RAMEND < 3000
#define USE_MULTI_BLOCK_IO 0
#else  // RAMEND
#define USE_MULTI_BLOCK_IO 1
#endif  // RAMEND
#endif  // USE_MULTI_BLOCK_IO
//------------------------------------------------------------------------------

/**
 * Set USE_SEPARATE_FAT_CACHE non-zero to use a second 512 byte cache
 * for FAT table entries.  Improves performance for large writes that
 * are not a multiple of 512 bytes.
 */
#ifndef USE_SEPARATE_FAT_CACHE
#ifdef __arm__
#define USE_SEPARATE_FAT_CACHE 1
#else  // __arm__
#define USE_SEPARATE_FAT_CACHE 0
#endif  // __arm__
#endif  // USE_SEPARATE_FAT_CACHE

//------------------------------------------------------------------------------
/**
 * Set MAINTAIN_FREE_CLUSTER_COUNT nonzero to keep the count of free clusters
 * updated.  This will increase the speed of the freeClusterCount() call
 * after the first call.  Extra flash will be required.
 */
#ifndef MAINTAIN_FREE_CLUSTER_COUNT
#define MAINTAIN_FREE_CLUSTER_COUNT 0
#endif  // MAINTAIN_FREE_CLUSTER_COUNT

//------------------------------------------------------------------------------
/**
 * Set DESTRUCTOR_CLOSES_FILE non-zero to close a file in its destructor.
 *
 * Causes use of lots of heap in ARM.
 */
#ifndef DESTRUCTOR_CLOSES_FILE
#define DESTRUCTOR_CLOSES_FILE 0
#endif  // DESTRUCTOR_CLOSES_FILE

//------------------------------------------------------------------------------
/**
 * Call flush for endl if ENDL_CALLS_FLUSH is non-zero
 *
 * The standard for iostreams is to call flush.  This is very costly for
 * SdFat.  Each call to flush causes 2048 bytes of I/O to the SD.
 *
 * SdFat has a single 512 byte buffer for I/O so it must write the current
 * data block to the SD, read the directory block from the SD, update the
 * directory entry, write the directory block to the SD and read the data
 * block back into the buffer.
 *
 * The SD flash memory controller is not designed for this many rewrites
 * so performance may be reduced by more than a factor of 100.
 *
 * If ENDL_CALLS_FLUSH is zero, you must call flush and/or close to force
 * all data to be written to the SD.
 */
#ifndef  ENDL_CALLS_FLUSH
#define ENDL_CALLS_FLUSH 0
#endif  // ENDL_CALLS_FLUSH
//------------------------------------------------------------------------------
/**
 * Allow FAT12 volumes if FAT12_SUPPORT is non-zero.
 * FAT12 has not been well tested.
 */
#ifndef FAT12_SUPPORT
#define FAT12_SUPPORT 0
#endif  // FAT12_SUPPORT
//------------------------------------------------------------------------------
/**
 *  Enable Extra features for Arduino.
 */
 #define ENABLE_ARDUINO_FEATURES 0  ////////////////////////FIX THIS /////////////////
#ifndef ENABLE_ARDUINO_FEATURES
#if defined(ARDUINO) || defined(PLATFORM_ID) || defined(DOXYGEN)
#define ENABLE_ARDUINO_FEATURES 1
#else  //  #if defined(ARDUINO) || defined(DOXYGEN)
#define ENABLE_ARDUINO_FEATURES 0
#endif  //  defined(ARDUINO) || defined(DOXYGEN)
#endif  // ENABLE_ARDUINO_FEATURES

namespace SDCard{

}


#endif /* SDCARD_FATFS_FATLIBCONFIG_H_ */