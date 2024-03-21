//************************************************************
//  Copyright (c) 深圳市赛元微电子股份有限公司
//	文件名称:		OTADriver.c
//	作者:				赛元OTA应用团队
//	模块功能:		OTA库函数C文件
//  最后更正日期:2023年5月10日
// 	版本: 			V1.0.0
//  说明:				用于实现OTA的bootlaod功能
//*************************************************************

#include"..\H\OTADriver.h"
#include"SC_Init.h"
#include "..\H\Decrypt_TEA.H"
#include "..\H\OTADriver_Statement.H"
#include"..\H\OTADriver_IAP.h"

/*<DriverVarStart>*/

typedef enum
{
	NORMAL,     //APP正常
	DOWNLOADED, //下载完成
	COPYING,    //拷贝
    OTAING,     //OTA下载中
} APPSPACE_STATE;

#if(Encryption == 1)//如果数据加密了在此进行解密
unsigned int code Key_TEA[4] = 
{
	(EncryptionKeyH>>16)&0xffff,
	(EncryptionKeyH)&0xffff,
	(EncryptionKeyL>>16)&0xffff,
	(EncryptionKeyL)&0xffff,
};
#endif


/* 使用串口定义 */
#if (UartSelect==0)
#define SendData(UartData) UART0_SendData8((uint8_t)UartData)
#define ReceiveData() UART0_ReceiveData8()
#define READ_RI UART0_GetFlagStatus(UART0_FLAG_RI)
#define CLEAR_RI UART0_ClearFlag(UART0_FLAG_RI)
#define READ_TI UART0_GetFlagStatus(UART0_FLAG_TI)
#define CLEAR_TI UART0_ClearFlag(UART0_FLAG_TI)
#define UART_INT_NUMBER 4
#endif

#if (UartSelect==1)
#define SendData(UartData) USCI0_UART_SendData8(UartData)
#define ReceiveData() USCI0_UART_ReceiveData8()
#define READ_RI USCI0_GetFlagStatus(USCI0_UART_FLAG_RI)
#define CLEAR_RI USCI0_ClearFlag(USCI0_UART_FLAG_RI)
#define READ_TI USCI0_GetFlagStatus(USCI0_UART_FLAG_TI)
#define CLEAR_TI USCI0_ClearFlag(USCI0_UART_FLAG_TI)
#define UART_INT_NUMBER 14
#endif

#if (UartSelect==2)
#define SendData(UartData) USCI1_UART_SendData8(UartData)
#define ReceiveData() USCI1_UART_ReceiveData8()
#define READ_RI USCI1_GetFlagStatus(USCI1_UART_FLAG_RI)
#define CLEAR_RI USCI1_ClearFlag(USCI1_UART_FLAG_RI)
#define READ_TI USCI1_GetFlagStatus(USCI1_UART_FLAG_TI)
#define CLEAR_TI USCI1_ClearFlag(USCI1_UART_FLAG_TI)
#define UART_INT_NUMBER 15
#endif

#if (UartSelect==3)
#define SendData(UartData) USCI2_UART_SendData8(UartData)
#define ReceiveData() USCI2_UART_ReceiveData8()
#define READ_RI USCI2_GetFlagStatus(USCI2_UART_FLAG_RI)
#define CLEAR_RI USCI2_ClearFlag(USCI2_UART_FLAG_RI)
#define READ_TI USCI2_GetFlagStatus(USCI2_UART_FLAG_TI)
#define CLEAR_TI USCI2_ClearFlag(USCI2_UART_FLAG_TI)
#define UART_INT_NUMBER 16
#endif

#if (UartSelect==4)
#define SendData(UartData) USCI3_UART_SendData8(UartData)
#define ReceiveData() USCI3_UART_ReceiveData8()
#define READ_RI USCI3_GetFlagStatus(USCI3_UART_FLAG_RI)
#define CLEAR_RI USCI3_ClearFlag(USCI3_UART_FLAG_RI)
#define READ_TI USCI3_GetFlagStatus(USCI3_UART_FLAG_TI)
#define CLEAR_TI USCI3_ClearFlag(USCI3_UART_FLAG_TI)
#define UART_INT_NUMBER 17
#endif

