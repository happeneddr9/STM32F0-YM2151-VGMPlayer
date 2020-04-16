/*
 * 7Segment.h
 *
 *  Created on: 2020年4月1日
 *      Author: user
 */

#ifndef DISPLAY_7SEGMENT_7SEGMENT_H_
#define DISPLAY_7SEGMENT_7SEGMENT_H_

#include "GPIO/GPIO.h"
#include "Print/Print.h"

/**
 * Common cathode or common anode
 *
 */

namespace SevSegBase {
		using DataBus7b = const GpioFastParallel<GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin>;
		using DataBus8b = const GpioFastParallel<GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin, GpioPin>;
		using SevSegPin = DataBus7b;				// A ~ G without dot
		using SevSegPin_dot = DataBus8b;			// A ~ G with dot

		using TableType = const uint8_t;
		using const_TableTypePtr = const uint8_t * const;
		using TableTypePtr = const_TableTypePtr;


		/* Default CC & CA 7-Segment Code Table */
		/* High to Low : DP G F E D C B A		*/

		/* Common Cathode */
		static constexpr TableType SegNumberTableCC[10] = {
				0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
		};

		/* Common Anode */
		static constexpr TableType SegNumberTableCA[10] = {
				0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
		};

		/* Common Cathode */
		static constexpr TableType SegHexTableCC[16] = {
				0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
				0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
		};

		/* Common Anode */
		static constexpr TableType SegHexTableCA[16] = {
				0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
				0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
		};

		static constexpr const_TableTypePtr SegTable[] = {
				SegNumberTableCC,
				SegNumberTableCA,
				SegHexTableCC,
				SegHexTableCA
		};

		enum common {
			Cathode = 0,
			Anode,
			CathodeHex,
			AnodeHex
		};

};


template <class DataBusType,
		SevSegBase::common SegCom = SevSegBase::Cathode,
		SevSegBase::TableTypePtr TablePtr = nullptr,
		size_t TableLen = ((SegCom >= SevSegBase::CathodeHex) ? 16 : 10)>
class SevSegOneDigit {
	public:
		using BusType = DataBusType;
		using TableType = SevSegBase::TableType;
		using TableTypePtr = SevSegBase::const_TableTypePtr;

	public:
		SevSegOneDigit(BusType& DataBus) :
				DataBus(DataBus) {

		}

		void begin() const {
			this->DataBus.begin(OUTPUT);
		}

		void setValue(uint8_t num) const {
			if (num >= SegTableLen)
				num = 0;
			DataBus.setValue(Table[num]);
		}

		void setDot(uint32_t digi, bool en) const {
			DataBus[DataBus.width - 1].setValue(en);
		}

		SevSegOneDigit& operator=(uint8_t num) {
			setValue(num);
			return *this;
		}

		const SevSegOneDigit& operator=(uint8_t num) const {
			setValue(num);
			return *this;
		}

	private:

	protected:
		BusType& DataBus;

		static constexpr TableTypePtr Table =
				(TablePtr == nullptr) ?
						SevSegBase::SegTable[static_cast<size_t>(SegCom)] :
						reinterpret_cast<TableTypePtr>(TablePtr);
		static constexpr uint8_t SegTableLen = TableLen;
};

template <class DataBusType,
		SevSegBase::common SegCom = SevSegBase::Cathode,
		size_t total_digits = 4,
		GPIO_Status scan_Status = LOW,
		SevSegBase::TableTypePtr TablePtr = nullptr,
		size_t TableLen = ((SegCom >= SevSegBase::CathodeHex) ? 16 : 10)>
class SevSegments : public Print {
	public:
		using BusType = DataBusType;
		using TableType = SevSegBase::TableType;
		using TableTypePtr = SevSegBase::const_TableTypePtr;
		using OneDigitsType = SevSegOneDigit<BusType, SegCom, TablePtr, TableLen>;

		static constexpr size_t Total_Digits = total_digits;



		static_assert(total_digits > 0, "Digits Can't be 0!");
		static_assert(TableLen > 0, "Table Size Can't be 0!");
	public:
		SevSegments(BusType& Bus, const GpioPin scan_package[total_digits]) :
				DataBus(Bus) { //, subDigits{Bus}
			scan = 0;
			write_pos = total_digits - 1;
			for (uint32_t i = 0; i < total_digits; i++) {
				scan_pin[i] = scan_package[i];
				subDigitsValue[i] = Table[0];
			}
		}

