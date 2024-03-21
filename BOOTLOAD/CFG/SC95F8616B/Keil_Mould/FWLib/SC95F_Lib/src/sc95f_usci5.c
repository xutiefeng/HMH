//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����	: sc95f_USCI5.c
//	����		:
//	ģ�鹦��	: USCI5�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��        :���ļ���������SC95Fϵ��оƬ
//*************************************************************

#include "sc95f_USCI5.h"

#if defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined (SC95F7619B)\
 || defined (SC95R602)  || defined (SC95R605)
 
#define Select_USCI5()          \
  do {                       \
    USXINX &= 0xF8;          \
    USXINX |= 0X05;          \
  } while(0)

#define USCI5_SetNull() TMCON = (TMCON & 0X3F)
#define USCI5_SetSPI()	TMCON = (TMCON & 0X3F) | 0X40
#define USCI5_SetTWI()	TMCON = (TMCON & 0X3F) | 0X80
#define USCI5_SetUART()	TMCON = (TMCON & 0X3F) | 0XC0

#define US5CON0 USXCON0
#define US5CON1 USXCON1
#define US5CON2 USXCON2
#define US5CON3 USXCON3


void USCI5_DeInit(void)
{
  Select_USCI5();
  USCI5_SetNull();
  US5CON0 = 0X00;
  US5CON1 = 0X00;
  US5CON2 = 0X00;
  US5CON3 = 0X00;
  IE2 &= (~0X10);
  IP2 &= (~0X10);
}

