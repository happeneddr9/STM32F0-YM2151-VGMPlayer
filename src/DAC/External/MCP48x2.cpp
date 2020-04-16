/**
 * @Name    : MCP48x2.cpp
 * @Date    : 2019年12月26日 下午6:55:52
 * @Author  : happe
 * @brief   :
 */

#include "MCP48x2.h"

MCP48x2::MCP48x2(Spi_dma& spi)
		: mSPI(spi) {
	Gain = VRef_x2;
}


void MCP48x2::write(DAC_Ch_Selection CHx, uint16_t val, SHDN Vout) {
	uint8_t tmp[2];
	tmp[0] = (val & 0x0FFF) >> 8;
	tmp[1] = val & 0xFF;


	tmp[0] |= CHx | Gain | Vout;

	while (mSPI.isBusy() == Spi_dma::DMA_Busy) {

	}

	mSPI.Write_n(&tmp[0], 2);
}
