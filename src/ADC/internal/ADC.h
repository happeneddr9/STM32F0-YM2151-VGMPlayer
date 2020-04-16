/*
 * ADC.h
 *
 *  Created on: 2020年3月27日
 *      Author: user
 */

#ifndef ADC_INTERNAL_ADC_H_
#define ADC_INTERNAL_ADC_H_

#include "GPIO/GPIO.h"
#include "SYSTEM/DMA.hpp"
#include "ADC_Defines.h"
#include "Arduino/miniThread.h"
#include <algorithm>


class ADC_Class {
	public:

		typedef ADC_Defines::CFGR1 CFGR1_t;

		ADC_Class(ADC_TypeDef * ADC_) {
			RCC->APB2ENR |= RCC_APB2Periph_ADC1;
			CLKConfig(RCC_ADCCLK_PCLK_Div2);		// default use the Maximum speed
			ADCx = ADC_;
		}

		void startConversion() const {
			ADCx->CR |= static_cast<uint32_t>(ADC_CR_ADSTART);
		}

		void cmd(FunctionalState NewState) const {
			if (NewState != DISABLE) {
				/* Set the ADEN bit to Enable the ADC peripheral */
				ADCx->CR |= static_cast<uint32_t>(ADC_CR_ADEN);
			} else {
				/* Set the ADDIS to Disable the ADC peripheral */
				ADCx->CR |= static_cast<uint32_t>(ADC_CR_ADDIS);
			}
		}

		void DMA_cmd(FunctionalState NewState) const {
			if (NewState != DISABLE) {
				/* Enable the selected ADC DMA request */
				ADCx->CFGR1 |= static_cast<uint32_t>(ADC_CFGR1_DMAEN);
			} else {
				/* Disable the selected ADC DMA request */
				ADCx->CFGR1 &= static_cast<uint32_t>(~ADC_CFGR1_DMAEN);
			}
		}

		void DMARequestModeConfig(uint32_t ADC_DMARequestMode) const {
			ADCx->CFGR1 &= static_cast<uint32_t>(~ADC_CFGR1_DMACFG);
			ADCx->CFGR1 |= static_cast<uint32_t>(ADC_DMARequestMode);
		}

		ITStatus getITFlag(uint32_t ADC_FLAG) const {
			FlagStatus bitstatus = RESET;
			uint32_t tmpreg = 0;

			if (static_cast<uint32_t>(ADC_FLAG & 0x01000000)) {
				tmpreg = ADCx->CR & 0xFEFFFFFF;
			} else {
				tmpreg = ADCx->ISR;
			}

			/* Check the status of the specified ADC flag */
			if ((tmpreg & ADC_FLAG) != static_cast<uint32_t>(RESET)) {
				/* ADC_FLAG is set */
				bitstatus = SET;
			} else {
				/* ADC_FLAG is reset */
				bitstatus = RESET;
			}
			/* Return the ADC_FLAG status */
			return bitstatus;
		}

		uint32_t getCalibrationFactor() const {
			uint32_t tmpreg = 0, calibrationcounter = 0, calibrationstatus = 0;

			/* Set the ADC calibartion */
			ADCx->CR |= static_cast<uint32_t>(ADC_CR_ADCAL);

			/* Wait until no ADC calibration is completed */
			do {
				calibrationstatus = ADCx->CR & ADC_CR_ADCAL;
				calibrationcounter++;
			} while ((calibrationcounter != CALIBRATION_TIMEOUT)
					&& (calibrationstatus != 0x00));

			if (static_cast<uint32_t>(ADCx->CR & ADC_CR_ADCAL) == RESET) {
				/*Get the calibration factor from the ADC data register */
				tmpreg = ADCx->DR;
			} else {
				/* Error factor */
				tmpreg = 0x00000000;
			}
			return tmpreg;
		}

		void initStruct(const ADC_InitTypeDef& ADC_InitStruct) const {
			initStruct(&ADC_InitStruct);
		}

