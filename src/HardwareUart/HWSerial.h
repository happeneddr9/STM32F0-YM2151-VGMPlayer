/*
 * HWSerial.h
 *
 *  Created on: 2019年8月31日
 *      Author: happe
 *
 *  Build Log:
 *  	2019/8/31
 *  		可發送字元 字串
 *  		可發送 整數 浮點數
 *  		接收程式建置完成
 *
 *  		浮點列印新增科學記號版本
 *
 *  	2019/9/1
 *  		新增 print 64位元 整數
 *  		修復float double列印科學記號時的小bug
 *
 *  	2019/09/08
 *  		新增find(const char *)
 *  		可以搜尋緩衝區內是否有對應的字串
 *
 *  	2019/09/13
 *  		新增顏色轉換專用程式
 *  		修改了UART的呼叫方式
 *
 *  	2019/11/08
 *  		新增print(std::string)
 *  		HWSerial 加入了 << 輸出流的重載，功能與print相同，適用於格式化字串
 *
 *   		HWSerial added operator << output stream, features same as print function,
 *   	    suitable for formatting string
 *
 *   	2019/11/10
 *   		修復了Print函式列印std::string時的小bug
 *   		暫時移除find()
 *
 *   		Fixed bug of Print function in printing std::string
 *   		Remove find() temporarily
 *
 *   	2019/11/12
 *   		<< 輸出流的重載移置Print.h
 *
 *   		operator << output stream moved to Print.h
 *
 *   	2019/11/15
 *   	    新增Gpio Port 和 Gpio Pin映射
 *
 *   	    Added Gpio Port and Pin Mapping Struct
 *
 *   	2019/12/12
 *   		Gpio映射改用GpioPin結構宣告
 *
 *   		used GpioPin Struct replace Gpio Port and Pin (Increase Readability)
 *		2019/12/17
 *			新增parseInt()，將緩衝區內的字串轉換為int
 *
 *			add parseInt(), convert in buffer string to integer
 *
 *   	# TODO: to add Gpio Mapping with AF(Alternate Function)
 *		# TODO: to add USART RCC setup function
 *		# TODO: to add TX interrupt, even DMA
 *
 */

#ifndef HWSERIAL_H_
#define HWSERIAL_H_

#include "main.h"
#include "Arduino/Basic.h"
#include "Print/Print.h"
#include "Print/WString.h"
#include "FIFObuffer/FIFObuffer.h"
#include "FIFObuffer/RingBuffer.h"
#include "GPIO/GPIO.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>


#include "Print/stream/istream.h"

#include "HWSerial_Config.h"


/*
 *  Now Define in Print.h
 #define DEC 10
 #define HEX 16
 #define OCT 8
 #ifdef BIN
 #undef BIN
 #endif
 #define BIN 2*/

#define buff_rx_size 64

#if buff_rx_size > 255
typedef uint16_t header_size;   //if data buffer length over then 255, need use larger variable
#else
typedef uint8_t header_size;    //save memory in tiny place, it can save 2 bytes
#endif

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
//Still Empty
#endif

class HWSerial : public cpp_io::istream {
	public:


		HWSerial(USART_TypeDef * UARTx, IRQn vect, uint8_t IRQ_Priority = 0);
		~HWSerial(void);

		uint8_t begin(uint32_t baud, const HWSerial_PinMap::USART_PinMap_TXRX& PinMap);
		uint8_t begin(uint32_t baud, const HWSerial_PinMap::USART_PinMap_ALL& PinMap);

		inline void end(void);

		size_t available(void);
		uint8_t peek(void);
		uint8_t read(void);

		inline void LoadDatain(uint8_t datain) {    //載入資料 在接受中斷時使用
			buff_rx.push_back(datain);
		}

		inline void LoadStopbit(void) {
			RX_Done = buff_rx.available();   // not Empty
		}

		inline void RX_Done_Clear(void) {
			RX_Done = false;
		}

		inline void IRQ_Handler(void) {
			if (getIT(USART_IT_RXNE) != RESET) {
				LoadDatain(static_cast<uint16_t>(UARTx->RDR & static_cast<uint16_t>(0x01FF)));
				clearIT(USART_IT_RXNE);
			}

			if (getIT(USART_IT_IDLE) != RESET) {
				LoadStopbit();
				clearIT(USART_IT_IDLE);
			}
		}

		inline bool getIT(uint32_t IT) {
			uint32_t bitpos = 0, itmask = 0, usartreg = 0;
			/* Get the USART register index */
			usartreg = (static_cast<uint16_t>(IT) >> 0x08);
			/* Get the interrupt position */
			itmask = IT & static_cast<uint32_t>(0x000000FF);
			itmask = static_cast<uint32_t>(0x01) << itmask;

			if (usartreg == 0x01) {				/* The IT  is in CR1 register */
				itmask &= UARTx->CR1;
			} else if (usartreg == 0x02) {		/* The IT  is in CR2 register */
				itmask &= UARTx->CR2;
			} else {							/* The IT  is in CR3 register */
				itmask &= UARTx->CR3;
			}

			bitpos = IT >> 0x10;
			bitpos = static_cast<uint32_t>(0x01) << bitpos;
			bitpos &= UARTx->ISR;

			return (itmask != static_cast<uint16_t>(RESET)) &&
				   (bitpos != static_cast<uint16_t>(RESET));
		}

		inline void clearIT(uint32_t IT) {
			uint32_t bitpos = 0, itmask = 0;

			bitpos = IT >> 0x10;
			itmask = (static_cast<uint32_t>(0x01) << static_cast<uint32_t>(bitpos));
			UARTx->ICR = static_cast<uint32_t>(itmask);
		}

		void clearBuffer(void);

		/*inline uint32_t getBaudRate(void) {
		 return this->BaudRate;
		 }*/

		bool find(const char * str);  //TODO: still no idea how to do
		int32_t parseInt(void);

		happe::String readString(void);

		virtual size_t write(uint8_t);
		//override Print.write(uint8_t)
		//using Print::write; // pull in write(str) and write(buf, size) from Print
		//uint32_t all_data;

	private:
		inline void cmd(FunctionalState state) {
			USART_Cmd(UARTx, state);
			USART_ITConfig(UARTx, USART_IT_RXNE, state);   // 收到一 Byte 資料時中斷
			USART_ITConfig(UARTx, USART_IT_IDLE, state);   // 空閒時中斷，用於接收完畢
		}


		bool ready2read(void) override;
		int getch(void) override;
		int peekch(void) override;
	protected:
		USART_TypeDef * UARTx;
		bool RX_Done;
		ringbuffer_t<uint8_t, buff_rx_size, uint8_t> buff_rx;
		/*header_size buff_rx_head;
		 header_size buff_rx_tail;
		 uint8_t buff_rx[buff_rx_size];*/
};



#endif /* HWSERIAL_H_ */
