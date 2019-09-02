
#ifndef __LED_H
#define __LED_H


#include "stm32f1xx_hal.h"



#define     LED1_ON         HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)
#define     LED1_OFF        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)
#define     LED1_TOGGLE     HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6)

#define     LED2_ON         HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET)
#define     LED2_OFF        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET)
#define     LED2_TOGGLE     HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_7)

#define     LED3_ON         HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET)
#define     LED3_OFF        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET)
#define     LED3_TOGGLE     HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_8)

#define     LED4_ON         HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET)
#define     LED4_OFF        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET)
#define     LED4_TOGGLE     HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9)





void LED_Init(void);
void LED_TEST(void);


#endif







