//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_USCI3.c
//	����:
//	ģ�鹦��: 		USCI3�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
// ˵��:				���ļ���������SC95Fϵ��оƬ
//*************************************************************

#include "sc95f_USCI3.h"

#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		 || defined (SC95R751) || defined (SC95F7619B) || defined (SC95R602)  || defined (SC95R605)
#if defined(SC95F8x3x) || defined(SC95F7x3x)

#define Select_USCI3()          \
  do {                       \
    USXINX &= 0xF8;          \
    USXINX |= 0X03;          \
  } while(0)

#define USCI3_SetNull()	TMCON = (TMCON & 0XCF)
#define USCI3_SetSPI()	TMCON = (TMCON & 0XCF) | 0X10
#define USCI3_SetTWI()	TMCON = (TMCON & 0XCF) | 0X20
#define USCI3_SetUART()	TMCON = (TMCON & 0XCF) | 0X30

#define US3CON0 USXCON0
#define US3CON1 USXCON1
#define US3CON2 USXCON2
#define US3CON3 USXCON3

#else

#define Select_USCI3()          \
  do {                       \
    USXINX &= 0xF8;          \
    USXINX |= 0X03;          \
  } while(0)

#define USCI3_SetNull()	TMCON = (TMCON & 0X3F)
#define USCI3_SetSPI()	TMCON = (TMCON & 0X3F) | 0X40
#define USCI3_SetTWI()	TMCON = (TMCON & 0X3F) | 0X80
#define USCI3_SetUART()	TMCON = (TMCON & 0X3F) | 0XC0

#define US3CON0 USXCON0
#define US3CON1 USXCON1
#define US3CON2 USXCON2
#define US3CON3 USXCON3

#endif

void USCI3_DeInit(void)
{
  Select_USCI3();
  USCI3_SetNull();
  US3CON0 = 0X00;
  US3CON1 = 0X00;
  US3CON2 = 0X00;
  US3CON3 = 0X00;
  IE2 &= (~0X04);
  IP2 &= (~0X04);
}