/******************************* SPI���� *************************************/
/**************************************************
*��������:void USCI5_SPI_Init(USCI5_SPI_FirstBit_TypeDef FirstBit, USCI5_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI5_SPI_Mode_TypeDef Mode,
							 USCI5_SPI_ClockPolarity_TypeDef ClockPolarity, USCI5_SPI_ClockPhase_TypeDef ClockPhase,USCI5_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI5_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI��ʼ�����ú���
*��ڲ���:
USCI5_SPI_FirstBit_TypeDef:FirstBit:���ȴ���λѡ��MSB/LSB��
USCI5_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPIʱ��Ƶ��ѡ��
USCI5_SPI_Mode_TypeDef:Mode:SPI����ģʽѡ��
USCI5_SPI_ClockPolarity_TypeDef:ClockPolarity:SPIʱ�Ӽ���ѡ��
USCI5_SPI_ClockPhase_TypeDef:ClockPhase:SPIʱ����λѡ��
USCI5_SPI_TXE_INT_TypeDef:SPI_TXE_INT:���ͻ������ж�����ѡ��,�ù�����USCI5оƬ����Ч
USCI5_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16λ
*���ڲ���:void
**************************************************/
void USCI5_SPI_Init(USCI5_SPI_FirstBit_TypeDef FirstBit,
                    USCI5_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI5_SPI_Mode_TypeDef Mode,
                    USCI5_SPI_ClockPolarity_TypeDef ClockPolarity, USCI5_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI5_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI5_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetSPI();//USCI5����ΪSPIģʽ
  SPI_TXE_INT = USCI5_SPI_TXE_DISINT; //SPI_TXE_INT�ù�����USCI5оƬ����Ч
  US5CON1 = US5CON1 & (~0X05) | FirstBit | TransmissionMode;
  US5CON0 = US5CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*��������:void USCI5_TransmissionMode(USCI5_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI ����ģʽ���ú���
*��ڲ���:
USCI5_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16eλ
*���ڲ���:void
**************************************************/
void USCI5_TransmissionMode(USCI5_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetSPI();//USCI5����ΪSPIģʽ
  if(TransmissionMode == USCI5_SPI_DATA8)
  {
    US5CON1 &= 0xFD;
  }
  else
  {
    US5CON1 |= 0x02;
  }
}

/*****************************************************
*��������:void USCI5_SPI_Cmd(FunctionalState NewState)
*��������:SPI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI5_SPI_Cmd(FunctionalState NewState)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetSPI();//USCI5����ΪSPIģʽ

  if(NewState != DISABLE)
  {
    US5CON0 |= 0X80;
  }
  else
  {
    US5CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI5_SPI_SendData_8(uint8_t Data)
*��������:USCI5 SPI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI5_SPI_SendData_8(uint8_t Data)
{
  Select_USCI5();//ѡ�����USCI5
  US5CON2 = Data;
}

/*****************************************************
*��������:uint8_t USCI5_SPI_ReceiveData_8(void)
*��������:���US5CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI5_SPI_ReceiveData_8(void)
{
  Select_USCI5();//ѡ�����USCI5
  return US5CON2;
}

/*****************************************************
*��������:void USCI5_SPI_SendData_16(uint16_t Data)
*��������:US5CON2 SPI��������
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI5_SPI_SendData_16(uint16_t Data)
{
  Select_USCI5();//ѡ�����USCI5
  US5CON3 = (uint8_t)(Data >> 8);
  US5CON2 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI5_SPI_ReceiveData_16(void)
*��������:���US5CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI5_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  Select_USCI5();//ѡ�����USCI5
  SPI_data = (uint16_t)((US5CON3 << 8) | US5CON2);
  return SPI_data;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
*��������:SPI0 �жϴ���
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ����� SPI0 ��Ϣ�� USCI5_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
void
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  Select_USCI5();//ѡ�����USCI5

  if(USCI5_HANDLE->RxState == USCI5_STATE_BUSY)
  {
    /* ������û�н������ */
    if(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize)
    {
      /* SPI����16λͨ��ģʽ */
      if((US5CON1 & USCI5_SPI_DATA16) != 0)
      {
        *(USCI5_HANDLE->pRxBuffPtr.Size_u16 + USCI5_HANDLE->RxXferCount) = USCI5_SPI_ReceiveData_16();			//��ȡ16λ����
      }
      /* SPI����8λͨ��ģʽ */
      else
      {
        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = USCI5_SPI_ReceiveData_8();			//��ȡ16λ����
      }

      TempStatus = Status_BUSY;
      USCI5_HANDLE->RxXferCount++;			//���յ����ݣ�����ֵ��1

      /* ���ݽ�����ɣ�״̬λ�����޸� */
      if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
      {
        USCI5_HANDLE->RxState = USCI5_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI5_HANDLE->TxState == USCI5_STATE_BUSY)
    {

      USCI5_HANDLE->TxXferCount++;			//���ݷ�����ɣ�����ֵ��1
      /* ������û�з������ */
      if(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize)
      {
        /* SPI����16λͨ��ģʽ */
        if((US5CON1 & USCI5_SPI_DATA16) != 0)
        {
          USCI5_SPI_SendData_16(*(USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount));			//����16λ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          USCI5_SPI_SendData_8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount));			//����16λ����
        }
        TempStatus = Status_BUSY;
      }
      /* ���ݷ�����ɣ�״̬λ�����޸�  */
      else if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
      {
        USCI5_HANDLE->TxState = USCI5_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* ��SPI��������ʱ��SPI��Ҫ�������ݣ�����ͬ�����յ����� */
    else if(USCI5_HANDLE->RxXferSize)
    {
      USCI5_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�½��մ�������
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI5 ״̬
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI5();//ѡ�����USCI5

  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return USCI5_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((USXCON0 & USCI5_SPI_MODE_MASTER) != 0)
    {
      return USCI5_SPI_TransmitReceive(USCI5_HANDLE, pData, pData, Size, Timeout);			//��ת������ģʽ�����շ�����
    }

    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);			 //����ǰ��������жϱ�־
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize)
    {
      /* �ȴ�SPI�жϱ�־λ���� */
      if(USCI5_GetFlagStatus(USCI5_SPI_FLAG_SPIF))
      {
        /* SPI����16λͨ��ģʽ */
        if((US5CON1 & USCI5_SPI_DATA16) != 0)
        {
          *(USCI5_HANDLE->pRxBuffPtr.Size_u16 + USCI5_HANDLE->RxXferCount) = USCI5_SPI_ReceiveData_16();			//��ȡ16λ����,���ҵ�ַ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = USCI5_SPI_ReceiveData_8();			//��ȡ16λ����,���ҵ�ַ����
        }
        USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);				//�����־λ
        USCI5_HANDLE->RxXferCount++;	//����������������1
        TimeoutCnt = 0;			//��ʱ����ֵ����
      }
      else
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
            return USCI5_STATE_TIMEOUT;				//���س�ʱ����
        }
        WDTCON |= 0x10;          //ι����������ֹ���Ź���λ
      }
    }
    if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
    {
      USCI5_HANDLE->RxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;//���մ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t *pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:USCI5 ״̬
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 == 0) || (Size == 0U))
    {
      return USCI5_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US5CON0 & USCI5_SPI_MODE_MASTER) != 0)
    {
      return USCI5_SPI_TransmitReceive_IT(USCI5_HANDLE, pData, pData, Size);			//��ת���ж�ģʽ�����շ�����
    }

    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//״̬����Ϊ����æµ��
    USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
