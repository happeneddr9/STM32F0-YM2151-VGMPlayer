/**
 * @Name    : SysCall.h
 * @Date    : 2020年1月1日 下午2:53:36
 * @Author  : happe
 * @brief   :
 */

#ifndef SDCARD_SYSCALL_H_
#define SDCARD_SYSCALL_H_

#include "Arduino/Basic.h"

#ifndef F
/** Define macro for strings stored in flash. */
#define F(str) (str)
#endif  // F

namespace SDCard {
	inline SystemTicker_t curTimeMS() {
		return millis();
	}
//------------------------------------------------------------------------------
	/**
	 * \class SysCall
	 * \brief SysCall - Class to wrap system calls.
	 */
	class SysCall {
		public:
			/** Halt execution of this thread. */
			static void halt() {
				/*while (1) {
					yield();
				}*/
				delay(100);
			}
			/** Yield to other threads. */
			static void yield();
	};

	inline void SysCall::yield() {}
}


#endif /* SDCARD_SYSCALL_H_ */
