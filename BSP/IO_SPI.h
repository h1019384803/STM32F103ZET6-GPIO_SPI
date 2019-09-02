#ifndef __IO_SPI_H
#define __IO_SPI_H
#include "stm32f1xx_hal.h"



#define _SPI_CPOL   					0		//ѡ��CLK�ڿ���ʱ�ĵ�ƽ״̬��0������ʱΪ�͵�ƽ��1������ʱΪ�ߵ�ƽ
#define _SPI_CPHA   					0		//ѡ��SPI�����ڵڼ������زɼ����ݣ�0���ڵ�һ�����زɼ����ݣ�1���ڵڶ������زɼ�����

#define SPI_CS(x)          				(x ?  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET) )       
#define SPI_CLK(x)         				(x ?  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET) )       
#define SPI_MOSI(x)        				(x ?  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET) )       
#define SPI_MISO           				HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)      


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//��������
extern void SPI_Init(void);
extern void SPI_DELAY(uint16_t del);
extern uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT);


#endif


