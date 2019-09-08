#include "MX25L8006E.h"
#include "IO_SPI.h"
#include "USART.h"

uint32_t MX25L8006E_ID;

uint8_t MX25_SECTOR[_MX25_SECTOR_SIZE_];//4Kbyte


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
	
	SPI_DELAY(100);
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

	SPI_DELAY(100);	
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
	SPI_WRITE_READ_BYTE((Address&0xFF0000)>>16);
	SPI_WRITE_READ_BYTE((Address&0xFF00)>>8);
	SPI_WRITE_READ_BYTE(Address&0xFF);
 
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
	MX25L8006E_Wait_Busy(); 
	
	SPI_CS(0);
	
	SPI_WRITE_READ_BYTE(MX25_WR_PAGE_CMD);
	SPI_WRITE_READ_BYTE((Address&0xFF0000)>>16);
	SPI_WRITE_READ_BYTE((Address&0xFF00)>>8);
	SPI_WRITE_READ_BYTE(Address&0xFF);
 
	for(i = 0;i < Len;i ++)
	{
		SPI_WRITE_READ_BYTE(Buffer[i]);
	}
	   
	SPI_CS(1); 
	
	MX25L8006E_Wait_Busy();   				
}    


/*******************************************************************************
函数名称        ：MX25L8006E_WRITE_DATE_READY
函数参数        ：*Buffer：要写入数据的首地址
                  Address：要写入FLASH IC的首地址
                  Len：要写入的数据长度  
函数返回值      ：void
函数说明        ：写入数据，因为一次最多可以连续写入256个字节，所以，
				  需要判断写入的数据的长度
*******************************************************************************/
void MX25L8006E_WRITE_DATE_READY(uint8_t *Buffer,uint32_t Address,uint16_t Len) 
{
    uint16_t number;
	
	while(1)
	{
		if(Len > 256)
		{
			number = 256; 
		}
		else
		{
			number = Len;
		}
		
		MX25L8006E_WRITE_PAGE(Buffer,Address,number);
	
		if(number == Len)
		{
			break;
		}
		else
		{
			Len = Len - number;//算出还剩余多少字节没写入
			Address += number;//因为已经写入了number个字节，所以地址需要偏移number个地址
			Buffer += number;//Buffer指针也要增加number个偏移	
		}	
	}
}   

/*******************************************************************************
函数名称        ：MX25L8006E_WRITE_CHEACK_ERASE
函数参数        ：*Buffer：要写入数据的首地址
                  Address：要写入FLASH IC的首地址
                  Len：要写入的数据长度  
函数返回值      ：void
函数说明        ：由于FLASH IC只有在存储为0xFF时才能够写入数据，如果不为0xFF，则需要
				  进行擦除操作，这里正是检测FLASH IC是否能够写入，并且在不影响其他地址
				  数据的前提下，写入数据。
*******************************************************************************/
void MX25L8006E_WRITE_CHEACK_ERASE(uint8_t *Buffer,uint32_t Address,uint32_t Len) 
{
    uint16_t i;
	uint16_t sec_num;//当前地址所在的扇区
	uint16_t sec_remain;//当前地址到扇区结束地址还有多少空间
	uint16_t sec_addr;
	
	if(Len == 0)
	{
		return;
	}
	
	sec_num = Address/4096;//每一个扇区有4096个地址，所以地址除以4096就可以得出当前地址所在的扇区号
	sec_addr = Address%4096;//取出当前地址在对应的扇区的偏移地址。
	sec_remain = 4096 - sec_addr;//得出当前地址到扇区的结束地址所剩余的空间 
	
	if(sec_remain > Len)//这里主要是判断是否需要跨越扇区写入数据，如果当前扇区的剩余地址足够容纳要写入的数据个数，则不需要跨域扇区写数据，如果不够，则需要考虑跨扇区写数据。
	{
		sec_remain = Len;//该扇区的剩余地址足够容纳Len个要写入的数据。
	}
	
	do
	{
		MX25L8006E_READ_DATA(MX25_SECTOR,sec_num*4096,4096);//读出整个扇区的数据
		
		for(i = 0;i < sec_remain;i ++)
		{
			if(MX25_SECTOR[i+sec_addr] != 0xFF)
			{
				break;		//判断要写入的地址的数据是否为0xFF
			}	
		}
		
		if(i == sec_remain)		//如果所有要写入的地址的数据都是0xFF,则不需要擦除
		{
			MX25L8006E_WRITE_DATE_READY(Buffer,Address,sec_remain); 
		}
		else
		{
			MX25L8006E_ERASE_SECTOR(sec_num*4096);//擦除整个扇区
			
			for(i = 0;i < sec_remain;i ++)
			{
				MX25_SECTOR[i+sec_addr] = Buffer[i];//为了不改变其它扇区的位置，只赋值给需要写入的地址	
			}			
			
			MX25L8006E_WRITE_DATE_READY(MX25_SECTOR,sec_num*4096,4096);	
		}	
		
		if(sec_remain == Len)//如果相等，说明要写入的数据已经写完
		{
			break;
		}
		else
		{
			sec_num += 1;//写入数据到下一个扇区地址
			Buffer += sec_remain;//数组地址偏移
			Address += sec_remain;//因为写入了sec_remain个数据，所以地址要偏移sec_remain个地址 	
			sec_addr = 0;//因为是从下一个扇区的开始地址开始写，所以这里为0
			Len -= sec_remain;//因为写入了sec_remain个数据，所以要写入的总长度要减去已经写入了的数据sec_remain
			
			if(Len > 4096)
			{
				sec_remain = 4096;
			}
			else
			{
				sec_remain = Len;
			}			
		}
		
	}while(1);
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
    MX25L8006E_Wait_Busy();
	
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
    MX25L8006E_Wait_Busy();
	
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
    MX25L8006E_Wait_Busy();
	
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_CHIP_ERASE_CMD);

    SPI_CS(1); 
    
    MX25L8006E_Wait_Busy();  
} 


