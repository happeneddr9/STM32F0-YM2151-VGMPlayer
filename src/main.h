#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

#include "Arduino/ArduinoLib.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
//#include "stm32f0_discovery.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* XXX: if you want to overclock your STM32, you can change the PLLMULL's value in "system_stm32f0xx.c" */
#define SystemCoreClockSet 48000000
const uint32_t F_CPU = SystemCoreClockSet;
//#define SysTickInit(frequency) SysTick_Config(F_CPU / frequency)

/* if Enable  comment, using 64bit SystemTicker
 * if Disable comment, using 32bit SystemTicker
 */
//#define Use64bitSystemTicker
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus

#endif


#endif /* __MAIN_H */



/****END OF FILE****/
