/**
 * @Name    : DMA.hpp
 * @Date    : 2019年12月2日 下午8:28:30
 * @Author  : happe
 * @brief   :
 */

#ifndef SYSTEM_DMA_HPP_
#define SYSTEM_DMA_HPP_

#include "Arduino/Basic.h"
#include "Nvic.hpp"

namespace DMA_Defines {

	enum Dma_ISR {
		GIFx 		= 0x01,
		TCIFx 		= 0x02,
		HTIFx 		= 0x04,
		TEIFx 		= 0x08,
	};

	/* -DMA CCR Register bits offset- */
	enum CCR_offset : uint8_t {
		EN 			= 0,
		TCIE 		= 1,
		HTIE 		= 2,
		TEIE 		= 3,
		DIR 		= 4,
		CIRC 		= 5,
		PINC 		= 6,
		MINC 		= 7,
		PSIZE 		= 8,
		MSIZE 		= 10,
		PL 			= 12,
		MEM2MEM 	= 14
	};

	/* -Defines- */
	/**
	 * @brief	: Memory Bytes Width
	 */
	enum Memory_BW : uint8_t {
		W8_bits 		= 0x00,
		W16_bits 		= 0x01,
		W32_bits 		= 0x02
	};

	/**
	 * @brief	: Priority Level
	 */
	enum Priority {
		Low 			= (0x00) << PL,
		Medium 			= (0x01) << PL,
		High 			= (0x02) << PL,
		VeryHigh 		= (0x03) << PL
	};

	/**
	 * @brief	: Priority Level
	 */
	enum Dir {
		P2M 			= (0 << DIR),
		M2P 			= (1 << DIR)
	};

	/**
	 * @brief	: Interrupt Enable Bit Index
	 */
	enum ITEN {
		TCI 			= (1 << TCIE),
		HTI 			= (1 << HTIE),
		TEI 			= (1 << TEIE)
	};

	constexpr uint8_t Dma_channel_n(DMA_Channel_TypeDef * DMAx) {
		if (DMAx == DMA1_Channel1)
			return 0x00;
		else if (DMAx == DMA1_Channel2)
			return 0x01;
		else if (DMAx == DMA1_Channel3)
			return 0x02;
		else if (DMAx == DMA1_Channel4)
			return 0x03;
		else if (DMAx == DMA1_Channel5)
			return 0x04;
#if defined(STM32F072) || defined(STM32F091)
		else if (DMAx == DMA1_Channel6)
		return 0x05;
		else if (DMAx == DMA1_Channel7)
		return 0x06;
#endif

#if defined(STM32F091)
		else if (DMAx == DMA2_Channel1)
		return 0x10;
		else if (DMAx == DMA2_Channel2)
		return 0x11;
		else if (DMAx == DMA2_Channel3)
		return 0x12;
		else if (DMAx == DMA2_Channel4)
		return 0x13;
		else if (DMAx == DMA2_Channel5)
		return 0x14;
#endif
		else
			return -1;
	}

	/* -Main Class- */

	class Dma {
		public:

			Dma(DMA_Channel_TypeDef * DMAx, IRQn vect)
					: mDMA(DMAx), DMAyCHx(Dma_channel_n(DMAx)), IToffset((DMAyCHx & 0x07) << 2){
				RCC->AHBENR |= RCC_AHBENR_DMA1EN;   // TODO: func

				Nvic::configureIrq(vect, ENABLE, 1);
				cmd(DISABLE);
			}

			void init(DMA_InitTypeDef& DMA_InitStruct) const {
				init(&DMA_InitStruct);
			}

			void init(DMA_InitTypeDef * DMA_InitStruct) const {
				uint32_t tmpreg = 0;
				/*--------------------------- DMAy Channelx CCR Configuration ----------------*/
				/* Get the DMAy_Channelx CCR value */
				tmpreg = mDMA->CCR;

				/* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
				tmpreg &= CCR_CLEAR_MASK;

				/* Configure DMAy Channelx: data transfer, data size, priority level and mode */
				/* Set DIR bit according to DMA_DIR value */
				/* Set CIRC bit according to DMA_Mode value */
				/* Set PINC bit according to DMA_PeripheralInc value */
				/* Set MINC bit according to DMA_MemoryInc value */
				/* Set PSIZE bits according to DMA_PeripheralDataSize value */
				/* Set MSIZE bits according to DMA_MemoryDataSize value */
				/* Set PL bits according to DMA_Priority value */
				/* Set the MEM2MEM bit according to DMA_M2M value */
				tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode
						| DMA_InitStruct->DMA_PeripheralInc
						| DMA_InitStruct->DMA_MemoryInc
						| DMA_InitStruct->DMA_PeripheralDataSize
						| DMA_InitStruct->DMA_MemoryDataSize
						| DMA_InitStruct->DMA_Priority
						| DMA_InitStruct->DMA_M2M;

				/* Write to DMAy Channelx CCR */
				mDMA->CCR = tmpreg;

				/*--------------------------- DMAy Channelx CNDTR Configuration --------------*/
				/* Write to DMAy Channelx CNDTR */
				mDMA->CNDTR = DMA_InitStruct->DMA_BufferSize;

				/*--------------------------- DMAy Channelx CPAR Configuration ---------------*/
				/* Write to DMAy Channelx CPAR */
				mDMA->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

				/*--------------------------- DMAy Channelx CMAR Configuration ---------------*/
				/* Write to DMAy Channelx CMAR */
				mDMA->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
			}

