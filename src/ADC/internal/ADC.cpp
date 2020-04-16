/*
 * ADC.cpp
 *
 *  Created on: 2020年4月1日
 *      Author: user
 */

#include "ADC.h"

ADC_Class::CFGR1_t ADC_Class::tmpreg = {0};
miniThread ADC_DMA_Class::timeout = ADC_DMA_Class::TIMEOUT;
