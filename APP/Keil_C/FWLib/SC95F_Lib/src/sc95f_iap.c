//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_iap.c
//	����:				��ԪӦ���Ŷ�
//	ģ�鹦��: 		IAP�̼��⺯��H�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
// ˵��:				���ļ���������SC95Fϵ��оƬ
//*************************************************************
#include "sc95f_iap.h"

#if defined (SC95F8x6x) || defined (SC95F7x6x) || defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined (SC95R751)\
		|| defined (SC95F7610B) || defined (SC95F7619B) || defined (SC95R602)  || defined (SC95R605)  || defined (SC95F7x7x)\
		|| defined (SC95F8x7x)  || defined (SC95R503)
/* IAP�̼��⺯��ͷ�ļ� */
#include "IAP_Option_EW.h"

/**************************************************
*��������:void IAP_SectorEraseOption(Address,Data,IAP_MemType)
*��������:IAP��������
*��ڲ���:uint32_t:Address:IAP������ַ
*         IAP_MemType_TypeDef:IAP_MemType:IAP��������
*���ڲ���:void
**************************************************/
#define IAP_SectorEraseOption(Address,IAP_MemType) \
do{\
if(IAP_MemType == IAP_MEMTYPE_ROM)\
{\
	IAP_CodeSectorEraseOption(Address);\
}\
else if(IAP_MemType == IAP_MEMTYPE_EEPROM)\
{\
	IAP_EEPROMSectorEraseOption(Address);\
}\
}while(0);

/**************************************************
*��������:void IAP_ProgramByteOption(Address,Data,IAP_MemType)
*��������:IAP��Byteд��
*��ڲ���:uint32_t:Address:IAP������ַ
*         uint8_t:Data:д�������
*         IAP_MemType_TypeDef:IAP_MemType:IAP��������
*���ڲ���:void
**************************************************/
#define IAP_ProgramByteOption(Address,Data,IAP_MemType) \
do{\
if(IAP_MemType == IAP_MEMTYPE_ROM)\
{\
	IAP_CodeProgramByteOption(Address,Data);\
}\
else if(IAP_MemType == IAP_MEMTYPE_EEPROM)\
{\
	IAP_EEPROMProgramByteOption(Address,Data);\
}\
}while(0);

#endif

/**************************************************
*��������:void IAP_DeInit(void)
*��������:IAP��ؼĴ�����λ��ȱʡֵ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void IAP_DeInit(void)
{
  IAPKEY = 0X00;
  IAPADL = 0X00;
  IAPADH = 0X00;
  IAPADE = 0X00;
  IAPDAT = 0X00;
  IAPCTL = 0X00;
}

#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x) || defined (SC95R751)
/* 128KbFlash��IAP���� */
/**************************************************
*��������:uint8_t IAP_ReadByte(uint16_t Address, IAP_MemType_TypeDef IAP_MemType)
*��������:IAP��һ���ֽ�
*��ڲ���:
uint32_t:Address:IAP������ַ
IAP_MemType_TypeDef:IAP_MemType:IAP��������
*���ڲ���:
uint8_t:�������ֽ�����
**************************************************/
uint8_t IAP_ReadByte(uint32_t Address, IAP_MemType_TypeDef IAP_MemType)
{
  uint8_t tmpbyte;
  BitStatus tmpbit;
  uint8_t xdata tmpROMBNK;
  uint8_t tmpIAPADE;

  tmpbit = (BitStatus)EA;			//�����жϱ�־λ״̬
  /* ���浱ǰIAP���������Bank���� */
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;

  /* ������������жϣ���Ҫ�ر����жϣ���ֹIAP�������� */
  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  /* �߼���ַתMCU��ַ */
  if(Address >= 0x10000)  //�������64K����Ҫ����Bank�Ĵ���ָ��Ŀ���ַ
  {
    if(Address < 0x18000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x20;
      Address = (Address - 0x8000);
    }
    else if(Address < 0x20000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x30;
      Address = (Address - 0x10000);
    }
    else
    {
      return 0x00;			//����Flash Rom��Χ��С������0x00
    }
  }
  else
  {
    ROMBNK = (ROMBNK & 0xCF) | 0x10;
  }

  IAPADE = IAP_MemType;				//ѡ����Ҫ������IAP����
  tmpbyte = *((uint8_t code*)Address);			//��ȡ��ַ�д洢������

  /* �ָ�ԭIAP���������Bank���� */
  IAPADE = tmpIAPADE;
  ROMBNK = tmpROMBNK;

  /* �ָ�ԭ�ж��ܿ��� */
  if(tmpbit != RESET)
  {
    enableInterrupts();
  }

  return tmpbyte;			//���ض���������
}