#if (UartSelect==5)
#define SendData(UartData) USCI4_UART_SendData8(UartData)
#define ReceiveData() USCI4_UART_ReceiveData8()
#define READ_RI USCI4_GetFlagStatus(USCI4_UART_FLAG_RI)
#define CLEAR_RI USCI4_ClearFlag(USCI4_UART_FLAG_RI)
#define READ_TI USCI4_GetFlagStatus(USCI4_UART_FLAG_TI)
#define CLEAR_TI USCI4_ClearFlag(USCI4_UART_FLAG_TI)
#define UART_INT_NUMBER 18
#endif



unsigned char data InterruputMode _at_ 0x20;

void (*UartReceive)(char Data);
unsigned char BOOT_WaitTime=0;

bit	 UartSeneFlag=0;
unsigned char APP_UPDATA_FLAG_ARRAY[] = {APP_UPDATA_FLAG_NUMBER};

unsigned char Option1Buff;
unsigned char Option2Buff;
APPMemoryInfoStruct RUN_APPMemoryInfoData;
APPMemoryInfoStruct BACKUPS_APPMemoryInfoData;

IAPDATA_INFO  IAP_Pack;
unsigned long  IAP_Checksum=0;
unsigned long IAP_Size=0;



//====================================================
void ReadAPPMemoryInfo();
void ReadBACKSMemoryInfo();
void IAP_Reset(unsigned char mode);
/*****************************************************
*函数名称: void OTA_Init(void)
*函数功能: OTA初始化，保存APP Option、EA信息
*入口参数：
*出口参数：void 
*****************************************************/
void OTA_Init(void UartReceiveHandle(char Data),char *UartReceiveBuf)
{
#if(PROJECT_SWITCH==1)
	{
		char temp=UpdateSucceededFlagNumber[0];  //避免UpdateSucceededFlagNumber被优化
		temp +=(char)VersionDate;                //避免VersionDate被优化
	}
#endif	
	IAP_Pack.State =OTA_READY;
    IAP_Pack.PacketNumber=0;
    IAP_Pack.Size=0;
    IAP_Pack.Memory=UartReceiveBuf;
	
	UartReceive=UartReceiveHandle;
}
/**************************************************
*函数名称：OTA_JumpApp
*函数功能：修改跳转标志位，并且开启
*入口参数：void
*出口参数：void
**************************************************/
void OTA_JumpApp(void)
{
#if LDROM_MODE
  IAP_Reset(APP);					//软件复位
#else
	(*(void(code *)(void))(RUN_APP_STARTUP_ADDRESS))();  //跳转到APP程序入口
#endif	
}

/**************************************************
*函数名称：OTA_JumpBoot
*函数功能：替换到Boot函数
*入口参数：void
*出口参数：void
**************************************************/
void OTA_JumpBoot(void)
{
#if LDROM_MODE
  IAP_Reset(BOOT);					//软件复位
#else
  (*(void(code *)(void))(0x0000))();  //跳转到BOOT程序入口
#endif	
}

