/**
 * @Name    : ios.h
 * @Date    : 2020年1月6日 下午8:10:09
 * @Author  : happe
 * @brief   : like C++ STD ios
 */

#ifndef PRINT_IOS_H_
#define PRINT_IOS_H_

#include <cstdio>
#include "Print/Print.h"

namespace cpp_io {
	class ios_base : public Print {

	};

	/**
	 *	class ios
	 *	brief Error and state information for all streams
	 */
	class ios : public ios_base {

	};
}


#endif /* PRINT_IOS_H_ */
