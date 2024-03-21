#ifndef _OTADriver_Statement_H_
#define _OTADriver_Statement_H_

#define BOOT 0X00               //BOOT模式
#define APP	 0x01				//APP模式
#define OTA_UARTHANDLE() Uart_Handle()

typedef enum
{
		OTA_STANDBY  = 0x00,
        OTA_READY    = 0x01,
	    OTA_BUSY     = 0x02,
        OTA_IAPWRITED   = 0x03,
        OTA_COMPLETE = 0x04,
	    OTA_FAULT    = 0x0F,
} OTASTATE;

typedef struct APPMemoryInfoStruct
{
	unsigned char UpdateSucceededFlagNumber[4];
	unsigned long APPSize;
	unsigned long APPChecksums;
	unsigned long InitFlag;
}APPMemoryInfoStruct;

typedef struct _IAPDATA_INFO
    {
		OTASTATE      State;
        unsigned int  PacketNumber;   //数据包序号
        unsigned int  Size;           //数据长度
        unsigned char *Memory;        //数据
    } IAPDATA_INFO;
	
#include "..\H\OTADriver.h"
#include "IOTCONFIG.H"
//=======================================================================	
extern unsigned char code UpdateSucceededFlagNumber[4];
extern unsigned long code VersionDate;
extern IAPDATA_INFO IAP_Pack;
extern unsigned long  IAP_Checksum;
extern unsigned long IAP_Size;
extern unsigned char BOOT_WaitTime;
	
extern void BootLoadInit();
extern void BootLoad_DeInit();
extern void  GOTO_Reset(unsigned char mode);
extern void  OTA_JumpBoot(void);		
extern void  OTA_JumpApp(void);	
extern void  SetInterruptAPP();
extern void  SetInterruptBOOT();


extern void OTA_Init(void UartReceiveHandle(char Data),char *UartReceiveBuf);
extern void UART_SendByte(uint8_t UartData);
extern void UART_SendString(char *str);		
extern void Uart_Handle(void);
		
unsigned long ReadAppDate();		
unsigned char AppCompleteCheck();		
unsigned char AppUpdataFlagCheck(char region);
unsigned char IAP_BacksArea_CrcCheck();

void EraseSpace(unsigned long BeginAddr,unsigned long EndAddr);
unsigned int SaveOTA_to_BacksArea(IAPDATA_INFO *IAP_Pack);
unsigned char BackupsToRunCopy();
unsigned char WriteAPPMemoryInfo(APPMemoryInfoStruct APPMemoryInfo);
unsigned char WriteBackupsAppData(unsigned int OffsetAddress, unsigned char *Data, unsigned int Length);
unsigned char IAP_Check_APFLASH(unsigned char Crc);

unsigned long CheckSum32(uint32_t cs, u8 *Pdata, uint32_t length);

#endif
