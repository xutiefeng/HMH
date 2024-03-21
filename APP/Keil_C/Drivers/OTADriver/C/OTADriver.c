//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����:		OTADriver.c
//	����:				��ԪOTAӦ���Ŷ�
//	ģ�鹦��:		OTA�⺯��C�ļ�
//  ����������:2023��5��10��
// 	�汾: 			V1.0.0
//  ˵��:				����ʵ��OTA��bootlaod����
//*************************************************************

#include"..\H\OTADriver.h"
#include"SC_Init.h"
#include "..\H\Decrypt_TEA.H"
#include "..\H\OTADriver_Statement.H"
#include"..\H\OTADriver_IAP.h"

/*<DriverVarStart>*/

typedef enum
{
	NORMAL,     //APP����
	DOWNLOADED, //�������
	COPYING,    //����
    OTAING,     //OTA������
} APPSPACE_STATE;

#if(Encryption == 1)//������ݼ������ڴ˽��н���
unsigned int code Key_TEA[4] = 
{
	(EncryptionKeyH>>16)&0xffff,
	(EncryptionKeyH)&0xffff,
	(EncryptionKeyL>>16)&0xffff,
	(EncryptionKeyL)&0xffff,
};
#endif


/* ʹ�ô��ڶ��� */
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
*��������: void OTA_Init(void)
*��������: OTA��ʼ��������APP Option��EA��Ϣ
*��ڲ�����
*���ڲ�����void 
*****************************************************/
void OTA_Init(void UartReceiveHandle(char Data),char *UartReceiveBuf)
{
#if(PROJECT_SWITCH==1)
	{
		char temp=UpdateSucceededFlagNumber[0];  //����UpdateSucceededFlagNumber���Ż�
		temp +=(char)VersionDate;                //����VersionDate���Ż�
	}
#endif	
	IAP_Pack.State =OTA_READY;
    IAP_Pack.PacketNumber=0;
    IAP_Pack.Size=0;
    IAP_Pack.Memory=UartReceiveBuf;
	
	UartReceive=UartReceiveHandle;
}
/**************************************************
*�������ƣ�OTA_JumpApp
*�������ܣ��޸���ת��־λ�����ҿ���
*��ڲ�����void
*���ڲ�����void
**************************************************/
void OTA_JumpApp(void)
{
#if LDROM_MODE
  IAP_Reset(APP);					//�����λ
#else
	(*(void(code *)(void))(RUN_APP_STARTUP_ADDRESS))();  //��ת��APP�������
#endif	
}

/**************************************************
*�������ƣ�OTA_JumpBoot
*�������ܣ��滻��Boot����
*��ڲ�����void
*���ڲ�����void
**************************************************/
void OTA_JumpBoot(void)
{
#if LDROM_MODE
  IAP_Reset(BOOT);					//�����λ
#else
  (*(void(code *)(void))(0x0000))();  //��ת��BOOT�������
#endif	
}

