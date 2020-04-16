/*
 * VGM_Player.cpp
 *
 *  Created on: 2020年2月21日
 *  Author: happe
 */

#include "VGM_Player.h"

VGM_Player::VGM_Player(SdFat& SD, SdFile& file, Timer& Timerx, const SoundChip& opm,
		ST77xx& tft, const GpioPin (&button)[4], HWSerial& Serial, const GpioPin swi_pin) :
		SD(SD), file(file), Timerx(Timerx), Serial(Serial), opm(opm), tft(tft),
		NextButton(button[0]), RandButton(button[1]),
		PrevButton(button[2]), ModeButton(button[3]),
		FreqSwitch(swi_pin) {


}

/* You should initialize your TFT LCD before call that, also the SPI */
void VGM_Player::begin(void) {
	NextButton.begin(INPUT_PULLUP);
	PrevButton.begin(INPUT_PULLUP);
	RandButton.begin(INPUT_PULLUP);
	ModeButton.begin(INPUT_PULLUP);
	FreqSwitch.begin(OUTPUT);


	/* Reset the YM2151 */
	ResetChips();

	/* Initialize the TFT Screen */
	//tft.begin();
	SDinit();

	tft.fillScreen(Pixel::Black);
	tft.setCursor(0, 0);
	tft.setTextColor(Pixel::Green);
	tft.println("Welcome to VGM player!\n");
	delay(500);

	tft.setTextColor(Pixel::Magenta);
	tft << "Total files: " << (numberOfFiles - 1) << "\n\n";
	delay(500);
	if (numberOfvgmFiles > 0) {
		tft << numberOfvgmFiles << " vgm files found\n\n";
		delay(1000);
		tft.setTextColor(Pixel::Orange);
		tft << "LET'S ROCK!";
		delay(400);
	} else {
		tft.setTextColor(Pixel::Red);
		tft << "No vgm file found!\n\nPlease check you already save vgm into SD card!";
		while(1);
	}
	Serial.println("LET'S ROCK!");
}

/* Infinite VGM player LOOP */
void VGM_Player::run(void (*func)()) {

	// 44.1KHz tick
	ISRconfig(func);

	randomSeed(micros() + millis());

	// Begin
	newTrack(RND);
	vgmVerify();
	ResetChips();
	//Debug_GPIO.toggle();

	for (;;) {	// 無限迴圈
		if (waitSamples == 0) {			// 等待結束
			waitSamples += parseVGM();	// 解碼VGM資料
			topUpBuffer();				// 讀資料到緩衝區
		}
		// 播放完畢 若播放完畢，loopCount會加1
		if (loopCount >= maxLoops && playMode != LOOP) {
			bool nextTrack = false;
			if (playMode == SHUFFLE)		// 隨機
				nextTrack = newTrack(RND);
			if (playMode == IN_ORDER)		// 依序播放
				nextTrack = newTrack(NEXT);
			if (nextTrack) {
				vgmVerify();		// VGM資料較驗
				ResetChips();		// 重置 YM2151 & YM3012
			}
		}
		if (Serial.available() > 0)	// USART RX 緩衝區不為空
			handleSerialIn();		// USART 輸入
		handleButtons();			// 按鈕輸入
		drawTimebar();				// 時間進度條更新
	}
}