/**************************************************
*��������:uint8_t IAP_ReadByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType)
*��������:IAP��ȡ���ȿ�����ֽ�����
*��ڲ���:
uint32_t:Address:IAP������ַ
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:ArraySize:��Ҫ��ȡ����ĳ���
uint8_t *:ByteArray:��Ŷ�ȡ���ݵ�����ָ��
*���ڲ���:
uint8_t:�ɹ���ȡ�������ݳ���
**************************************************/
uint8_t IAP_ReadByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType)
{
  BitStatus tmpbit;
  uint8_t xdata tmpROMBNK;
  uint8_t tmpIAPADE;
  uint8_t tmpCnt;

  tmpbit = (BitStatus)EA;			//�����жϱ�־λ״̬
  /* ���浱ǰIAP���������Bank���� */
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;

  /* ������������жϣ���Ҫ�ر����жϣ���ֹIAP�������� */
  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  //�߼���ַתMCU��ַ
  if(Address >= 0x10000)  //�������64K����Ҫ����Bank�Ĵ���ָ��Ŀ���ַ
  {
    if(Address < 0x18000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x20;
      Address = (Address - 0x8000);
    }
    else if(Address < 0x20000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x30;
      Address = (Address - 0x10000);
    }
    else
    {
      return 0x00;			//����Flash Rom��Χ��С������0x00
    }
  }
  else
  {
    ROMBNK = (ROMBNK & 0xCF) | 0x10;
  }
  IAPADE = IAP_MemType;				//ѡ����Ҫ������IAP����

  tmpCnt = 0;
  do
  {
    /* ��ȡ�����з�����ַ��� */
    if(Address >= 0x10000)
    {
      Address = (Address - 0x8000);
      ROMBNK = ROMBNK + 0x10;
    }

    ByteArray[tmpCnt] = *((uint8_t code*)Address);			//��ȡ��ַ�д洢������
    Address++;
  }
  while(++tmpCnt < ArraySize);

  /* �ָ�ԭIAP���������Bank���� */
  ROMBNK = tmpROMBNK;
  IAPADE = tmpIAPADE;

  /* �ָ�ԭ�ж��ܿ��� */
  if(tmpbit != RESET)
  {
    enableInterrupts();
  }

  return tmpCnt;
}

#if defined(SC95F8x3x) || defined(SC95F7x3x)
/**************************************************
*��������:void IAP_ProgramByte(uint16_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*��������:IAPд��һ���ֽ�
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:Data:д�������
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:WriteTimeLimit:IAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:
ErrorStatus:д��ɹ�/ʧ��
**************************************************/
ErrorStatus IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                            uint8_t WriteTimeLimit)
{
  BitStatus tmpbit;
  uint8_t xdata tmpROMBNK;
  uint8_t tmpIAPADE;
  uint8_t tmpData;

  /* UID��������д���� */
  if((IAP_MemType == IAP_MEMTYPE_UID))
    return ERROR;

  tmpbit = (BitStatus)EA;
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  /* �߼���ַתMCU��ַ */
  if(Address >= 0x10000)  //�������64K����Ҫ����Bank�Ĵ���ָ��Ŀ���ַ
  {
    if(Address < 0x18000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x20;
      Address = (Address - 0x8000);
    }
    else if(Address < 0x20000)
    {

      ROMBNK = (ROMBNK & 0xCF) | 0x30;
      Address = (Address - 0x10000);
    }
    else
    {
      return 0x00;			//����Flash Rom��Χ��С������0x00
    }
  }
  else
  {
    ROMBNK = (ROMBNK & 0xCF) | 0x10;
  }

  IAPADE = IAP_MemType;
  IAPDAT = Data;
  IAPADH = (unsigned char)(Address >> 8);
  IAPADL = (unsigned char)(Address);

  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x10;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
	_nop_();
  _nop_();

  tmpData = *((uint8_t code*)Address);			//�ض�д�������

  ROMBNK = tmpROMBNK;
  IAPADE = tmpIAPADE;
  EA = tmpbit;

  /* �ж�ʵ��д�����ݺ����������Ƿ�һ��*/
  if(Data != tmpData)
    return ERROR;
  else
    return SUCCESS;
}