/******************************* SPI���� *************************************/
/**************************************************
*��������:void USCI3_SPI_Init(USCI3_SPI_FirstBit_TypeDef FirstBit, USCI3_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI3_SPI_Mode_TypeDef Mode,
							 USCI3_SPI_ClockPolarity_TypeDef ClockPolarity, USCI3_SPI_ClockPhase_TypeDef ClockPhase,USCI3_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI3_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI��ʼ�����ú���
*��ڲ���:
USCI3_SPI_FirstBit_TypeDef:FirstBit:���ȴ���λѡ��MSB/LSB��
USCI3_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPIʱ��Ƶ��ѡ��
USCI3_SPI_Mode_TypeDef:Mode:SPI����ģʽѡ��
USCI3_SPI_ClockPolarity_TypeDef:ClockPolarity:SPIʱ�Ӽ���ѡ��
USCI3_SPI_ClockPhase_TypeDef:ClockPhase:SPIʱ����λѡ��
USCI3_SPI_TXE_INT_TypeDef:SPI_TXE_INT:���ͻ������ж�����ѡ��,�ù�����USCI3оƬ����Ч
USCI3_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16λ
*���ڲ���:void
**************************************************/
void USCI3_SPI_Init(USCI3_SPI_FirstBit_TypeDef FirstBit,
                    USCI3_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI3_SPI_Mode_TypeDef Mode,
                    USCI3_SPI_ClockPolarity_TypeDef ClockPolarity, USCI3_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI3_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI3_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetSPI();//USCI3����ΪSPIģʽ
  SPI_TXE_INT = USCI3_SPI_TXE_DISINT; //SPI_TXE_INT�ù�����USCI3оƬ����Ч
  US3CON1 = US3CON1 & (~0X05) | FirstBit | TransmissionMode;
  US3CON0 = US3CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*��������:void USCI3_TransmissionMode(USCI3_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI ����ģʽ���ú���
*��ڲ���:
USCI3_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16eλ
*���ڲ���:void
**************************************************/
void USCI3_TransmissionMode(USCI3_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetSPI();//USCI3����ΪSPIģʽ
  if(TransmissionMode == USCI3_SPI_DATA8)
  {
    US3CON1 &= 0xFD;
  }
  else
  {
    US3CON1 |= 0x02;
  }
}

/*****************************************************
*��������:void USCI3_SPI_Cmd(FunctionalState NewState)
*��������:SPI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI3_SPI_Cmd(FunctionalState NewState)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetSPI();//USCI3����ΪSPIģʽ

  if(NewState != DISABLE)
  {
    US3CON0 |= 0X80;
  }
  else
  {
    US3CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI3_SPI_SendData_8(uint8_t Data)
*��������:USCI3 SPI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI3_SPI_SendData_8(uint8_t Data)
{
  Select_USCI3();//ѡ�����USCI3
  US3CON2 = Data;
}

/*****************************************************
*��������:uint8_t USCI3_SPI_ReceiveData_8(void)
*��������:���US3CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI3_SPI_ReceiveData_8(void)
{
  Select_USCI3();//ѡ�����USCI3
  return US3CON2;
}

/*****************************************************
*��������:void USCI3_SPI_SendData_16(uint16_t Data)
*��������:US3CON2 SPI��������
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI3_SPI_SendData_16(uint16_t Data)
{
  Select_USCI3();//ѡ�����USCI3
  US3CON3 = (uint8_t)(Data >> 8);
  US3CON2 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI3_SPI_ReceiveData_16(void)
*��������:���US3CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI3_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  Select_USCI3();//ѡ�����USCI3
  SPI_data = (uint16_t)((US3CON3 << 8) | US3CON2);
  return SPI_data;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
*��������:SPI0 �жϴ���
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ����� SPI0 ��Ϣ�� USCI3_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
void
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  Select_USCI3();//ѡ�����USCI3

  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    /* ������û�н������ */
    if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
    {
      /* SPI����16λͨ��ģʽ */
      if((US3CON1 & USCI3_SPI_DATA16) != 0)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_16();			//��ȡ16λ����
      }
      /* SPI����8λͨ��ģʽ */
      else
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_8();			//��ȡ16λ����
      }

      TempStatus = Status_BUSY;
      USCI3_HANDLE->RxXferCount++;			//���յ����ݣ�����ֵ��1

      /* ���ݽ�����ɣ�״̬λ�����޸� */
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
      {
        USCI3_HANDLE->RxState = USCI3_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
    {

      USCI3_HANDLE->TxXferCount++;			//���ݷ�����ɣ�����ֵ��1
      /* ������û�з������ */
      if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
      {
        /* SPI����16λͨ��ģʽ */
        if((US3CON1 & USCI3_SPI_DATA16) != 0)
        {
          USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));			//����16λ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));			//����16λ����
        }
        TempStatus = Status_BUSY;
      }
      /* ���ݷ�����ɣ�״̬λ�����޸�  */
      else if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxState = USCI3_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* ��SPI��������ʱ��SPI��Ҫ�������ݣ�����ͬ�����յ����� */
    else if(USCI3_HANDLE->RxXferSize)
    {
      USCI3_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�½��մ�������
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI3 ״̬
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI3();//ѡ�����USCI3

  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return USCI3_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((USXCON0 & USCI3_SPI_MODE_MASTER) != 0)
    {
      return USCI3_SPI_TransmitReceive(USCI3_HANDLE, pData, pData, Size, Timeout);			//��ת������ģʽ�����շ�����
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			 //����ǰ��������жϱ�־
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
    {
      /* �ȴ�SPI�жϱ�־λ���� */
      if(USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))
      {
        /* SPI����16λͨ��ģʽ */
        if((US3CON1 & USCI3_SPI_DATA16) != 0)
        {
          *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_16();			//��ȡ16λ����,���ҵ�ַ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_8();			//��ȡ16λ����,���ҵ�ַ����
        }
        USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//�����־λ
        USCI3_HANDLE->RxXferCount++;	//����������������1
        TimeoutCnt = 0;			//��ʱ����ֵ����
      }
      else
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
            return USCI3_STATE_TIMEOUT;				//���س�ʱ����
        }
        WDTCON |= 0x10;          //ι����������ֹ���Ź���λ
      }
    }
    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//���մ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:USCI3 ״̬
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x04 == 0) || (Size == 0U))
    {
      return USCI3_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US3CON0 & USCI3_SPI_MODE_MASTER) != 0)
    {
      return USCI3_SPI_TransmitReceive_IT(USCI3_HANDLE, pData, pData, Size);			//��ת���ж�ģʽ�����շ�����
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//״̬����Ϊ����æµ��
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
uint32_t Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI3 ״̬
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			 //����ǰ�����־λ
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US3CON1 & USCI3_SPI_DATA16) != 0) /* SPI����16λͨ��ģʽ */
      {
        USCI3_SPI_SendData_16(*USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount);			//����16λ���ݲ������ݵ�ַ����
      }
      else/* SPI����8λͨ��ģʽ */
      {
        USCI3_SPI_SendData_8(*USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);			//����8λ���ݲ������ݵ�ַ����
      }
      while(!USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))			//�ȴ��������
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if(TimeoutCnt++ > Timeout)
        {
          USCI3_HANDLE->TxState = USCI3_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }
      USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//����ǰ�������־λ
      TimeoutCnt = 0;			//��ʱ����������
      USCI3_HANDLE->TxXferCount++;	//��������������
    }
    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI3_HandleInfoDef *USCI3_HANDLE:ָ����� SPI0 ��Ϣ�� USCI3_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI3 ״̬
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{

  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* û�п���USCI3�жϻ�����������СΪ0�����ش��� */
    if((IE2 & 0x04 == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = 0;			//���ͼ�������0

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US3CON1 & USCI3_SPI_DATA16) != 0)
    {
      USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//״̬λ�޸�Ϊæµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_TransmitReceive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*��������:����ģʽ�շ�һ��������
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ����� SPI0 ��Ϣ�� USCI3_HandleInfoDef �ṹ���ָ��
uint8_t*:pTxData:ָ�������ݻ����ָ��
uint8_t*:pRxData:ָ��������ݻ����ָ��
uint16_t:Size:�������ݵĴ�С
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:��������״̬
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_TransmitReceive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if((USCI3_HANDLE->TxState == USCI3_STATE_READY) && (USCI3_HANDLE->RxState == USCI3_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //ָ����������ݵĵ�ַ
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI3_HANDLE->TxXferSize = USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize);
    {

      if((US0CON1 & USCI3_SPI_DATA16) != 0)
      {
        USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount) = USCI3_SPI_ReceiveData_16();
      }
      else
      {
        USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount) = USCI3_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//��ʱ����ֵ����
      USCI3_HANDLE->TxXferCount++;			//����������ֵ��1
      USCI3_HANDLE->TxXferCount++;			//����������ֵ��1
    }
    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_HANDLE->RxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI3_SPI_TransmitReceive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
*��������:SPI���ж�ģʽ�·��ͺͽ��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI3_HandleInfoDef *USCI3_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:���ͺͽ������ݵĴ����
uint16_t:Size:�����ͺͽ��յ�������
*���ڲ���:
StatusTypeDef:USCI3 ״̬
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_TransmitReceive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
  if((USCI3_HANDLE->TxState == USCI3_STATE_READY) && (USCI3_HANDLE->RxState == USCI3_STATE_READY))
  {
    Select_USCI3();//ѡ�����USCI3

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x04 == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI3_SPI_Cmd(DISABLE);
    USCI3_HANDLE->TxState = USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI3_SPI_Cmd(ENABLE);

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US3CON1 & USCI3_SPI_DATA16) != 0)
    {
      USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16));			//����16λ����
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8));			//����8λ����
    }
    /* ״̬����æµ��״̬ */

    return  Status_OK;
  }
  else
  {
    return Status_ERROR;
  }
}