void VGM_Player::SDinit(void) {

	tft.setTextSize(1);
	tft.setTextColor(Pixel::Green);
	tft.setRotation(3);
	tft.fillScreen(Pixel::Black);
	tft.setCursor(0, 0);


	/* Check & Initialize the SD Card */
	if (!SD.begin()) {
		SD.errorHalt(&tft);
		tft.print("\nNo Card Respond!\n"
				  "Wrong CS pin?\nSPI problem?\n"
				  "or Just forgot to Plug-in the Card?");
		uint16_t font_x = tft.getCursorX();
		uint16_t font_y = tft.getCursorY();
		uint8_t wait_val = 0;

		tft.setTextColor(Pixel::Green, Pixel::Black);
		while (!SD.begin()) {

			tft.print('.');
			wait_val++;
			if (wait_val >= 3) {
				wait_val = 0;
				delay(250);
				tft.setCursor(font_x, font_y);
				tft.print("   ");
				tft.setCursor(font_x, font_y);
			}
			delay(250);
		}
	}

	numberOfvgmFiles = 0;
	numberOfFiles = 0;

	SD.vwd()->rewind();			// rewind to the first file

	/* Calculate How Many file the SD have */
	while (file.openNext(SD.vwd(), SDCard::O_READ)) {
		/*uint32_t VerifyTitle = 0;
		if (file.isFile() && file.fileSize() >= 0x40)
			file.read(&VerifyTitle, 4);
		if (VerifyTitle == 0x206D6756) {
			numberOfvgmFiles++;
		}*/
		if(isVGM(file)){
			numberOfvgmFiles++;
		}

		file.close();
		numberOfFiles++;
	}

	randomSeed(millis());

	file.close();
	SD.vwd()->rewind();			// rewind to the first file

}

void VGM_Player::handleSDfail(const char * str) {
	file.close();
	ResetChips();
	clearBuffers();
	tft.fillScreen(Pixel::Black);
	tft.setCursor(0, 0);
	tft.setTextColor(Pixel::Green);
	tft.println(str);
	delay(500);
	tft.println("\nSD Fix handler\n");
	delay(500);
	SDinit();
	waitSamples = 100;
	playMode = IN_ORDER;
	loopCount++;
	delay(100);
}

/* Print this Track information on TFT LCD Screen */
bool VGM_Player::drawTrackInfo(bool upAll) {

	static uint16_t Mode_color;
	if (upAll) {
		TrackColor = Pixel::HSV2RGB(rand() & 0xFF);

		tft.fillScreen(Pixel::Black);								// Clear All
		tft.setCursor(0, 0);										// Back to (0, 0)
		tft.setTextColor(Pixel::HSV2RGB(rand() & 0xFF));// Random Hue Color
		header.chips_usage(tft);

		tft.setCursor(tft.getCursorX(), tft.getCursorY() + 2);
		tft.setTextColor(Pixel::Light_Grey);
		tft << "\nTrack:\n";
		tft.setTextColor(TrackColor);
		tft << gd3.enTrackName;

		tft.setCursor(tft.getCursorX(), tft.getCursorY() + 2);
		tft.setTextColor(Pixel::Light_Grey);
		tft << "\nAuthor:\n";
		tft.setTextColor(Pixel::HSV2RGB(rand() & 0xFF));
		tft << gd3.enAuthor;

		tft.setCursor(tft.getCursorX(), tft.getCursorY() + 2);
		tft.setTextColor(Pixel::Light_Grey);
		if (gd3.enGameName != "") {
			tft << "\nGame Name:\n";
			tft.setTextColor(Pixel::HSV2RGB(rand() & 0xFF));
			tft << gd3.enGameName;
		} else {
			tft << "\nSystem:\n";
			tft.setTextColor(Pixel::HSV2RGB(rand() & 0xFF));
			tft << gd3.enSystemName;
		}

		tft.setCursor(tft.getCursorX(), tft.getCursorY() + 2);
		tft.setTextColor(Pixel::Light_Grey);
		tft << "\nMode:";
		Mode_color = Pixel::HSV2RGB(rand() & 0xFF);
		Mode_x = tft.getCursorX();
		Mode_y = tft.getCursorY();

		timebar_x = 0;
		timebar_y = tft.height() - 4;
		timebar_color = random(256);

		timebar_moveSample = timebar_total / tft.width();
	}

	tft.setTextColor(Mode_color);
	tft.setCursor(Mode_x, Mode_y);
	/*int16_t x1, y1;
	uint16_t w1, h1;
	tft.getTextBounds(InOR_str, Mode_x, Mode_y, &x1, &y1, &w1, &h1);*/
	tft.fillRect(Mode_x, Mode_y, 24, 8, Pixel::Black);
	if (playMode == LOOP)
		tft << "LOOP";
	else if (playMode == SHUFFLE)
		tft << "SHUF";
	else						// playMode = in order
		tft << "InOR";

	if (upAll) {
		tft.setTextColor(Pixel::Light_Grey);
		tft << " Time: ";
		tft.setTextColor(Pixel::HSV2RGB(rand() & 0xFF));
		uint32_t Total_time = (header.totalSamples / 44100);		// Song Total length(seconds)
		uint32_t Total_min = ((Total_time % 3600) / 60);
		uint32_t Total_sec = Total_time % 60;

		print2num(tft, Total_min);
		tft.print(':');
		print2num(tft, Total_sec);



		if (header.loopNumSamples != 0) {
			tft.print(" + ");
			Total_time = (header.loopNumSamples / 44100);				// Song Total length(seconds)
			Total_min = ((Total_time % 3600) / 60);
			Total_sec = Total_time % 60;

			print2num(tft, Total_min);
			tft.print(':');
			print2num(tft, Total_sec);
		}
		drawOPM_TL(0, 0xFF);
	}

	return true;
}

