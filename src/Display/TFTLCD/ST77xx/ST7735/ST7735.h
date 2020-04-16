/**
 * @Name    : ST7735.h
 * @Date    : 2020年1月29日 下午9:02:58
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_ST7735_ST7735_H_
#define DISPLAY_ST7735_ST7735_H_

#include "Arduino/Basic.h"
#include "Commands.h"
#include "Display/TFTLCD/ST77xx/ST77xx.h"

namespace st7735{
	class ST7735 : public ST77xx {
		public:
			ST7735(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin);
			~ST7735() = default;

			void begin(SPI_Prescaler sck_speed = SPI_Prescaler::fPCLK_4, uint8_t options = INITR_GREENTAB);

			// Differences between displays (usu. identified by colored tab on
			// plastic overlay) are odd enough that we need to do this 'by hand':
			void initB(void);                             // for ST7735B displays
			void initR(uint8_t options = INITR_GREENTAB); // for ST7735R

			void setRotation(uint8_t r) override;
		private:
			uint8_t tabcolor;
		protected:

	};
}

using st7735::ST7735;

#endif /* DISPLAY_ST7735_ST7735_H_ */
