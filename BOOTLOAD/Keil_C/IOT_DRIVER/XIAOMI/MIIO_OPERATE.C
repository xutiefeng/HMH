#include "IOTCONFIG_XIAOMI.H"

unsigned char  IOT_Uart_DataBuf[256];

unsigned char XMClient_Buffer[XMODERN_BUFFER_SIZE];
volatile System_t System;
XmodemStatus_t XS_Result;
MIIO_Result_t miio_result = MIR_NA;	


volatile MIIO_IAP_t MIIO_IAP;


XmodemClient_t XMClient;

u32 File_crc;
u8  FileCrcerror=0;
u8 SaveXmodemData[256];

//====================================================================================================
//====================================================================================================
bool CmpStr(char *Uart_Rdata,unsigned char* string)
{
	unsigned int i=0;
    while(*string)
    {
		if(Uart_Rdata[i++]!=*string++)
		{
			return false;
		}
    }
	return true;
}
//====================================================================================================
//====================================================================================================
void MIIO_IAP_InData(s8 Data)
{
	MIIO_IAP.Buffer.Memory[MIIO_IAP.Buffer.Count]=Data;
	MIIO_IAP.Buffer.Count++;
	if(MIIO_IAP.Buffer.Count>sizeof(MIIO_IAP.Buffer.Memory))
	{
		MIIO_IAP.Buffer.Count=0;
	}
	if(Data=='\r')
	{	
		MIIO_IAP.State=MI_Rx;
	}
}
//====================================================================================================
//====================================================================================================
MIIO_Result_t MIIO_IAP_Cmd(char *cmd,                     //
                           char *reply1,              //
                           char *reply2,              //
                           u32 TimeOut,               //
                           void UR_Send(char *Udata)) //
{
	
	if((MIIO_IAP.State!=MI_Tx)&&(MIIO_IAP.State!=MI_Rx))
	{
		UR_Send(cmd);
		MIIO_IAP.State=MI_Tx;
		MIIO_IAP.Time.Count=0;
	}
	if(MIIO_IAP.State==MI_Rx)
	{
		MIIO_IAP.Buffer.Count=0;
		MIIO_IAP.State=MI_Wait;
		if(CmpStr(MIIO_IAP.Buffer.Memory,reply1))
		{
			return MIR_R1;
		}
		if(CmpStr(MIIO_IAP.Buffer.Memory,reply2))
		{
			return MIR_R2;
		}		
		return MIR_NA;
	}
	if(MIIO_IAP.Time.Count>TimeOut)
	{
		MIIO_IAP.Time.Count=0;
		MIIO_IAP.State=MIR_TO;
	}
	
	return MIR_NA;
}


void XmodemClient_ModeInit(XmodemModeVER_t modever, //
                           u8 *PackMemory)          // Size 133Byte
{
	XMClient.ModeVER=modever;
	XMClient.Time.Limit=1000;  //1s
	XMClient.Package.Memory=PackMemory;
	XMClient.Package.Count=0;
	XMClient.Package.Size=0;
	XMClient.Phase=XP_Start;
}

