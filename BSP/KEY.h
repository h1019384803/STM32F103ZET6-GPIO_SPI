
#ifndef __KEY_H
#define __KEY_H


#include "stm32f1xx_hal.h"



#define     KEY1_READ       HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_8)
#define     KEY2_READ       HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)


void KEY_Init(void);
void KEY_SCAN(void);

#endif