/* Print the notes that the channel playing, total 8 channel */
void VGM_Player::drawOPM_TL(uint8_t newVal, uint8_t CH) {
	const uint16_t bar_width = tft.width() / 8;
	const uint16_t bar_bottom = (tft.height() - 3) - timebar_width;
	uint16_t TL_maxrange = bar_bottom - (Mode_x + 9);

	//uint8_t OCT_val = (newVal & 0x70) >> 4;
	//uint8_t Note_val = newVal & 0x0F;
	uint8_t Tone_val = ((newVal & 0x7F) * TL_maxrange) >> 8;

	for (uint8_t i = 0; i < 8; i++) {
		if ((CH & (1 << i)) > 0) {

			if (OPM_TL[i] != Tone_val || Tone_val == 0) {
				// clear Old
				tft.fillRect(bar_width * i, (bar_bottom - OPM_TL[i]),
							 bar_width, 2, Pixel::Black);
				// draw New Val
				tft.fillRect(bar_width * i, (bar_bottom - Tone_val),
							 bar_width, 2, TrackColor);
				OPM_TL[i] = Tone_val;
			}
		}
	}
}

void VGM_Player::drawTimebar(void) {
	if(timebar_Sample_now - timebar_Sample_last >= timebar_moveSample){
		timebar_Sample_last = timebar_Sample_now;
		Pixel::RGB color_bar = Pixel::HSV2RGB(timebar_color + (timebar_x >> 1));
		tft.drawFastVLine(timebar_x - 1, timebar_y, timebar_width, color_bar);
		timebar_x++;	// move a step
		tft.drawFastVLine(timebar_x - 1, timebar_y, timebar_width, Pixel::White);
	}
}

/* Timer ISR configure */
void VGM_Player::ISRconfig(void (*func)()) {

	// function_view<void(void)> func = [this]() {tick(); };		// Iduno Y it dosn't wok?

	Timerx.IT_Config(0, (F_CPU / 44100) - 1);		// 44.1kHz interrupt setup
	Timerx.onInterrupt(func);
	Timerx.start();
}

bool VGM_Player::isVGM(SdFile& f){
	uint32_t VGMTitle = 0;
	if (f.isOpen() && f.isFile() && f.fileSize() >= 0x40){
		f.seekSet(0);
		f.read(&VGMTitle, 4);
	}
	if (VGMTitle == 0x206D6756) {
		return true;
	}
	return false;
}