/******************************* TWI���� *************************************/
/**************************************************
*��������:void USCI3_TWI_Slave_Init(uint8_t TWI_Address)
*��������:USCI3 TWI �ӻ���ʼ�����ú���
*��ڲ���:
uint8_t:TWI_Address:TWI��Ϊ�ӻ�ʱ��7λ�ӻ���ַ
*���ڲ���:void
**************************************************/
void USCI3_TWI_Slave_Init(uint8_t TWI_Address)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetTWI();//USCI3����ΪTWIģʽ
  US3CON2 = TWI_Address << 1;
}

/**************************************************
*��������:void USCI3_TWI_MasterCommunicationRate(USCI3_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*��������:USCI3 TWI����ģʽ��ͨѶ�����趨
*��ڲ���:
USCI3_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI����ģʽ��ͨѶ����
*���ڲ���:void
**************************************************/
void USCI3_TWI_MasterCommunicationRate(USCI3_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetTWI();//USCI3����ΪTWIģʽ

  US3CON1 = TWI_MasterCommunicationRate;
}

/**************************************************
*��������:void USCI3_TWI_Start(void)
*��������:USCI3 TWI ��ʼλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI3_TWI_Start(void)
{
  Select_USCI3();//ѡ�����USCI3
  US3CON1 |= 0x20;
}

/**************************************************
*��������:void USCI3_TWI_MasterModeStop(void)
*��������:USCI3 TWI����ģʽֹͣλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI3_TWI_MasterModeStop(void)
{
  Select_USCI3();//ѡ�����USCI3
  US3CON1 |= 0x10;
}

/**************************************************
*��������:void USCI3_TWI_SlaveClockExtension(void)
*��������:USCI3 TWI�ӻ�ģʽʱ���ӳ�����λ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI3_TWI_SlaveClockExtension(FunctionalState NewState)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetTWI();//USCI3����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US3CON1 |= 0x40;
  }
  else
  {
    US3CON1 &= 0XBF;
  }
}

/**************************************************
*��������:void USCI3_TWI_AcknowledgeConfig(FunctionalState NewState)
*��������:TWI����Ӧ��ʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI3_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetTWI();//USCI3����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US3CON0 |= 0X08;
  }
  else
  {
    US3CON0 &= 0XF7;
  }
}

/**************************************************
*��������:void USCI3_TWI_GeneralCallCmd(FunctionalState NewState)
*��������:TWIͨ�õ�ַ��Ӧʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI3_TWI_GeneralCallCmd(FunctionalState NewState)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetTWI();//USCI3����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US3CON2 |= 0X01;
  }
  else
  {
    US3CON2 &= 0XFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI3_GetTWIStatus(USCI3_TWIState_TypeDef USCI3_TWIState)
*��������:��ȡTWI״̬
*��ڲ���:
USCI3_TWIState_TypeDef:USCI3_TWIState:TWI״̬����
*���ڲ���:
FlagStatus:USCI3_TWI��־״̬
*****************************************************/
FlagStatus USCI3_GetTWIStatus(USCI3_TWIState_TypeDef USCI3_TWIState)
{
  if((US3CON0 & 0x07) == USCI3_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*��������:void USCI3_TWI_Cmd(FunctionalState NewState)
*��������:TWI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI3_TWI_Cmd(FunctionalState NewState)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetTWI();//USCI3����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US3CON0 |= 0X80;
  }
  else
  {
    US3CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI3_TWI_SendAddr(uint8_t Addr,USCI3_TWI_RWType RW)
*��������:TWI���͵�ַ����д����
*��ڲ���:
uint8_t:Addr:���͵ĵ�ַ
USCI3_TWI_RWType:RW:��д����
*���ڲ���:void
*****************************************************/
void USCI3_TWI_SendAddr(uint8_t Addr, USCI3_TWI_RWType RW)
{
  US3CON3 = (Addr << 1) | RW;
}


/*****************************************************
*��������:void USCI3_TWI_SendData(uint8_t Data)
*��������:TWI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI3_TWI_SendData(uint8_t Data)
{
  US3CON3 = Data;
}


/*****************************************************
*��������:uint8_t USCI3_TWI_ReceiveData(void)
*��������:���US3CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI3_TWI_ReceiveData(void)
{
  return US3CON3;
}
/*****************************************************
*�������ƣ�StatusTypeDef USCI3_TWI_Wait_TWIF(USCI3_HandleInfoDef* USCI3_HANDLE,uint32_t Timeout)
*�������ܣ��ȴ�TWIF����
*��ڲ�����
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Wait_TWIF(USCI3_HandleInfoDef* USCI3_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  Select_USCI3();//ѡ�����USCI3
  while(!(USCI3_GetFlagStatus(USCI3_TWI_FLAG_TWIF)))			//�ȴ������źŷ������
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* ��ʱ����״̬ */
      if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
        USCI3_HANDLE->TxState = USCI3_STATE_TIMEOUT;
      if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
        USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			//�����־λ
  return Status_OK;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI3_TWI_Master_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ƣ�StatusTypeDef USCI3_TWI_Master_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ܣ�����8λ����ʱ��������ѯģʽ�·��ʹ�������
*��ڲ�����
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*��pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI3_TWI_Start();			//���������ź�
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)          //���״̬��״̬
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US3CON3 = (slaveAddr << 1) & 0xFE;			//���͵�ַ�Ͷ�дλ
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendData) == RESET)          //���״̬��״̬
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI3_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8  + USCI3_HANDLE->TxXferCount);             //TWI��������
      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI3_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI3_HANDLE->TxXferCount++;
    }
    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize);

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI3_TWI_MasterModeStop();

    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }

  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Slave_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�·��ʹ�������
