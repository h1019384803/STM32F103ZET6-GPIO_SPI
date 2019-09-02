#include "MX25L8006E.h"
#include "IO_SPI.h"
#include "USART.h"

uint8_t FLASH_IC_READY;//0：NOT REDAY 1：READY
uint8_t FLASH_DATA[_MX25_SECTOR_SIZE_];//4Kbyte


/*******************************************************************************
函数名称        ：MX25L8006E_READ_ID
函数参数        ：void
函数返回值      ：返回制造商标识和设备ID，读取成功会返回：0xC22014
函数说明        ：当发送完MX25_RD_MANUFACTER_DEVICE_ID_CMD命令之后，FLASH IC
                  会不断的反馈ID的信息，如果读取成功会反馈0xC22014的数值。
                  如果片选信号不置高，则FLASH IC会不断循环的反馈ID的信息，也就
                  是说第4个读取操作，读取回来的是0xC2。
                  
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
函数名称        ：MX25L8006E_READ_REMS
函数参数        ：根据传入的参数值来决定读取信息的顺序，如果dat=0x00，则读取
                  成功返回：0xC213，如果dat=0x01，则读取成功返回：0x13C2
函数返回值      ：读取成功返回：0xC213或0x13C2
函数说明        ：当发送完MX25L8006E_READ_REMS命令之后，需要再发送2个字节的空操作，
                  然后再发送1个字节的数据，FLASH IC根据第3个字节的参数，返回对应的
                  数据。
                  
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
函数名称        ：MX25L8006E_WRITE_ENABLE
函数参数        ：void
函数返回值      ：void
函数说明        ：取消FLASH ID写保护
*******************************************************************************/
void MX25L8006E_WRITE_ENABLE(void)
{  
    SPI_CS(0);
    SPI_WRITE_READ_BYTE(MX25_WR_ENABLE_CMD);        
    SPI_CS(1);
}   

/*******************************************************************************
函数名称        ：MX25L8006E_WRITE_DISABLE
函数参数        ：void
函数返回值      ：void
函数说明        ：使能FLASH ID写保护
*******************************************************************************/
void MX25L8006E_WRITE_DISABLE(void)
{  
    SPI_CS(0);   
    SPI_WRITE_READ_BYTE(0x06);     
    SPI_CS(1);
}  

/*******************************************************************************
函数名称        ：MX25L8006E_READ
函数参数        ：*Buffer：存储读取数据的首地址
                  Address：要读取的数据的首地址
                  Len：要读取的数据长度  
函数返回值      ：void
函数说明        ：读取FLASH IC内指定地址的数据长度，因为MX25L8006E是1Mbyte的
                   存储IC，地址长度为0x00000000~0x000FFFFF，所以地址长度有3个字节
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
函数名称        ：MX25L8006E_Wait_Busy
函数参数        ：void
函数返回值      ：void
函数说明        ：读取FLASH IC的状态寄存器，通过状态寄存器的bit0位来判断IC是否
                   处于忙的状态，如果bit0 = 1，这说明IC还处于忙的状态；如果bit0 = 0，
                   则说明IC处于空闲状态，可以进行其他操作
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
函数名称        ：MX25L8006E_WRITE_PAGE
函数参数        ：*Buffer：要写入数据的首地址
                  Address：要写入FLASH IC的首地址
                  Len：要写入的数据长度  
函数返回值      ：void
函数说明        ：在执行页写入操作前，需要取消写保护功能。
                  页写入只能连续写入256个字节，如果写入超过256个字节，则会覆盖
                  之前写入的数据。  
                  执行完页写入功能之后，需要等待FLASH IC从忙状态转为空闲。  
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
函数名称        ：MX25L8006E_ERASE_SECTOR
函数参数        ：Address：需要擦除的扇区地址
函数返回值      ：void
函数说明        ：在执行扇区擦除操作前，需要取消写保护功能。
                  需要注意的是，扇区的单位是4K字节，擦除的是传入地址参数所在的扇区。  
                  执行完扇区擦除功能之后，需要等待FLASH IC从忙状态转为空闲。  
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
函数名称        ：MX25L8006E_ERASE_BLOCK
函数参数        ：Address：需要擦除的块区地址
函数返回值      ：void
函数说明        ：在执行块区擦除操作前，需要取消写保护功能。
                  需要注意的是，块区的单位是64K字节，擦除的是传入地址参数所在的块区。  
                  执行完块区擦除功能之后，需要等待FLASH IC从忙状态转为空闲。  
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
函数名称        ：MX25L8006E_ERASE_CHIP
函数参数        ：擦除整片FLASH IC
函数返回值      ：void
函数说明        ：在执行块区擦除操作前，需要取消写保护功能。
                  整片擦除FLASH IC的操作需要执行的时间比较久  
                  执行完块区擦除功能之后，需要等待FLASH IC从忙状态转为空闲。  
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
函数名称        ：MX25L8006E_Init
函数参数        ：void
函数返回值      ：void
函数说明        ：初始化FLASH IC，通过读取IC的ID来判断FLASH IC是否存在 
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
函数名称        ：MX25L8006E_TEST1
函数参数        ：void
函数返回值      ：void
函数说明        ：FLASH IC测试程序,读取整片IC的数据，通过UART输出。 
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
	
    
    //for(sector_count = 0;sector_count < 256;sector_count++)//MX25L8006E总共有256个扇区
    //{
        //address = sector_count*_MX25_SECTOR_SIZE_;
        
        address = 0;
        MX25L8006E_READ_DATA(FLASH_DATA,address,_MX25_SECTOR_SIZE_);
        
        printf("第0个扇区的数据\r\n");
        
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
    
    
    
        
        
//	MX25L8006E_ERASE_SECTOR(2);  //传0 1 2都是擦除第一个扇区 
//	
//    MX25L8006E_READ_DATA(FLASH_DATA,0x00000000,_MX25_SECTOR_SIZE_);
    
    __NOP();       
}









