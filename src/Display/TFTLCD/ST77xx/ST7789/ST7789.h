/**
 * @Name    : ST7789.h
 * @Date    : 2020年2月18日 下午4:44:25
 * @Author  : happe
 * @brief   :
 */

#ifndef DISPLAY_ST77XX_ST7789_ST7789_H_
#define DISPLAY_ST77XX_ST7789_ST7789_H_

#include "Arduino/Basic.h"
#include "Commands.h"
#include "Display/TFTLCD/ST77xx/ST77xx.h"

namespace st7789 {
	class ST7789 : public ST77xx {
		public:
			ST7789(Spi_dma& Spix, int16_t W, int16_t H, GpioPin CSPin, GpioPin DCPin, GpioPin ResetPin);
			~ST7789() = default;

			void init(uint16_t width, uint16_t height);
			void setRotation(uint8_t m) override;

		private:

		protected:

	};
}

using st7789::ST7789;


#endif /* DISPLAY_ST77XX_ST7789_ST7789_H_ */