* ע��Ĭ�Ͽ���ʱ���ӳ�ģʽ
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    Select_USCI3();
    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI3_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ���ַƥ��
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI3_GetTWIStatus(USCI3_TWI_SlaveSendData) == SET)           //���״̬��״̬
      {

        US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);             //TWI��������

        if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)             //�ȴ������źŷ������
        {
          USCI3_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
          return Status_TIMEOUT;
        }
        USCI3_HANDLE->TxXferCount++;

        if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize - 1)

          USCI3_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��

      }
      else if(USCI3_GetTWIStatus(USCI3_TWI_SlaveDisableACK) == SET)
      {

        USCI3_HANDLE->TxXferCount++;
      }
      else
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize);

    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveDisableACK) == RESET)           //���״̬��״̬
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      return Status_ERROR;
    }

    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Master_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
*���ڲ���:
StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI3();
      IE2 &= ~(0x04);			//�ر��ж�

      USCI3_HANDLE->TxState = USCI3_STATE_WAIT;			//���ͽ���æµ��
      USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI3_TWI_Start();			//���������ź�

      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI3_TWI_MasterModeStop();
        IE2 |= 0x04;	//�����ж�
        return Status_TIMEOUT;
      }

      if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI3_TWI_MasterModeStop();
        IE2 |= 0x04;	//�����ж�
        return Status_ERROR;
      }

      IE2 |= 0x04;						//�����ж�
      US3CON3 = (slaveAddr << 1) & 0xFE;//���͵�ַ�Ͷ�дλ

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Master_Transmit_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Transmit_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendData) == SET)
    {
      if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxXferCount++;			//��ַ֡Ҳ��Ϊ���ݼ���
        US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);             //TWI��������
        return Status_BUSY;
      }
      else if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI3_TWI_MasterModeStop();
        USCI3_HANDLE->TxState = USCI3_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI3_TWI_MasterModeStop();
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaUACK) == SET)
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_HANDLE->TxXferCount++;
      USCI3_TWI_MasterModeStop();
      if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxState = USCI3_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI3_TWI_MasterModeStop();
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI3_HANDLE->TxState == USCI3_STATE_WAIT)
  {
    //��ַ�ɹ���Ӧ
    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;
    US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Slave_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ӻ�ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
* ע
*��ڲ���:*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI3();

      USCI3_TWI_AcknowledgeConfig(DISABLE);         //����AA
      USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI3_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI3_HANDLE->TxState = USCI3_STATE_WAIT;			//���ͽ���æµ��
      USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI3_TWI_AcknowledgeConfig(ENABLE);         //����AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Slave_Transmit_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�·��ʹ�������
*��ڲ���:*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Transmit_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
  {

    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveSendData) == SET)
    {
      if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxXferCount++;			//��һ���ж��ǽ��ܵ���ַ��
        US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);
        if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize - 1)
        {
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        }
        return Status_BUSY;
      }
      else
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.�����ݴ�������У����������޸���AA����λ
    2.���յ�����������UACK */
    else if((USCI3_GetTWIStatus(USCI3_TWI_SlaveDisableACK) == SET) || (USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
		 USCI3_HANDLE->TxXferCount++;
      if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)			//���������ѷ���
      {
        USCI3_HANDLE->TxState = USCI3_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �������δ���͹��̳���
      1.���ݷ��ͺ���ܵ�UACK
      2.TWI���ǹ����ڴӻ�����״̬ */
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI3_HANDLE->TxState == USCI3_STATE_WAIT)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveSendData) == SET)
    {
      US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);
      USCI3_HANDLE->TxState = USCI3_STATE_BUSY;
    }
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Master_Receive(USCI3_HandleInfoDef* USCI3_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*��������:����8λ����ʱ��������ѯģʽ�½��մ�������
*��ڲ���:*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
					 Timeout    ��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    USCI3_TWI_Start();			//���������ź�
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      USCI3_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    US3CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI3_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* ���ݽ��չ��� */
    do
    {
      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI3_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
      {
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI3_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI��������
      USCI3_HANDLE->RxXferCount++;
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);                                     //�ر�AA
        if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			  USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI3_TWI_MasterModeStop();
          return Status_TIMEOUT;
        }
        if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaUACK) == RESET)           //���״̬��״̬
        {
          USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			  USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI3_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI��������
        USCI3_HANDLE->RxXferCount++;
      }
    }
    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize);
    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
	 USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI3_TWI_MasterModeStop();

    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Slave_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�½��մ�������
