#include "MX25L8006E.h"
#include "IO_SPI.h"
#include "USART.h"

uint32_t MX25L8006E_ID;

uint8_t MX25_SECTOR[_MX25_SECTOR_SIZE_];//4Kbyte


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
	
	SPI_DELAY(100);
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

	SPI_DELAY(100);	
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
��������        ��MX25L8006E_WRITE_DATE_READY
��������        ��*Buffer��Ҫд�����ݵ��׵�ַ
                  Address��Ҫд��FLASH IC���׵�ַ
                  Len��Ҫд������ݳ���  
��������ֵ      ��void
����˵��        ��д�����ݣ���Ϊһ������������д��256���ֽڣ����ԣ�
				  ��Ҫ�ж�д������ݵĳ���
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
			Len = Len - number;//�����ʣ������ֽ�ûд��
			Address += number;//��Ϊ�Ѿ�д����number���ֽڣ����Ե�ַ��Ҫƫ��number����ַ
			Buffer += number;//Bufferָ��ҲҪ����number��ƫ��	
		}	
	}
}   

/*******************************************************************************
��������        ��MX25L8006E_WRITE_CHEACK_ERASE
��������        ��*Buffer��Ҫд�����ݵ��׵�ַ
                  Address��Ҫд��FLASH IC���׵�ַ
                  Len��Ҫд������ݳ���  
��������ֵ      ��void
����˵��        ������FLASH ICֻ���ڴ洢Ϊ0xFFʱ���ܹ�д�����ݣ������Ϊ0xFF������Ҫ
				  ���в����������������Ǽ��FLASH IC�Ƿ��ܹ�д�룬�����ڲ�Ӱ��������ַ
				  ���ݵ�ǰ���£�д�����ݡ�
*******************************************************************************/
void MX25L8006E_WRITE_CHEACK_ERASE(uint8_t *Buffer,uint32_t Address,uint32_t Len) 
{
    uint16_t i;
	uint16_t sec_num;//��ǰ��ַ���ڵ�����
	uint16_t sec_remain;//��ǰ��ַ������������ַ���ж��ٿռ�
	uint16_t sec_addr;
	
	if(Len == 0)
	{
		return;
	}
	
	sec_num = Address/4096;//ÿһ��������4096����ַ�����Ե�ַ����4096�Ϳ��Եó���ǰ��ַ���ڵ�������
	sec_addr = Address%4096;//ȡ����ǰ��ַ�ڶ�Ӧ��������ƫ�Ƶ�ַ��
	sec_remain = 4096 - sec_addr;//�ó���ǰ��ַ�������Ľ�����ַ��ʣ��Ŀռ� 
	
	if(sec_remain > Len)//������Ҫ���ж��Ƿ���Ҫ��Խ����д�����ݣ������ǰ������ʣ���ַ�㹻����Ҫд������ݸ���������Ҫ��������д���ݣ��������������Ҫ���ǿ�����д���ݡ�
	{
		sec_remain = Len;//��������ʣ���ַ�㹻����Len��Ҫд������ݡ�
	}
	
	do
	{
		MX25L8006E_READ_DATA(MX25_SECTOR,sec_num*4096,4096);//������������������
		
		for(i = 0;i < sec_remain;i ++)
		{
			if(MX25_SECTOR[i+sec_addr] != 0xFF)
			{
				break;		//�ж�Ҫд��ĵ�ַ�������Ƿ�Ϊ0xFF
			}	
		}
		
		if(i == sec_remain)		//�������Ҫд��ĵ�ַ�����ݶ���0xFF,����Ҫ����
		{
			MX25L8006E_WRITE_DATE_READY(Buffer,Address,sec_remain); 
		}
		else
		{
			MX25L8006E_ERASE_SECTOR(sec_num*4096);//������������
			
			for(i = 0;i < sec_remain;i ++)
			{
				MX25_SECTOR[i+sec_addr] = Buffer[i];//Ϊ�˲��ı�����������λ�ã�ֻ��ֵ����Ҫд��ĵ�ַ	
			}			
			
			MX25L8006E_WRITE_DATE_READY(MX25_SECTOR,sec_num*4096,4096);	
		}	
		
		if(sec_remain == Len)//�����ȣ�˵��Ҫд��������Ѿ�д��
		{
			break;
		}
		else
		{
			sec_num += 1;//д�����ݵ���һ��������ַ
			Buffer += sec_remain;//�����ַƫ��
			Address += sec_remain;//��Ϊд����sec_remain�����ݣ����Ե�ַҪƫ��sec_remain����ַ 	
			sec_addr = 0;//��Ϊ�Ǵ���һ�������Ŀ�ʼ��ַ��ʼд����������Ϊ0
			Len -= sec_remain;//��Ϊд����sec_remain�����ݣ�����Ҫд����ܳ���Ҫ��ȥ�Ѿ�д���˵�����sec_remain
			
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
    MX25L8006E_Wait_Busy();
	
    SPI_CS(0);
    
    SPI_WRITE_READ_BYTE(MX25_CHIP_ERASE_CMD);

    SPI_CS(1); 
    
    MX25L8006E_Wait_Busy();  
} 


/*******************************************************************************
��������        ��MX25L8006E_Init
��������        ��void
��������ֵ      ��void
����˵��        ����ʼ��FLASH IC����ȡFLASH IC��IDֵ
*******************************************************************************/
void MX25L8006E_Init(void)
{
    MX25L8006E_ID = MX25L8006E_READ_ID();
    
	printf("   \n");
	
	if(MX25L8006E_ID == MX25_ID)
	{
		
		printf("FLASH IC��IDΪ��%x!\n",MX25L8006E_ID);
	}
	else	
	{
		printf("�޷�ʶ��FLASH IC!\n");
	}
}

/*******************************************************************************
��������        ��MX25L8006E_TEST1
��������        ��void
��������ֵ      ��void
����˵��        ��FLASH IC���Գ���,������д�뵽ĳ��������Ȼ���ȡ�������������ݣ�
				  ���ͨ��UART��ӡ������ 
*******************************************************************************/
uint8_t SWAP[4096];
void MX25L8006E_TEST1(void)
{   
    uint32_t i = 0;
	
	uint16_t num_sec;	
    uint32_t address;
	
	//uint8_t SWAP[4096];//���ܿ���4K�ľֲ���������Ϊջ�Ŀռ�ֻ��0x400��1024���ֽڣ��������̫��ջ�ռ������������
	 
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
	
    printf("��%d���������������£�\n",num_sec);
	
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
��������        ��MX25L8006E_TEST2
��������        ��void
��������ֵ      ��void
����˵��        ��FLASH IC���Գ���,��ȡ��ƬIC�����ݣ�ͨ��UART����� 
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
	address -= 50;	//�ӵ�3�������ĵ���50����ַ��ʼ
	
	MX25L8006E_READ_DATA(&i,address+100,1);	
	
	MX25L8006E_WRITE_CHEACK_ERASE(MX_TABLE,address,101);//��Ҫע�����ֻд��address+100�ĵ�ַ����Ϊaddress~address+100�Ѿ���101�����ݣ�������address+101��ַд�����ݵ� 
	
	MX25L8006E_READ_DATA(MX_TABLE,address,200);
	
}	


/*******************************************************************************
��������        ��MX25L8006E_TEST3
��������        ��void
��������ֵ      ��void
����˵��        �� 
*******************************************************************************/
void MX25L8006E_TEST3(void)
{
	uint8_t TX_BUFF[] = "��ã���ӭʱʹ��MX25L8006E��";	
	uint8_t RX_BUFF[sizeof(TX_BUFF)];

    if(MX25L8006E_ID != MX25_ID)
    {
        return;    
    }
	
	MX25L8006E_WRITE_CHEACK_ERASE(TX_BUFF,5*4096,sizeof(TX_BUFF));//��TX_BUF�����ֵд�뵽FLASH IC��
	
	while(1)
	{
	
		MX25L8006E_READ_DATA(RX_BUFF,5*4096,sizeof(TX_BUFF));//��ȡFLASH IC����ֵ��RX_BUF������
	
		printf("RX_BUF�ڵ�����Ϊ��%s\n",RX_BUFF);//��ӡRX_BUF�����ֵ����֤�Ƿ�д��ɹ���  
		HAL_Delay(1000);
	}
}