/**************************************************
*�������ƣ�IAP_Reset
*�������ܣ���λ
*��ڲ�����mode ��BOOT����λִ��LDROM���� APP����λִ��APP����
*���ڲ�����void  
**************************************************/
void IAP_Reset(unsigned char mode)
{
    
	IAPKEY = 0XF0;	//ʹ��IAP
	if(mode==BOOT)
	{
		IAPADE = 0X03;   //MOVC ���LDROM
		IAPCTL |= 0x80;	//BTLD����λ�����´θ�λ����LDROM
	}
	else
	{	
		IAPADE = 0X00;   //MOVC ���APROM
		IAPCTL &= ~0x80;//BTLD����λ��0���´θ�λ�����APROM
	}
	PCON |= 0X08;	//�����λ
	_nop_();		//�ȴ�8��_nop_()
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
*��������: void  SetInterruptApp()
*��������: �����ж�ΪAPP�жϹ���
*��ڲ�����
*���ڲ�����
*****************************************************/
void  SetInterruptAPP()
{
	InterruputMode=0xFF;
}
/*****************************************************
*��������: SetInterruptBoot()
*��������: �����ж�ΪBOOTLOADER�жϹ���
*��ڲ�����
*���ڲ�����
*****************************************************/
void  SetInterruptBOOT()
{
	InterruputMode=0x00;
}
/*****************************************************
*��������:void UART_SendData8(uint8_t Data)
*��������:UART����8λ����
*��ڲ���:uint8_t:Data:���͵�����
*���ڲ���:void
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
*��������:void UART_SendString(char *str)
*��������:UART�����ַ���
*��ڲ���:char *str:���͵�����
*���ڲ���:void
*****************************************************/
void UART_SendString(char *str)
{
    while (*str != '\0')
    {
		UART_SendByte(*str++);			
    }
}
/**************************************************
*�������ƣ�Uart_Handle
*�������ܣ�uart�жϴ���
*��ڲ�����
*���ڲ�����void  
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
*��������: unsigned char SaveOTA_to_BacksArea(IAPDATA_INFO IAP_Pack)
*��������: ��OTA����д�뱸�����������سɹ�д���ֽ���
*��ڲ�����OTA���ݰ�
*���ڲ�����д�뵽�������ֽ��� 
*****************************************************/
unsigned int SaveOTA_to_BacksArea(IAPDATA_INFO *IAP_Pack)
{
	uint16_t  IapLen=0;
		if(IAP_Pack->PacketNumber==0)   //�װ����ݣ�����������
		{
			EraseSpace(BACKUPS_APP_BEGIN_ADDRESS,BACKUPS_APP_END_ADDRESS);	//������������
			IAP_Size=0;
			IAP_Checksum=0;			
		}
		IapLen=IAP_Pack->Size;

		if((((IAP_Pack->PacketNumber)*(IAP_Pack->Size))>=RUN_APP_BEGIN_ADDRESS)&&(((IAP_Pack->PacketNumber)*(IAP_Pack->Size))<RUN_APP_END_ADDRESS))  //��д����Ч����
		{
			{
			#if(Encryption == 1)//������ݼ������ڴ˽��н���
				unsigned char i,k;
				for(k=0;k<IapLen/4;k++)
				{
					for(i=0;i<4;i++) //�������ܵ�ֵ����TeaChunck��
					{
						TeaChunck.Chunk8bit[i] = IAP_Pack->Memory[i+k*4];	
					}
					decrypt();	//����
					for(i=0;i<4;i++) //��TeaChunck��ȡ�ý��ܵĽ��
					{
						IAP_Pack->Memory[i+k*4] = TeaChunck.Chunk8bit[i];
					}
				}
			#endif			
			}				
			if(((IAP_Pack->PacketNumber)*(IAP_Pack->Size))==(RUN_APP_END_ADDRESS-(IAP_Pack->Size)))
			{
				IapLen -=0x10;         //������Ϣ������д��
			}		
			if(WriteBackupsAppData(((IAP_Pack->PacketNumber)*(IAP_Pack->Size)-RUN_APP_BEGIN_ADDRESS),IAP_Pack->Memory,IapLen))//д�뱸����
			{
				IAP_Checksum=CheckSum32(IAP_Checksum,IAP_Pack->Memory,IapLen); //����̼�����Check
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
*��������: void BootInit(void)
*��������: BootLoad��ʼ��������APP Option��EA��Ϣ
*��ڲ�����
*���ڲ�����void 
*****************************************************/
void BootLoadInit()
{
	OPINX = 0xC2;
	Option2Buff = OPREG;       //��C2�ļĴ���ֵ����
	OPREG |= 0X0C;
	
	OPINX = 0xC1;
	Option1Buff = OPREG;	   //��C1�ļĴ���ֵ����
	
	//��ʱ500MS BTM
    BTMCON = 0X85;	
}
/*****************************************************
*��������: void BootLoad_DeInit(void)
*��������: �ָ�APP Option��EA��Ϣ
*��ڲ�����
*���ڲ�����void 
*****************************************************/
void BootLoad_DeInit()
{
	OPINX = 0xC2;
	OPREG=Option2Buff;       //�ָ�C2�ļĴ���
	
	OPINX = 0xC1;
	OPREG=Option1Buff ;	   //�ָ�C1�ļĴ���
}


/*****************************************************
*��������: void EraseSpace(unsigned long BeginAddr,unsigned long EndAddr)
*��������: ����ռ�
*��ڲ�����BeginAddr �����ʼ��ַ��EndAddr ���������ַ
*���ڲ�����void 
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
*��������: APPMemoryInfoStruct ReadAPPMemoryInfo()
*��������: ��ȡ��������Ϣ
*��ڲ�����
*���ڲ�����
*****************************************************/
void ReadAPPMemoryInfo()
{
	IAP_ReadNByte(RUN_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &RUN_APPMemoryInfoData);
}
/*****************************************************
*��������: APPMemoryInfoStruct ReadAPPMemoryInfo()
*��������: ��ȡ��������Ϣ
*��ڲ�����
*���ڲ�����
*****************************************************/
void ReadBACKSMemoryInfo()
{
	IAP_ReadNByte(BACKUPS_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &BACKUPS_APPMemoryInfoData);
}
///*****************************************************
//*��������: unsigned long ReadAppDate(char region)
//*��������: ��APP�汾����
//*��ڲ�����
//*���ڲ�����unsigned long������
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
*��������: unsigned char WriteAPPMemoryInfo(char region,APPMemoryInfoStruct APPMemoryInfo)
*��������: д�������洢����Ϣ
*��ڲ�����APPMemoryInfo:�洢����Ϣ
*���ڲ�����
*****************************************************/
unsigned char WriteAPPMemoryInfo(APPMemoryInfoStruct APPMemoryInfo)
{
		IAP_WriteNByte(BACKUPS_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &APPMemoryInfo);
	return 1;	
}

/*****************************************************
*��������: unsigned char WriteBackupsAppData(unsigned int OffsetAddress, unsigned char *Data, unsigned int Length)
*��������: дAPP����
*��ڲ�����OffsetAddress��ƫ�Ƶ�ַ��Data �����ݣ� Length�����ݳ���
*���ڲ�����
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
*��������: unsigned char AppUpdataFlagCheck(char region)
*��������: APP������ɱ�־���
*��ڲ�����region 0����ȡ��������Ϣ 1����ȡ��������Ϣ
*���ڲ�����unsigned char 1��������� 
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
*��������: unsigned char AppCompleteCheck(char region)
*��������: ����������
*��ڲ�����
*���ڲ�����unsigned char 1����������
*****************************************************/
unsigned char AppCompleteCheck()
{
	APPSPACE_STATE RunState=OTAING;

	unsigned long AppDate,BackData;
	
	AppDate=ReadAppDate(0);   //��ȡ�������汾������Ϣ
	
	if(((AppDate&0xF0000000)>0x90000000)||
	   ((AppDate&0x0F000000)>0x09000000)||
	   ((AppDate&0x00F00000)>0x00900000)||
	   ((AppDate&0x000F0000)>0x00090000)||
	   ((AppDate&0x0000F000)>0x00009000)||
	   ((AppDate&0x00000F00)>0x00000900)||
	   ((AppDate&0x000000F0)>0x00000090)||
	   ((AppDate&0x0000000F)>0x00000009))
	{
		AppDate=0;    //�汾���ڸ�ʽ�Ƿ�
	}
	
	BackData=ReadAppDate(1); //��ȡ�������汾������Ϣ

	ReadBACKSMemoryInfo(); //��ȡ��������Ϣ
	ReadAPPMemoryInfo();  //��ȡ��������Ϣ

	
	if((BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[0]==APP_UPDATA_FLAG_ARRAY[0])&&
		(BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[1]==APP_UPDATA_FLAG_ARRAY[1])&&
		(BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[2]==APP_UPDATA_FLAG_ARRAY[2])&&
		(BACKUPS_APPMemoryInfoData.UpdateSucceededFlagNumber[3]==APP_UPDATA_FLAG_ARRAY[3]))    //����������������
	{
		RunState=DOWNLOADED;  //�����������������й̼�
	}
	else
	{
		RunState=OTAING;  //��OTA����
	}
	
	do{
		if(RunState==DOWNLOADED) //�̼����������
		{
			if(BackData>AppDate)   //�и��°汾,���³���
			{
				RunState=COPYING;  
				break;
			}
			#if FORCE_UPDATA    //ǿ�Ƹ��³���
				  RunState=COPYING;
				  break;
			#endif					
		}
		if((RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[0]==APP_UPDATA_FLAG_ARRAY[0])&&
		  (RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[1]==APP_UPDATA_FLAG_ARRAY[1])&&
		  (RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[2]==APP_UPDATA_FLAG_ARRAY[2])&&
		  (RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[3]==APP_UPDATA_FLAG_ARRAY[3]))    //APP������������
	    {  
			RunState=NORMAL;
		}
		else
		{
			if(RunState==DOWNLOADED)        //APP�����벻�������ұ�������������
			{
				RunState=COPYING;//���³���
			}
		}
	}while(0);
//	
	if(RunState==NORMAL)         //����Ҫ��������,��APP���г���
	{
		return 1;
	}
	if(RunState==COPYING)        //�����������뿽����������
	{
		if(BackupsToRunCopy())   //��һ��COPY
		{
			return 1;
		}
		if(BackupsToRunCopy())   //��һ��COPY���ɹ�,�ٴ�COPY
		{
			return 1;
		}		
	}
	return	0;
}
/*****************************************************
*��������: unsigned char IAP_Update_Finish(unsigned char Crc)
*��������: ���OTA���
*��ڲ�����
*���ڲ�����0 ��ʧ�� 1���ɹ�
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
		//���ø�����ɱ�־
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[0] = APP_UPDATA_FLAG_ARRAY[0];
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[1] = APP_UPDATA_FLAG_ARRAY[1];
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[2] = APP_UPDATA_FLAG_ARRAY[2];
		APPMemoryInfoTemp.UpdateSucceededFlagNumber[3] = APP_UPDATA_FLAG_ARRAY[3];
		APPMemoryInfoTemp.InitFlag = INITFLAG; //��־����ԭʼAPP
		//д��洢����Ϣ
		WriteAPPMemoryInfo(APPMemoryInfoTemp);			
		return 1;
	}
	
	return 0;
}
/*****************************************************
*��������: unsigned char IAP_Check_APFLASH()
*��������: �ж�������APP�Ƿ�Ϊ�ʼ��APP,����������������걸��
*��ڲ�����Crc 0���ϵ粻��APP������CRCУ�飻 1���ϵ��APP������CRCУ�飬�ϵ�����ʱ��䳤
*���ڲ�����0 ������ 1����
*****************************************************/
unsigned char IAP_Check_APFLASH(unsigned char Crc)
{
	unsigned long Iap_crc=0;
	unsigned long i;
	unsigned char Data;
	
	ReadAPPMemoryInfo();	//��ȡ��������Ϣ
	
	if((RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[0]==APP_UPDATA_FLAG_ARRAY[0])&&
		(RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[1]==APP_UPDATA_FLAG_ARRAY[1])&&
		(RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[2]==APP_UPDATA_FLAG_ARRAY[2])&&
		(RUN_APPMemoryInfoData.UpdateSucceededFlagNumber[3]==APP_UPDATA_FLAG_ARRAY[3]))    //����������
	{
		if(Crc==0)
		{
			return 1;                              //�����CRC
		}
		if(RUN_APPMemoryInfoData.InitFlag != INITFLAG) //APP�Ƿ�Ϊ������APP
		{
			return 1;                              //����APP�����CRC
		}
		else
		{
			for(i=0;i<RUN_APPMemoryInfoData.APPSize;i++)
			{
				WDTCON |=0X10;
				Data=IAP_ReadOneByte(RUN_APP_BEGIN_ADDRESS+i);
				Iap_crc=CheckSum32(Iap_crc, &Data, 1);
			}
			if(Iap_crc==(RUN_APPMemoryInfoData.APPChecksums&0xFFFFFFFF)) //����������CRC����ȷ��
			{
				return 1;
			}
		}
	}
	return	0;
}
/*****************************************************
*��������: unsigned char BackupsToRunCopy()
*��������: �����������뿽����������
*��ڲ�����
*���ڲ�����0 ������ʧ�� 1�������ɹ�
*****************************************************/
unsigned char BackupsToRunCopy()
{
	unsigned int i = 0;
	unsigned char BackupsCode = 0;

	 ReadBACKSMemoryInfo();//��ȡ��������Ϣ

	//���������
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
	IAP_WriteNByte(RUN_APP_MEMORY_INFO_ADRESS, sizeof(APPMemoryInfoStruct), &BACKUPS_APPMemoryInfoData);  //����APP����Ϣ
	
			#if FORCE_UPDATA    //ǿ�Ƹ��³���
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