/* Mount file and prepare for playback. Returns true if file is found. */
bool VGM_Player::newTrack(newTrackMode fileStrategy, happe::String request) {

	ready = false;


	redo:

	memset(fileName, 0x00, MAX_FILE_NAME_SIZE);
	{
		SdFile nextFile;						// it'll be destroyed when it find a VGM
		do {
			nextFile.close();

			switch (fileStrategy) {
				case NOT_A_MODE: {
					/* Nothing */
				}
					break;
				case FIRST_START: {
					SD.vwd()->rewind();
					nextFile.openNext(SD.vwd(), SDCard::O_READ);
					//nextFile.getName(fileName, MAX_FILE_NAME_SIZE);
					//nextFile.close();
					currentFileNumber = 0;
				}
					break;
				case NEXT: {
					if (currentFileNumber + 1 >= numberOfFiles) {
						SD.vwd()->rewind();
						currentFileNumber = 0;
					} else
						currentFileNumber++;
					nextFile.openNext(SD.vwd(), SDCard::O_READ);
					//nextFile.getName(fileName, MAX_FILE_NAME_SIZE);
					//nextFile.close();
				}
					break;
				case PREV: {
					SD.vwd()->rewind();
					currentFileNumber =
							(currentFileNumber != 0) ?
									currentFileNumber - 1 : numberOfFiles - 1;

					for (uint32_t i = 0; i <= currentFileNumber; i++) {
						nextFile.close();
						nextFile.openNext(SD.vwd(), SDCard::O_READ);
					}
					//nextFile.getName(fileName, MAX_FILE_NAME_SIZE);
					//nextFile.close();
				}
					break;
				case RND: {
					randomSeed(millis());
					uint32_t randomFile = currentFileNumber;
					if (numberOfFiles > 1) {
						while (randomFile == currentFileNumber)
							randomFile = random(numberOfFiles - 1);
					}
					currentFileNumber = randomFile;
					SD.vwd()->rewind();
					nextFile.openNext(SD.vwd(), SDCard::O_READ);
					{
						for (uint32_t i = 0; i < randomFile; i++) {
							nextFile.close();
							nextFile.openNext(SD.vwd(), SDCard::O_READ);
						}
					}
					//nextFile.getName(fileName, MAX_FILE_NAME_SIZE);
					//nextFile.close();
				}
					break;
				case REQUEST: {
					SD.vwd()->rewind();
					bool fileFound = false;
					Serial.print("REQUEST: ");
					Serial.println(request);
					for (uint32_t i = 0; i < numberOfFiles; i++) {
						nextFile.close();
						nextFile.openNext(SD.vwd(), SDCard::O_READ);
						nextFile.getName(fileName, MAX_FILE_NAME_SIZE);
						happe::String tmpFN = happe::String(fileName);
						tmpFN.trim();
						request.trim();
						if (tmpFN == request) {
							currentFileNumber = i;
							fileFound = true;
							break;
						}
					}
					nextFile.close();
					if (fileFound) {
						Serial.println("File found!");
					} else {
						Serial.println("Not found! Continue");
						ready = true;
						return false;
					}
				}
					break;
			}



			if(!nextFile.isOpen())
				nextFile.open(fileName, SDCard::O_READ);
			else
				nextFile.getName(fileName, MAX_FILE_NAME_SIZE);
		} while (!isVGM(nextFile));

		nextFile.close();
	}

	//cmdPos = 0;
	//bufferPos = 0;
	waitSamples = 127;
	loopCount = 0;

	if (file.isOpen())
		file.close();

	file.open(fileName, SDCard::O_READ);

	// This file is VGM for sure, guarantee!
	if (!file.isOpen()) {
		goto redo;
	}

	/*
	uint32_t VerifyTitle = 0;
	if (file.isFile()) {
		file.read(&VerifyTitle, 4);
		file.seekSet(0);
		if (VerifyTitle != 0x206D6756) {
			file.close();
			Serial.println("Not VGM, Skip!");
			goto redo;
			//return newTrack(fileStrategy);
		}

	} else {
		file.close();
		goto redo;
	}*/

	/*if (!isVGM(file)) {
		file.close();
		Serial.println("Not VGM, Skip!");
		goto redo;
	}*/

	ResetChips();
	clearBuffers();
	memset(&loopPreBuffer, 0, LOOP_PREBUF_SIZE);	// clear all the Buffer
	header.reset();
	fillBuffer();

	//VGM Header Offset: 0x00 ~ 0x3C
	/*header.indent = readBuffer32();
	 header.EoF = readBuffer32();
	 header.version = readBuffer32();
	 header.sn76489Clock = readBuffer32();
	 header.ym2413Clock = readBuffer32();
	 header.gd3Offset = readBuffer32();
	 header.totalSamples = readBuffer32();
	 header.loopOffset = readBuffer32();
	 header.loopNumSamples = readBuffer32();
	 header.rate = readBuffer32();
	 header.snX = readBuffer32();
	 header.ym2612Clock = readBuffer32();
	 header.ym2151Clock = readBuffer32();
	 header.vgmDataOffset = readBuffer32();
	 header.segaPCMClock = readBuffer32();
	 header.spcmInterface = readBuffer32();*/

	// read first 64 Bytes
	for (size_t i = 0; i < 0x40; i++) {
		header[i] = readBuffer();
	}

	// Offset: 0x40
	// Jump to VGM data start and compute loop location
	if (header.vgmDataOffset == 0x0C)
		header.vgmDataOffset = 0x40;
	else
		header.vgmDataOffset += 0x34;

	if (header.vgmDataOffset > 0x40) {
		// uint8_t * Trackinfo = reinterpret_cast<uint8_t *>(&(header[0x40]));			// 剩下的資料
		for (uint32_t i = 0x40; i < header.vgmDataOffset; i++) {
			/*
			if (Trackinfo < (reinterpret_cast<uint8_t *>(&(
					(header.version >= 0x160) ? header[0xC8] : header[0x7C]) + 0x04)))
				*(Trackinfo++) = readBuffer();
			else
				readBuffer();
			*/
			if(i < sizeof(header))
				header[i] = readBuffer();

		}
	}

	if (header.loopOffset == 0x00) {
		header.loopOffset = header.vgmDataOffset;
	} else
		header.loopOffset += 0x1C;

	timebar_total = header.totalSamples + header.loopNumSamples;		// first time playback time

	if (header.loopNumSamples != 0) {
		have_loop = 1;
	}else{
		have_loop = 0;
	}

	prebufferLoop();

	{
		auto calculatefileSize = [&](Print& os, uint32_t size) {
			uint32_t decimal = 0;
			uint8_t sci;

			const char * unit_str = " kMG";
			char buf[4] = "  B";

			for(sci = 0; size >= 1024; sci++){
				decimal = (size) & 0x1FF;				// get remainder of divided by 1024
				size >>= 10;						// divided by 1024
			}

			decimal += 50;
			if (decimal >= 1024) {
				decimal -= 1024;
				size += 1;
			}

			decimal = map(decimal, 0, 1023, 0, 99);

			buf[1] = unit_str[sci];

			if(sci > 0){
				os << size << '.' << print2num(os, decimal) << buf;
			} else {
				os << size << buf;
			}
		};

		tft.fillScreen(Pixel::Black);								// Clear All
		tft.setCursor(0, 0);										// Back to (0, 0)



		tft.setTextColor(Pixel::Light_Grey);
		tft << "Name: ";											// file Name
		tft.setTextColor(Pixel::Green);
		tft << fileName;

		tft.setTextColor(Pixel::Light_Grey);
		tft	<< "\n\nFile Size: ";
		tft.setTextColor(Pixel::GreenYellow);
		calculatefileSize(tft, file.fileSize());					// file Size

/*
		tft.setTextColor(Pixel::Light_Grey);
		tft << "\n\nCreate at \n";
		tft.setTextColor(Pixel::Pink);
		file.printCreateDateTime(&tft);								// file Create DateTime
*/

		tft.setTextColor(Pixel::Light_Grey);
		tft << "\n\nLast Modified at\n";

		tft.setTextColor(Pixel::Orange);
		file.printModifyDateTime(&tft);								// file last Modified DateTime


		/**
		 *	##############################
		 *	Name: 'fileName'
		 *	File Size: 'calculatefileSize()'
		 *	Last Modified at
		 *	'file.printModifyDateTime()'
		 *	##############################
		 *
		 */
		Serial.printlnDelimiter('#', 30);
		Serial.print("Name: ");
		Serial.println(fileName);
		Serial.print("File Size: ");
		calculatefileSize(Serial, file.fileSize());
		Serial.println("\r\nLast Modified at");
		file.printModifyDateTime(&Serial);
		Serial.println();
		Serial.printlnDelimiter('#', 30);
	}

	delay(select_delay);
	/* λ: delay with Lambda! */
	if (delay(500, [&]() {		// capture all by reference
		if (this->NextButton.read() != true ||
			this->PrevButton.read() != true ||
			this->RandButton.read() != true) {
			return true;
		}
		return false;
	})){

		fileStrategy = checkButtons();

		if(select_delay == 500){				// 連續選歌
			select_delay = 100;
		} else if (select_delay > 40) {
			select_delay -= 10;
		}
		goto redo;
	}

	select_delay = 500;						// 第一次按下按鈕時的切換速度較慢

	/*if (header.ym2151Clock >= 3700000) {	// Switch the FM-Synth work frequency
		FreqSwitch.setHigh();
	} else {
		FreqSwitch.setLow();
	}*/

	FreqSwitch.setState(header.ym2151Clock >= 3789773);

	ResetChips();


	return true;
}

