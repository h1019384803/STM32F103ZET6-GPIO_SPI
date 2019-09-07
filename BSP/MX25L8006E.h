#ifndef __MX25_FLASH_H
#define __MX25_FLASH_H
#include "stm32f1xx_hal.h"


#define _MX25_SECTOR_SIZE_                  4096



//MX25L8006E 命令
#define	MX25_WR_ENABLE_CMD				    0x06	//取消写保护，置位状态寄存器的bit1位WEL
#define MX25_WR_DISABLE_CMD				    0x04	//使能写保护，清除状态寄存器的bit1位WEL
#define MX25_RD_STATUS_REG_CMD			    0x05	//读取状态寄存器的值，MX25L8006E返回状态寄存器的值
#define MX25_WR_STATUS_REG_CMD			    0x01	//写状态寄存器，将指定的值写入到状态寄存器
#define MX25_RD_DATA_CMD				    0x03	//读取IC内的数据，后面紧跟着要读取数据的地址，每读取一次，地址自动加1，当地址自动递增超过IC的容量地址时，地址会被清0，即从0地址开始
#define MX25_FAST_RD_DATA_CMD			    0x0B	//以最快的时钟频读取IC内的数据，后面紧跟着要读取数据的地址。根据MX25L8006E的数据手册，时钟频率最大可以到86MHZ
#define MX25_FAST_RD_DUAL_CMD			    0x3B	//通过2根数据线读取数据，具体可以查看MX25L8006E的数据手册
#define MX25_WR_PAGE_CMD				    0x02	//页写入命令，后面紧跟着要写入数据的地址，每写入一次数据，地址会自动加1，也写入只能连续写入256个字节，如果超过256个字节，则会自动回到传入的地址
#define MX25_SECTOR_ERASE_CMD			    0x20	//扇区擦除命令，每次擦除4K字节，后面紧跟要擦除扇区的地址，需要注意的是，地址可以是FLASH IC内的任意一个地址，但是由于擦除的是一个扇区，所以擦除的是传入的地址所在的那一个扇区，比如传入的是0~4095中的任意一个地址擦除的都是扇区0
#define MX25_BLOCK_ERASE_CMD			    0xD8    //块擦除命令，一个块包含16个扇区，跟扇区擦除一样，传入的地址也可以是FLASH IC的任意一个地址，但是擦除的是该地址所在的块区。
#define MX25_CHIP_ERASE_CMD				    0xC7    //擦除整片FLASH，擦除时间要很久
#define MX25_POWER_DOWN_CMD				    0xB9	//使FLASH IC进入掉电模式，可以省掉。
#define MX25_RELEASE_POWER_DOWN_CMD		    0xAB	//使FLASH IC从掉电模式中唤醒
#define MX25_RD_DEVICE_ID_CMD			    0xAB	//读取ID命令，该命令之后，在第4个字节时反馈ID信息0x13
#define MX25_RD_ID_CMD	                    0x90	//读取ID命令，先传入3个字节的参数，根据1、2个参数随便，FLASH IC根据第3个参数来决定反馈的信息的顺序，如果第3个参数是0x00，则反馈0xC2 0x13，如果第3个参数是0x01,则反馈0x13 0x20
#define MX25_RD_MANUFACTER_DEVICE_ID_CMD    0x9F	//读取ID命令，返回1个字节的制造商标识和2个字节的设备标识，根据MX25L8006E数据手册返回的是 0xC2 0x20 0x14

//MX25L8006E ID
#define MX25_ID							    0xC22014//用来检测FLASH IC是否存在

#define DUMMY_BYTE						    0xFF	//用来读取FLASH IC时，同时发送给给FLASH IC的数据


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//函数声明

extern uint32_t MX25L8006E_READ_ID(void);
extern uint16_t MX25L8006E_READ_REMS(uint8_t dat);

extern void MX25L8006E_WRITE_ENABLE(void);
extern void MX25L8006E_WRITE_DISABLE(void);
extern void MX25L8006E_Wait_Busy(void);
extern void MX25L8006E_READ_DATA(uint8_t *Buffer,uint32_t Address,uint32_t Len);
extern void MX25L8006E_WRITE_PAGE(uint8_t *Buffer,uint32_t Address,uint16_t Len);
extern void MX25L8006E_WRITE_DATE_READY(uint8_t *Buffer,uint32_t Address,uint16_t Len);
extern void MX25L8006E_WRITE_CHEACK_ERASE(uint8_t *Buffer,uint32_t Address,uint32_t Len); 
extern void MX25L8006E_ERASE_SECTOR(uint32_t Address);
extern void MX25L8006E_ERASE_BLOCK(uint32_t Address);
extern void MX25L8006E_ERASE_CHIP(void);

extern void MX25L8006E_Init(void);
extern void MX25L8006E_TEST1(void);
extern void MX25L8006E_TEST2(void);

extern void MX25L8006E_TEST3(void);

#endif


