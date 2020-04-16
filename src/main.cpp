/**			<! Coding with UTF-8 >
 *   Date      :   2019/11/09     YYYY-MM-DD
 *   Author    :   Happeneddr9
 *   Font use  :   Consolas with YaHei 雅黑體
 *
 *   @brief:	YM2151 VGM Player code
 *
 *		UTF-8 TextTest: マフェット
 */

/**
 *	系統參數請到 Basic.h 或 main.h 處修改
 *	System parameter defined in header "Basic.h" or "main.h"
 */
#include "Arduino/Basic.h"         		// use delay()  millis()
#include "HardwareUart/HWSerial0.h"     // use Serial  硬體UART串列阜

#include "TIMER/TIMER3.hpp"				// I implemented only timer3, others you need to call IRQHandler.

#include "Sound/VGM_Player.h"			// VGM Player Class Object

// <!# Warning!: PA13 and PA14 using for the ST-LINK, so avoid to use them for GPIO !>

Spi_dma Spi(SPI1, SPI1_IRQn);

extern "C" {	// IRQ Handler
	// function name called DMA1_Channel2_3_IRQHandler in not F030 devices.
	void DMA1_CH2_3_IRQHandler(void) {
		Spi.IRQ_handler();
	}
}

inline void tick();


SdFat SD(PB1, Spi);
SdFile file;

// the lowest is D0, 8-bits data bus.
// auto DataBus = make_GpioFastParallel(PB8, PB9, PB7, PB6, PB5, PB4, PB3, PA15);
YM2151::DataBusType DataBus(PB8, PB9, PB7, PB6, PB5, PB4, PB3, PA15);

// YM2151			Bus		CS	  RD    WR    A0   IRQ    IC
const YM2151 opm(DataBus, PA0, NoPin, PC13, PC14, NoPin, PC15);
// RD and IRQ pin not used.

//ST7735 tft(Spi, 128, 160, PA3, PA2, PA1);
ST7789 tft(Spi, 135, 240, PB0, PA1, PA2);
const GpioPin buttons[4] = {PB15, PB14, PB13, PB12};					// control buttons array.
VGM_Player player(SD, file, Timer3, opm, tft, buttons, Serial, PA4);

// Timer for clock generator.
const Timer Timer15(TIM::Timer15, TIM15_IRQn);

int main(void) {
	SysTickInit(1000);     // 系統時鐘頻率 預設1kHz 此設定會影響到 delay() millis() std::chrono
	Serial.begin(38400, Usart1::PinMap_TXRX::DefaultMap);

	Spi.begin();
	tft.init(135, 240);		// ST7789
	//tft.begin();			// ST7735

	tft.setTextSize(1);
	tft.setTextColor(Pixel::Green);
	tft.setRotation(3);					// Rotation to suit for your screen.
	tft.fillScreen(Pixel::Black);
	tft.setCursor(0, 0);

	// Generate a 4MHz in PA3, for YM2151
	Timer15.GenerateClock(4000000);
	//Timer15.Clock_Config(23, 1);
	Timer15.PWM_Config(PA3, PWM::Channel2, GpioAF::AF0);
	Timer15.PWM_Compare_CH2(1);
	Timer15.start();

	player.begin();
	player.run(tick);



	for (;;) {

	}
}

inline void tick(void) {
	player.tick();
}

