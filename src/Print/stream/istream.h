/**
 * @Name    : istream.h
 * @Date    : 2020年1月6日 下午8:09:39
 * @Author  : happe
 * @brief   : like C++ STD istream
 * 			  Coming Soon!
 */

#ifndef PRINT_ISTREAM_H_
#define PRINT_ISTREAM_H_

#include "ios.h"


namespace cpp_io {
	class istream : public virtual ios {
		public:

			istream() {

			}

			istream& operator>>(int& n) {
				readInt(n);
				return *this;
			}



		private:
			size_t readChar(char& c);
			size_t readInt(int& n);
			size_t readFloat(float& n);
			size_t readDouble(double& n);



		protected:
			virtual bool ready2read(void) = 0;
			virtual int getch(void) = 0;
			virtual int peekch(void) = 0;
			int16_t readSkip();
	};
}

#endif /* PRINT_ISTREAM_H_ */
