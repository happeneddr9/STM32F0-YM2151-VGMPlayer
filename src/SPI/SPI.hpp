/**
 * @Name    : SPI.hpp
 * @Date    : 2019年12月10日 下午6:47:14
 * @Author  : happe
 * @brief   : SPI interface
 *
 * @Log     :
 * 		2019/12/14
 * 			新增無中斷SPI lite
 *
 * 			add Spi_lite
 * 		2019/12/15
 * 			新增DMA SPI
 *
 * 			add SPI with DMA
 * 			## not Working
 * 		2019/12/21
 * 			DMA SPI Transmit Finally Working   # DMA TC interrupt mode
 */

#ifndef SPI_SPI_HPP_
#define SPI_SPI_HPP_

#include "GPIO/GPIO.h"
#include "SPI_config.h"
#include "SYSTEM/DMA.hpp"
#include "Arduino/Basic.h"

// using While in Read Write
class Spi_lite {
	public:
		Spi_lite(SPI_TypeDef * addr)
				: SPIx(addr) {

			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //TODO: func

		}

		void begin(const SPI_PinMap::FullDuplex& PinMap = SPI_PinMap::DefaultMap) const {

			Gpio SCLK = PinMap.SCLK;
			Gpio MOSI = PinMap.MOSI;
			Gpio MISO = PinMap.MISO;

			SCLK.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);
			MOSI.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);
			MISO.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullUp);

			SCLK.setAF(GpioAF::AF0);
			MOSI.setAF(GpioAF::AF0);
			MISO.setAF(GpioAF::AF0);

			SPI_InitTypeDef SPI_InitStruct;

			disable();
			SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
			SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
			SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
			SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
			SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
			SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
			SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
			SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
			SPI_InitStruct.SPI_CRCPolynomial = 7;
			SPI_Init(SPIx, &SPI_InitStruct);

			SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
			enable();
		}

		void setClockPrescaler(uint16_t Prescaler) {
			disable();
			SPIx->CR1 &= ~(0x38);
			SPIx->CR1 |= Prescaler;
			enable();
		}

		inline bool readyToReceive() const {
			return !SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE);
		}

		inline bool readyToSend() const {
			return !SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE);
		}

		inline uint8_t ReadWriteByte(uint8_t Data) {

			uint32_t spixbase = 0x00;

			spixbase = (uint32_t) SPIx;
			spixbase += 0x0C;
			uint8_t retry = 0;
			while (readyToSend()) {
				if (retry++ > 200) {
					return 0xFF;
				}
			}
			*(__IO uint8_t *) spixbase = Data;
			retry = 0;
			while (readyToReceive()) {
				if (retry++ > 200) {
					return 0xFF;
				}
			}
			return *(__IO uint8_t *) spixbase;
		}

		/*inline void setNSS(bool val) {
		 _NSSpin.setState(val);
		 }*/

		inline bool hasError() const {
			FlagStatus status;
			if ((status = SPI_I2S_GetFlagStatus(SPIx, SPI_FLAG_CRCERR | SPI_FLAG_MODF | SPI_I2S_FLAG_OVR)) != 0)
				return true;

			return false;
		}

		inline void enable() const {
			SPI_Cmd(SPIx, ENABLE);
		}

		inline void disable() const {
			SPI_Cmd(SPIx, DISABLE);
		}

	private:
		/*enum {
		 E_SPI_ERROR = 1
		 };*/
	protected:
		SPI_TypeDef * SPIx;
		//Gpio _NSSpin;
};

static uint16_t dummy_byte = 0;


class Spi_dma {

	public:
		enum Debug_Status {
			idle = 0,
			DMA_Busy = 1
		};

		Spi_dma(GpioPin _nss, SPI_TypeDef * addr, IRQn vect,
				Dma DmaTx = Dma(DMA1_Channel3, DMA1_Channel2_3_IRQn),
				Dma DmaRx = Dma(DMA1_Channel2, DMA1_Channel2_3_IRQn))
				: SPIx(addr), DMA_RX(DmaRx), DMA_TX(DmaTx) {

			_NSSpin = _nss;
			_NSSpin.init(GpioMode::Out);
			RCC->APB2ENR |= RCC_APB2Periph_SPI1;  		//TODO: func
			RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
			Nvic::configureIrq(vect, ENABLE);
			HWCS = true;
		}

		Spi_dma(SPI_TypeDef * addr, IRQn vect,
				Dma DmaTx = Dma(DMA1_Channel3, DMA1_Channel2_3_IRQn),
				Dma DmaRx = Dma(DMA1_Channel2, DMA1_Channel2_3_IRQn))
				: SPIx(addr), DMA_RX(DmaRx), DMA_TX(DmaTx){

			_NSSpin = NoPin;
			RCC->APB2ENR |= RCC_APB2Periph_SPI1;  		//TODO: func
			RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
			Nvic::configureIrq(vect, ENABLE);
			HWCS = false;
		}

