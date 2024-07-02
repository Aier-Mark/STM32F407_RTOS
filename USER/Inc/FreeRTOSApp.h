#ifndef __FREERTOS_APP_H
#define __FREERTOS_APP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"

/* Public functions -------------------------------------------------------------------*/

void freeRTOS_Init(void);

#endif // !__FREERTOS_H