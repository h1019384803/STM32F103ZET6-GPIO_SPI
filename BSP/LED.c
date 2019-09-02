
#include "LED.h"


//*****************************************************************************
//
//
//*****************************************************************************
void LED_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
     
    //HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);       

    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);      
    
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);  

}


//*****************************************************************************
//
//
//*****************************************************************************
void LED_TEST(void)
{
    while(1)
    {
        LED1_ON;
        LED2_OFF;
        LED3_OFF;
        LED4_OFF;
        HAL_Delay(500);
        LED1_OFF;
        LED2_ON;
        LED3_OFF;
        LED4_OFF;
        HAL_Delay(500);        
        LED1_OFF;
        LED2_OFF;
        LED3_ON;
        LED4_OFF;
        HAL_Delay(500);
        LED1_OFF;
        LED2_OFF;
        LED3_OFF;
        LED4_ON;
        HAL_Delay(500);        
    
    }
}








