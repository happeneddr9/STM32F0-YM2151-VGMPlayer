/*
 * RTC.h
 *
 *  Created on: 2019年11月18日
 *      Author: happe
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#include "main.h"

namespace RealTime {

	enum Month_sign {     //列舉 月份
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	enum Week_sign {     //列舉 星期
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};

	struct nowClock {    //日期結構 7 bytes
			uint8_t yy;
			uint8_t mm;
			uint8_t dd;
			uint8_t week;
			uint8_t h;
			uint8_t m;
			uint8_t s;
	};

	struct nowTime {
			uint8_t h;
			uint8_t m;
			uint8_t s;
	};

	const char week_map[] = "SUNMONTUEWEDTHUFRISAT"; //星期
	const char month_map[] = "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC"; //3 Byte 月份符號  January = 0
	const uint8_t daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //1-12月的天數

	uint16_t date2days(uint16_t y, uint8_t m, uint8_t d);
	uint32_t time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s);
	nowClock UNIX2Clock(uint32_t t);
	uint32_t Clock2UNIX(nowClock n);
}

#endif /* RTC_RTC_H_ */
