#ifndef __IOTCONFIG_XIAOMI_H
#define __IOTCONFIG_XIAOMI_H

#include "IOTCONFIG.H"

#include "CRC_util.h"
#include "MIIO_OPERATE.h"


#define false FALSE
#define true  TRUE

#define IAP_TIME_WINDOW   (ISP_TIME_WINDOW*1000)  // ?s
#define IAP_TIME_BOOT     (5000)  // 5s
#define IAP_TIME_BOOTFIAL (12000) // 10s

    typedef enum
    {
        sMIIO_Standby,
        sMIIO_Ready,
        sIAP,
        sAP,
        sMIIO_Fault,
		sMIIO_Model,
		sMIIO_Ble_config,
		sMIIO_Mcu_version,
		sMIIO_Get_down,	
    } SystemMode_t;

    typedef struct
    {
        SystemMode_t Mode;
        u32 TimeCount;
        u32 TimeLimit;
    } System_t;
	
    extern System_t System;
    extern u8 XMClient_Buffer[];
    extern XmodemStatus_t XS_Result;
    extern MIIO_Result_t miio_result;	
    extern unsigned char  IOT_Uart_DataBuf[];

    extern void Systime_Count();
    extern void  IOT_Init(char Select);
    extern void  IOT_Uart_Receiver(char Data);
    extern IOT_State  IOT_Work();
    extern void  OTA_FirmwareDownload(IAPDATA_INFO *IAP_Pack);
		
    extern void Systime_Time(u8 slice);
    extern void Systime_Set(SystemMode_t mode, u32 TimeLimit);
    extern void MIIO_OTA_Init(SystemMode_t mode);
#endif	