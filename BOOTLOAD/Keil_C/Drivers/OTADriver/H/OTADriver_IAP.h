#ifndef _OTADRIVER_IAP_H_
#define _OTADRIVER_IAP_H_

#include "..\H\OTADriver.h"
#include "SC_Init.h"
#include "..\H\OTADriver_Statement.H"

#define	IAP_MEMTYPE_APROM		0x00							//IAP��������ΪROM
#define	IAP_MEMTYPE_LDROM		0x03							//IAP��������ΪROM


unsigned char IAP_WriteOneByte(unsigned long IAP_IapAddr,unsigned char Write_IAP_IapData);	//д���ֽ�IAP����

unsigned char IAP_WriteNByte(unsigned long IAP_IapAddr, int Length, void* Write_IAP_IapData);//дN�ֽ�IAP����
	
unsigned char IAP_ReadOneByte(unsigned long IAP_IapAddr);	//�����ֽ�IAP����

void IAP_ReadNByte(unsigned long IAP_IapAddr,int Length, void *BuffAddr);

void IAP_Erase_512B(unsigned long IAP_IapAddr);	//��������

#endif