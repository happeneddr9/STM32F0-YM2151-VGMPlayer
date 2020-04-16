/*
 * DS3231.cpp
 *
 *  Created on: 2019年11月18日
 *      Author: happe
 */

#include "DS3231.h"

void DS3231::begin(uint8_t addr) {
	CLOCK_ADDRESS = addr;
	Wire.beginTransmission(addr);
	Wire.write(0x0E);
	Wire.write(0x00);
	Wire.write(0x08);
	Wire.endTransmission();
}

uint8_t DS3231::getSecond() {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	return bcdToDec(Wire.read());
}

uint8_t DS3231::getMinute() {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x01);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	return bcdToDec(Wire.read());
}

uint8_t DS3231::getHour() {
	uint8_t buff;
	uint8_t hour;
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x02);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	buff = Wire.read();
	if (buff & 0x40) {
		if (buff & 0x20) {
			hour = bcdToDec(buff & 0x1F) + 12;
		} else {
			hour = bcdToDec(buff & 0x1F);
		}
	} else {
		hour = bcdToDec(buff & 0x3F);
	}
	return hour;
}

uint8_t DS3231::getWeek() {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x03);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	return bcdToDec(Wire.read());
}

uint8_t DS3231::getDate() {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x04);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	return bcdToDec(Wire.read());
}

uint8_t DS3231::getMonth() {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x05);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	return bcdToDec(Wire.read() & 0x7F);
}

uint8_t DS3231::getYear() {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x06);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 1);
	delay(1);
	return bcdToDec(Wire.read());
}

//-------set----------------------------------
void DS3231::setNowTIME(void) {
	RealTime::nowTime tmp;
	const char * ptr = __TIME__;
	//const char * date_ptr = __DATE__;
	tmp.h = str2num(&ptr[0]);
	tmp.m = str2num(&ptr[3]);
	tmp.s = str2num(&ptr[6]);
	setTime(tmp);
}

void DS3231::setTime(RealTime::nowTime val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x00);
	Wire.write(decToBcd(val.s));
	Wire.write(decToBcd(val.m));
	Wire.write(decToBcd(val.h));
	Wire.endTransmission();
}

void DS3231::setSecond(uint8_t val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x00);
	Wire.write(decToBcd(val));
	Wire.endTransmission();
}

void DS3231::setMinute(uint8_t val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x01);
	Wire.write(decToBcd(val));
	Wire.endTransmission();
}

void DS3231::setHour(uint8_t val, uint8_t hr12) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x02);
	if (hr12) {
		hr24 = 0;
		if (val > 12) {                              //PM
			val -= 11;
			Wire.write((decToBcd(val) & 0x1F) | 0x60);
		} else {                                      //AM
			Wire.write((decToBcd(val) & 0x1F) | 0x40);
		}
	} else {
		hr24 = 1;
		Wire.write(decToBcd(val) & 0x3F);
	}
	Wire.endTransmission();
}

void DS3231::setWeek(uint8_t val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x03);
	Wire.write(decToBcd(val));
	Wire.endTransmission();
}

void DS3231::setDate(uint8_t val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x04);
	Wire.write(decToBcd(val));
	Wire.endTransmission();
}

void DS3231::setMonth(uint8_t val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x05);
	Wire.write(decToBcd(val));
	Wire.endTransmission();
}

void DS3231::setYear(uint8_t val) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x06);
	Wire.write(decToBcd(val));
	Wire.endTransmission();
}

void DS3231::setAgingOffset(int8_t ppm) {
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x10);
	Wire.write(ppm);
	Wire.endTransmission();
}

RealTime::nowClock DS3231::getTimeStruct() {
	RealTime::nowClock reTime;
	Wire.beginTransmission(CLOCK_ADDRESS);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(CLOCK_ADDRESS, 7);
	delay(1);  //接受速度太快
	reTime.s = bcdToDec(Wire.read());
	reTime.m = bcdToDec(Wire.read());
	reTime.h = Wire.read();
	reTime.week = bcdToDec(Wire.read());
	reTime.dd = bcdToDec(Wire.read());
	reTime.mm = bcdToDec(Wire.read());
	reTime.yy = bcdToDec(Wire.read());

	if (reTime.h & 0x40) {
		if (reTime.h & 0x20) {
			reTime.h = bcdToDec(reTime.h & 0x1F) + 12;
		} else {
			reTime.h = bcdToDec(reTime.h & 0x1F);
		}
	} else {
		reTime.h = bcdToDec(reTime.h & 0x3F);
	}
	return reTime;
}

uint8_t DS3231::str2num(const char * str) {       //字串 轉 0~99
	uint8_t n;
	if (str[0] >= '0' && str[0] <= '9') {
		if (str[1] >= '0' && str[1] <= '9') {
			n = ((str[0] - '0') * 10) + (str[1] - '0');
			return n;
		}
	}
	return 0;
}
uint8_t DS3231::decToBcd(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}
uint8_t DS3231::bcdToDec(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}