		void initStruct(const ADC_InitTypeDef * ADC_InitStruct) const {
			uint32_t tmpreg;
			/* Get the ADCx CFGR value */
			tmpreg = ADCx->CFGR1;

			/* Clear SCANDIR, RES[1:0], ALIGN, EXTSEL[2:0], EXTEN[1:0] and CONT bits */
			tmpreg &= CFGR1_CLEAR_MASK;

			/*---------------------------- ADCx CFGR Configuration ---------------------*/

			/* Set RES[1:0] bits according to ADC_Resolution value */
			/* Set CONT bit according to ADC_ContinuousConvMode value */
			/* Set EXTEN[1:0] bits according to ADC_ExternalTrigConvEdge value */
			/* Set EXTSEL[2:0] bits according to ADC_ExternalTrigConv value */
			/* Set ALIGN bit according to ADC_DataAlign value */
			/* Set SCANDIR bit according to ADC_ScanDirection value */

			tmpreg |= static_cast<uint32_t>(ADC_InitStruct->ADC_Resolution
					| (static_cast<uint32_t>(ADC_InitStruct->ADC_ContinuousConvMode) << 13)
					| ADC_InitStruct->ADC_ExternalTrigConvEdge
					| ADC_InitStruct->ADC_ExternalTrigConv
					| ADC_InitStruct->ADC_DataAlign
					| ADC_InitStruct->ADC_ScanDirection);

			/* Write to ADCx CFGR */
			ADCx->CFGR1 = tmpreg;
		}

		void CLKConfig(uint32_t RCC_ADCCLK) const {
			/* Clear ADCPRE bit */
			RCC->CFGR &= ~RCC_CFGR_ADCPRE;
			/* Set ADCPRE bits according to RCC_PCLK value */
			RCC->CFGR |= RCC_ADCCLK & 0xFFFF;

			/* Clear ADCSW bit */
			RCC->CFGR3 &= ~RCC_CFGR3_ADCSW;
			/* Set ADCSW bits according to RCC_ADCCLK value */
			RCC->CFGR3 |= RCC_ADCCLK >> 16;
		}

		void continuousConvMode(FunctionalState mode) const {
			tmpreg.reg = ADCx->CFGR1;
			tmpreg.CONT = static_cast<uint32_t>(mode);
			ADCx->CFGR1 = tmpreg.reg;
		}

		void dataAlign(uint32_t align) const {
			tmpreg.reg = ADCx->CFGR1;
			tmpreg.ALIGN = 0;
			tmpreg.reg |= align;
			ADCx->CFGR1 = tmpreg.reg;
		}

		void externalTrigConv(uint32_t trig) const {
			tmpreg.reg = ADCx->CFGR1;
			tmpreg.EXTSEL = 0;
			tmpreg.reg |= trig;
			ADCx->CFGR1 = tmpreg.reg;
		}

		void externalTrigConvEdge(uint32_t trig) const {
			tmpreg.reg = ADCx->CFGR1;
			tmpreg.EXTEN = 0;
			tmpreg.reg |= trig;
			ADCx->CFGR1 = tmpreg.reg;
		}

		void resolution(uint32_t res) const {

			tmpreg.reg = ADCx->CFGR1;
			tmpreg.RES = 0;
			tmpreg.reg |= res;
			ADCx->CFGR1 = tmpreg.reg;
		}

		void scanDirection(uint32_t dir) const {
			tmpreg.reg = ADCx->CFGR1;
			tmpreg.SCANDIR = 0;
			tmpreg.reg |= dir;
			ADCx->CFGR1 = tmpreg.reg;
		}

		inline void sampleTime(uint32_t ADC_SampleTime) const {
			tmpreg.reg = 0;
			ADCx->SMPR = tmpreg.reg & ADC_SMPR1_SMPR;
		}

