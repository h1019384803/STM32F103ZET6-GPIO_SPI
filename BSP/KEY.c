
#include "KEY.h"
#include "LED.h"


uint8_t KEY_FLAG;

//*****************************************************************************
//
//
//*****************************************************************************
void KEY_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);    
    
}


uint8_t READ_KEY_ID(void)
{
    if(KEY1_READ == 0)
    {
        return 1;
    }

    if(KEY2_READ == 0)
    {
        return 2;
    }    
    
    return 0;
}

//*****************************************************************************
//
//
//*****************************************************************************
void KEY_SCAN(void)
{
    uint8_t KEY_ID;
    
    KEY_ID = READ_KEY_ID();
    
    if(KEY_ID != 0)
    {
        if(KEY_FLAG == 0)
        {
            HAL_Delay(20);
            KEY_ID = READ_KEY_ID();
            
            if(KEY_ID != 0)
            {
                KEY_FLAG = 1;
                
                if(KEY_ID == 1)
                {
                    
                     
                  
                    
                   
                }
                else if(KEY_ID == 2)
                {
                    
                    
                }
            }
        }
    }
    else
    {
        KEY_FLAG = 0;
    }
}