		/*
		 *  @brief : SPI Default CLK Prescaler = 8
		 */
		void begin(const SPI_PinMap::FullDuplex& PinMap = SPI_PinMap::DefaultMap,
				   SPI_DataSize data_size = SPI_DataSize::DataSize_8b) const {
			{
				Gpio SCLK = PinMap.SCLK;
				Gpio MOSI = PinMap.MOSI;
				Gpio MISO = PinMap.MISO;

				/*SCLK.init(GpioMode::AF);
				 MOSI.init(GpioMode::AF);
				 MISO.init(GpioMode::AF);*/

				SCLK.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullDown);
				MOSI.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullDown);
				MISO.init(GpioMode::AF, GpioOutType::PP, GpioSpeed::s50MHz, GpioPuPd::PullDown);

				SCLK.setAF(PinMap.PinAF);		// TODO: func select
				MOSI.setAF(PinMap.PinAF);
				MISO.setAF(PinMap.PinAF);
			}


			SPI_InitTypeDef SPI_InitStruct;

			disable();
			SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
			SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
			SPI_InitStruct.SPI_DataSize = static_cast<uint16_t>(data_size);
			SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
			SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
			//SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
			//SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
			SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
			SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
			SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
			SPI_InitStruct.SPI_CRCPolynomial = 7;
			SPI_Init(SPIx, &SPI_InitStruct);
			SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);

			DMA_RX.ClearConfig();
			DMA_RX.Memory2Memory(false);
			DMA_RX.CircularMode(false);
			DMA_RX.PeripheralIncrement(false);
			DMA_RX.MemorySize(DMA_Defines::W8_bits);
			DMA_RX.PeripheralSize(DMA_Defines::W8_bits);
			DMA_RX.DataDirection(DMA_Defines::P2M);
			DMA_RX.PriorityLevel(DMA_Defines::High);

			DMA_TX.ClearConfig();
			DMA_TX.Memory2Memory(false);
			DMA_TX.CircularMode(false);
			DMA_TX.PeripheralIncrement(false);
			DMA_TX.MemorySize(DMA_Defines::W8_bits);
			DMA_TX.PeripheralSize(DMA_Defines::W8_bits);
			DMA_TX.DataDirection(DMA_Defines::M2P);
			DMA_TX.PriorityLevel(DMA_Defines::Low);

			setCSHigh();

			SPIx->CR2 |= SPI_I2S_DMAReq_Tx;
			SPIx->CR2 |= SPI_I2S_DMAReq_Rx;

			//Nvic::configureIrq(DMA1_Channel2_3_IRQn, ENABLE);
			//SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
		}

		void setClockPrescaler(SPI_Prescaler Prescaler = SPI_Prescaler::fPCLK_4) const {
			while(isBusy());
			disable();
			SPIx->CR1 &= (static_cast<uint16_t>(~__make_mask(3, SPI_Defines::CR1_BR)));
			SPIx->CR1 |= static_cast<uint16_t>(Prescaler);
			enable();
		}

		void setDataWidth(SPI_DataSize DataSize = SPI_DataSize::DataSize_8b){
			while(isBusy());
			disable();
			DMA_Defines::Memory_BW MEM_BW = DMA_Defines::W8_bits;
			if(DataSize > SPI_DataSize::DataSize_8b)
				MEM_BW = DMA_Defines::W16_bits;

			DMA_TX.PeripheralSize(MEM_BW);
			DMA_TX.MemorySize(MEM_BW);
			DMA_RX.PeripheralSize(MEM_BW);
			DMA_RX.MemorySize(MEM_BW);
			SPIx->CR2 &= (static_cast<uint16_t>(~__make_mask(4, SPI_Defines::CR2_DS)));
			SPIx->CR2 |= static_cast<uint16_t>(DataSize);
			enable();
		}

		finline bool isBusy() const {
			yield();
			return (DMA_TX.isBusy() || DMA_RX.isBusy());
		}

		inline void setCSPin(GpioPin&& _CS)  {
			setCSPin(GpioFast(_CS));
		}

		/*inline void setCSPin(GpioFast&& _CS) {
			_NSSpin = _CS;
			_NSSpin.init(GpioMode::Out);
		}*/

		inline void setCSPin(GpioFast& _CS) {
			setCSPin(std::move(_CS));
		}

		inline void setCSPin(GpioFast&& _CS)  {
			_NSSpin = _CS;
			_NSSpin.init(GpioMode::out);
		}

		finline void setCSHigh() const {
			_NSSpin.setHigh();
		}

		finline void setCSLow() const {
			_NSSpin.setLow();
		}

		finline void CSEnable() const {
			setCSLow();
		}

		finline void CSDisable() const {
			setCSHigh();
		}


		inline void write(uint16_t data, bool hardwareCS = true) {
			WriteRead(&data, &dummy_byte, 1, true, false, hardwareCS);
			while (isBusy());
		}

		inline uint16_t read(uint16_t dummy = 0x00, bool hardwareCS = true) {
			uint16_t tmp;
			dummy_byte = dummy;

			WriteRead(&dummy_byte, &tmp, 1, false, true, hardwareCS);
			while (isBusy());
			return tmp;
		}

		void Dummy_n(uint16_t len, bool hardwareCS = true, uint16_t dummy_val = 0x00){
			dummy_byte = dummy_val;
			WriteRead(&dummy_byte, &dummy_byte, len, false, false, hardwareCS);
		}

		void Write_n(const void * Data, uint16_t len, bool hardwareCS = true)  {
			WriteRead(Data, &dummy_byte, len, true, false, hardwareCS);
		}

		void Read_n(void * Data, uint16_t len, bool hardwareCS = true, uint16_t dummy_val = 0x00)  {
			dummy_byte = dummy_val;
			WriteRead(&dummy_byte, Data, len, false, true, hardwareCS);
		}

		void WriteRead(const void * Data, void * Read_Data, uint16_t len,
					   bool TransMemoryInc = true, bool ReadMemoryInc = true,
					   bool hardwareCS = true)  {

			if (len == 0)
				return;
			DMA_TX.cmd(DISABLE);
			DMA_RX.cmd(DISABLE);

			/* Clear the selected DMA flags */
			/*DMA_ClearFlag(DMA1_FLAG_GL2);
			DMA_ClearFlag(DMA1_FLAG_GL3);
			DMA_ClearFlag(DMA1_FLAG_TC2);
			DMA_ClearFlag(DMA1_FLAG_TC3);*/
			//DMA_TX.clearITFlag(DMA_Defines::GIFx);
			//DMA_RX.clearITFlag(DMA_Defines::GIFx);

			HWCS = hardwareCS;
			DMA_TX.MemoryIncrement(TransMemoryInc);			// 位址是否遞增
			DMA_RX.MemoryIncrement(ReadMemoryInc);
			/* DMA channel Tx of SPI Configuration */
			//DMA_TX.ITConfig(DMA_Defines::TCI, ENABLE);

			/* DMA channel Rx of SPI Configuration */
			DMA_RX.ITConfig(DMA_Defines::TCI, ENABLE);

			DMA_TX.Ready(&(SPIx->DR), Data, len);
			DMA_RX.Ready(&(SPIx->DR), Read_Data, len);
			//DMA_ITConfig(DMA_RX.Channel(), DMA_IT_TC, ENABLE);
			enable();
			if(HWCS)
				_NSSpin.setLow();
			DMA_TX.enable();
			DMA_RX.enable();

			/*__IT_DMA_RX_TC();*/
		}

		finline void enable() const {
			SPIx->CR1 |= SPI_CR1_SPE;
		}

		finline void disable() const {
			SPIx->CR1 &= static_cast<uint16_t>(~(static_cast<uint16_t>(SPI_CR1_SPE)));
		}

		// put it into IRQ handler
		inline void IRQ_handler(void) {
			if (isTransDoneIT()) {
				__IT_DMA_RX_TC();
				clearTransDoneIT();
			}
		}

		finline void __IT_DMA_RX_TC(void) {
			if(HWCS)												// not a NoPin
				_NSSpin.setHigh();									// CS 拉高
			//DMA_TX.ITConfig(DMA_Defines::TCI, DISABLE);				// 關閉DMA TX TC中斷
			DMA_RX.ITConfig(DMA_Defines::TCI, DISABLE);				// 關閉DMA RX TC中斷

			DMA_TX.disable();									// 關閉DMA TX
			DMA_RX.disable();									// 關閉DMA RX

			//SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
			//SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, DISABLE);

			/* Clear DMA1 global flags */
			/*DMA_ClearFlag(DMA1_FLAG_GL3);
			DMA_ClearFlag(DMA1_FLAG_GL2);*/
			//DMA_TX.clearITFlag(DMA_Defines::GIFx);
			//DMA_RX.clearITFlag(DMA_Defines::GIFx);
			disable();
			//SPIx->CR2 &= ~SPI_I2S_DMAReq_Tx;
			//SPIx->CR2 &= ~SPI_I2S_DMAReq_Rx;
		}

		inline bool isTransDoneIT(void){
			return DMA_RX.getITFlag(DMA_Defines::TCIFx);
		}

		finline void clearTransDoneIT(void) {
			DMA_RX.clearITFlag(DMA_Defines::TCIFx);
		}


	private:
		bool HWCS;


	protected:

		SPI_TypeDef * SPIx;
		Dma DMA_RX;
		Dma DMA_TX;
		GpioFast _NSSpin;

};



#endif /* SPI_SPI_HPP_ */
