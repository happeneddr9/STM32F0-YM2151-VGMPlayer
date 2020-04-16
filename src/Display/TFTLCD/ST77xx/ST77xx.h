/**
 * @Name    : ST77xx.h
 * @Date    : 2020年2月18日 下午4:04:14
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_ST77XX_ST77XX_H_
#define DISPLAY_ST77XX_ST77XX_H_

#include "Display/TFTLCD/SPITFT.h"
#include "ST77xx_cmd.h"

namespace st77xx {
	class ST77xx : public SPITFT {
		public:
			ST77xx(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin);

			void setRotation(uint8_t r) override;
			void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;
			void enableDisplay(bool enable);
			void enableTearing(bool enable);
		private:

		protected:
			int16_t _xstart = 0;   			///< Internal framebuffer X offset
			int16_t _ystart = 0;   			///< Internal framebuffer Y offset
			void begin(SPI_Prescaler sck_speed = SPI_Prescaler::fPCLK_4) override;
			void commonInit(const uint8_t *cmdList);
			void displayInit(const uint8_t *addr);
			void setColRowStart(int8_t col, int8_t row);
	};
}

using st77xx::ST77xx;


#endif /* DISPLAY_ST77XX_ST77XX_H_ */
