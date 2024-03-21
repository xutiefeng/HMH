#ifndef _OTADRIVER_IAP_H_
#define _OTADRIVER_IAP_H_

#include "..\H\OTADriver.h"
#include "SC_Init.h"
#include "..\H\OTADriver_Statement.H"

#define	IAP_MEMTYPE_APROM		0x00							//IAP操作区域为ROM
#define	IAP_MEMTYPE_LDROM		0x03							//IAP操作区域为ROM


unsigned char IAP_WriteOneByte(unsigned long IAP_IapAddr,unsigned char Write_IAP_IapData);	//写单字节IAP操作

unsigned char IAP_WriteNByte(unsigned long IAP_IapAddr, int Length, void* Write_IAP_IapData);//写N字节IAP操作
	
unsigned char IAP_ReadOneByte(unsigned long IAP_IapAddr);	//读单字节IAP操作

void IAP_ReadNByte(unsigned long IAP_IapAddr,int Length, void *BuffAddr);

void IAP_Erase_512B(unsigned long IAP_IapAddr);	//扇区擦除

#endif