		inline void channelConfig(uint32_t ADC_Channel) const {
			/* Configure the ADC Channel */
			ADCx->CHSELR |= static_cast<uint32_t>(ADC_Channel);
		}

		inline void channelConfig(uint32_t ADC_Channel, uint32_t ADC_SampleTime) const {
			/* Configure the ADC Channel */
			ADCx->CHSELR |= static_cast<uint32_t>(ADC_Channel);
			sampleTime(ADC_SampleTime);
		}

		uint32_t channelUsed() const {
			uint32_t tmpreg = ADCx->CHSELR;
			return _Popcount_(tmpreg);
		}


	private:

	protected:
		ADC_TypeDef * ADCx;

		static CFGR1_t tmpreg;
		static constexpr uint32_t CFGR1_CLEAR_MASK = 0xFFFFD203;
		static constexpr uint32_t CALIBRATION_TIMEOUT = 0x0000F000;

};

class ADC_DMA_Class : public ADC_Class {
	public:
		/* enums */
		enum Resolution : uint32_t {
			Data_12b = ADC_Resolution_12b,
			Data_10b = ADC_Resolution_10b,
			Data_8b = ADC_Resolution_8b,
			Data_6b = ADC_Resolution_6b,
		};

		enum SampleTime : uint32_t {
			SampleTime_1N5 = 0,
			SampleTime_7N5,
			SampleTime_13N5,
			SampleTime_28N5,
			SampleTime_41N5,
			SampleTime_55N5,
			SampleTime_71N5,
			SampleTime_239N5,
		};

	public:
		ADC_DMA_Class(ADC_TypeDef * ADC_,
				Dma DmaADC = Dma(DMA1_Channel1, DMA1_Channel1_IRQn),	// default is DMA1 Channel1
				const GpioPin * package = ADC1_AnalogPin)
			: ADC_Class(ADC_),  DMA_ADCx(DmaADC), ADC_Pin_ptr(package) {

		}

		void begin(const ADC_InitTypeDef& ADC_InitStruct) const {
			begin(&ADC_InitStruct);
		}

		void begin(const ADC_InitTypeDef * ADC_InitStruct) const {
			initStruct(ADC_InitStruct);

			getCalibrationFactor();
			cmd(ENABLE);
			DMA_cmd(ENABLE);
			DMARequestModeConfig(ADC_DMAMode_Circular);

			while(!getITFlag(ADC_FLAG_ADRDY));

			//Configure DMA for transfer
			//NOTE: Memory and peripheral sizes will need to be changed for
			//conversions larger than 8bits.
			DMA_InitTypeDef D;
			D.DMA_BufferSize = 0;
			D.DMA_MemoryBaseAddr = 0;
			D.DMA_MemoryDataSize = 0;
			D.DMA_PeripheralBaseAddr = 0;
			D.DMA_PeripheralDataSize = 0;


			D.DMA_DIR = DMA_DIR_PeripheralSRC;
			D.DMA_M2M = DMA_M2M_Disable;
			D.DMA_MemoryInc = DMA_MemoryInc_Enable;
			D.DMA_Mode = DMA_Mode_Circular;
			D.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			D.DMA_Priority = DMA_Priority_VeryHigh;
			DMA_ADCx.disable();
			DMA_ADCx.init(&D);
			setResolution(static_cast<Resolution>(ADC_InitStruct->ADC_Resolution));
			DMA_ADCx.enable();
			/*DMA_ADCx.DataDirection(DMA_Defines::P2M);
			DMA_ADCx.Memory2Memory(false);

			DMA_ADCx.PeripheralIncrement(false);
			DMA_ADCx.MemoryIncrement(true);
			DMA_ADCx.CircularMode(false);
			DMA_ADCx.PriorityLevel(DMA_Defines::VeryHigh);*/

		}

		inline bool isIRQ_Handler(void) const {
			if (DMA_ADCx.getITFlag(DMA_Defines::TCIFx) != RESET) {
				DMA_ADCx.clearITFlag(DMA_Defines::TCIFx);
				return true;
			}
			return false;
		}

