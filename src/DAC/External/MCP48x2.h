/**
 * @Name    : MCP48x2.h
 * @Date    : 2019年12月26日 下午6:56:10
 * @Author  : happe
 * @brief   :
 */

#ifndef DAC_EXTERNAL_MCP48X2_H_
#define DAC_EXTERNAL_MCP48X2_H_

#include "Arduino/Basic.h"
#include "SPI/SPI.hpp"

class MCP48x2 {

	public:
		enum gain : uint8_t {
			VRef_x1 = 0x20,
			VRef_x2 = 0x00
		};

		enum DAC_Ch_Selection : uint8_t {
			DAC_A = 0x00,
			DAC_B = 0x80
		};

		enum SHDN : uint8_t{
			Active = 0x10,
			High_Z = 0x00
		};

	public:
		MCP48x2(Spi_dma& spi);
		inline void GainSel(gain GA){
			Gain = GA;
		}

		void write(DAC_Ch_Selection CHx, uint16_t val, SHDN Vout = Active);
	private:

	protected:
		Spi_dma& mSPI;
		gain Gain;
};

#endif /* DAC_EXTERNAL_MCP48X2_H_ */