bool VGM_Player::vgmVerify() {
	if (header.indent != 0x206D6756) { // VGM. Indent check
		Serial.println("Verify fail");
		newTrack(NEXT);
		return false;
	}
	readGD3();
	printTrackInfo();
	drawTrackInfo();
	ready = true;
	return true;
}

void VGM_Player::printTrackInfo() {
	Serial.printlnDelimiter('*', 30);
	Serial.println(gd3.enGameName);
	Serial.println(gd3.enTrackName);
	Serial.println(gd3.enAuthor);
	Serial.println(gd3.enSystemName);
	Serial.println(gd3.releaseDate);
	Serial.print("Version: ");
	Serial.println(header.version, HEX);
	Serial.printlnDelimiter('*', 30);
}

void VGM_Player::readGD3() {
	uint32_t prevLocation = file.curPosition();
	uint32_t tag = 0;
	gd3.reset();
	file.seekSet(header.gd3Offset + 0x14);
	for (int i = 0; i < 4; i++) {
		tag += static_cast<uint32_t>(file.read());
	}
	//Get GD3 tag bytes and add them up for an easy comparison.
	if (tag != 0xFE) { //GD3 tag bytes do not sum up to the constant. No valid GD3 data detected.
		Serial.print("INVALID GD3 SUM:");
		Serial.println(tag);
		file.seekSet(prevLocation);
		return;
	}
	for (int i = 0; i < 4; i++) {
		file.read();
	} //Skip version info
	  //uint8_t v[4];
	file.read(&(gd3.size), 4);
	//gd3.size = uint32_t(v[0] + (v[1] << 8) + (v[2] << 16) + (v[3] << 24));
	char a, b;
	uint8_t itemIndex = 0;
	for (uint32_t i = 0; i < gd3.size; i++) {
		a = file.read();
		b = file.read();
		if (a + b == 0) { //Double 0 detected
			itemIndex++;
			continue;
		}
		switch (itemIndex) {
			case 0:
				gd3.enTrackName += a;
				break;
			case 1:
				// japanese track name
				break;
			case 2:
				gd3.enGameName += a;
				break;
			case 3:
				// japanese Game name
				break;
			case 4:
				gd3.enSystemName += a;
				break;
			case 5:
				// japanese System name
				break;
			case 6:
				gd3.enAuthor += a;
				break;
			case 7:
				// japanese Author name
				break;
			case 8:
				gd3.releaseDate += a;
				break;
			default:
				//IGNORE CONVERTER NAME + NOTES
				break;
		}
	}
	file.seekSet(prevLocation);		// back to starting point
}

