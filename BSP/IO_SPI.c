#include "IO_SPI.h"


/*******************************************************************************
��������        ��SPI_Init
��������        ��void
��������ֵ      ��void
����˵��        ��SPI��ʼ��������
                  CS��   PA4�������
                  CLK��  PA5������� 
                  MOSI�� PA7�������
                  MISO�� PA6��������                  
*******************************************************************************/
void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();    
    
    //PA4��PA5��PA7�������
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //PA6��������
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);    
    
    SPI_CS(1);
    SPI_MOSI(0);
  
#if (_SPI_CPOL == 0)    

    SPI_CLK(0);//����ʱΪ�͵�ƽ  
    
#else

    SPI_CLK(1);
    
#endif


	HAL_Delay(100);//�ϵ���ʱһ��ʱ��

}

/*******************************************************************************
��������        ��SPI_DELAY
��������        ����ʱ����
��������ֵ      ��void
����˵��        ��NOP��ʱ                
*******************************************************************************/
void SPI_DELAY(uint16_t del)
{
	uint8_t i;
	for(i=0;i<del;i++)
	{
        __NOP();    
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}

/*******************************************************************************
��������        ��SPI_WRITE_READ_BYTE
��������        ��SPI���͵�����
��������ֵ      ��SPI��ȡ������
����˵��        ��ͨ��_SPI_CPOL��_SPI_CPHA��������SPI 4�ֲ�ͬ��ͨ��ģʽ         
*******************************************************************************/
#if (_SPI_CPOL == 0)&&(_SPI_CPHA == 0)

uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(0);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }     
        TX_DAT <<= 1;    
        
        SPI_DELAY(_SPI_DELAY_);
                
        SPI_CLK(1);
        
        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;    
        }   
		
        SPI_DELAY(_SPI_DELAY_);			
    }
    
    SPI_CLK(0);//����ʱSCKΪ�͵�ƽ
    
    return RX_DAT;
}

#elif (_SPI_CPOL == 0)&&(_SPI_CPHA == 1)
    
uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(1);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }    

        TX_DAT <<= 1;    
        
        SPI_DELAY(_SPI_DELAY_);
        
        SPI_CLK(0);

        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;    
        }
		
        SPI_DELAY(_SPI_DELAY_);		
		
    }
    
    return RX_DAT;
}

#elif (_SPI_CPOL == 1)&&(_SPI_CPHA == 0)

uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(1);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }    

        TX_DAT <<= 1;    
    
        SPI_DELAY(_SPI_DELAY_);
    
        SPI_CLK(0);
       
        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;  			
        }   
		
        SPI_DELAY(_SPI_DELAY_); 		
    }

    SPI_SCK(1);//����ʱSCKΪ�ߵ�ƽ
    
    return RX_DAT;
}

#elif (_SPI_CPOL == 1)&&(_SPI_CPHA == 1)

uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(0);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }    

        TX_DAT <<= 1;    
   
        SPI_DELAY(_SPI_DELAY_);
    
        SPI_CLK(1);

        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;    
        }
		
		SPI_DELAY(_SPI_DELAY_);			
			
    }
   
    return RX_DAT;
}

#endif