		void enableIT(FunctionalState en = ENABLE) const {
			DMA_ADCx.disable();
			DMA_ADCx.CircularMode(en == ENABLE ? false : true);
			DMA_ADCx.ITConfig(DMA_Defines::TCI, en);
			DMA_ADCx.clearITFlag(DMA_Defines::TCIFx);
			DMA_ADCx.enable();
		}

		/* Need IQR handler */
		void getAnalogsIT(const volatile void * MMdir, size_t n) const {
			setAddr(MMdir, n);

			startConversion();
		}

		void setAddr(const volatile void * MMdir, size_t n) const {
			DMA_ADCx.disable();
			DMA_ADCx.Ready(&(ADCx->DR), MMdir, n);
			DMA_ADCx.enable();
		}

		/* wait it converted all */
		bool getAnalogs() const {
			bool isTimeout = false;
			/*DMA_ADCx.disable();
			DMA_ADCx.Ready(&(ADCx->DR), MMdir, n);
			timeout.reset();
			DMA_ADCx.enable();*/
			timeout.reset();
			startConversion();
			while (!DMA_ADCx.getITFlag(DMA_Defines::TCIFx)) {
				if (timeout) {
					isTimeout = true;
					break;
				}
			}
			DMA_ADCx.clearITFlag(DMA_Defines::TCIFx);
			//DMA_ADCx.disable();
			return !isTimeout;
		}

		inline void setSampleTime(SampleTime sam_t) const {
			/* Configure the ADC Channel */
			this->sampleTime(static_cast<uint32_t>(sam_t));
		}

		uint32_t addPin(const GpioPin newPin) const {
			tmpreg.reg = isAnalogPin(newPin);
			ADCx->CHSELR |= tmpreg.reg;
			return tmpreg.reg;
		}

		uint32_t removePin(const GpioPin oldPin) const {
			tmpreg.reg = isAnalogPin(oldPin);
			ADCx->CHSELR &= ~(tmpreg.reg);
			return tmpreg.reg;
		}

		uint32_t addPin(const GpioPin * Pin_pag, size_t n) const {
			tmpreg.reg = 0;
			for(size_t i = 0; i < n; i++)
				tmpreg.reg |= isAnalogPin(Pin_pag[i]);
			ADCx->CHSELR |= tmpreg.reg;
			return tmpreg.reg;
		}

		uint32_t removePin(const GpioPin * Pin_pag, size_t n) const {
			tmpreg.reg = 0;
			for (size_t i = 0; i < n; i++)
				tmpreg.reg |= isAnalogPin(Pin_pag[i]);
			ADCx->CHSELR &= ~(tmpreg.reg);
			return tmpreg.reg;
		}

		uint32_t isAnalogPin(const GpioPin xPin) const {
			for (size_t i = 0; i < ADC_Pin_len; ++i) {
				if (ADC_Pin_ptr[i] == xPin) {
					return (1U << i);
				}
			}
			return 0;
		}

		void setResolution(Resolution newRES) const {
			tmpreg.reg = ADCx->CFGR1;
			tmpreg.reg &= ~static_cast<uint32_t>(Data_6b);
			tmpreg.reg |= static_cast<uint32_t>(newRES);
			ADCx->CFGR1 = tmpreg.reg;

			DMA_Defines::Memory_BW tmp = (newRES >= Data_8b) ?
					DMA_Defines::W8_bits : DMA_Defines::W16_bits;

			DMA_ADCx.MemorySize(tmp);
			DMA_ADCx.PeripheralSize(tmp);
		}


	private:

	protected:
		Dma DMA_ADCx;
		const GpioPin * ADC_Pin_ptr;

	protected:
		static constexpr uint32_t ADC_Pin_len = 16;
		static constexpr uint32_t TIMEOUT = 300;
		static miniThread timeout;
};


#endif /* ADC_INTERNAL_ADC_H_ */
