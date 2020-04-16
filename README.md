# STM32F0 YM2151 VGMPlayer

### Description
This is a little project reference from [AidanHockey5/YM2151_VGM_STM32](https://github.com/AidanHockey5/YM2151_VGM_STM32), but this is not using BluePill(STM32F103), is STM32F0 serise; using SW4STM32 with standard peripheral library.

Compared with BluePill, the STM32F0 series chip has a slower main clock, less Flash and RAM, and fewer peripherals, but that is more than enough for this project, and it is very cheap.

Using STM32F030C8 as the brain of this project, ST77xx TFT LCD for displaying VGM information, and SdFat for SD cards and file system control, most of the libraries have been optimized for STM32F0 series.

### Requirements
- Since some files used lambda or some "modern" features, it only work with c++11 or later.
- Have STM32F0 chips or development board, potatoes isn't works well.
- Able to read sentences with awful grammar.

### Libraries Reference
ST77xx library source: [ST77xx TFT LCD](https://github.com/adafruit/Adafruit-ST7735-Library) \
SD cards and FileSystem library source: [SdFat](https://github.com/greiman/SdFat)

### Difference with original