/**************************************************
*��������:uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,uint8_t WriteTimeLimit)
*��������:IAPд�볤�ȿ�����ֽ�����
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:ArraySize:��Ҫд������ĳ���
uint8_t*:ByteArray:д�����ݵ�����ָ��
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:WriteTimeLimit:IAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:
uint8_t:����������д��ɹ��ĳ���
**************************************************/
uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,
                             uint8_t WriteTimeLimit)
{
  BitStatus tmpbit;
  uint8_t xdata tmpROMBNK;
  uint8_t tmpIAPADE;
  uint8_t tmpCnt;

  /* UID��������д���� */
  if((IAP_MemType == IAP_MEMTYPE_UID))
    return 0;

  tmpbit = (BitStatus)EA;
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;
  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  //�߼���ַתMCU��ַ
  if(Address >= 0x10000)  //�������64K����Ҫ����Bank�Ĵ���ָ��Ŀ���ַ
  {
    if(Address < 0x18000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x20;
      Address = (Address - 0x8000);
    }
    else if(Address < 0x20000)
    {

      ROMBNK = (ROMBNK & 0xCF) | 0x30;
      Address = (Address - 0x10000);
    }
    else
    {
      return 0x00;			//����Flash Rom��Χ��С������0x00
    }
  }
  else
  {
    ROMBNK = (ROMBNK & 0xCF) | 0x10;
  }

  IAPADE = IAP_MemType;

  tmpCnt = 0;
  do
  {
    /* ��ȡ�����з�����ַ��� */
    if(Address >= 0x10000)
    {
      Address = (Address - 0x8000);
      ROMBNK = ROMBNK + 0x10;
    }

    IAPDAT = ByteArray[tmpCnt];
    IAPADH = (unsigned char)(Address >> 8);
    IAPADL = (unsigned char)(Address);
    IAPKEY = WriteTimeLimit;

    IAPCTL = 0x10;
    IAPCTL |= 0x02;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
	  _nop_();
    _nop_();

    if(ByteArray[tmpCnt] != *((uint8_t code*)Address))
      break;
    Address++;
  }
  while(++tmpCnt < ArraySize);
  ROMBNK = tmpROMBNK;
  IAPADE = tmpIAPADE;
  EA = tmpbit;

  return tmpCnt;
}

/**************************************************
*��������:void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
*��������:IAP��������
*��ڲ���:
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint32_t:IAP_SectorEraseAddress:IAP��������Ŀ���ַ
uint8_t:WriteTimeLimit:IAP����ʱ��(ֵ����ڵ���0x40)
*���ڲ���:void
**************************************************/
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
{
  uint8_t tmpbit;
  uint8_t xdata tmpROMBNK;
  uint8_t tmpIAPADE;

  /* UID�������������������� */
  if((IAP_MemType == IAP_MEMTYPE_UID))
    return;

  tmpbit = (BitStatus)EA;
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  //�߼���ַתMCU��ַ
  if(IAP_SectorEraseAddress >= 0x10000)  //�������64K����Ҫ����Bank�Ĵ���ָ��Ŀ���ַ
  {
    if(IAP_SectorEraseAddress < 0x18000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x20;
      IAP_SectorEraseAddress = (IAP_SectorEraseAddress - 0x8000);
    }
    else if(IAP_SectorEraseAddress < 0x20000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x30;
      IAP_SectorEraseAddress = (IAP_SectorEraseAddress - 0x10000);
    }
  }
  else
  {
    ROMBNK = (ROMBNK & 0xCF) | 0x10;
  }

  IAPADE = IAP_MemType;
  IAPADH = (uint8_t)(IAP_SectorEraseAddress >> 8); //����IAPĿ���ַ��λֵ
  IAPADL = (uint8_t)IAP_SectorEraseAddress;        //����IAPĿ���ַ��λֵ
  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x20;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
	_nop_();
  _nop_();

  IAPADE = tmpIAPADE;
  tmpROMBNK = ROMBNK;
  if(tmpbit != RESET)
  {
    enableInterrupts();
  }
}

