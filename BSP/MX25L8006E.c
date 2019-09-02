#include "MX25L8006E.h"
#include "IO_SPI.h"
#include "USART.h"

uint8_t FLASH_IC_READY;//0��NOT REDAY 1��READY
uint8_t FLASH_DATA[_MX25_SECTOR_SIZE_];//4Kbyte


/*******************************************************************************
��������        ��MX25L8006E_READ_ID
��������        ��void
��������ֵ      �����������̱�ʶ���豸ID����ȡ�ɹ��᷵�أ�0xC22014
����˵��        ����������MX25_RD_MANUFACTER_DEVICE_ID_CMD����֮��FLASH IC
                  �᲻�ϵķ���ID����Ϣ�������ȡ�ɹ��ᷴ��0xC22014����ֵ��
                  ���Ƭѡ�źŲ��øߣ���FLASH IC�᲻��ѭ���ķ���ID����Ϣ��Ҳ��
                  ��˵��4����ȡ��������ȡ��������0xC2��
                  
*******************************************************************************/
uint32_t MX25L8006E_READ_ID(void)
{
    uint32_t DEVICE_ID = 0;
    
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_RD_MANUFACTER_DEVICE_ID_CMD);    
    
    DEVICE_ID = SPI_WRITE_READ_BYTE(DUMMY_BYTE);    
    DEVICE_ID <<= 8;
    DEVICE_ID |= SPI_WRITE_READ_BYTE(DUMMY_BYTE);
    DEVICE_ID <<= 8;            
    DEVICE_ID |= SPI_WRITE_READ_BYTE(DUMMY_BYTE);    //0x14 
 
    SPI_CS(1);
    
    return DEVICE_ID;
}

/*******************************************************************************
��������        ��MX25L8006E_READ_REMS
��������        �����ݴ���Ĳ���ֵ��������ȡ��Ϣ��˳�����dat=0x00�����ȡ
                  �ɹ����أ�0xC213�����dat=0x01�����ȡ�ɹ����أ�0x13C2
��������ֵ      ����ȡ�ɹ����أ�0xC213��0x13C2
����˵��        ����������MX25L8006E_READ_REMS����֮����Ҫ�ٷ���2���ֽڵĿղ�����
                  Ȼ���ٷ���1���ֽڵ����ݣ�FLASH IC���ݵ�3���ֽڵĲ��������ض�Ӧ��
                  ���ݡ�
                  
*******************************************************************************/
uint16_t MX25L8006E_READ_REMS(uint8_t dat)
{
    uint16_t DEVICE_ID = 0;
    
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_RD_ID_CMD);    
    
    SPI_WRITE_READ_BYTE(DUMMY_BYTE);    
    SPI_WRITE_READ_BYTE(DUMMY_BYTE);    
    SPI_WRITE_READ_BYTE(dat);    
    
    DEVICE_ID = SPI_WRITE_READ_BYTE(DUMMY_BYTE);  
    DEVICE_ID <<= 8;
    DEVICE_ID |= SPI_WRITE_READ_BYTE(DUMMY_BYTE); 
    
    SPI_CS(1);

    return DEVICE_ID;
}    

/*******************************************************************************
��������        ��MX25L8006E_WRITE_ENABLE
��������        ��void
��������ֵ      ��void
����˵��        ��ȡ��FLASH IDд����
*******************************************************************************/
void MX25L8006E_WRITE_ENABLE(void)
{  
    SPI_CS(0);
    SPI_WRITE_READ_BYTE(MX25_WR_ENABLE_CMD);        
    SPI_CS(1);
}   

/*******************************************************************************
��������        ��MX25L8006E_WRITE_DISABLE
��������        ��void
��������ֵ      ��void
����˵��        ��ʹ��FLASH IDд����
*******************************************************************************/
void MX25L8006E_WRITE_DISABLE(void)
{  
    SPI_CS(0);   
    SPI_WRITE_READ_BYTE(0x06);     
    SPI_CS(1);
}  