*��ڲ���:
USCI3_HandleInfoDef:*USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    /* ���յ�ַ���� */
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }

    /* �������ݹ��� */
    do
    {
      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        return Status_TIMEOUT;
      }

      if(USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI��������
        USCI3_HANDLE->RxXferCount++;
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UAC
      }
      else if(USCI3_GetTWIStatus(USCI3_TWI_SlaveIdle) == SET)
      {
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
        {
          *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;
          USCI3_HANDLE->RxXferCount++;
        }
        else
        {
          USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_ERROR;
        }
      }
      else
      {
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        return Status_ERROR;
      }

    }
    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize);

    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Master_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    IE2 &= ~(0x04);	//�ر��ж�
    USCI3_HANDLE->RxState = USCI3_STATE_WAIT;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //����AA
    USCI3_TWI_Start();			//���������ź�

    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, 0xFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI3_TWI_MasterModeStop();
      IE2 |= 0x04;	//�����ж�
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI3_TWI_MasterModeStop();
      IE2 |= 0x04;	//�����ж�
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    IE2 |= 0x04;	//�����ж�
    US3CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Master_Receive_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�½��մ�������
*��ڲ���:*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI��������
        USCI3_HANDLE->RxXferCount++;
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			  USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI3_TWI_MasterModeStop();
          USCI3_HANDLE->RxState = USCI3_STATE_READY;	//�������
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
			USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI3_TWI_MasterModeStop();
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else if(USCI3_HANDLE->RxState == USCI3_STATE_WAIT)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == SET)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_BUSY;
    }
    return Status_BUSY;
  }
  return Status_ERROR;
}