#elif defined (SC95F8x6x) || defined (SC95F7x6x) || defined (SC95R751)

/**************************************************
*��������:void IAP_ProgramByte(uint16_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*��������:IAPд��һ���ֽ�
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:Data:д�������
IAP_MemType_TypeDef:IAP_MemType:IAP��������ROM��
uint8_t:WriteTimeLimitIAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:
ErrorStatus:д��ɹ�/ʧ��
**************************************************/
ErrorStatus IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                            uint8_t WriteTimeLimit)
{
  uint8_t tmpData ;
	uint8_t tmpIAPADE;
	BitStatus tmpbit;
	
	tmpIAPADE = IAPADE;
	tmpbit = (BitStatus)EA;

	if(tmpbit != RESET)
  {
    disableInterrupts();
  }
	
  if(Address >= 0x20000)
  {
    return 0x00;			//����Flash Rom��Χ��С������0x00
  }

  WriteTimeLimit = 0x00;	//�������95ϵ���ͺ�����Ч
  IAP_ProgramByteOption(Address, Data, IAP_MemType);
  tmpData = IAP_ReadByte(Address, IAP_MemType);
	
	IAPADE = tmpIAPADE;
	if(tmpbit != RESET)
  {
    enableInterrupts();
  }
	
  if(tmpData != Data)
    return ERROR;
  else
    return SUCCESS;
}

/**************************************************
*��������:uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,uint8_t WriteTimeLimit)
*��������:IAPд�볤�ȿ�����ֽ�����
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:ArraySize:��Ҫд������ĳ���
uint8_t*:ByteArray:д�����ݵ�����ָ��
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:WriteTimeLimitIAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:
uint8_t:����������д��ɹ��ĳ���
**************************************************/
uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,
                             uint8_t WriteTimeLimit)
{
  uint8_t tmpCnt ;
  uint8_t tmpData;
  uint32_t tmpAddress;
	uint8_t tmpIAPADE;
	BitStatus tmpbit;
	
	tmpIAPADE = IAPADE;
	tmpbit = (BitStatus)EA;

	if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  if(Address >= 0x20000)
  {
    return 0x00;			//����Flash Rom��Χ��С������0x00
  }

  WriteTimeLimit = 0x00;	//�������95ϵ���ͺ�����Ч

  tmpCnt = 0;
  do
  {
    tmpAddress = Address + tmpCnt;
    IAP_ProgramByteOption(tmpAddress, ByteArray[tmpCnt], IAP_MemType);
    tmpData = IAP_ReadByte(tmpAddress, IAP_MemType);
    if(tmpData != ByteArray[tmpCnt])
      break;
  }
  while(++tmpCnt < ArraySize);
	
	IAPADE = tmpIAPADE;
	if(tmpbit != RESET)
  {
    enableInterrupts();
  }
  return tmpCnt;
}

/**************************************************
*��������:void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
*��������:IAP��������
*��ڲ���:
IAP_MemType_TypeDef:IAP_MemType:IAP��������ROM��
uint32_t:IAP_SectorEraseAddress:IAP��������Ŀ���ַ
uint8_t:WriteTimeLimit:IAP����ʱ��(ֵ����ڵ���0x40)
*���ڲ���:void
**************************************************/
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
{
  WriteTimeLimit = 0x00;	//���ͺ������Ч
  IAP_SectorEraseOption(IAP_SectorEraseAddress, IAP_MemType);
}

#endif