/*******************************************************************************
��������        ��MX25L8006E_READ
��������        ��*Buffer���洢��ȡ���ݵ��׵�ַ
                  Address��Ҫ��ȡ�����ݵ��׵�ַ
                  Len��Ҫ��ȡ�����ݳ���  
��������ֵ      ��void
����˵��        ����ȡFLASH IC��ָ����ַ�����ݳ��ȣ���ΪMX25L8006E��1Mbyte��
                   �洢IC����ַ����Ϊ0x00000000~0x000FFFFF�����Ե�ַ������3���ֽ�
*******************************************************************************/
void MX25L8006E_READ_DATA(uint8_t *Buffer,uint32_t Address,uint32_t Len) 
{
    uint32_t i = 0;
    
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_RD_DATA_CMD);
    SPI_WRITE_READ_BYTE(Address>>16);
    SPI_WRITE_READ_BYTE(Address>>8);
    SPI_WRITE_READ_BYTE(Address);
 
    for(i = 0;i < Len;i ++)
    {
        Buffer[i] = SPI_WRITE_READ_BYTE(DUMMY_BYTE);
    }
       
    SPI_CS(1);    
}

/*******************************************************************************
��������        ��MX25L8006E_Wait_Busy
��������        ��void
��������ֵ      ��void
����˵��        ����ȡFLASH IC��״̬�Ĵ�����ͨ��״̬�Ĵ�����bit0λ���ж�IC�Ƿ�
                   ����æ��״̬�����bit0 = 1����˵��IC������æ��״̬�����bit0 = 0��
                   ��˵��IC���ڿ���״̬�����Խ�����������
*******************************************************************************/
void MX25L8006E_Wait_Busy(void)
{
	uint8_t status = 0;
	
    SPI_CS(0);
	
	SPI_WRITE_READ_BYTE(MX25_RD_STATUS_REG_CMD);
	
	do
	{
		status = SPI_WRITE_READ_BYTE(DUMMY_BYTE);
		
	}while(status&0x01);	 
	 	
    SPI_CS(1);	
}

/*******************************************************************************
��������        ��MX25L8006E_WRITE_PAGE
��������        ��*Buffer��Ҫд�����ݵ��׵�ַ
                  Address��Ҫд��FLASH IC���׵�ַ
                  Len��Ҫд������ݳ���  
��������ֵ      ��void
����˵��        ����ִ��ҳд�����ǰ����Ҫȡ��д�������ܡ�
                  ҳд��ֻ������д��256���ֽڣ����д�볬��256���ֽڣ���Ḳ��
                  ֮ǰд������ݡ�  
                  ִ����ҳд�빦��֮����Ҫ�ȴ�FLASH IC��æ״̬תΪ���С�  
*******************************************************************************/
void MX25L8006E_WRITE_PAGE(uint8_t *Buffer,uint32_t Address,uint16_t Len) 
{
    uint16_t i;
    
	MX25L8006E_WRITE_ENABLE();	
	
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_WR_PAGE_CMD);
    SPI_WRITE_READ_BYTE(Address>>16);
    SPI_WRITE_READ_BYTE(Address>>8);
    SPI_WRITE_READ_BYTE(Address);
 
    for(i = 0;i < Len;i ++)
    {
        SPI_WRITE_READ_BYTE(Buffer[i]);
    }
       
    SPI_CS(1); 
    
    MX25L8006E_Wait_Busy();   
}    

/*******************************************************************************
��������        ��MX25L8006E_ERASE_SECTOR
��������        ��Address����Ҫ������������ַ
��������ֵ      ��void
����˵��        ����ִ��������������ǰ����Ҫȡ��д�������ܡ�
                  ��Ҫע����ǣ������ĵ�λ��4K�ֽڣ��������Ǵ����ַ�������ڵ�������  
                  ִ����������������֮����Ҫ�ȴ�FLASH IC��æ״̬תΪ���С�  
*******************************************************************************/
void MX25L8006E_ERASE_SECTOR(uint32_t Address) 
{
	MX25L8006E_WRITE_ENABLE();
	
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_SECTOR_ERASE_CMD);
    SPI_WRITE_READ_BYTE(Address>>16);
    SPI_WRITE_READ_BYTE(Address>>8);
    SPI_WRITE_READ_BYTE(Address);
   
    SPI_CS(1); 
    
    MX25L8006E_Wait_Busy();  
} 

