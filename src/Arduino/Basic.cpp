/*
 * Basic.cpp
 *
 *  Created on: 2019年10月1日
 *      Author: happe
 */

#include "Basic.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <cmath>

#include <chrono>   // It Work!!!

volatile SystemTicker_t System_Clock = 0;

void randomSeed(unsigned long seed) {
	if (seed != 0) {
		srand(seed);
	}
}

int random(void){
	return std::rand();
}

int random(int howbig) {
	if (howbig == 0) {
		return 0;
	}
	return std::rand() % howbig;
}

int random(int howsmall, int howbig) {
	if (howsmall >= howbig) {
		return howsmall;
	}
	int diff = howbig - howsmall;
	return random(diff) + howsmall;
}

extern "C" char* sbrk(int incr);
int FreeStack(void) {
	char top = 't';
	return &top - reinterpret_cast<char*>(sbrk(0));
}

void pinMode(GpioPin pinx, pinMode_Status mode_in) {
	uint8_t modex = static_cast<uint8_t>(mode_in);
	GpioMode mode = static_cast<GpioMode>(modex & 0x03);
	GpioOutType GpioOT = static_cast<GpioOutType>((modex & 0x0C) >> 2);
	GpioSpeed GpioSP = GpioSpeed::s50MHz;
	GpioPuPd GpioPP = static_cast<GpioPuPd>((modex & 0x30) >> 4);

	Gpio tmp(pinx);
	tmp.init(mode, GpioOT, GpioSP, GpioPP);
}

/*void ADC_setup(ADC_TypeDef * ADCx) {

 //ADC_WaitModeCmd(ADC1, ENABLE);
 //ADCx->CFGR1 |= ADC_CFGR1_WAIT;
 //ADC_AutoPowerOffCmd(ADC1, ENABLE);
 //ADCx->CFGR1 |= ADC_CFGR1_AUTOFF;
 //(#) Activate the ADC peripheral using ADC_Cmd() function.
 //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));   //wait until it stable
 //ADC_DMARequestModeConfig(ADCx, ADC_DMAMode_Circular);
 }*/

#if !defined(STM32F051)
void analogPin(GpioPin pinx) {
	ADC_TypeDef * ADCx = isAnalogPin(pinx.getID());
	if (ADCx == nullptr)
		return;
	//uint32_t ADC_ch = getAnalogChannel(pinx.getID());

	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	Gpio tmp(pinx);
	tmp.init(GpioMode::Analog);

	ADC_DeInit(ADCx);
	ADC_InitTypeDef ADCsetup;
	ADCsetup.ADC_ContinuousConvMode = DISABLE;
	ADCsetup.ADC_DataAlign = ADC_DataAlign_Right;
	ADCsetup.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADCsetup.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
	ADCsetup.ADC_Resolution = ADC_Resolution_12b;
	ADCsetup.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADCx, &ADCsetup);
	ADC_GetCalibrationFactor(ADCx);
	asm volatile("nop; nop;");

	//ADC_DMACmd(ADCx, ENABLE);  //to use DMA for Fast Conv
	uint32_t tmpreg = 0;
	tmpreg &= ~ADC_SMPR1_SMPR;
	tmpreg |= (uint32_t) ADC_SampleTime_28_5Cycles;
	ADCx->SMPR = tmpreg;

	ADC_Cmd(ADCx, ENABLE);

	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_ADRDY)) {

	}

}
#endif

void digitalWrite(GpioPin pinx, GPIO_Status HL) {
	Gpio tmp(pinx);
	tmp = HL;
}

void digitalWrite(GpioPin pinx, bool HL) {
	digitalWrite(pinx, static_cast<GPIO_Status>(HL));
}

bool digitalRead(GpioPin pinx) {
	Gpio tmp(pinx);
	return tmp.read();
}

#if !defined(STM32F051)
uint16_t analogRead(GpioPin pinx) {
	ADC_TypeDef * ADCx = isAnalogPin(pinx.getID());
	if (ADCx == nullptr)
		return 0;

	uint32_t ADC_ch = getAnalogChannel(pinx.getID());
	//ADC_ChannelConfig(ADCx, ADC_ch, ADC_SampleTime_28_5Cycles);

	ADCx->CHSELR = ADC_ch;

	ADC_Cmd(ADCx, ENABLE);

	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_ADRDY)) {

	}

	ADC_StartOfConversion(ADCx);
	// Wait until conversion completion
	while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET) {

	}
	ADC_StopOfConversion(ADCx);

	return (uint16_t) ADCx->DR;

}
#endif

void yield(void){
	// if used Watchdogs, put yield code here.
}


void delay(uint32_t delayTime) {
	uint32_t delaymsNow = System_Clock;
	while (System_Clock - delaymsNow < delayTime) {
		yield();
	}
}

// TODO: looking for a better way.
void delayMicroseconds(uint32_t delayTime) {
	uint32_t delaymsNow = micros();
	while (micros() - delaymsNow < delayTime) {
		yield();
	}
}


void *operator new(size_t size) {
	return malloc(size);
}

void *operator new[](size_t size) {
	return malloc(size);
}

void operator delete(void * ptr) {
	free(ptr);
}

void operator delete[](void * ptr) {
	free(ptr);
}


//for support std::chrono
namespace std {
	namespace chrono {
		system_clock::time_point system_clock::now() noexcept
		{
			// We assume System_Clock has a 1 msec resolution and thus will wrap around every 49.7 days... which should be enough for now.
			return time_point(milliseconds(System_Clock));
		}

		steady_clock::time_point steady_clock::now() noexcept
		{
			// We assume HAL_GetTick() has a 1 msec resolution and thus will wrap around every 49.7 days... which should be enough for now.
			return time_point(milliseconds(System_Clock));
		}
	}
}

extern "C" {
	void SysTick_Handler(void) {
		System_Clock++;
	}
}
