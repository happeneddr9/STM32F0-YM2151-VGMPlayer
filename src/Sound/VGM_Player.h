/*
 * VGM_Player.h
 *
 *  Created on: 2020年2月21日
 *  Author: happe
 *
 *  code reference from Aidan Lawrence's YM2151 Arcade Classic
 *
 *  @Build Log:
 *  	2020/02/29:
 *  		Added colorful time bar to show current playback progress!
 *  		Added support for internal file loop playback!
 */

#ifndef SOUND_VGM_PLAYER_H_
#define SOUND_VGM_PLAYER_H_

#include "Arduino/Basic.h"         //use delay()  millis()
#include "HardwareUart/HWSerial.h"     //use Serial  硬體UART串列阜
#include "GPIO/GPIO.h"
#include "SPI/SPI.hpp"

#include "SDcard/SdFat.h"
#include "Sound/YM2151/YM2151.h"
#include "Sound/TrackInfo.h"
#include "FIFObuffer/RingBuffer.h"
#include "TIMER/TIMER.h"

#include "Display/TFTLCD/ST77xx/ST7735/ST7735.h"
#include "Display/TFTLCD/ST77xx/ST7789/ST7789.h"

#include <cmath>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <atomic>
#include <wchar.h>
#include <chrono>
#include <utility>
#include <tuple>
#include <string>
#include <functional>


class VGM_Player {
	public:

		enum newTrackMode : uint8_t {
			NOT_A_MODE,
			FIRST_START,
			NEXT,
			PREV,
			RND,
			REQUEST
		};

		enum PlayMode : uint8_t {
			LOOP,
			SHUFFLE,
			IN_ORDER
		};

		VGM_Player(SdFat& SD, SdFile& file, Timer& Timerx, const SoundChip& opm,
				   ST77xx& tft, const GpioPin (&button)[4], HWSerial& Serial,
				   const GpioPin swi_pin = NoPin);

		~VGM_Player() = default;

		/**
		 * @brief:	initialize the VGM Player.
		 * 			You should initialize your TFT LCD before call that, also the SPI.
		 */
		void begin(void);

		/* @brief:	start VGM Player, it's infinity loop */
		void run(void (*func)());

		void SDinit(void);


		// Prototypes
		/* @brief:	Remove useless meta files */
		void removeMeta();

		/* @brief:	push bytes to loopPreBuffer from SD until it's full*/
		void prebufferLoop();

		/* @brief:	push all of bytes to cmdBuffer from loopPreBuffer */
		void injectPrebuffer();

		/* @brief:	push bytes to cmdBuffer from SD until it's full */
		void fillBuffer();

		/* @brief:	push a byte to cmdBuffer from SD if it's not full */
		bool topUpBuffer();

		/* @brief:	let cmdBuffer to be empty */
		void clearBuffers();

		/* @brief:	handle USART Serial request */
		void handleSerialIn();

		newTrackMode checkButtons();

		/* @brief:	handle buttons state */
		void handleButtons();

		/* @brief:	handle SD card read problem */
		void handleSDfail(const char * str);

		/* @brief:	update this track's GD3 info */
		void readGD3();

		/**
		 * @brief:	設定Timer的中斷頻率為44.1kHz, 並設置要調用的函式
		 * @brief:	Set the interrupt frequency of Timer to 44.1kHz, and set the function to be called
		 *
		 * @param:	func		interrupt callback handler's pointer
		 */
		void ISRconfig(void (*func)());

		/**
		 * @brief:	印出該曲目的詳細資料，如曲目名、作者明及系統平台的資訊，以及撥放模式。
		 * @brief:	draw the detailed info of this track, such as the track name,
		 * 			author name and system info etc, and also the playback mode.
		 *
		 * @param:	upAll	if true, it'll update all of this track's information,
		 * 					else only update the playback mode.
		 *
		 */
		bool drawTrackInfo(bool upAll = true);

		/**
		 * @brief:	印出各通道正在撥放的音階
		 * @brief:	draw each channel notes bar in screens bottom that playing.
		 *
		 * @param:	newVal		the value that will override old one.
		 * @param:	CH			which channel will be update. 8-bits corresponds to 8 channels.
		 */
		void drawOPM_TL(uint8_t newVal, uint8_t CH);

		void drawTimebar(void);

		/**
		 * @brief:	準備另一首曲子，可以選擇開啟順序，或是直接指定檔案名稱。
		 * @brief:	Mount file and prepare for playback. you can select opens sequence,
		 * 			or Specify the file name directly.
		 *
		 * @param:	newTrackMode fileStrategy		select opens sequence, next, prev, or random etc.
		 * @param:	happe::String request			new track request, default is null.
		 *
		 * @return: return true if file is found, vice versa.
		 */
		bool newTrack(newTrackMode fileStrategy, happe::String request = "");