#else
/* 64Kb������Flash��IAP���� */
/**************************************************
*��������:uint8_t IAP_ReadByte(uint16_t Address, IAP_MemType_TypeDef IAP_MemType)
*��������:IAP��һ���ֽ�
*��ڲ���:
uint32_t:Address:IAP������ַ
IAP_MemType_TypeDef:IAP_MemType:IAP��������ROM��UID��
*���ڲ���:
uint8_t:�������ֽ�����
**************************************************/
uint8_t IAP_ReadByte(uint32_t Address, IAP_MemType_TypeDef IAP_MemType)
{
  uint8_t tmpbyte;
  BitStatus tmpbit;
	uint8_t tmpIAPADE;
	
	tmpIAPADE = IAPADE;
  tmpbit = (BitStatus)EA;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  IAPADE = IAP_MemType;
  tmpbyte = *((uint8_t code*)Address);
  IAPADE = tmpIAPADE;

  if(tmpbit != RESET)
  {
    enableInterrupts();
  }

  return tmpbyte;
}

/**************************************************
*��������:uint8_t IAP_ReadByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType)
*��������:IAP��ȡ���ȿ�����ֽ�����
*��ڲ���:
uint32_t:Address:IAP������ַ
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:ArraySize:��Ҫ��ȡ����ĳ���
uint8_t *:ByteArray:��Ŷ�ȡ���ݵ�����ָ��
*���ڲ���:
uint8_t���ɹ���ȡ�����ֽ����鳤��
**************************************************/
uint8_t IAP_ReadByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType)
{
  uint8_t tmpCnt;
  BitStatus tmpbit;
	uint8_t tmpIAPADE;
	
	tmpIAPADE = IAPADE;
  tmpbit = (BitStatus)EA;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  IAPADE = IAP_MemType;

  tmpCnt = 0;
  do
  {
    ByteArray[tmpCnt] = *((uint8_t code*)Address);
    Address++;
  }
  while(++tmpCnt < ArraySize);

  IAPADE = tmpIAPADE;
  if(tmpbit != RESET)
  {
    enableInterrupts();
  }

  return tmpCnt;
}

#if defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined (SC95F7610B) || defined (SC95F7619B) || defined (SC95R602) || defined (SC95R605)\
    || defined (SC95F7x7x) || defined (SC95F8x7x)|| defined (SC95R503)
/**************************************************
*��������:ErrorStatus IAP_ProgramByte(uint16_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*��������:IAPд��һ���ֽ�
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:Data:д�������
IAP_MemType_TypeDef:IAP_MemType:IAP��������ROM��
uint8_t:WriteTimeLimitIAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:void
ErrorStatus:д��ɹ�/ʧ��
**************************************************/
ErrorStatus IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                            uint8_t WriteTimeLimit)
{
  uint8_t tmpData;
	uint8_t tmpIAPADE;
	BitStatus tmpbit;
		/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */

//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	uint8_t tempPWMCON1 = PWMCON1;
//	uint8_t tempPWMCON0 = PWMCON0;
//#endif
	/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */

//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	if(!(PWMCON0 & 0x80))
//	{
//    PWMCON1 = 0x00;
//	  PWMCON0 |= 0x80;
//	}	
//#endif	
	tmpIAPADE = IAPADE;
	tmpbit = (BitStatus)EA;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }
	
  WriteTimeLimit = 0x00;	//���ͺ������Ч
  IAP_ProgramByteOption(Address, Data, IAP_MemType);
  tmpData = IAP_ReadByte(Address, IAP_MemType);
 /* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */
	
//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	PWMCON1 = tempPWMCON1;
//	PWMCON0 = tempPWMCON0;
//#endif
	IAPADE = tmpIAPADE;
	if(tmpbit != RESET)
  {
    enableInterrupts();
  }
	
  if(tmpData != Data)
    return ERROR;
  else
    return SUCCESS;
}