XmodemStatus_t XmodemClient_Proess(IAPDATA_INFO *OTA_Data)
{
	static u16  IAP_Count;
	static u8  ErrorCount;
	u16 crc=0;
	
	u8  IapLen;
	
	if(XMClient.Phase==XP_Start)
	{
		UART_SendByte(XR_C);
		XMClient.Phase=XP_Packing;
		XMClient.State=XS_Receive;
		XMClient.Time.Count=0;
		ErrorCount=0;
		IAP_Size=0;
		IAP_Count=1;
		FileCrcerror=0;
		File_crc=0;
	}
	if((XMClient.Time.Count>XMClient.Time.Limit))//&&(XMClient.Phase==XS_Receive))  //接收超时
	{
		ErrorCount+=1;
		UART_SendByte(XR_NACK);
		XMClient.Time.Count=0;
	}
		
	if((XMClient.Phase==XS_Receive)&&(XMClient.Package.Size>=1))
	{
		if(XMClient.Package.Memory[0]==XR_EOT) //完成发送
		{
			u8  i,j;
			
			for(i=0;i<255;i++)
			{
				if((0xFF==SaveXmodemData[255-i])&&(0x1A==SaveXmodemData[255-i-1]))
				{
					break;
				}
					
			}
			i=i+2+4;
			File_crc=CRC32_Xmodem(File_crc,&SaveXmodemData[0],256-i);

			for(j=0;j<4;j++)
			{
				if((uint8_t)File_crc!=SaveXmodemData[256-i+j])
				{
					FileCrcerror=1;
					break;
				}
				File_crc=File_crc>>8;
			}				
			
			XMClient.Time.Count=0;
			XMClient.Package.Size=0;

			XMClient.State=XS_ReceiveEnd;
			XMClient.Phase=XP_PackEnd;
			UART_SendByte(XR_ACK);
			return XS_Finish;
		}
		if(XMClient.Package.Memory[0]==XR_CAN) //取消发送
		{
		
			
			
			XMClient.Time.Count=0;
			XMClient.Package.Size=0;

			XMClient.State=XS_Error;
			XMClient.Phase=XP_PackEnd;
			UART_SendByte(XR_ACK);
			return XS_Fault;
		}		
		if(XMClient.Package.Size>XMODERN_BUFFER_SIZE) //接收一帧错误
		{
			XMClient.Time.Count=0;
			ErrorCount+=1;
			XMClient.Package.Size=0;
			UART_SendByte(XR_NACK);
		}				
		if((XMClient.Package.Memory[0]==XR_SOH)&&(XMClient.Package.Size==XMODERN_BUFFER_SIZE))
		{
			XMClient.Time.Count=0;
		    XMClient.Package.Size=0;
			if(XMClient.Package.Memory[1]==(IAP_Count&0xFF))
			{
				ErrorCount=0;
				
				{
					u8 i;
					if(IAP_Count>=3)
					{			
						File_crc=CRC32_Xmodem(File_crc,&SaveXmodemData[0],XMODERN_BUFFER_SIZE-5);
					}					
					for(i=0;i<128;i++)
					{
						SaveXmodemData[i]=SaveXmodemData[i+128];
						SaveXmodemData[i+128]=XMClient.Package.Memory[3+i];
					}
			    }				
				
				
				crc=CRC16_Xmodem(crc,&XMClient.Package.Memory[3],XMODERN_BUFFER_SIZE-5);
				if((crc-XMClient.Package.Memory[XMODERN_BUFFER_SIZE-2]*0x100-XMClient.Package.Memory[XMODERN_BUFFER_SIZE-1])==0)
				{
		
					OTA_Data->PacketNumber=IAP_Count-1;
	                OTA_Data->Size=XMODERN_BUFFER_SIZE-5;
					for(IapLen=0;IapLen<OTA_Data->Size;IapLen++)
					{
	                    OTA_Data->Memory[IapLen]=XMClient.Package.Memory[IapLen+3];
					}
					
					IAP_Count++;
				}
				else  //crc 错误
				{
					ErrorCount+=2;
					UART_SendByte(XR_NACK);
				}
			}
			else  //包号错误
			{
				ErrorCount+=2;
				UART_SendByte(XR_NACK);
			}
		}
		
	}
	if(ErrorCount>=25)  //错误太多，发送CAN
	{
		UART_SendByte(XR_CAN);
		UART_SendByte(XR_CAN);
		UART_SendByte(XR_CAN);
						
		XMClient.Package.Size=0;
		XMClient.State=XS_Error;
		XMClient.Phase=XP_PackEnd;;
		return XS_Fault;
	}	
	return XS_Comm;
}
void XmodemClient_ACK(void)
{
	UART_SendByte(XR_ACK);
}
void XmodemClient_ReceiveHandle(u8 Data)
{
	XMClient.Package.Memory[XMClient.Package.Count]=Data;
	XMClient.Package.Count++;
	XMClient.Package.Size++;
	
	if((XMClient.Package.Memory[0]==XR_CAN)||(XMClient.Package.Memory[0]==XR_EOT))
	{
		XMClient.Package.Count=0;
	}
	
	if(XMClient.Package.Count>=XMODERN_BUFFER_SIZE)
	{
		XMClient.Package.Count=0;
	}
}
//====================================================================================================
//====================================================================================================
void  IOT_Uart_Receiver(char Data)
{
    switch (System.Mode)
    {
        case sMIIO_Standby:
        case sMIIO_Ready:
        case sMIIO_Fault:
        MIIO_IAP_InData(Data);
        break;
        case sIAP:
        XmodemClient_ReceiveHandle(Data);
        break;
        case sAP:
        MIIO_IAP_InData(Data); //test
        break;
        default:
        MIIO_IAP_InData(Data);
        break;
    }		
}
//====================================================================================================
//====================================================================================================
void  IOT_Init(char Select)
{
        if(Select==0)
        { 
                MIIO_OTA_Init(sMIIO_Model);    //连接平台模式
        }
        else
        { 
                MIIO_OTA_Init(sMIIO_Standby);  //直接下载固件模式
        }	
}
//====================================================================================================
//====================================================================================================
IOT_State IOT_Work()
{
	static unsigned char Send_IOT_State=0;
	
	char   Net_State=IOT_NORMAL;
	
	if((u8)System.Mode>=sMIIO_Model)
	{
		if(((u8)System.Mode==sMIIO_Get_down)||(Send_IOT_State>32))
		{
			         if(System.TimeCount>=300)  //300mS
                    {
                        System.TimeCount=0;
                        UART_SendString("get_down\r");
						Send_IOT_State++;
						Send_IOT_State &=0x7F;
                    }
                    if(MIIO_IAP.State==MI_Rx)
                    {
                        MIIO_IAP.State=MI_Wait;
                        MIIO_IAP.Buffer.Count=0;
                        if((u8)System.Mode==sMIIO_Get_down)
						{
							if(CmpStr(&MIIO_IAP.Buffer.Memory,"down none\r"))
							{
								UART_SendString("result\r");
							}
							if(CmpStr(&MIIO_IAP.Buffer.Memory,"down unknown_command\r"))
							{
								UART_SendString("result\r");
							}
							if(CmpStr(&MIIO_IAP.Buffer.Memory,"down MIIO_mcu_version_req\r"))
							{
								UART_SendString("mcu_version 0001\r");
							}				
							if(CmpStr(&MIIO_IAP.Buffer.Memory,"down get_properties"))
							{
								UART_SendString("result\r");
							}
						}
                        if(CmpStr(&MIIO_IAP.Buffer.Memory,"down update_fw\r"))
                        {
                            Systime_Set(sMIIO_Standby, IAP_TIME_WINDOW); //设置固件升级命令
							
							Net_State=IOT_GET_FW;  //收到固件升级命令
                        }								
                    }
		}
		else
		{
			if(System.TimeCount>=300)  //300mS
           {
				System.TimeCount=0;
				Send_IOT_State++;
			   Send_IOT_State &=0x7F;
           }
			switch ((u8)System.Mode)
			{
				case sMIIO_Model:
                {
			
                    miio_result = MIIO_IAP_Cmd("model sasesi.srser.sas01\r", //
                    "ok",                 //
                    "error",              //
                    100,                  //
                    UART_SendString);    //
                    if (miio_result == MIR_R1)
                    {
                        Systime_Set(sMIIO_Ble_config, IAP_TIME_WINDOW);
                    }
                    break;
				}
				case sMIIO_Ble_config:
                {
                    miio_result = MIIO_IAP_Cmd("ble_config set 156 0001\r", //
                    "ok",                 //
                    "error",              //
                    100,                  //
                    UART_SendString);    //
                    if (miio_result == MIR_R1)
                    {
                        Systime_Set(sMIIO_Mcu_version, IAP_TIME_WINDOW);
                    }
                    break;
				}	
				case sMIIO_Mcu_version:
                {
                    miio_result = MIIO_IAP_Cmd("mcu_version 0001\r", //
                    "ok",                 //
                    "error",              //
                    100,                  //
                    UART_SendString);    //
                    if (miio_result == MIR_R1)
                    {
                        Systime_Set(sMIIO_Get_down, IAP_TIME_WINDOW);
                        MIIO_IAP.Time.Count=0;
                    }
                    break;
				}	
				default:
					System.Mode = sMIIO_Model;
					break;
			}				
		}
	}
	return  Net_State;
}
//====================================================================================================
//====================================================================================================
void OTA_FirmwareDownload(IAPDATA_INFO *IAP_Pack)
{
        switch ((u8)System.Mode)
        {
            case sMIIO_Standby:    //发送升级请求
                miio_result = MIIO_IAP_Cmd("get_down\r",     //
                "down update_fw", //
                "down none",      //
                100,              //
                UART_SendString);
                if (miio_result == MIR_R1) // MIIO need update
                {
                    Systime_Set(sMIIO_Ready, IAP_TIME_BOOT);
                }
                break;
            case sMIIO_Ready:     //应答IOT平台，准备接收固件
                miio_result = MIIO_IAP_Cmd("result \"ready\"\r", //
                "ok",                 //
                "error",              //
                100,                  //
                UART_SendString);    //
                if (miio_result == MIR_R1)
                {
                    Systime_Set(sIAP, IAP_TIME_BOOT);
                }
                break;
            case sIAP:       //下载固件数据
                System.TimeCount=0;
				
				if((IAP_Pack->State==OTA_IAPWRITED)&&(IAP_Pack->Size==0)) //完成一包数据接收,且写入IAP完成
				{
					XmodemClient_ACK();            //回应IOT平台ACK,准备接收下一包数据
					IAP_Pack->State=OTA_READY;
				}
				if(IAP_Pack->State==OTA_FAULT)      //IAP写入出错
				{
					IAP_Pack->State=OTA_READY;
					XS_Result = XS_Fault;
				}
			
			
                XS_Result = XmodemClient_Proess(IAP_Pack);//(&IAP_Pack);  //下载固件，存放于IAP_Pack
    
                if (XS_Result == XS_Finish)        //下载进程结束
                {
                        if(FileCrcerror==0)					
						{
							AppCompleteCheck();													//下载成功，检查代码版本并copy到APP区
							IAP_Pack->State=OTA_COMPLETE;
					        XmodemClient_ACK();
						}
						else
						{
							IAP_Pack->State=OTA_FAULT;
							MIIO_IAP.ReSendCount = 0;
							Systime_Set(sMIIO_Fault, IAP_TIME_BOOTFIAL);
							XmodemClient_ACK();
						}					
                }
                else if (XS_Result == XS_Fault)
                {
                    MIIO_IAP.ReSendCount = 0;
                    Systime_Set(sMIIO_Fault, IAP_TIME_BOOTFIAL);
                }
                else
                {
                    Systime_Set(sIAP, IAP_TIME_BOOT);
                }
                break;
            case sAP:   //跳转到运行区运行
                {	
                break;
            }
            case sMIIO_Fault:  //出错处理
                {
                    unsigned char i;
                    for(i=0;i<10;i++)
                    {
                        UART_SendString("mcu_boot fail\r");
                        System.TimeCount=0;
                        while(System.TimeCount<200)
                        {
                            WDTCON |=0X10;
                        }
                    }
                    System.Mode = sMIIO_Standby;					
                    break;
            }
            default:
             
                break;
        }		
}
//====================================================================================================
//====================================================================================================
void Systime_Count()
{
	System.TimeCount++;
	XMClient.Time.Count++;
	MIIO_IAP.Time.Count++;
}
//====================================================================================================
//====================================================================================================
void Systime_Set(SystemMode_t mode, u32 TimeLimit)
{
    System.Mode      = mode;
    System.TimeCount = 0;
    System.TimeLimit = TimeLimit;
    MIIO_IAP.State=MI_Wait;
}
//====================================================================================================
//====================================================================================================
void MIIO_OTA_Init(SystemMode_t mode)
{
	XmodemClient_ModeInit(XM_CRC,XMClient_Buffer);
    Systime_Set(mode, IAP_TIME_WINDOW);	
}
//====================================================================================================
//====================================================================================================