void VGM_Player::removeMeta() { // Remove useless meta files
	SdFile tmpFile;
	while (tmpFile.openNext(SD.vwd(), SDCard::O_READ)) {
		memset(fileName, 0x00, MAX_FILE_NAME_SIZE);
		tmpFile.getName(fileName, MAX_FILE_NAME_SIZE);
		if (fileName[0] == '.') {
			if (!SD.remove(fileName))
				if (!tmpFile.rmRfStar()) {
					Serial.print("FAILED TO DELETE META FILE");
					Serial.println(fileName);
				}
		}
		if (happe::String(fileName) == "System Volume Information") {
			if (!tmpFile.rmRfStar())
				Serial.println("FAILED TO REMOVE SVI");
		}
		tmpFile.close();
	}
	tmpFile.close();
	SD.vwd()->rewind();
}

// Keep a small cache of commands right at the loop point to prevent excessive SD seeking lag
void VGM_Player::prebufferLoop() {
	uint32_t prevPos = file.curPosition();
	file.seekSet(header.loopOffset);
	file.read(loopPreBuffer, LOOP_PREBUF_SIZE);
	file.seekSet(prevPos);
}

// On loop, inject the small prebuffer back into the main ring buffer
void VGM_Player::injectPrebuffer() {
	for (size_t i = 0; i < LOOP_PREBUF_SIZE; i++)
		cmdBuffer.push_back(loopPreBuffer[i]);
	file.seekSet(header.loopOffset + LOOP_PREBUF_SIZE);
	//cmdPos = LOOP_PREBUF_SIZE - 1;
}