/*******************************************************************************
��������        ��MX25L8006E_ERASE_BLOCK
��������        ��Address����Ҫ�����Ŀ�����ַ
��������ֵ      ��void
����˵��        ����ִ�п�����������ǰ����Ҫȡ��д�������ܡ�
                  ��Ҫע����ǣ������ĵ�λ��64K�ֽڣ��������Ǵ����ַ�������ڵĿ�����  
                  ִ���������������֮����Ҫ�ȴ�FLASH IC��æ״̬תΪ���С�  
*******************************************************************************/
void MX25L8006E_ERASE_BLOCK(uint32_t Address) 
{
	MX25L8006E_WRITE_ENABLE();
	
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_BLOCK_ERASE_CMD);
    SPI_WRITE_READ_BYTE(Address>>16);
    SPI_WRITE_READ_BYTE(Address>>8);
    SPI_WRITE_READ_BYTE(Address);
   
    SPI_CS(1); 
    
    MX25L8006E_Wait_Busy();  
} 

/*******************************************************************************
��������        ��MX25L8006E_ERASE_CHIP
��������        ��������ƬFLASH IC
��������ֵ      ��void
����˵��        ����ִ�п�����������ǰ����Ҫȡ��д�������ܡ�
                  ��Ƭ����FLASH IC�Ĳ�����Ҫִ�е�ʱ��ȽϾ�  
                  ִ���������������֮����Ҫ�ȴ�FLASH IC��æ״̬תΪ���С�  
*******************************************************************************/
void MX25L8006E_ERASE_CHIP(void) 
{
	MX25L8006E_WRITE_ENABLE();
	
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_CHIP_ERASE_CMD);

    SPI_CS(1); 
    
    MX25L8006E_Wait_Busy();  
} 

/*******************************************************************************
��������        ��MX25L8006E_Init
��������        ��void
��������ֵ      ��void
����˵��        ����ʼ��FLASH IC��ͨ����ȡIC��ID���ж�FLASH IC�Ƿ���� 
*******************************************************************************/
void MX25L8006E_Init(void)
{
	uint32_t status = 0;
    
    status = MX25L8006E_READ_ID();
    
    if(status == MX25_ID)
    {
        FLASH_IC_READY = 1;    
    }
    else
    {
        FLASH_IC_READY = 0;    
    }         
}

/*******************************************************************************
��������        ��MX25L8006E_TEST1
��������        ��void
��������ֵ      ��void
����˵��        ��FLASH IC���Գ���,��ȡ��ƬIC�����ݣ�ͨ��UART����� 
*******************************************************************************/
void MX25L8006E_TEST1(void)
{   
//    uint16_t sector_count;
    uint32_t i = 0;
    uint32_t address;
    
    if(FLASH_IC_READY == 0)
    {
        return;    
    }
    
	
	//MX25L8006E_ERASE_CHIP();
	
    
    //for(sector_count = 0;sector_count < 256;sector_count++)//MX25L8006E�ܹ���256������
    //{
        //address = sector_count*_MX25_SECTOR_SIZE_;
        
        address = 0;
        MX25L8006E_READ_DATA(FLASH_DATA,address,_MX25_SECTOR_SIZE_);
        
        printf("��0������������\r\n");
        
        for(i = 0;i < 4096;i++)
        {
            if(i%16 == 0)    
            {
                printf("\r\n");
               
            }
            
            printf("%x\t",FLASH_DATA[i]);
            
            
        }
        printf("\r\n");
        
            
        
    //}
    
    
    
        
        
//	MX25L8006E_ERASE_SECTOR(2);  //��0 1 2���ǲ�����һ������ 
//	
//    MX25L8006E_READ_DATA(FLASH_DATA,0x00000000,_MX25_SECTOR_SIZE_);
    
    __NOP();       
}