/**************************************************
*函数名称：IAP_Reset
*函数功能：软复位
*入口参数：mode ：BOOT：复位执行LDROM程序 APP：复位执行APP程序
*出口参数：void  
**************************************************/
void IAP_Reset(unsigned char mode)
{
    
	IAPKEY = 0XF0;	//使能IAP
	if(mode==BOOT)
	{
		IAPADE = 0X03;   //MOVC 针对LDROM
		IAPCTL |= 0x80;	//BTLD控制位置起，下次复位进入LDROM
	}
	else
	{	
		IAPADE = 0X00;   //MOVC 针对APROM
		IAPCTL &= ~0x80;//BTLD控制位清0，下次复位后进入APROM
	}
	PCON |= 0X08;	//软件复位
	_nop_();		//等待8个_nop_()
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
/*****************************************************
*函数名称: void  SetInterruptApp()
*函数功能: 设置中断为APP中断功能
*入口参数：
*出口参数：
*****************************************************/
void  SetInterruptAPP()
{
	InterruputMode=0xFF;
}
/*****************************************************
*函数名称: SetInterruptBoot()
*函数功能: 设置中断为BOOTLOADER中断功能
*入口参数：
*出口参数：
*****************************************************/
void  SetInterruptBOOT()
{
	InterruputMode=0x00;
}
/*****************************************************
*函数名称:void UART_SendData8(uint8_t Data)
*函数功能:UART发送8位数据
*入口参数:uint8_t:Data:发送的数据
*出口参数:void
*****************************************************/
void UART_SendByte(uint8_t UartData)
{
	unsigned int i=0;
	UartSeneFlag=0;
	SendData(UartData);
	while(!UartSeneFlag)
	{
		if(2000<i++)
		{
			WDTCON |=0X10;
			break;
		}
	}
}
/*****************************************************
*函数名称:void UART_SendString(char *str)
*函数功能:UART发送字符串
*入口参数:char *str:发送的数据
*出口参数:void
*****************************************************/
void UART_SendString(char *str)
{
    while (*str != '\0')
    {
		UART_SendByte(*str++);			
    }
}
/**************************************************
*函数名称：Uart_Handle
*函数功能：uart中断处理
*入口参数：
*出口参数：void  
**************************************************/
void Uart_Handle(void)
{
	char Data;
	if(READ_TI)
	{
		CLEAR_TI;
		UartSeneFlag=1;
	}
	if(READ_RI)
	{
		CLEAR_RI;
		Data=ReceiveData();
		UartReceive(Data);
	}

}
/*****************************************************
*函数名称: unsigned char SaveOTA_to_BacksArea(IAPDATA_INFO IAP_Pack)
*函数功能: 将OTA数据写入备份区，并返回成功写入字节数
*入口参数：OTA数据包
*出口参数：写入到备份区字节数 
*****************************************************/
unsigned int SaveOTA_to_BacksArea(IAPDATA_INFO *IAP_Pack)
{
	uint16_t  IapLen=0;
		if(IAP_Pack->PacketNumber==0)   //首包数据，擦除备份区
		{
			EraseSpace(BACKUPS_APP_BEGIN_ADDRESS,BACKUPS_APP_END_ADDRESS);	//擦除备份区域
			IAP_Size=0;
			IAP_Checksum=0;			
		}
		IapLen=IAP_Pack->Size;

		if((((IAP_Pack->PacketNumber)*(IAP_Pack->Size))>=RUN_APP_BEGIN_ADDRESS)&&(((IAP_Pack->PacketNumber)*(IAP_Pack->Size))<RUN_APP_END_ADDRESS))  //仅写入有效区域
		{
			{
			#if(Encryption == 1)//如果数据加密了在此进行解密
				unsigned char i,k;
				for(k=0;k<IapLen/4;k++)
				{
					for(i=0;i<4;i++) //将待解密的值存入TeaChunck中
					{
						TeaChunck.Chunk8bit[i] = IAP_Pack->Memory[i+k*4];	
					}
					decrypt();	//解密
					for(i=0;i<4;i++) //从TeaChunck中取得解密的结果
					{
						IAP_Pack->Memory[i+k*4] = TeaChunck.Chunk8bit[i];
					}
				}
			#endif			
			}				
			if(((IAP_Pack->PacketNumber)*(IAP_Pack->Size))==(RUN_APP_END_ADDRESS-(IAP_Pack->Size)))
			{
				IapLen -=0x10;         //运行信息区不能写入
			}		
			if(WriteBackupsAppData(((IAP_Pack->PacketNumber)*(IAP_Pack->Size)-RUN_APP_BEGIN_ADDRESS),IAP_Pack->Memory,IapLen))//写入备份区
			{
				IAP_Checksum=CheckSum32(IAP_Checksum,IAP_Pack->Memory,IapLen); //计算固件包的Check
				IAP_Size +=IapLen;
			}
			else
			{
				IapLen=0;
			}
		}
	return IapLen;
}
/*****************************************************
*函数名称: void BootInit(void)
*函数功能: BootLoad初始化，保存APP Option、EA信息
*入口参数：
*出口参数：void 
*****************************************************/
void BootLoadInit()
{
	OPINX = 0xC2;
	Option2Buff = OPREG;       //将C2的寄存器值保存
	OPREG |= 0X0C;
	
	OPINX = 0xC1;
	Option1Buff = OPREG;	   //将C1的寄存器值保存
	
	//定时500MS BTM
    BTMCON = 0X85;	
}
/*****************************************************
*函数名称: void BootLoad_DeInit(void)
*函数功能: 恢复APP Option、EA信息
*入口参数：
*出口参数：void 
*****************************************************/
void BootLoad_DeInit()
{
	OPINX = 0xC2;
	OPREG=Option2Buff;       //恢复C2的寄存器
	
	OPINX = 0xC1;
	OPREG=Option1Buff ;	   //恢复C1的寄存器
}


/*****************************************************
*函数名称: void EraseSpace(unsigned long BeginAddr,unsigned long EndAddr)
*函数功能: 清除空间
*入口参数：BeginAddr 清除起始地址，EndAddr 清除结束地址
*出口参数：void 
*****************************************************/
void EraseSpace(unsigned long BeginAddr,unsigned long EndAddr)
{
	for(; BeginAddr < EndAddr; BeginAddr += 512)
	{
	   WDTCON |=0X10;
	   IAP_Erase_512B(BeginAddr);
	}
}

/*****************************************************
*函数名称: APPMemoryInfoStruct ReadAPPMemoryInfo()
*函数功能: 读取运行区信息
*入口参数：
*出口参数：
*****************************************************/
void ReadAPPMemoryInfo()
{
	IAP_ReadNByte(RUN_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &RUN_APPMemoryInfoData);
}
/*****************************************************
*函数名称: APPMemoryInfoStruct ReadAPPMemoryInfo()
*函数功能: 读取备份区信息
*入口参数：
*出口参数：
*****************************************************/
void ReadBACKSMemoryInfo()
{
	IAP_ReadNByte(BACKUPS_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &BACKUPS_APPMemoryInfoData);
}
///*****************************************************
//*函数名称: unsigned long ReadAppDate(char region)
//*函数功能: 读APP版本日期
//*入口参数：
//*出口参数：unsigned long：日期
//*****************************************************/
unsigned long ReadAppDate(char region)
{
	unsigned long Date;
	if(region)
	{
		IAP_ReadNByte(BACKUPS_APP_INFO_ADDRESS, sizeof(unsigned long), &Date);
	}
	else
	{
		IAP_ReadNByte(RUN_APP_INFO_ADDRESS, sizeof(unsigned long), &Date);
	}
	
	return Date;
}
/*****************************************************
*函数名称: unsigned char WriteAPPMemoryInfo(char region,APPMemoryInfoStruct APPMemoryInfo)
*函数功能: 写备份区存储区信息
*入口参数：APPMemoryInfo:存储区信息
*出口参数：
*****************************************************/
unsigned char WriteAPPMemoryInfo(APPMemoryInfoStruct APPMemoryInfo)
{
		IAP_WriteNByte(BACKUPS_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &APPMemoryInfo);
	return 1;	
}

/*****************************************************
*函数名称: unsigned char WriteBackupsAppData(unsigned int OffsetAddress, unsigned char *Data, unsigned int Length)
*函数功能: 写APP数据
*入口参数：OffsetAddress：偏移地址，Data ：数据， Length：数据长度
*出口参数：
*****************************************************/
unsigned char WriteBackupsAppData(unsigned int OffsetAddress, unsigned char *Data, unsigned int Length)
{

    if(OffsetAddress + Length > APP_SPACE_SIZE - 0x10)
	return 0;
	
	if(IAP_WriteNByte((unsigned long)BACKUPS_APP_BEGIN_ADDRESS + OffsetAddress ,Length, Data) == 0)
		return 0;
	
	return 1;
}

/*****************************************************
*函数名称: unsigned char AppUpdataFlagCheck(char region)
*函数功能: APP更新完成标志检查
*入口参数：region 0：读取运行区信息 1：读取备份区信息
*出口参数：unsigned char 1：更新完成 
*****************************************************/
unsigned char AppUpdataFlagCheck(char region)
{
	unsigned char i;
    unsigned long CheckAddr=RUN_APP_MEMORY_INFO_ADRESS;
	
	if(region)
	{
		CheckAddr=BACKUPS_APP_MEMORY_INFO_ADRESS;
	}
	
	 
		for(i=0;i<sizeof(APP_UPDATA_FLAG_ARRAY);i++)
		{
			if(APP_UPDATA_FLAG_ARRAY[i]!=IAP_ReadOneByte(CheckAddr+i))
				return 0;
		}

	return 1;
}

/*****************************************************
*函数名称: unsigned char AppCompleteCheck(char region)
*函数功能: 检查代码完整
*入口参数：
*出口参数：unsigned char 1：代码完整
*****************************************************/
unsigned char AppCompleteCheck()
{
	APPSPACE_STATE RunState=OTAING;

	unsigned long AppDate,BackData;
	
	AppDate=ReadAppDate(0);   //读取运行区版本日期信息
	
	if(((AppDate&0xF0000000)>0x90000000)||
	   ((AppDate&0x0F000000)>0x09000000)||
	   ((AppDate&0x00F00000)>0x00900000)||
	   ((AppDate&0x000F0000)>0x00090000)||
	   ((AppDate&0x0000F000)>0x00009000)||
	   ((AppDate&0x00000F00)>0x00000900)||
	   ((AppDate&0x000000F0)>0x00000090)||
	   ((AppDate&0x0000000F)>0x00000009))
	{
		AppDate=0;    //版本日期格式非法
	}
	
	BackData=ReadAppDate(1); //读取备份区版本日期信息

	ReadBACKSMemoryInfo(); //读取备份区信息
	ReadAPPMemoryInfo();  //读取运行区信息

	
	if((BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[0]==APP_UPDATA_FLAG_ARRAY[0])&&
		(BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[1]==APP_UPDATA_FLAG_ARRAY[1])&&
		(BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[2]==APP_UPDATA_FLAG_ARRAY[2])&&
		(BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[3]==APP_UPDATA_FLAG_ARRAY[3]))    //备份区代码是完整
	{
		RunState=DOWNLOADED;  //备份区代码已下载有固件
	}
	else
	{
		RunState=OTAING;  //待OTA下载
	}
	
	do{
		if(RunState==DOWNLOADED) //固件已下载完成
		{
			if(BackData>AppDate)   //有更新版本,更新程序
			{
				RunState=COPYING;  
				break;
			}
			#if FORCE_UPDATA    //强制更新程序
				  RunState=COPYING;
				  break;
			#endif					
		}
		if((RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[0]==APP_UPDATA_FLAG_ARRAY[0])&&
		  (RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[1]==APP_UPDATA_FLAG_ARRAY[1])&&
		  (RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[2]==APP_UPDATA_FLAG_ARRAY[2])&&
		  (RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[3]==APP_UPDATA_FLAG_ARRAY[3]))    //APP区代码是完整
	    {  
			RunState=NORMAL;
		}
		else
		{
			if(RunState==DOWNLOADED)        //APP区代码不完整，且备份区代码完整
			{
				RunState=COPYING;//更新程序
			}
		}
	}while(0);
//	
	if(RunState==NORMAL)         //不需要升级代码,到APP运行程序
	{
		return 1;
	}
	if(RunState==COPYING)        //将备份区代码拷贝到运行区
	{
		if(BackupsToRunCopy())   //第一次COPY
		{
			return 1;
		}
		if(BackupsToRunCopy())   //第一次COPY不成功,再次COPY
		{
			return 1;
		}		
	}
	return	0;
}
/*****************************************************
*函数名称: unsigned char IAP_Update_Finish(unsigned char Crc)
*函数功能: 检查OTA结果
*入口参数：
*出口参数：0 ：失败 1：成功
*****************************************************/
unsigned char IAP_BacksArea_CrcCheck()
{
	APPMemoryInfoStruct APPMemoryInfoTemp;
	unsigned long crc=0;
	unsigned long i;
	unsigned char Data;
	for(i=0;i<IAP_Size;i++)
	{
		WDTCON |=0X10;
		Data=IAP_ReadOneByte(BACKUPS_APP_BEGIN_ADDRESS+i);
		crc=CheckSum32(crc, &Data, 1);
	}

	if(crc==IAP_Checksum)
	{	
		APPMemoryInfoTemp.APPSize = IAP_Size;
		APPMemoryInfoTemp.APPChecksums=IAP_Checksum;
		//设置更新完成标志
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[0] = APP_UPDATA_FLAG_ARRAY[0];
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[1] = APP_UPDATA_FLAG_ARRAY[1];
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[2] = APP_UPDATA_FLAG_ARRAY[2];
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[3] = APP_UPDATA_FLAG_ARRAY[3];
		APPMemoryInfoTemp.InitFlag = INITFLAG; //标志不是原始APP
		//写入存储区信息
		WriteAPPMemoryInfo(APPMemoryInfoTemp);			
		return 1;
	}
	
	return 0;
}
/*****************************************************
*函数名称: unsigned char IAP_Check_APFLASH()
*函数功能: 判断运行区APP是否为最开始的APP,如果不是则检查程序的完备性
*入口参数：Crc 0：上电不对APP区进行CRC校验； 1：上电对APP区进行CRC校验，上电引导时间变长
*出口参数：0 ：不是 1：是
*****************************************************/
unsigned char IAP_Check_APFLASH(unsigned char Crc)
{
	unsigned long Iap_crc=0;
	unsigned long i;
	unsigned char Data;
	
	ReadAPPMemoryInfo();	//读取运行区信息
	
	if((RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[0]==APP_UPDATA_FLAG_ARRAY[0])&&
		(RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[1]==APP_UPDATA_FLAG_ARRAY[1])&&
		(RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[2]==APP_UPDATA_FLAG_ARRAY[2])&&
		(RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[3]==APP_UPDATA_FLAG_ARRAY[3]))    //代码是完整
	{
		if(Crc==0)
		{
			return 1;                              //不检查CRC
		}
		if(RUN_APPMemoryInfoData.InitFlag != INITFLAG) //APP是否为出厂的APP
		{
			return 1;                              //出厂APP不检查CRC
		}
		else
		{
			for(i=0;i<RUN_APPMemoryInfoData.APPSize;i++)
			{
				WDTCON |=0X10;
				Data=IAP_ReadOneByte(RUN_APP_BEGIN_ADDRESS+i);
				Iap_crc=CheckSum32(Iap_crc, &Data, 1);
			}
			if(Iap_crc==(RUN_APPMemoryInfoData.APPChecksums&0xFFFFFFFF)) //升级后程序的CRC是正确的
			{
				return 1;
			}
		}
	}
	return	0;
}
/*****************************************************
*函数名称: unsigned char BackupsToRunCopy()
*函数功能: 将备份区代码拷贝到运行区
*入口参数：
*出口参数：0 ：拷贝失败 1：拷贝成功
*****************************************************/
unsigned char BackupsToRunCopy()
{
	unsigned int i = 0;
	unsigned char BackupsCode = 0;

	 ReadBACKSMemoryInfo();//读取备份区信息

	//清除代码区
	EraseSpace(RUN_APP_BEGIN_ADDRESS,RUN_APP_END_ADDRESS);
//	
	for(i=0;i<BACKUPS_APPMemoryInfoData.APPSize;i++)
	{
		WDTCON |=0X10;
		BackupsCode = IAP_ReadOneByte(BACKUPS_APP_BEGIN_ADDRESS + i);
		if(!IAP_WriteOneByte(RUN_APP_BEGIN_ADDRESS + i,BackupsCode))
		{
			return 0;
		}
	}
	IAP_WriteNByte(RUN_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &BACKUPS_APPMemoryInfoData);  //更新APP区信息
	
			#if FORCE_UPDATA    //强制更新程序
	            IAP_Erase_512B(BACKUPS_APP_MEMORY_INFO_ADRESS);
			#endif				
	return 1;
}

/******************************************************************************
 * Name:    Check-32
 * Poly:   
 * Init:    
 * Refin:   
 * Refout:  
 * Xorout:  
 * Alias:   
 *****************************************************************************/
unsigned long CheckSum32(uint32_t cs, u8 *Pdata, uint32_t length)
{
    uint32_t i;
    for (i = 0; i < length; i++)
    {
        cs += Pdata[i];
    }

    return cs;
}
//#endif

/*<DriverFunEnd>*/