/**************************************************
*��������:uint8_t uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,uint8_t WriteTimeLimit)
*��������:IAPд�볤�ȿ�����ֽ�����
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:ArraySize:��Ҫд������ĳ���
uint8_t*:ByteArray:д�����ݵ�����ָ��
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:WriteTimeLimitIAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:
uint8_t:����������д��ɹ��ĳ���
**************************************************/
uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,
                             uint8_t WriteTimeLimit)
{
	uint8_t tmpIAPADE;
	BitStatus tmpbit;
  uint8_t tmpCnt ;
  uint8_t tmpData;
  uint32_t tmpAddress;
 /* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */

//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	uint8_t tempPWMCON1 = PWMCON1;
//	uint8_t tempPWMCON0 = PWMCON0;
//#endif
	/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */

//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	if(!(PWMCON0 & 0x80))
//	{
//    PWMCON1 = 0x00;
//	  PWMCON0 |= 0x80;
//	}	
//#endif
	
	tmpIAPADE = IAPADE;
	tmpbit = (BitStatus)EA;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }
	
  WriteTimeLimit = 0x00;	//�������95ϵ���ͺ�����Ч
  tmpCnt = 0;
  do
  {
    tmpAddress = Address + tmpCnt;
    IAP_ProgramByteOption(tmpAddress, ByteArray[tmpCnt], IAP_MemType);
    tmpData = IAP_ReadByte(tmpAddress, IAP_MemType);
    if(tmpData != ByteArray[tmpCnt])
      break;
  }
  while(++tmpCnt < ArraySize);
		
	/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */
	
//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	PWMCON1 = tempPWMCON1;
//	PWMCON0 = tempPWMCON0;
//#endif
	
	
	IAPADE = tmpIAPADE;
	if(tmpbit != RESET)
  {
    enableInterrupts();
  }
  return tmpCnt;
}

/**************************************************
*��������:void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
*��������:IAP��������
*��ڲ���:
IAP_MemType_TypeDef:IAP_MemType:IAP��������ROM��
uint32_t:IAP_SectorEraseAddress:IAP��������Ŀ���ַ
uint8_t:WriteTimeLimit:IAP����ʱ��(ֵ����ڵ���0x40)
*���ڲ���:void
**************************************************/
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
{
  WriteTimeLimit = 0x00;	//���ͺ������Ч
		/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */

//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	uint8_t tempPWMCON1 = PWMCON1;
//	uint8_t tempPWMCON0 = PWMCON0;
//#endif	
	/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */

//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	if(!(PWMCON0 & 0x80))
//	{
//    PWMCON1 = 0x00;
//	  PWMCON0 |= 0x80;
//	}	
//#endif
	
  IAP_SectorEraseOption(IAP_SectorEraseAddress, IAP_MemType);
	/* SC95F8x7x/7x7x��SC95R503�ͺ���ʹ���ⲿ�����ڽ���IAP����ʱ����ʹ��PWM0 */
	
//#if defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
//	PWMCON1 = tempPWMCON1;
//	PWMCON0 = tempPWMCON0;
//#endif	
}
#else
/**************************************************
*��������:void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint16_t IAP_SectorEraseAddress)
*��������:IAP��������
*��ڲ���:
IAP_MemType_TypeDef:IAP_MemType:IAP��������ROM��
uint32_t:IAP_SectorEraseAddress:IAP��������Ŀ���ַ
uint8_t:WriteTimeLimit:IAP����ʱ��(ֵ����ڵ���0x40)
*���ڲ���:void
**************************************************/
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
{
	uint8_t tmpIAPADE;
	BitStatus tmpbit;
	
	tmpIAPADE = IAPADE;
	tmpbit = (BitStatus)EA;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }
	
  /* UID�������������������� */
  if((IAP_MemType == IAP_MEMTYPE_UID))
    return;
	
	
  IAPADE = IAP_MemType;
  IAPADH = (uint8_t)(IAP_SectorEraseAddress >> 8); //����IAPĿ���ַ��λֵ
  IAPADL = (uint8_t)IAP_SectorEraseAddress;        //����IAPĿ���ַ��λֵ
  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x20;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
	_nop_();
  _nop_();
	
  IAPADE = tmpIAPADE;
	
	if(tmpbit != RESET)
  {
    enableInterrupts();
  }
}