// Completely fill command buffer
void VGM_Player::fillBuffer() {
	while (!topUpBuffer()) {
	};
}

// Add to buffer from SD card. Returns true when buffer is full
bool VGM_Player::topUpBuffer() {
	if (cmdBuffer.full())
		return true;
	if (cmdBuffer.available() >= file.fileSize())
		return true;

	uint8_t data;
	if (file.read(&data, 1) >= 0) {							// check is it read data successfully.
		cmdBuffer.push_back(data);
	} else {
		handleSDfail("SD reading fail!");					// SD fail handler
	}

	//bufferPos = 0;
	return false;
}

void VGM_Player::clearBuffers() {
	//bufferPos = 0;
	cmdBuffer.clear();
}

uint8_t VGM_Player::readBuffer() {
	if (cmdBuffer.empty()) { //Buffer exauhsted prematurely. Force replenish
		topUpBuffer();
	}
	//bufferPos++;
	//cmdPos++;
	return cmdBuffer.pop_front();
}

uint16_t VGM_Player::readBuffer16() {
	uint16_t d;
	uint8_t v0 = readBuffer();
	uint8_t v1 = readBuffer();
	d = uint16_t(v0 + (v1 << 8));
	//bufferPos += 2;
	//cmdPos += 2;
	return d;
}

uint32_t VGM_Player::readBuffer32() {
	uint32_t d;
	uint8_t v0 = readBuffer();
	uint8_t v1 = readBuffer();
	uint8_t v2 = readBuffer();
	uint8_t v3 = readBuffer();
	d = uint32_t(v0 + (v1 << 8) + (v2 << 16) + (v3 << 24));
	//bufferPos += 4;
	//cmdPos += 4;
	return d;
}

// Read 32 bits right off of the SD card.
uint32_t VGM_Player::readSD32() {
	uint32_t d;
	//byte v[4];
	file.read(reinterpret_cast<uint8_t *>(&d), 4);
	//d = uint32_t(v[0] + (v[1] << 8) + (v[2] << 16) + (v[3] << 24));
	return d;
}

