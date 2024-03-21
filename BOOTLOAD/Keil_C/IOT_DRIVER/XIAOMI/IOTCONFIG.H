#ifndef __IOTCONFIG_H
#define __IOTCONFIG_H

#include "SC_Init.h"
#include "SCDriver_list.h"

typedef enum
{
	IOT_NORMAL  =0,
	IOT_GET_FW  =1,
    IOT_DOWN_FW =2,
} IOT_State;
	
extern unsigned char  IOT_Uart_DataBuf[];

extern void Systime_Count();
extern void  IOT_Init(char Select);
extern void  IOT_Uart_Receiver(char Data);
extern IOT_State  IOT_Work();
extern void  OTA_FirmwareDownload(IAPDATA_INFO *IAP_Pack);	
#endif	