			inline DMA_Channel_TypeDef * Channel(void) const {
				return mDMA;
			}

			ITStatus getITFlag(Dma_ISR DMA_IT) const {
				ITStatus bitstatus = RESET;
				if ((DMA1->ISR & (DMA_IT << IToffset)) != static_cast<uint32_t>(RESET)) {
					bitstatus = SET;
				} else {
					bitstatus = RESET;
				}
				return bitstatus;
			}

			finline void clearITFlag(Dma_ISR DMA_IT) const {
				/* Clear the selected DMA interrupt pending bits */
				DMA1->IFCR = (DMA_IT << IToffset);
			}

			inline void Ready(const volatile void * PPAddr,
							   const volatile void * MemAddr,
							   uint32_t Data_n) const {
				mDMA->CNDTR = Data_n;								// Number of Data Register
				mDMA->CPAR = reinterpret_cast<uint32_t>(PPAddr);	// Peripheral Address Register
				mDMA->CMAR = reinterpret_cast<uint32_t>(MemAddr);	// Memory Address Register
			}

			finline void setCounter(uint32_t Data_n) const {
				mDMA->CNDTR = Data_n;
			}

			finline bool isBusy() const {
				return mDMA->CCR & DMA_CCR_EN;
			}

			finline void ITConfig(ITEN IT_type, FunctionalState NewState) const {
				if (NewState != DISABLE)
					mDMA->CCR |= static_cast<uint32_t>(IT_type);
				else
					mDMA->CCR &= ~(static_cast<uint32_t>(IT_type));
			}

			inline void ClearConfig(void) const {
				cmd(DISABLE);
				mDMA->CCR &= (static_cast<uint32_t>(0xFFFF800F));				// Mask All Config
			}

			inline void DataDirection(Dir direct) const {
				ForceSetBit(mDMA->CCR, direct, DIR);
				/*mDMA->CCR &= ~static_cast<uint32_t>(direct);
				mDMA->CCR |= static_cast<uint32_t>(direct);*/
			}

			inline void Memory2Memory(bool n) const {
				ForceSetBit(mDMA->CCR, n, MEM2MEM);
				/*mDMA->CCR &= ~(1 << MEM2MEM);
				mDMA->CCR |= (static_cast<uint8_t>(n) << MEM2MEM);*/
			}

			inline void PeripheralSize(Memory_BW byte_width) const {
				ForceSetBits(mDMA->CCR, byte_width, 2, PSIZE);
				/*mDMA->CCR &= ~__make_mask(2, PSIZE);
				mDMA->CCR |= (byte_width << PSIZE);*/
			}

			inline void MemorySize(Memory_BW byte_width) const {
				ForceSetBits(mDMA->CCR, byte_width, 2, MSIZE);
				/*mDMA->CCR &= ~__make_mask(2, MSIZE);
				mDMA->CCR |= (byte_width << MSIZE);*/
			}

			inline void PriorityLevel(Priority PP_level) const {
				mDMA->CCR &= ~(static_cast<uint32_t>(__make_mask(2, PL)));
				mDMA->CCR |= PP_level;
			}

			inline void MemoryIncrement(bool n) const {
				ForceSetBit(mDMA->CCR, n, MINC);
				/*mDMA->CCR &= ~(1 << MINC);
				mDMA->CCR |= (static_cast<uint8_t>(n) << MINC);*/
			}

			inline void PeripheralIncrement(bool n) const {
				ForceSetBit(mDMA->CCR, n, PINC);
				/*mDMA->CCR &= ~(1 << PINC);
				mDMA->CCR |= (static_cast<uint8_t>(n) << PINC);*/
			}

			inline void CircularMode(bool n) const {
				ForceSetBit(mDMA->CCR, n, CIRC);
				/*mDMA->CCR &= ~(1 << CIRC);
				mDMA->CCR |= (static_cast<uint8_t>(n) << CIRC);*/
			}

			inline void cmd(FunctionalState NewState) const {
				if (NewState != DISABLE) {
					enable();
				} else {
					disable();
				}
			}

			finline void enable(void) const {
				/* Enable the selected DMAy Channelx */
				mDMA->CCR |= DMA_CCR_EN;
			}

			finline void disable(void) const {
				/* Disable the selected DMAy Channelx */
				mDMA->CCR &= static_cast<uint32_t>(~DMA_CCR_EN);
			}

		private:

		protected:
			DMA_Channel_TypeDef * mDMA;
			uint8_t DMAyCHx;
			uint8_t IToffset;

			static constexpr uint32_t CCR_CLEAR_MASK = static_cast<uint32_t>(0xFFFF800F);
	};
}

using DMA_Defines::Dma;

#endif /* SYSTEM_DMA_HPP_ */