// Execute next VGM command set. Return back wait time in samples
uint16_t VGM_Player::parseVGM() {
	uint8_t cmd = readBuffer();
	switch (cmd) {

		/* Sound Chips Command handler */
		case 0x51 ... 0x5F: {				// Yamaha Sound Chips handler command
			uint8_t a = readBuffer();
			uint8_t d = readBuffer();

			switch (cmd) {
				case 0x54:					// 0x54 is a command value for YM2151
					opm.sendData(a, d);
					if (a >= 0x28 && a < 0x30) {
						drawOPM_TL(d, 1 << (a & 0x07));
					}
					break;
			}
			break;
		}

		/* Wait Samples handler */
		case 0x61:
			return readBuffer16();
		case 0x62:
			return 735;		// Wait 735 Samples
		case 0x63:
			return 882;		// Wait 882 Samples
		case 0x70 ... 0x7F: {
			return (cmd & 0x0F) + 1;
		}

		/* PCMs, PWMs or DACs ROM Datas block handler */
		case 0x67:			// Ignore PCM data blocks
		{
			// to be continued (may Oki MSM6295)
			if (readBuffer() != 0x66) 	// it should be 0x66
				break;
			readBuffer(); 	// Datatype
			uint32_t pcmSize = readBuffer32(); 			// Payload size;
			for (uint32_t i = 0; i < pcmSize; i++)
				readBuffer();
			break;
		}

		/* End of Tracks handler */
		case 0x66: {
			ready = false;
			clearBuffers();
			//cmdPos = 0;
			injectPrebuffer();

			if (have_loop == 0) {
				if (playMode != LOOP) {
					loopCount++;
				}
				timebar_total = (header.loopNumSamples != 0) ? header.loopNumSamples : header.totalSamples;
				tft.fillRect(0, timebar_y, tft.width(), timebar_width, Pixel::Black);
				timebar_Sample_now = 0;
				timebar_Sample_last = 0;
				timebar_x = 0;

				timebar_x = 0;

				timebar_moveSample = timebar_total / tft.width();
			} else {
				have_loop--;
			}

			ready = true;
		}
			return 0;

		/* reserved */
			/*case 0x30 ... 0x3F:
			 readBuffer();
			 break;
			 case 0x40 ... 0x4E:
			 if (header.version >= 0x0160)
			 readBuffer();
			 else
			 readBuffer16();
			 break;
			 case 0xA1 ... 0xAF:
			 readBuffer16();
			 break;*/

		case 0xB5 ... 0xBF: // Ignore common secondary PCM chips
			readBuffer16();
			break;
		case 0xC0 ... 0xC3: // Ignore SegaPCM:
			readBuffer();
			readBuffer();
			readBuffer();
			break;
			/*case 0xD7 ... 0xDF:
			 readBuffer();
			 readBuffer();
			 readBuffer();
			 break;
			 case 0xE2 ... 0xFF:
			 readBuffer32();
			 break;*/

		default:
			/*Serial << "error Cmd: 0x";
			 Serial.println(cmd, HEX);*/
			return 0;
	}
	return 0;
}

// Poll the serial port
void VGM_Player::handleSerialIn() {
	bool nextTrack = false;
	while (Serial.available()) {
		char serialCmd = Serial.read();
		switch (serialCmd) {
			case '+':
				nextTrack = newTrack(NEXT);
				break;
			case '-':
				nextTrack = newTrack(PREV);
				break;
			case '*':
				nextTrack = newTrack(RND);
				break;
			case '/':
				playMode = SHUFFLE;
				break;
			case '.':
				playMode = LOOP;
				break;
			case '?':
				printTrackInfo();
				break;
			case '!':
				playMode = IN_ORDER;
				break;
			case 'r': {			// "r:Trackname"
				//std::string req = Serial.readString();
				happe::String req = Serial.readString();
				req.remove(0, 1); //Remove colon character
				nextTrack = newTrack(REQUEST, req);
			}
				break;
			default:
				continue;
		}
	}
	drawTrackInfo(false);
	if (nextTrack) {
		vgmVerify();
		ResetChips();
	}
}

VGM_Player::newTrackMode VGM_Player::checkButtons() {
	newTrackMode modes = NOT_A_MODE;
	if (NextButton.read() != true)
		modes = NEXT;
	if (PrevButton.read() != true)
		modes = PREV;
	if (RandButton.read() != true)
		modes = RND;
	return modes;
}

void VGM_Player::handleButtons() {
	//Check for button input
	static bool buttonLock = false;
	static uint32_t lock_timer;
	bool nextTrack = false;
	//uint32_t count = 0;
	newTrackMode modes = checkButtons();

	if(modes != NOT_A_MODE)
		nextTrack = newTrack(modes);

	if (ModeButton.read() != true && !buttonLock) {
		if (playMode == SHUFFLE)
			playMode = LOOP;
		else if (playMode == LOOP)
			playMode = IN_ORDER;
		else if (playMode == IN_ORDER)
			playMode = SHUFFLE;
		drawTrackInfo(false);
		buttonLock = true;
		lock_timer = millis();
	}

	if (millis() - lock_timer >= 50) {
		lock_timer = millis();
		if (buttonLock) {
			if (ModeButton.read())
				buttonLock = false;
		}
	}

	if (nextTrack) {
		vgmVerify();
		ResetChips();
		delay(10);
	}
}