/*****************************************************
*��������:void USCI3_TWI_Slave_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size))
*��������:�ӻ�ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
		   *pData	ָ��ָ��洢��
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI3_HANDLE->RxState = USCI3_STATE_WAIT;			//���ͽ���æµ��
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_TWI_Slave_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�½��մ�������
*��ڲ���:
USCI3_HandleInfoDef:*USCI3_HANDLE ָ��ָ��USCI3������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI��������
        USCI3_HANDLE->RxXferCount++;
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
        {
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        }
      }
      else
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);
        return Status_ERROR;
      }
      return Status_BUSY;
    }
    else if(USCI3_GetTWIStatus(USCI3_TWI_SlaveIdle) == SET)
    {
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;
        USCI3_HANDLE->RxXferCount++;
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        USCI3_HANDLE->RxState = USCI3_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }
  }
  else if(USCI3_HANDLE->RxState == USCI3_STATE_WAIT)
  {
    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;
  }
}

/******************************* UART���� *************************************/
/**************************************************
*��������:void USCI3_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI3_UART_Mode_TypeDef Mode, USCI3_UART_RX_TypeDef RxMode)
*��������:UART��ʼ�����ú���
*��ڲ���:
uint32_t:UARTFsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
USCI3_UART_Mode_TypeDef:Mode:UART1����ģʽ
USCI3_UART_RX_TypeDef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void USCI3_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI3_UART_Mode_TypeDef Mode,
                     USCI3_UART_RX_TypeDef RxMode)
{
  Select_USCI3();//ѡ�����USCI3
  USCI3_SetUART();//USCI3����ΪUARTģʽ
  US3CON0 = US3CON0 & 0X0F | Mode | RxMode;

  if(Mode == USCI3_UART_Mode_8B)
  {
    if(BaudRate == USCI3_UART_BaudRate_FsysDIV12)
    {
      US3CON0 &= 0XDF;
    }
    else if(BaudRate == USCI3_UART_BaudRate_FsysDIV4)
    {
      US3CON0 |= 0X20;
    }
  }
  else
  {
    US3CON2 = UARTFsys / BaudRate / 256;
    US3CON1 = UARTFsys / BaudRate % 256;
  }
}

/*****************************************************
*��������:void USCI3_UART_SendData8(uint8_t Data)
*��������:USCI3 UART1����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI3_UART_SendData8(uint8_t Data)
{
  US3CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI3_UART_ReceiveData8(void)
*��������:���US3CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI3_UART_ReceiveData8(void)
{
  return US3CON3;
}

/*****************************************************
*��������:void USCI3_UART_SendData9(uint16_t Data)
*��������:UART����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI3_UART_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if(Data_9Bit)
  {
    US3CON0 |= 0x08;
  }
  else
  {
    US3CON0 &= 0xf7;
  }

  US3CON3 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI3_UART_ReceiveData9(void)
*��������:���US3CON3�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI3_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US3CON3 + ((uint16_t)(US3CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*��������:StatusTypeDef  USCI3_UART_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI3_UART_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  uint32_t delaytime = 0;
  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* ��鷢���������Ƿ�Ϊ0 */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_UART_FLAG_TI); //����ǰ��������жϱ�־��д1/д0����
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI3_UART_SendData9(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI3_UART_SendData8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));
      }

      /* �ȴ�������� */
      delaytime = 0;
      while(!(US3CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI3_HANDLE->TxState = USCI3_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
      }

      USCI3_ClearFlag(USCI3_UART_FLAG_TI); //�����жϱ�־�����д1/д0����
      USCI3_HANDLE->TxXferCount++;	//��������������
    }

    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_UART_Transmit_IT(USCI3_HandleInfoDef *USCI3_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI3_UART_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* û�п���USCI3�жϻ�����������СΪ0�����ش��� */
    if(((IE2 & 0x04) == 0) || (USCI3_HANDLE->TxXferSize == 0U))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->TxXferCount = 0;			//���ͼ���������

    /* ���͵�1֡���� */
    if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
    {
      /* ����9λ���� */
      USCI3_UART_SendData9(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));
    }
    else
    {
      /* ����8λ���� */
      USCI3_UART_SendData8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef  USCI3_UART_Transmit_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:*USCI3_HANDLE ָ��ָ��UART������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI3_UART_Transmit_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  Select_USCI3();//ѡ�����USCI3

  /* ���ڷ����߳� */
  if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
  {
    USCI3_HANDLE->TxXferCount++;
    /* �ж���һ�η����Ƿ������һ�� */
    if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI3_UART_SendData9(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI3_UART_SendData8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));
      }
      return Status_OK;
    }
    else if(USCI3_HANDLE->TxXferCount)
    {
      /* ������� */
      USCI3_HANDLE->TxState = USCI3_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_UART_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
USCI3_USCI3_HANDLEInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3_UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI3_UART_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  Select_USCI3();//ѡ�����USCI3

  /* �����ս����������� */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if((IE2 & 0x04 != 0) || (Size == 0U) || ((US3CON0 & 0x10) != USCI3_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//���ͽ���æµ��
    USCI3_ClearFlag(USCI3_UART_FLAG_RI);			 //����ǰ��������жϱ�־
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)  //�ж��Ƿ������������
    {
      if(USCI3_GetFlagStatus(USCI3_UART_FLAG_RI))  //�жϽ��ձ�־λ
      {
        USCI3_ClearFlag(USCI3_UART_FLAG_RI);			//���ձ�־λ����

        /* ������������δΪ0���������� */
        if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
        {
          /* ����9λ���� */
          *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData9();
        }
        else
        {
          /* ����8λ���� */
          *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData8();
        }

        USCI3_HANDLE->RxXferCount++;	//��������������
        delaytime = 0;//�յ����ݣ���ʱ��������
      }
      if(delaytime++ > Timeout) //��ʱ���
      {
        USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;//���ͳ�ʱ
        return Status_TIMEOUT;
      }
    }

    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_UART_Receive_IT(USCI3_HandleInfoDef *USCI3_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI3_USCI3_HANDLEInfoDef*:USCI3_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI3_UART_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* ����Ƿ���USCI3_UART�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
    if(((IE2 & 0x04) == 0) || (Size == 0U) || ((US3CON0 & 0x10) != USCI3_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
    USCI3_HANDLE->RxXferSize = Size;			//���´����յ�������
    USCI3_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI3_UART_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI3_HandleInfoDef*:USCI3_HANDLE:ָ��ָ��USCI3������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI3_UART_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  Select_USCI3();//ѡ�����USCI3

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    /* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
    if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
      {
        /* ����9λ���� */
        *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData9();
      }
      else
      {
        /* ����8λ���� */
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData8();
      }
      USCI3_HANDLE->RxXferCount++;
      /* �ж���һ�η����Ƿ������һ�� */
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
      {
        /* ������� */
        USCI3_HANDLE->RxState = USCI3_STATE_READY;
        return Status_OK;
			}
      return Status_BUSY;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:FlagStatus USCI3_GetFlagStatus(USCI3_Flag_TypeDef USCI3_FLAG)
*��������:���USCI3��־״̬
*��ڲ���:
USCI3_Flag_TypeDef:USCI3_FLAG:�����ȡ�ı�־λ
*���ڲ���:
FlagStatus:USCI3��־״̬
*****************************************************/
FlagStatus USCI3_GetFlagStatus(USCI3_Flag_TypeDef USCI3_FLAG)
{
  FlagStatus bitstatus = RESET;
  Select_USCI3();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
	  if((USCI3_FLAG == USCI3_SPI_FLAG_SPIF) || (USCI3_FLAG == USCI3_SPI_FLAG_WCOL) || (USCI3_FLAG == USCI3_SPI_FLAG_TXE))
		{
			if((USCI3_FLAG & US3CON1) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }	
	  }
	}else if((TMCON & 0xC0)==0x80)  //TWIģʽ
  {
		if(USCI3_FLAG == USCI3_TWI_FLAG_TXRXnE)
		{
			if((USCI3_FLAG & US3CON1) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
		}
		else
    {
			if((USCI3_FLAG & US3CON0) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    } 
  }else if((TMCON & 0xC0)==0xC0)  //UARTģʽ
  {
    if((USCI3_FLAG & US3CON0) != (uint8_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  return bitstatus;
}

/*****************************************************
*��������:void USCI3_ClearFlag(USCI3_Flag_TypeDef USCI3_FLAG)
*��������:���USCI3��־״̬
*��ڲ���:
USCI3_Flag_TypeDef:USCI3_FLAG:��������ı�־λ
*���ڲ���:void
*****************************************************/
void USCI3_ClearFlag(USCI3_Flag_TypeDef USCI3_FLAG)
{
  Select_USCI3();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
	  if((USCI3_FLAG == USCI3_SPI_FLAG_SPIF) || (USCI3_FLAG == USCI3_SPI_FLAG_WCOL) || (USCI3_FLAG == USCI3_SPI_FLAG_TXE))
		{
		  US3CON1 &= (~USCI3_FLAG); //�����Ĵ���US3CON1	
		}
	}else if((TMCON & 0xC0)==0x80)  //TWIģʽ
	{
	  if(USCI3_FLAG == USCI3_TWI_FLAG_TXRXnE)
    {
      US3CON1 &= (~USCI3_FLAG); //�����Ĵ���US3CON1	
    }
		else
    {
       US3CON0 &= (~USCI3_FLAG); //�����Ĵ���US3CON0
    }	
	}else if((TMCON & 0xC0)==0xC0)  //UARTģʽ
	{
	  if((USCI3_FLAG == USCI3_UART_FLAG_TI) || (USCI3_FLAG == USCI3_UART_FLAG_RI))
    {
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605)
     US3CON0 = US3CON0  & 0xFC | USCI3_FLAG;//д1����
#else
     US3CON0 &= (~USCI3_FLAG); //д0����
#endif
    }	
	}
}

/*****************************************************
*��������:void USCI3_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:USCI3�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void USCI3_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if(NewState != DISABLE)
  {
    IE2 |= 0x04;
  }
  else
  {
    IE2 &= 0xFB;
  }

  /************************************************************/
  if(Priority != LOW)
  {
    IP2 |= 0x04;
  }
  else
  {
    IP2 &= 0xFB;
  }
}

#endif

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/