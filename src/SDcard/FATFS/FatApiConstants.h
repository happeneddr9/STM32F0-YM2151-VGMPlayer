/**
 * @Name    : FatApiConstants.h
 * @Date    : 2020年1月1日 下午2:16:19
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_FATFS_FATAPICONSTANTS_H_
#define SDCARD_FATFS_FATAPICONSTANTS_H_

namespace SDCard{
	enum FCNTL : uint8_t{
		O_RDONLY 		= 0X00,  ///< Open for reading only.
		O_WRONLY 		= 0X01,  ///< Open for writing only.
		O_RDWR 			= 0X02,  ///< Open for reading and writing.
		O_AT_END 		= 0X04,  ///< Open at EOF.
		O_APPEND 		= 0X08,  ///< Set append mode.
		O_CREAT 		= 0x10,  ///< Create file if it does not exist.
		O_TRUNC 		= 0x20,  ///< Truncate file to zero length.
		O_EXCL 			= 0x40,  ///< Fail if the file exists.
		O_SYNC 			= 0x80,  ///< Synchronized write I/O operations.

		O_ACCMODE 		= (O_RDONLY | O_WRONLY | O_RDWR),  ///< Mask for access mode.

		O_READ 			= O_RDONLY,
		O_WRITE 		= O_WRONLY,
	};



	typedef uint8_t oflag_t;



	inline bool isWriteMode(oflag_t oflag) {
		oflag &= O_ACCMODE;
		return oflag == O_WRONLY || oflag == O_RDWR;
	}

	// FatFile class static and const definitions
	// flags for ls()
	/** ls() flag for list all files including hidden. */
	const uint8_t LS_A = 1;
	/** ls() flag to print modify. date */
	const uint8_t LS_DATE = 2;
	/** ls() flag to print file size. */
	const uint8_t LS_SIZE = 4;
	/** ls() flag for recursive list of subdirectories */
	const uint8_t LS_R = 8;

	// flags for timestamp
	/** set the file's last access date */
	const uint8_t T_ACCESS = 1;
	/** set the file's creation date and time */
	const uint8_t T_CREATE = 2;
	/** Set the file's write date and time */
	const uint8_t T_WRITE = 4;
}



#endif /* SDCARD_FATFS_FATAPICONSTANTS_H_ */