		void begin() {
			this->DataBus.begin(OUTPUT);
			for (uint32_t i = 0; i < total_digits; i++) {
				scan_pin[i].begin(OUTPUT);
				scan_pin[i].setState(!scan_Status);
			}
			setValue(0);
		}

		void setPos(size_t n) {
			write_pos = ((total_digits - 1) - n);
		}

		virtual size_t write(uint8_t n) override {
			char buf = static_cast<char>(n);
			uint8_t pos_inv = (total_digits - 1) - write_pos;
			if (buf != '.') {
				if (buf >= 'A') {
					buf -= ('A' - 10);
				} else if (buf >= '0') {
					buf -= '0';
				}
				subDigitsValue[pos_inv] &= ~DotMask;
				if (buf != ' ')
					subDigitsValue[pos_inv] |= Table[buf % TableLen]
							& DotMask;
				else
					subDigitsValue[pos_inv] |= DotMask;
				setDot(pos_inv, false);
				write_pos++;
				if(write_pos >= total_digits)
					write_pos = 0;
			} else {
				setDot(pos_inv, true);
			}
			return 1;
		}

		void flush() {
			for (uint32_t i = 0; i < total_digits; i++) {
				scan_pin[i].setState(!scan_Status);
			}
		}

		void setValue(uint32_t index, uint8_t val) {
			index %= total_digits;
			subDigitsValue[index] &= ~DotMask;
			subDigitsValue[index] |= Table[val % TableLen] & DotMask;
		}

		void setValue(uint32_t num) {
			size_t i = 0;
			// Process individual digits
			while (i < total_digits) {
				subDigitsValue[i] &= ~DotMask;
				subDigitsValue[i++] |= Table[num % TableLen] & DotMask;
				if(num != 0)
					num = num / TableLen;
			}
		}

		void setValueStr(const char * str) {
			size_t i = 0;
			size_t digi = 0;
			size_t digi_mask = total_digits - 1;


			char buf;
			// Process individual digits
			while (digi < total_digits) {
				buf = str[i];
				if (buf == '\0')
					break;

				if (buf != '.') {
					if (buf >= 'A') {
						buf -= ('A' - 10);
					} else if (buf >= '0') {
						buf -= '0';
					}
					subDigitsValue[digi_mask - digi] &= ~DotMask;
					if (buf != ' ')
						subDigitsValue[digi_mask - digi] |= Table[buf % TableLen] & DotMask;
					else
						subDigitsValue[digi_mask - digi] |= DotMask;
					setDot(digi, false);
					digi++;
				} else {
					setDot(digi, true);
				}
				i++;
			}
		}

		void setDot(uint32_t digi, bool en) {
			en = (en == true) ? scan_Status : !scan_Status;
			subDigitsValue[digi] &= ~(1 << 7);
			if(en == true)
				subDigitsValue[digi] |= (1 << 7);
		}

		uint8_t Scan(void) {
			scan_pin[scan].setState(!scan_Status);
			scan++;
			if (scan >= total_digits)
				scan = 0;
			DataBus.setValue(subDigitsValue[scan]);
			scan_pin[scan].setState(scan_Status);
			return scan;
		}

		uint8_t& operator[](size_t n) {
			return subDigitsValue[n];
		}

	private:

	protected:
		BusType& DataBus;
		uint8_t scan;
		uint8_t write_pos;
		uint8_t subDigitsValue[total_digits];
		GpioFast scan_pin[total_digits];
		//OneDigitsType subDigits[total_digits];
		//static constexpr uint32_t DataBusWidth = DataBus.width;

		static constexpr TableTypePtr Table =
				(TablePtr == nullptr) ?
						SevSegBase::SegTable[static_cast<size_t>(SegCom)] :
						reinterpret_cast<TableTypePtr>(TablePtr);
		static constexpr uint8_t SegTableLen = TableLen;
		static constexpr uint8_t _DotMask = (1U << (BusType::width - 1)) - 1;
		static constexpr uint8_t DotMask =
				(SegCom == SevSegBase::Anode || SegCom == SevSegBase::AnodeHex) ?
						_DotMask : ~_DotMask;
};



#endif /* DISPLAY_7SEGMENT_7SEGMENT_H_ */
