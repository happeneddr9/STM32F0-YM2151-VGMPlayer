# STM32F0 YM2151 VGMPlayer

### Description
This is a little project reference from [AidanHockey5/YM2151_VGM_STM32](https://github.com/AidanHockey5/YM2151_VGM_STM32), but this is not using BluePill(STM32F103), is STM32F0 serise; using SW4STM32 with standard peripheral library.

Compared with BluePill, the STM32F0 series chip has a slower main clock, less Flash and RAM, and fewer peripherals, but that is more than enough for this project, and it is very cheap.

Using STM32F030C8 as the brain of this project, ST77xx TFT LCD for displaying VGM information, and SdFat for SD cards and file system control, most of the libraries have been optimized for STM32F0 series.

STM32F030C8 has 64kB Flash and 8kB SRAM, this project usage:
```
   text	   data	    bss	    dec	    hex
  30192	    512	   3692	  34396	   865c
```
it's very tiny, I guess.

### Requirements
- Since some files used lambda or some "modern" features, it only work with c++11 or later.
- Have STM32F0 chips or development board, potatoes isn't works well.
- Able to read sentences with awful grammar.

### Features
- SPI interface is bound with DMA, can run at full-speed (12Mbps @48MHz).
- VGMPlayer functions packed into a class.
- Have a HSV and RGB color control library, you can choose colors you like.
- ST7735 and ST7789 TFT are both supported, change it in main.cpp.

### Libraries Reference and Ported
ST77xx library source: [ST77xx Library](https://github.com/adafruit/Adafruit-ST7735-Library) by Adafruit. \
SD cards and FileSystem library source: [SdFat](https://github.com/greiman/SdFat) by greiman.

### Difference with original
- Different microcontroller used.
- I cannot buy the LTC6903, so I used analog multiplexer for switching two different clock.
- From 5 buttons to 4, `Mode`, `Prev`, `Rand` and `Next` buttons, have three playmode.
- Screen is full-color in here.
- Have a timebar in the screen bottom, show you total time and how much time is left.
- Timebar is RGB!
- The song will loop once if it has non-zero loop offset.
- Added a SD Cards error handler, will be call when it can't get response from SD Card, code will be stuck in here until SD Card response.