uint32_t Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI5 ״̬
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);			 //����ǰ�����־λ
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US5CON1 & USCI5_SPI_DATA16) != 0) /* SPI����16λͨ��ģʽ */
      {
        USCI5_SPI_SendData_16(*USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount);			//����16λ���ݲ������ݵ�ַ����
      }
      else/* SPI����8λͨ��ģʽ */
      {
        USCI5_SPI_SendData_8(*USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount);			//����8λ���ݲ������ݵ�ַ����
      }
      while(!USCI5_GetFlagStatus(USCI5_SPI_FLAG_SPIF))			//�ȴ��������
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if(TimeoutCnt++ > Timeout)
        {
          USCI5_HANDLE->TxState = USCI5_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }
      USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);				//����ǰ�������־λ
      TimeoutCnt = 0;			//��ʱ����������
      USCI5_HANDLE->TxXferCount++;	//��������������
    }
    if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
    {
      USCI5_HANDLE->TxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI5_HandleInfoDef *USCI5_HANDLE:ָ����� SPI0 ��Ϣ�� USCI5_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI5 ״̬
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
{

  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* û�п���USCI5�жϻ�����������СΪ0�����ش��� */
    if((IE2 & 0x10 == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = 0;			//���ͼ�������0

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US5CON1 & USCI5_SPI_DATA16) != 0)
    {
      USCI5_SPI_SendData_16(*(USCI5_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI5_SPI_SendData_8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//״̬λ�޸�Ϊæµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_TransmitReceive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*��������:����ģʽ�շ�һ��������
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ����� SPI0 ��Ϣ�� USCI5_HandleInfoDef �ṹ���ָ��
uint8_t*:pTxData:ָ�������ݻ����ָ��
uint8_t*:pRxData:ָ��������ݻ����ָ��
uint16_t:Size:�������ݵĴ�С
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:��������״̬
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_TransmitReceive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if((USCI5_HANDLE->TxState == USCI5_STATE_READY) && (USCI5_HANDLE->RxState == USCI5_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI5_HANDLE->TxState = USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //ָ����������ݵĵ�ַ
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI5_HANDLE->TxXferSize = USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize);
    {

      if((US0CON1 & USCI5_SPI_DATA16) != 0)
      {
        USCI5_SPI_SendData_16(*(USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI5_GetFlagStatus(USCI5_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI5_HANDLE->RxState = USCI5_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount) = USCI5_SPI_ReceiveData_16();
      }
      else
      {
        USCI5_SPI_SendData_8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI5_GetFlagStatus(USCI5_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI5_HANDLE->RxState = USCI5_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount) = USCI5_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//��ʱ����ֵ����
      USCI5_HANDLE->TxXferCount++;			//����������ֵ��1
      USCI5_HANDLE->TxXferCount++;			//����������ֵ��1
    }
    if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
    {
      USCI5_HANDLE->TxState = USCI5_HANDLE->RxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }
}


/**********************************************************************************************************
*��������:StatusTypeDef USCI5_SPI_TransmitReceive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
*��������:SPI���ж�ģʽ�·��ͺͽ��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI5_HandleInfoDef *USCI5_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:���ͺͽ������ݵĴ����
uint16_t:Size:�����ͺͽ��յ�������
*���ڲ���:
StatusTypeDef:USCI5 ״̬
**********************************************************************************************************/
StatusTypeDef USCI5_SPI_TransmitReceive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
  if((USCI5_HANDLE->TxState == USCI5_STATE_READY) && (USCI5_HANDLE->RxState == USCI5_STATE_READY))
  {
    Select_USCI5();//ѡ�����USCI5

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI5_SPI_Cmd(DISABLE);
    USCI5_HANDLE->TxState = USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI5_SPI_Cmd(ENABLE);

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US5CON1 & USCI5_SPI_DATA16) != 0)
    {
      USCI5_SPI_SendData_16(*(USCI5_HANDLE->pTxBuffPtr.Size_u16));			//����16λ����
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI5_SPI_SendData_8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8));			//����8λ����
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
*��������:void USCI5_TWI_Slave_Init(uint8_t TWI_Address)
*��������:USCI5 TWI �ӻ���ʼ�����ú���
*��ڲ���:
uint8_t:TWI_Address:TWI��Ϊ�ӻ�ʱ��7λ�ӻ���ַ
*���ڲ���:void
**************************************************/
void USCI5_TWI_Slave_Init(uint8_t TWI_Address)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetTWI();//USCI5����ΪTWIģʽ
  US5CON2 = TWI_Address << 1;
}

/**************************************************
*��������:void USCI5_TWI_MasterCommunicationRate(USCI5_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*��������:USCI5 TWI����ģʽ��ͨѶ�����趨
*��ڲ���:
USCI5_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI����ģʽ��ͨѶ����
*���ڲ���:void
**************************************************/
void USCI5_TWI_MasterCommunicationRate(USCI5_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetTWI();//USCI5����ΪTWIģʽ

  US5CON1 = TWI_MasterCommunicationRate;
}

/**************************************************
*��������:void USCI5_TWI_Start(void)
*��������:USCI5 TWI ��ʼλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI5_TWI_Start(void)
{
  Select_USCI5();//ѡ�����USCI5
  US5CON1 |= 0x20;
}

/**************************************************
*��������:void USCI5_TWI_MasterModeStop(void)
*��������:USCI5 TWI����ģʽֹͣλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI5_TWI_MasterModeStop(void)
{
  Select_USCI5();//ѡ�����USCI5
  US5CON1 |= 0x10;
}

/**************************************************
*��������:void USCI5_TWI_SlaveClockExtension(void)
*��������:USCI5 TWI�ӻ�ģʽʱ���ӳ�����λ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI5_TWI_SlaveClockExtension(FunctionalState NewState)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetTWI();//USCI5����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US5CON1 |= 0x40;
  }
  else
  {
    US5CON1 &= 0XBF;
  }
}

/**************************************************
*��������:void USCI5_TWI_AcknowledgeConfig(FunctionalState NewState)
*��������:TWI����Ӧ��ʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI5_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetTWI();//USCI5����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US5CON0 |= 0X08;
  }
  else
  {
    US5CON0 &= 0XF7;
  }
}

/**************************************************
*��������:void USCI5_TWI_GeneralCallCmd(FunctionalState NewState)
*��������:TWIͨ�õ�ַ��Ӧʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI5_TWI_GeneralCallCmd(FunctionalState NewState)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetTWI();//USCI5����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US5CON2 |= 0X01;
  }
  else
  {
    US5CON2 &= 0XFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI5_GetTWIStatus(USCI5_TWIState_TypeDef USCI5_TWIState)
*��������:��ȡTWI״̬
*��ڲ���:
USCI5_TWIState_TypeDef:USCI5_TWIState:TWI״̬����
*���ڲ���:
FlagStatus:USCI5_TWI��־״̬
*****************************************************/
FlagStatus USCI5_GetTWIStatus(USCI5_TWIState_TypeDef USCI5_TWIState)
{
  if((US5CON0 & 0x07) == USCI5_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*��������:void USCI5_TWI_Cmd(FunctionalState NewState)
*��������:TWI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI5_TWI_Cmd(FunctionalState NewState)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetTWI();//USCI5����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US5CON0 |= 0X80;
  }
  else
  {
    US5CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI5_TWI_SendAddr(uint8_t Addr,USCI5_TWI_RWType RW)
*��������:TWI���͵�ַ����д����
*��ڲ���:
uint8_t:Addr:���͵ĵ�ַ
USCI5_TWI_RWType:RW:��д����
*���ڲ���:void
*****************************************************/
void USCI5_TWI_SendAddr(uint8_t Addr, USCI5_TWI_RWType RW)
{
  US5CON3 = (Addr << 1) | RW;
}


/*****************************************************
*��������:void USCI5_TWI_SendData(uint8_t Data)
*��������:TWI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI5_TWI_SendData(uint8_t Data)
{
  US5CON3 = Data;
}


/*****************************************************
*��������:uint8_t USCI5_TWI_ReceiveData(void)
*��������:���US5CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI5_TWI_ReceiveData(void)
{
  return US5CON3;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI5_TWI_Wait_TWIF(USCI5_HandleInfoDef* USCI5_HANDLE,uint32_t Timeout)
*�������ܣ��ȴ�TWIF����
*��ڲ�����
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Wait_TWIF(USCI5_HandleInfoDef* USCI5_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  while(!(USCI5_GetFlagStatus(USCI5_TWI_FLAG_TWIF)))			//�ȴ������źŷ������
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* ��ʱ����״̬ */
      if(USCI5_HANDLE->TxState == USCI5_STATE_BUSY)
        USCI5_HANDLE->TxState = USCI5_STATE_TIMEOUT;
      if(USCI5_HANDLE->RxState == USCI5_STATE_BUSY)
        USCI5_HANDLE->RxState = USCI5_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			//�����־λ
  return Status_OK;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI5_TWI_Master_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ܣ�����8λ����ʱ��������ѯģʽ�·��ʹ�������
*��ڲ�����
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*��pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Master_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  Select_USCI5();
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI5_TWI_Start();			//���������ź�
    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI5_GetTWIStatus(USCI5_TWI_MasterSendAddress) == RESET)          //���״̬��״̬
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US5CON3 = (slaveAddr << 1) & 0xFE;			//���͵�ַ�Ͷ�дλ
    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI5_GetTWIStatus(USCI5_TWI_MasterSendData) == RESET)          //���״̬��״̬
      {
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US5CON3 = *(USCI5_HANDLE->pTxBuffPtr.Size_u8  + USCI5_HANDLE->TxXferCount);             //TWI��������
      if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI5_HANDLE->TxXferCount++;
    }
    while(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize);

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI5_TWI_MasterModeStop();

    if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
    {
      USCI5_HANDLE->TxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }

  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Slave_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�·��ʹ�������
* ע��Ĭ�Ͽ���ʱ���ӳ�ģʽ
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Slave_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI5();
    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI5_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
    USCI5_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ���ַƥ��
    {
      USCI5_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI5_GetTWIStatus(USCI5_TWI_SlaveSendData) == SET)           //���״̬��״̬
      {

        US5CON3 = *(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount);             //TWI��������

        if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)             //�ȴ������źŷ������
        {
          USCI5_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
          return Status_TIMEOUT;
        }
        USCI5_HANDLE->TxXferCount++;

        if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize - 1)

          USCI5_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��

      }
      else if(USCI5_GetTWIStatus(USCI5_TWI_SlaveDisableACK) == SET)
      {

        USCI5_HANDLE->TxXferCount++;
      }
      else
      {
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        return Status_ERROR;
      }
    }
    while(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize);

    if(USCI5_GetTWIStatus(USCI5_TWI_SlaveDisableACK) == RESET)           //���״̬��״̬
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
      return Status_ERROR;
    }

    if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
    {
      USCI5_HANDLE->TxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Master_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
*���ڲ���:
StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Master_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x10) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI5();
      IE2 &= ~0x10;			//�ر��ж�

      USCI5_HANDLE->TxState = USCI5_STATE_WAIT;			//���ͽ���æµ��
      USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI5_TWI_Start();			//���������ź�

      if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_MasterModeStop();
        IE2 |= 0x10;	//�����ж�
        return Status_TIMEOUT;
      }

      if(USCI5_GetTWIStatus(USCI5_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
      {
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_MasterModeStop();
        IE2 |= 0x10;	//�����ж�
        return Status_ERROR;
      }

      IE2 |= 0x10;						//�����ж�
      US5CON3 = (slaveAddr << 1) & 0xFE;//���͵�ַ�Ͷ�дλ

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Master_Transmit_IRQHandler(USCI5_HandleInfoDef *USCI5_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Master_Transmit_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  if(USCI5_HANDLE->TxState == USCI5_STATE_BUSY)
  {
    if(USCI5_GetTWIStatus(USCI5_TWI_MasterSendData) == SET)
    {
      if(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize)
      {
        USCI5_HANDLE->TxXferCount++;			//��ַ֡Ҳ��Ϊ���ݼ���
        US5CON3 = *(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount);             //TWI��������
        return Status_BUSY;
      }
      else if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_MasterModeStop();
        USCI5_HANDLE->TxState = USCI5_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_MasterModeStop();
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI5_GetTWIStatus(USCI5_TWI_MasterReceivedaUACK) == SET)
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_HANDLE->TxXferCount++;
      USCI5_TWI_MasterModeStop();
      if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
      {
        USCI5_HANDLE->TxState = USCI5_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI5_TWI_MasterModeStop();
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI5_HANDLE->TxState == USCI5_STATE_WAIT)
  {
    //��ַ�ɹ���Ӧ
    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;
    US5CON3 = *(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Slave_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ӻ�ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
* ע
*��ڲ���:*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI5_TWI_Slave_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x10) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI5();

      USCI5_TWI_AcknowledgeConfig(DISABLE);         //����AA
      USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI5_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI5_HANDLE->TxState = USCI5_STATE_WAIT;			//���ͽ���æµ��
      USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI5_TWI_AcknowledgeConfig(ENABLE);         //����AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Slave_Transmit_IRQHandler(USCI5_HandleInfoDef *USCI5_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�·��ʹ�������
*��ڲ���:*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Slave_Transmit_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  if(USCI5_HANDLE->TxState == USCI5_STATE_BUSY)
  {

    if(USCI5_GetTWIStatus(USCI5_TWI_SlaveSendData) == SET)
    {
      if(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize)
      {
        USCI5_HANDLE->TxXferCount++;			//��һ���ж��ǽ��ܵ���ַ��
        US5CON3 = *(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount);
        if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize - 1)
        {
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        }
        return Status_BUSY;
      }
      else
      {
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.�����ݴ�������У����������޸���AA����λ
    2.���յ�����������UACK */
    else if((USCI5_GetTWIStatus(USCI5_TWI_SlaveDisableACK) == SET) || (USCI5_GetTWIStatus(USCI5_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI5_HANDLE->TxXferCount++;
      if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)			//���������ѷ���
      {
        USCI5_HANDLE->TxState = USCI5_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �������δ���͹��̳���
      1.���ݷ��ͺ���ܵ�UACK
      2.TWI���ǹ����ڴӻ�����״̬ */
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI5_HANDLE->TxState == USCI5_STATE_WAIT)
  {
    if(USCI5_GetTWIStatus(USCI5_TWI_SlaveSendData) == SET)
    {
      US5CON3 = *(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount);
      USCI5_HANDLE->TxState = USCI5_STATE_BUSY;
    }
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Master_Receive(USCI5_HandleInfoDef* USCI5_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*��������:����8λ����ʱ��������ѯģʽ�½��մ�������
*��ڲ���:*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
					 Timeout    ��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Master_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI5();
    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI5_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    USCI5_TWI_Start();			//���������ź�
    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      USCI5_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI5_GetTWIStatus(USCI5_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
      USCI5_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    US5CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ
    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI5_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI5_GetTWIStatus(USCI5_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI5_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* ���ݽ��չ��� */
    do
    {
      if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI5_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI5_GetTWIStatus(USCI5_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
      {
        USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI5_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;             //TWI��������
      USCI5_HANDLE->RxXferCount++;
      if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize - 1)
      {
        USCI5_TWI_AcknowledgeConfig(DISABLE);                                     //�ر�AA
        if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI5_TWI_MasterModeStop();
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_TIMEOUT;
        }
        if(USCI5_GetTWIStatus(USCI5_TWI_MasterReceivedaUACK) == RESET)           //���״̬��״̬
        {
          USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI5_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;             //TWI��������
        USCI5_HANDLE->RxXferCount++;
      }
    }
    while(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize);
    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI5_TWI_MasterModeStop();

    if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
    {
      USCI5_HANDLE->RxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Slave_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�½��մ�������
*��ڲ���:
USCI5_HandleInfoDef:*USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Slave_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI5();
    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI5_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    /* ���յ�ַ���� */
    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_TIMEOUT;
    }

    if(USCI5_GetTWIStatus(USCI5_TWI_SlaveReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }

    /* �������ݹ��� */
    do
    {
      if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        return Status_TIMEOUT;
      }

      if(USCI5_GetTWIStatus(USCI5_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
      {
        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;             //TWI��������
        USCI5_HANDLE->RxXferCount++;
        if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize - 1)
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UAC
      }
      else if(USCI5_GetTWIStatus(USCI5_TWI_SlaveIdle) == SET)
      {
        if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize - 1)
        {
          *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;
          USCI5_HANDLE->RxXferCount++;
        }
        else
        {
          USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_ERROR;
        }
      }
      else
      {
        USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        return Status_ERROR;
      }

    }
    while(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize);

    if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
    {
      USCI5_HANDLE->RxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Master_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI5_TWI_Master_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x10) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI5();
    IE2 &= ~0x10;	//�ر��ж�
    USCI5_HANDLE->RxState = USCI5_STATE_WAIT;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI5_TWI_AcknowledgeConfig(ENABLE);         //����AA
    USCI5_TWI_Start();			//���������ź�

    if(USCI5_TWI_Wait_TWIF(USCI5_HANDLE, 0xFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI5_TWI_MasterModeStop();
      IE2 |= 0x10;	//�����ж�
      return Status_TIMEOUT;
    }

    if(USCI5_GetTWIStatus(USCI5_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI5_TWI_MasterModeStop();
      IE2 |= 0x10;	//�����ж�
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    IE2 |= 0x10;	//�����ж�
    US5CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Master_Receive_IRQHandler(USCI5_HandleInfoDef *USCI5_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�½��մ�������
*��ڲ���:*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Master_Receive_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  if(USCI5_HANDLE->RxState == USCI5_STATE_BUSY)
  {
    if(USCI5_GetTWIStatus(USCI5_TWI_MasterReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize)
      {
        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;             //TWI��������
        USCI5_HANDLE->RxXferCount++;
        if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI5_TWI_MasterModeStop();
          USCI5_HANDLE->RxState = USCI5_STATE_READY;	//�������
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI5_TWI_MasterModeStop();
        USCI5_HANDLE->RxState = USCI5_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else if(USCI5_HANDLE->RxState == USCI5_STATE_WAIT)
  {
    if(USCI5_GetTWIStatus(USCI5_TWI_MasterReceivedaData) == SET)
    {
      USCI5_HANDLE->RxState = USCI5_STATE_BUSY;
    }
    return Status_BUSY;
  }
  return Status_ERROR;
}


/*****************************************************
*��������:void USCI5_TWI_Slave_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size))
*��������:�ӻ�ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
		   *pData	ָ��ָ��洢��
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI5_TWI_Slave_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x10) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI5();
    USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI5_ClearFlag(USCI5_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI5_HANDLE->RxState = USCI5_STATE_WAIT;			//���ͽ���æµ��
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI5_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_TWI_Slave_Receive_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�½��մ�������
*��ڲ���:
USCI5_HandleInfoDef:*USCI5_HANDLE ָ��ָ��USCI5������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI5_TWI_Slave_Receive_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  if(USCI5_HANDLE->RxState == USCI5_STATE_BUSY)
  {
    if(USCI5_GetTWIStatus(USCI5_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize)
      {
        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;             //TWI��������
        USCI5_HANDLE->RxXferCount++;
        if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize - 1)
        {
          USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        }
      }
      else
      {
        USCI5_TWI_AcknowledgeConfig(DISABLE);
        return Status_ERROR;
      }
      return Status_BUSY;
    }
    else if(USCI5_GetTWIStatus(USCI5_TWI_SlaveIdle) == SET)
    {
      if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize - 1)
      {
        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = US5CON3;
        USCI5_HANDLE->RxXferCount++;
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        USCI5_HANDLE->RxState = USCI5_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
      USCI5_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }
  }
  else if(USCI5_HANDLE->RxState == USCI5_STATE_WAIT)
  {
    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;
  }
}

/******************************* UART���� *************************************/
/**************************************************
*��������:void USCI5_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI5_UART_Mode_TypeDef Mode, USCI5_UART_RX_TypeDef RxMode)
*��������:UART��ʼ�����ú���
*��ڲ���:
uint32_t:UARTFsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
USCI5_UART_Mode_TypeDef:Mode:UART1����ģʽ
USCI5_UART_RX_TypeDef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void USCI5_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI5_UART_Mode_TypeDef Mode,
                     USCI5_UART_RX_TypeDef RxMode)
{
  Select_USCI5();//ѡ�����USCI5
  USCI5_SetUART();//USCI5����ΪUARTģʽ
  US5CON0 = US5CON0 & 0X0F | Mode | RxMode;

  if(Mode == USCI5_UART_Mode_8B)
  {
    if(BaudRate == USCI5_UART_BaudRate_FsysDIV12)
    {
      US5CON0 &= 0XDF;
    }
    else if(BaudRate == USCI5_UART_BaudRate_FsysDIV4)
    {
      US5CON0 |= 0X20;
    }
  }
  else
  {
    US5CON2 = UARTFsys / BaudRate / 256;
    US5CON1 = UARTFsys / BaudRate % 256;
  }
}

/*****************************************************
*��������:void USCI5_UART_SendData8(uint8_t Data)
*��������:USCI5 UART1����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI5_UART_SendData8(uint8_t Data)
{
  US5CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI5_UART_ReceiveData8(void)
*��������:���US5CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI5_UART_ReceiveData8(void)
{
  return US5CON3;
}

/*****************************************************
*��������:void USCI5_UART_SendData9(uint16_t Data)
*��������:UART����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI5_UART_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if(Data_9Bit)
  {
    US5CON0 |= 0x08;
  }
  else
  {
    US5CON0 &= 0xf7;
  }

  US5CON3 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI5_UART_ReceiveData9(void)
*��������:���US5CON3�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI5_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US5CON3 + ((uint16_t)(US5CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*��������:StatusTypeDef  USCI5_UART_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI5_UART_Transmit(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  uint32_t delaytime = 0;
  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* ��鷢���������Ƿ�Ϊ0 */
    if((IE2 & 0x10 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_UART_FLAG_TI); //����ǰ��������жϱ�־��д1/д0����
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US5CON0 & 0xC0) == USCI5_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI5_UART_SendData9(*(USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI5_UART_SendData8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount));
      }

      /* �ȴ�������� */
      delaytime = 0;
      while(!(US5CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI5_HANDLE->TxState = USCI5_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
      }

      USCI5_ClearFlag(USCI5_UART_FLAG_TI); //�����жϱ�־�����д1/д0����
      USCI5_HANDLE->TxXferCount++;	//��������������
    }

    if(USCI5_HANDLE->TxXferCount == USCI5_HANDLE->TxXferSize)
    {
      USCI5_HANDLE->TxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_UART_Transmit_IT(USCI5_HandleInfoDef *USCI5_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI5_UART_Transmit_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_READY)
  {
    /* û�п���USCI5�жϻ�����������СΪ0�����ش��� */
    if(((IE2 & 0x10) == 0) || (USCI5_HANDLE->TxXferSize == 0U))
    {
      return Status_ERROR;
    }

    USCI5_HANDLE->TxState = USCI5_STATE_BUSY;
    USCI5_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->TxXferCount = 0;			//���ͼ���������

    /* ���͵�1֡���� */
    if((US5CON0 & 0xC0) == USCI5_UART_Mode_11B)
    {
      /* ����9λ���� */
      USCI5_UART_SendData9(*(USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount));
    }
    else
    {
      /* ����8λ���� */
      USCI5_UART_SendData8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef  USCI5_UART_Transmit_IRQHandler(USCI5_HandleInfoDef *USCI5_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:*USCI5_HANDLE ָ��ָ��UART������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI5_UART_Transmit_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  Select_USCI5();//ѡ�����USCI5

  /* ���ڷ����߳� */
  if(USCI5_HANDLE->TxState == USCI5_STATE_BUSY)
  {
    USCI5_HANDLE->TxXferCount++;
    /* �ж���һ�η����Ƿ������һ�� */
    if(USCI5_HANDLE->TxXferCount < USCI5_HANDLE->TxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US5CON0 & 0xC0) == USCI5_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI5_UART_SendData9(*(USCI5_HANDLE->pTxBuffPtr.Size_u16 + USCI5_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI5_UART_SendData8(*(USCI5_HANDLE->pTxBuffPtr.Size_u8 + USCI5_HANDLE->TxXferCount));
      }
      return Status_OK;
    }
    else if(USCI5_HANDLE->TxXferCount)
    {
      /* ������� */
      USCI5_HANDLE->TxState = USCI5_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI5_HANDLE->TxState = USCI5_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_UART_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
USCI5_USCI5_HANDLEInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5_UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI5_UART_Receive(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  Select_USCI5();//ѡ�����USCI5

  /* �����ս����������� */
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if((IE2 & 0x10 != 0) || (Size == 0U) || ((US5CON0 & 0x10) != USCI5_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;			//���ͽ���æµ��
    USCI5_ClearFlag(USCI5_UART_FLAG_RI);			 //����ǰ��������жϱ�־
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize)  //�ж��Ƿ������������
    {
      if(USCI5_GetFlagStatus(USCI5_UART_FLAG_RI))  //�жϽ��ձ�־λ
      {
        USCI5_ClearFlag(USCI5_UART_FLAG_RI);			//���ձ�־λ����

        /* ������������δΪ0���������� */
        if((US5CON0 & 0xC0) == USCI5_UART_Mode_11B)
        {
          /* ����9λ���� */
          *(USCI5_HANDLE->pRxBuffPtr.Size_u16 + USCI5_HANDLE->RxXferCount) = USCI5_UART_ReceiveData9();
        }
        else
        {
          /* ����8λ���� */
          *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = USCI5_UART_ReceiveData8();
        }

        USCI5_HANDLE->RxXferCount++;	//��������������
        delaytime = 0;//�յ����ݣ���ʱ��������
      }
      if(delaytime++ > Timeout) //��ʱ���
      {
        USCI5_HANDLE->RxState = USCI5_STATE_TIMEOUT;//���ͳ�ʱ
        return Status_TIMEOUT;
      }
    }

    if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
    {
      USCI5_HANDLE->RxState = USCI5_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_UART_Receive_IT(USCI5_HandleInfoDef *USCI5_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI5_USCI5_HANDLEInfoDef*:USCI5_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI5_UART_Receive_IT(USCI5_HandleInfoDef* USCI5_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI5_HANDLE->RxState == USCI5_STATE_READY)
  {
    /* ����Ƿ���USCI5_UART�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
    if(((IE2 & 0x10) == 0) || (Size == 0U) || ((US5CON0 & 0x10) != USCI5_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI5_HANDLE->RxState = USCI5_STATE_BUSY;
    USCI5_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
    USCI5_HANDLE->RxXferSize = Size;			//���´����յ�������
    USCI5_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI5_UART_Receive_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI5_HandleInfoDef*:USCI5_HANDLE:ָ��ָ��USCI5������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI5_UART_Receive_IRQHandler(USCI5_HandleInfoDef* USCI5_HANDLE)
{
  Select_USCI5();//ѡ�����USCI5

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI5_HANDLE->RxState == USCI5_STATE_BUSY)
  {
    /* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
    if(USCI5_HANDLE->RxXferCount < USCI5_HANDLE->RxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US5CON0 & 0xC0) == USCI5_UART_Mode_11B)
      {
        /* ����9λ���� */
        *(USCI5_HANDLE->pRxBuffPtr.Size_u16 + USCI5_HANDLE->RxXferCount) = USCI5_UART_ReceiveData9();
      }
      else
      {
        /* ����8λ���� */
        *(USCI5_HANDLE->pRxBuffPtr.Size_u8 + USCI5_HANDLE->RxXferCount) = USCI5_UART_ReceiveData8();
      }
      USCI5_HANDLE->RxXferCount++;
      /* �ж���һ�η����Ƿ������һ�� */
      if(USCI5_HANDLE->RxXferCount == USCI5_HANDLE->RxXferSize)
      {
        /* ������� */
        USCI5_HANDLE->RxState = USCI5_STATE_READY;
        return Status_OK;
			}
        return Status_ERROR;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI5_HANDLE->RxState = USCI5_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:FlagStatus USCI5_GetFlagStatus(USCI5_Flag_TypeDef USCI5_FLAG)
*��������:���USCI5��־״̬
*��ڲ���:
USCI5_Flag_TypeDef:USCI5_FLAG:�����ȡ�ı�־λ
*���ڲ���:
FlagStatus:USCI5��־״̬
*****************************************************/
FlagStatus USCI5_GetFlagStatus(USCI5_Flag_TypeDef USCI5_FLAG)
{
  FlagStatus bitstatus = RESET;
  Select_USCI5();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
	  if((USCI5_FLAG == USCI5_SPI_FLAG_SPIF) || (USCI5_FLAG == USCI5_SPI_FLAG_WCOL) || (USCI5_FLAG == USCI5_SPI_FLAG_TXE))
		{
			if((USCI5_FLAG & US5CON1) != (uint8_t)RESET)
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
		if(USCI5_FLAG == USCI5_TWI_FLAG_TXRXnE)
		{
			if((USCI5_FLAG & US5CON1) != (uint8_t)RESET)
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
			if((USCI5_FLAG & US5CON0) != (uint8_t)RESET)
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
    if((USCI5_FLAG & US5CON0) != (uint8_t)RESET)
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
*��������:void USCI5_ClearFlag(USCI5_Flag_TypeDef USCI5_FLAG)
*��������:���USCI5��־״̬
*��ڲ���:
USCI5_Flag_TypeDef:USCI5_FLAG:��������ı�־λ
*���ڲ���:void
*****************************************************/
void USCI5_ClearFlag(USCI5_Flag_TypeDef USCI5_FLAG)
{
  Select_USCI5();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
	  if((USCI5_FLAG == USCI5_SPI_FLAG_SPIF) || (USCI5_FLAG == USCI5_SPI_FLAG_WCOL) || (USCI5_FLAG == USCI5_SPI_FLAG_TXE))
		{
		  US5CON1 &= (~USCI5_FLAG); //�����Ĵ���US5CON1	
		}
	}else if((TMCON & 0xC0)==0x80)  //TWIģʽ
	{
	  if(USCI5_FLAG == USCI5_TWI_FLAG_TXRXnE)
    {
      US5CON1 &= (~USCI5_FLAG); //�����Ĵ���US5CON1	
    }
		else
    {
       US5CON0 &= (~USCI5_FLAG); //�����Ĵ���US5CON0
    }	
	}else if((TMCON & 0xC0)==0xC0)  //UARTģʽ
	{
	  if((USCI5_FLAG == USCI5_UART_FLAG_TI) || (USCI5_FLAG == USCI5_UART_FLAG_RI))
    {
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605)
     US5CON0 = US5CON0  & 0xFC | USCI5_FLAG;//д1����
#else
     US5CON0 &= (~USCI5_FLAG); //д0����
#endif
    }	
	}
}

/*****************************************************
*��������:void USCI5_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:USCI5�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void USCI5_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if(NewState != DISABLE)
  {
    IE2 |= 0x10;
  }
  else
  {
    IE2 &= 0xEF;
  }

  /************************************************************/
  if(Priority != LOW)
  {
    IP2 |= 0x10;
  }
  else
  {
    IP2 &= 0xEF;
  }
}

#endif
/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/

