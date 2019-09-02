#ifndef __MX25_FLASH_H
#define __MX25_FLASH_H
#include "stm32f1xx_hal.h"


#define _MX25_SECTOR_SIZE_                  4096


extern uint8_t FLASH_IC_READY;//0��NOT REDAY 1��READY
extern uint8_t FLASH_DATA[_MX25_SECTOR_SIZE_];//4Kbyt



//MX25L8006E ����
#define	MX25_WR_ENABLE_CMD				    0x06	//ȡ��д��������λ״̬�Ĵ�����bit1λWEL
#define MX25_WR_DISABLE_CMD				    0x04	//ʹ��д���������״̬�Ĵ�����bit1λWEL
#define MX25_RD_STATUS_REG_CMD			    0x05	//��ȡ״̬�Ĵ�����ֵ��MX25L8006E����״̬�Ĵ�����ֵ
#define MX25_WR_STATUS_REG_CMD			    0x01	//д״̬�Ĵ�������ָ����ֵд�뵽״̬�Ĵ���
#define MX25_RD_DATA_CMD				    0x03	//��ȡIC�ڵ����ݣ����������Ҫ��ȡ���ݵĵ�ַ��ÿ��ȡһ�Σ���ַ�Զ���1������ַ�Զ���������IC��������ַʱ����ַ�ᱻ��0������0��ַ��ʼ
#define MX25_FAST_RD_DATA_CMD			    0x0B	//������ʱ��Ƶ��ȡIC�ڵ����ݣ����������Ҫ��ȡ���ݵĵ�ַ������MX25L8006E�������ֲᣬʱ��Ƶ�������Ե�86MHZ
#define MX25_FAST_RD_DUAL_CMD			    0x3B	//ͨ��2�������߶�ȡ���ݣ�������Բ鿴MX25L8006E�������ֲ�
#define MX25_WR_PAGE_CMD				    0x02	//ҳд��������������Ҫд�����ݵĵ�ַ��ÿд��һ�����ݣ���ַ���Զ���1��Ҳд��ֻ������д��256���ֽڣ��������256���ֽڣ�����Զ��ص�����ĵ�ַ
#define MX25_SECTOR_ERASE_CMD			    0x20	//�����������ÿ�β���4K�ֽڣ��������Ҫ���������ĵ�ַ����Ҫע����ǣ���ַ������FLASH IC�ڵ�����һ����ַ���������ڲ�������һ�����������Բ������Ǵ���ĵ�ַ���ڵ���һ�����������紫�����0~4095�е�����һ����ַ�����Ķ�������0
#define MX25_BLOCK_ERASE_CMD			    0xD8    //��������һ�������16������������������һ��������ĵ�ַҲ������FLASH IC������һ����ַ�����ǲ������Ǹõ�ַ���ڵĿ�����
#define MX25_CHIP_ERASE_CMD				    0xC7    //������ƬFLASH������ʱ��Ҫ�ܾ�
#define MX25_POWER_DOWN_CMD				    0xB9	//ʹFLASH IC�������ģʽ������ʡ����
#define MX25_RELEASE_POWER_DOWN_CMD		    0xAB	//ʹFLASH IC�ӵ���ģʽ�л���
#define MX25_RD_DEVICE_ID_CMD			    0xAB	//��ȡID���������֮���ڵ�4���ֽ�ʱ����ID��Ϣ0x13
#define MX25_RD_ID_CMD	                    0x90	//��ȡID����ȴ���3���ֽڵĲ���������1��2��������㣬FLASH IC���ݵ�3��������������������Ϣ��˳�������3��������0x00������0xC2 0x13�������3��������0x01,����0x13 0x20
#define MX25_RD_MANUFACTER_DEVICE_ID_CMD    0x9F	//��ȡID�������1���ֽڵ������̱�ʶ��2���ֽڵ��豸��ʶ������MX25L8006E�����ֲ᷵�ص��� 0xC2 0x20 0x14

//MX25L8006E ID
#define MX25_ID							    0xC22014//�������FLASH IC�Ƿ����

#define DUMMY_BYTE						    0xFF	//������ȡFLASH ICʱ��ͬʱ���͸���FLASH IC������


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//��������

extern uint32_t MX25L8006E_READ_ID(void);
extern uint16_t MX25L8006E_READ_REMS(uint8_t dat);

extern void MX25L8006E_WRITE_ENABLE(void);
extern void MX25L8006E_WRITE_DISABLE(void);
extern void MX25L8006E_Wait_Busy(void);
extern void MX25L8006E_READ_DATA(uint8_t *Buffer,uint32_t Address,uint32_t Len);
extern void MX25L8006E_WRITE_PAGE(uint8_t *Buffer,uint32_t Address,uint16_t Len);
extern void MX25L8006E_ERASE_SECTOR(uint32_t Address);
extern void MX25L8006E_ERASE_BLOCK(uint32_t Address);
extern void MX25L8006E_ERASE_CHIP(void);

extern void MX25L8006E_Init(void);
extern void MX25L8006E_TEST1(void);


#endif