/**************************************************
*��������:void IAP_ProgramByte(uint16_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*��������:IAPд��һ���ֽ�
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:Data:д�������
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:WriteTimeLimit:IAP����ʱ��(ֵ����ڵ���0x40)
*���ڲ���:
ErrorStatus:д��ɹ�/ʧ��
**************************************************/
ErrorStatus IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                            uint8_t WriteTimeLimit)
{
  BitStatus tmpbit;
  uint8_t TmpIAPADE;
  uint8_t tmpData;

  /* UID��������д���� */
  if((IAP_MemType == IAP_MEMTYPE_UID))
    return ERROR;

  tmpbit = (BitStatus)EA;
  TmpIAPADE = IAPADE;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }
	
  IAPADE = IAP_MemType;
  IAPDAT = Data;
  IAPADH = (uint8_t)(Address >> 8);
  IAPADL = (uint8_t)Address;
  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x10;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
	_nop_();
  _nop_();
	
  tmpData = *((uint8_t code*)Address);			//�ض�д�������
  IAPADE = TmpIAPADE;
	

  /* �ָ�ԭ�ж��ܿ��� */
  if(tmpbit != RESET)
  {
    enableInterrupts();
  }
  /* �ж�ʵ��д�����ݺ����������Ƿ�һ��*/
  if(Data != tmpData)
    return ERROR;
  else
    return SUCCESS;
}

/**************************************************
*��������:void IAP_ProgramByte(uint16_t Address,,  uint8_t* ByteArray,uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*��������:IAPд�볤�ȿ�����ֽ�����
*��ڲ���:
uint32_t:Address:IAP������ַ
uint8_t:ArraySize:��Ҫд������ĳ���
uint8_t*:ByteArray:д�����ݵ�����ָ��
IAP_MemType_TypeDef:IAP_MemType:IAP��������
uint8_t:WriteTimeLimitIAP����ʱ�ޣ�ֵ����ڵ���0x40��
*���ڲ���:
uint8_t:����������д��ɹ��ĳ���
**************************************************/
uint8_t IAP_ProgramByteArray(uint32_t Address, uint8_t* ByteArray, uint8_t ArraySize, IAP_MemType_TypeDef IAP_MemType,
                             uint8_t WriteTimeLimit)
{
  BitStatus tmpbit;
  uint8_t TmpIAPADE;
  uint8_t tmpCnt;

  /* UID��������д���� */
  if((IAP_MemType == IAP_MEMTYPE_UID))
    return 0;

  /* ��ȡ���жϿ���λ��������жϿ���λ */
  tmpbit = (BitStatus)EA;
  TmpIAPADE = IAPADE;

  if(tmpbit != RESET)
  {
    disableInterrupts();
  }

  IAPADE = IAP_MemType;

  tmpCnt = 0;
  do
  {

    IAPDAT = ByteArray[tmpCnt];
    IAPADH = (unsigned char)(Address >> 8);
    IAPADL = (unsigned char)(Address);
    IAPKEY = WriteTimeLimit;

    IAPCTL = 0x10;
    IAPCTL |= 0x02;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
	  _nop_();
    _nop_();

    if(ByteArray[tmpCnt] != *((uint8_t code*)Address))
      break;
    Address++;
  }
  while(++tmpCnt < ArraySize);
  IAPADE = TmpIAPADE;
	

  /* �ָ�ԭ�ж��ܿ��� */
  if(tmpbit != RESET)
  {
    enableInterrupts();
  }
  return tmpCnt;
}

#endif

#endif

/**************************************************
*��������:void IAP_BootLoaderControl(IAP_BTLDType_TypeDef IAP_BTLDType)
*��������:MCU�����λ��������������ã�����ѡ��ΪAPROM/LDROM����
*��ڲ���:
IAP_BTLDType_Ty	peDef:IAP_BTLDType:��λ�����������
*���ڲ���:void
**************************************************/
void IAP_BootLoaderControl(IAP_BTLDType_TypeDef IAP_BTLDType)
{
  IAPCTL = (IAPCTL & 0xFB) | IAP_BTLDType;
}

/******************* (C) COPYRIGHT 2022 SinOne Microelectronics *****END OF FILE****/