		/**
		 * @brief:	檢查檔案是否為"Vgm "作為開頭，如果是，則印出該檔案的GD3資訊，
		 * 			否則開啟下一個文件。
		 *
		 * 			<警告!>		如果有純文字檔的開頭是"Vgm ..."， 它依然會通過。
		 *
		 * @brief:	Verify file's header, is it starts with "VGM ",
		 * 			if it is, print its GD3 info on Serial debug line,
		 * 			otherwise, open next file.
		 *
		 * 			<Warning!> 	if you have a text file that starts with "Vgm ...", it will still pass.
		 *
		 * 	@return:	return true if it pass, vice versa.
		 */
		bool vgmVerify();

		/* @brief:	print the detailed info of this track to Serial port */
		void printTrackInfo();

		/**
		 * @brief:	return datas from ring-buffer
		 * @return:	return 8-bit, 16-bit, or 32-bit datas.
		 */
		uint8_t readBuffer();
		uint16_t readBuffer16();
		uint32_t readBuffer32();

		/**
		 * @brief:	return 32 bits data right off of the SD card.
		 * @return:	32 bits data from SD card.
		 */
		uint32_t readSD32();		// not used

		/**
		 * @brief:	VGM command handler
		 *
		 * @return:	wait Samples
		 */
		uint16_t parseVGM();

		/* Count at 44.1KHz(CD) */
		finline void tick() {					// TODO: looking for a better way to callback!
			if (!ready || cmdBuffer.empty())
				return;
			if (waitSamples > 0) {
				waitSamples--;
				timebar_Sample_now++;
			}
		}

		/* Reset the FM-Synth chip and the DAC chip */
		inline void ResetChips() {
			opm.reset();
		}

	protected:
		/**
		 * @brief:	最少列印2個位數，os可以帶入任何有繼承Print的Class
		 * 			print 2 digits number at the least.
		 *
		 * @param:	Print& os	It can be any Class reference that inherited Print Class.
		 * @param:	uint8_t n	witch number you wanna print it out.
		 */
		static Print& print2num(Print& os, uint8_t n) {
			if (n < 10)
				os.print('0');
			os.print(n);
			return os;
		}

		bool isVGM(SdFile& f);


	private:
		SdFat& SD;
		SdFile& file;

		Timer& Timerx;
		HWSerial& Serial;

		using VGMHeader = VGM_info::VGMHeader;
		using GD3 = VGM_info::GD3;

		VGMHeader header;
		GD3 gd3;

		uint16_t Mode_x, Mode_y;
		uint32_t timebar_moveSample;
		uint32_t timebar_total;
		uint32_t timebar_Sample_now;
		uint32_t timebar_Sample_last;
		uint16_t timebar_x, timebar_y;
		uint8_t timebar_color;
		uint8_t have_loop;
		uint32_t select_delay = 500;

		/* Parameter Configured */
		static constexpr uint16_t timebar_width = 3;
		static constexpr size_t MAX_FILE_NAME_SIZE = 128;	// file name buffer length
		static constexpr size_t CMD_BUFFER_SIZE = 1024;		// sizeof cmd buffer
		static constexpr size_t LOOP_PREBUF_SIZE = 512;		// sizeof loop prebuffer
		static constexpr uint8_t maxLoops = 1;				// loop times

		//Buffers
		char fileName[MAX_FILE_NAME_SIZE];
		uint32_t numberOfFiles = 0;
		uint32_t numberOfvgmFiles = 0;
		uint32_t currentFileNumber = 0;

		using RingBuffer = ringbuffer_t<uint8_t, CMD_BUFFER_SIZE, uint8_t>;
		RingBuffer cmdBuffer;
		uint8_t loopPreBuffer[LOOP_PREBUF_SIZE];

		//Counters
		//uint32_t bufferPos = 0;
		//uint32_t cmdPos = 0;
		uint16_t waitSamples = 0;

		//VGM Variables
		uint16_t loopCount = 0;

		volatile bool ready = false;
		PlayMode playMode = IN_ORDER;

		uint16_t TrackColor;

		// YM2151	Bus		CS	  RD    WR    A0   IQR    IC
		const SoundChip& opm;

		ST77xx& tft;

		GpioFast NextButton;
		GpioFast RandButton;
		GpioFast PrevButton;
		GpioFast ModeButton;

		GpioFast FreqSwitch;

		uint8_t OPM_TL[8] = { 0 };
};

#endif /* SOUND_VGM_PLAYER_H_ */