/*******************************************************************************
函数名称        ：MX25L8006E_Init
函数参数        ：void
函数返回值      ：void
函数说明        ：初始化FLASH IC，读取FLASH IC的ID值
*******************************************************************************/
void MX25L8006E_Init(void)
{
    MX25L8006E_ID = MX25L8006E_READ_ID();
    
	printf("   \n");
	
	if(MX25L8006E_ID == MX25_ID)
	{
		
		printf("FLASH IC的ID为：%x!\n",MX25L8006E_ID);
	}
	else	
	{
		printf("无法识别FLASH IC!\n");
	}
}

/*******************************************************************************
函数名称        ：MX25L8006E_TEST1
函数参数        ：void
函数返回值      ：void
函数说明        ：FLASH IC测试程序,将数据写入到某个扇区，然后读取整个扇区的数据，
				  最后通过UART打印出来。 
*******************************************************************************/
uint8_t SWAP[4096];
void MX25L8006E_TEST1(void)
{   
    uint32_t i = 0;
	
	uint16_t num_sec;	
    uint32_t address;
	
	//uint8_t SWAP[4096];//不能开辟4K的局部变量，因为栈的空间只有0x400即1024个字节，如果开辟太大栈空间会溢出造成死机
	 
    if(MX25L8006E_ID != MX25_ID)
    {
        return;    
    }
	
	for(i = 0;i<4096;i++)
	{
		SWAP[i] = i+1;
	}
	
	num_sec = 0;
	address = num_sec *4096;
	
	MX25L8006E_WRITE_CHEACK_ERASE(SWAP,address,4096);
	
	HAL_Delay(200);
	MX25L8006E_READ_DATA(SWAP,address,4096);
	
    printf("第%d个扇区的数据如下：\n",num_sec);
	
    for(i = 0;i < 4096;i++)
    {
		if(i%16 == 0)    
		{
			printf("  \n");
		   
		}
            
        printf("%x\t",SWAP[i]);          
    }     
}

/*******************************************************************************
函数名称        ：MX25L8006E_TEST2
函数参数        ：void
函数返回值      ：void
函数说明        ：FLASH IC测试程序,读取整片IC的数据，通过UART输出。 
*******************************************************************************/
void MX25L8006E_TEST2(void)
{
	uint8_t MX_TABLE[200];
	uint8_t i;
	uint32_t address;


    if(MX25L8006E_ID != MX25_ID)
    {
        return;    
    }
	
	for(i = 0;i<200;i++) 
	{
		MX_TABLE[i] = i+1;
	}
	
	
	address = 4*4096;
	address -= 50;	//从第3个扇区的倒数50个地址开始
	
	MX25L8006E_READ_DATA(&i,address+100,1);	
	
	MX25L8006E_WRITE_CHEACK_ERASE(MX_TABLE,address,101);//需要注意的是只写到address+100的地址，因为address~address+100已经有101个数据，不会在address+101地址写入数据的 
	
	MX25L8006E_READ_DATA(MX_TABLE,address,200);
	
	for(i = 0;i<101;i++)
	{
		printf("%d\r\n",MX_TABLE[i]);		
	}
}	


/*******************************************************************************
函数名称        ：MX25L8006E_TEST3
函数参数        ：void
函数返回值      ：void
函数说明        ： 
*******************************************************************************/
void MX25L8006E_TEST3(void)
{
	uint8_t TX_BUFF[] = "你好！欢迎时使用MX25L8006E。";	
	uint8_t RX_BUFF[sizeof(TX_BUFF)];

    if(MX25L8006E_ID != MX25_ID)
    {
        return;    
    }
	
	MX25L8006E_WRITE_CHEACK_ERASE(TX_BUFF,5*4096,sizeof(TX_BUFF));//将TX_BUF数组的值写入到FLASH IC中
	
	while(1)
	{
	
		MX25L8006E_READ_DATA(RX_BUFF,5*4096,sizeof(TX_BUFF));//读取FLASH IC的数值到RX_BUF数组中
	
		printf("RX_BUF内的数据为：%s\n",RX_BUFF);//打印RX_BUF数组的值，验证是否写入成功。  
		HAL_Delay(1000);
	}
}









