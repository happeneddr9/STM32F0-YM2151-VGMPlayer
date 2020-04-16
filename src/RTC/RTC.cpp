/*
 * RTC.cpp
 *
 *  Created on: 2019年11月18日
 *      Author: happe
 */

#include "RTC.h"

namespace RealTime {
	// 2001..2136  省略2000
	//年月日 轉 天數
	uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
		uint16_t days = d;
		for (uint8_t i = 1; i < m; ++i)
			days += *(daysInMonth + i - 1);
		if (m > 2 && y % 4 == 0)
			++days;
		return days + 365 * y + (y + 3) / 4 - 1;
	}

	//日時分秒 轉 秒   c
	uint32_t time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
		return ((days * 24L + h) * 60 + m) * 60 + s;
	}

	nowClock UNIX2Clock(uint32_t t) {         //格式 uint8_t: yy mm dd week h m s
		//t -= SECONDS_FROM_1970_TO_2000;   // bring to 2000 timestamp from 1970 //標準UNIX

		//這裡使用的UNIX是從  -2000/1/1 00:00:00 星期六-   開始

		nowClock tim;
		tim.week = ((t / 86400) + 6) % 7;		// 2000/1/1 星期六
		tim.s = t % 60;		//取秒
		t /= 60;
		tim.m = t % 60;		//取分
		t /= 60;
		tim.h = t % 24;		//取小時
		uint16_t days = t / 24;
		uint8_t leap;
		for (tim.yy = 0;; ++tim.yy) {      //取年(包含閏年)
			leap = tim.yy % 4 == 0;
			if (days < 365 + leap)
				break;
			days -= 365 + leap;
		}
		for (tim.mm = 1;; ++tim.mm) {      //取月份(含月份日數表)
			uint8_t daysPerMonth = *(daysInMonth + tim.mm - 1);
			if (leap && tim.mm == 2)      //閏年 2月多一天
				++daysPerMonth;
			if (days < daysPerMonth)
				break;
			days -= daysPerMonth;
		}
		tim.dd = days + 1;
		return tim;
	}

	uint32_t Clock2UNIX(nowClock n){
		return (time2long(date2days(n.yy, n.mm, n.dd), n.h, n.m, n.s));
	}
}

