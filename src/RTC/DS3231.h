/*
 * DS3231.h
 *
 *  Created on: 2019年11月18日
 *      Author: happe
 */

#ifndef RTC_DS3231_H_
#define RTC_DS3231_H_

#include "main.h"
#include "Arduino/Basic.h"
#include "RTC.h"
#include "I2CMaster/HWI2C_IT.h"

class DS3231 {
	private:
		uint8_t hr24;
		uint8_t CLOCK_ADDRESS;
	public:
		DS3231(HWWire& Wirex) : Wire(Wirex){
			hr24 = 1;
			CLOCK_ADDRESS = 0x00;
		}
		void begin(uint8_t);

		uint8_t getSecond();                     //return Second
		uint8_t getMinute();                     //return Minute
		uint8_t getHour();                       //return Hour
		uint8_t getWeek();                       //return Week
		uint8_t getDate();                       //return Day
		uint8_t getMonth();                      //return Month
		uint8_t getYear();                       //return Year - 2000

		RealTime::nowClock getTimeStruct();                //回傳時間結構

		void setNowTIME(void);
		void setTime(RealTime::nowTime val);
		void setSecond(uint8_t val);
		void setMinute(uint8_t val);
		void setHour(uint8_t val, uint8_t hr12);
		void setWeek(uint8_t val);
		void setDate(uint8_t val);
		void setMonth(uint8_t val);
		void setYear(uint8_t val);
		void setAgingOffset(int8_t ppm);         //修改RTC的誤差量

		inline uint8_t getHr24() {
			return hr24;
		}


	protected:
		HWWire& Wire;

		uint8_t str2num(const char * str);       //字串 轉 0~99
		uint8_t decToBcd(uint8_t val);           //數字 轉 BCD
		uint8_t bcdToDec(uint8_t val);           //BCD 轉 數字
};

#endif /* RTC_DS3231_H_ */
