//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_USCI1.c
//	����		:
//	ģ�鹦��: 		USCI1�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��:���ļ���������SC95Fϵ��оƬ
//*************************************************************

#include "sc95f_usci1.h"

/**************************************************
*��������:void USCI1_DeInit(void)
*��������:USCI1��ؼĴ�����λ��ȱʡֵ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI1_DeInit(void)
{
  OTCON &= 0X3F;
  US1CON0 = 0X00;
  US1CON1 = 0X00;
  US1CON2 = 0X00;
  US1CON3 = 0X00;
  IE2 &= (~0X01);
  IP2 &= (~0X01);
}

/**************************************************
*��������:void USCI1_SPI_Init(USCI1_SPI_FirstBit_TypeDef FirstBit, USCI1_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI1_SPI_Mode_TypeDef Mode,
							 USCI1_SPI_ClockPolarity_TypeDef ClockPolarity, USCI1_SPI_ClockPhase_TypeDef ClockPhase,USCI1_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI1_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI��ʼ�����ú���
*��ڲ���:
USCI1_SPI_FirstBit_TypeDef:FirstBit:���ȴ���λѡ��MSB/LSB��
USCI1_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPIʱ��Ƶ��ѡ��
USCI1_SPI_Mode_TypeDef:Mode:SPI����ģʽѡ��
USCI1_SPI_ClockPolarity_TypeDef:ClockPolarity:SPIʱ�Ӽ���ѡ��
USCI1_SPI_ClockPhase_TypeDef:ClockPhase:SPIʱ����λѡ��
USCI1_SPI_TXE_INT_TypeDef:SPI_TXE_INT:���ͻ������ж�����ѡ��,�ù�����RD8XX1XоƬ����Ч
USCI1_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16λ
*���ڲ���:void
**************************************************/
void USCI1_SPI_Init(USCI1_SPI_FirstBit_TypeDef FirstBit,
                    USCI1_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI1_SPI_Mode_TypeDef Mode,
                    USCI1_SPI_ClockPolarity_TypeDef ClockPolarity, USCI1_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI1_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI1_TransmissionMode_TypeDef TransmissionMode)
{

  OTCON = (OTCON & 0X3F) | 0X40;
  US1CON1 = US1CON1 & (~0X05) | FirstBit | SPI_TXE_INT | TransmissionMode;
  US1CON0 = US1CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*��������:void USCI1_TransmissionMode(USCI1_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI ����ģʽ���ú���
*��ڲ���:
USCI1_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16eλ
*���ڲ���:void
**************************************************/
void USCI1_TransmissionMode(USCI1_TransmissionMode_TypeDef TransmissionMode)
{
  if(TransmissionMode == USCI1_SPI_DATA8)
  {
    US1CON1 &= 0xFD;
  }
  else
  {
    US1CON1 |= 0x02;
  }
}

/*****************************************************
*��������:void USCI1_SPI_Cmd(FunctionalState NewState)
*��������:SPI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI1_SPI_Cmd(FunctionalState NewState)
{
  OTCON = (OTCON & 0X3F) | 0X40;

  if(NewState != DISABLE)
  {
    US1CON0 |= 0X80;
  }
  else
  {
    US1CON0 &= (~0X80);
  }
}
/*****************************************************
*��������:void USCI1_SPI_SendData_8(uint8_t Data)
*��������:USCI1 SPI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI1_SPI_SendData_8(uint8_t Data)
{
  US1CON2 = Data;
}

/*****************************************************
*��������:uint8_t USCI1_SPI_ReceiveData_8(void)
*��������:���US1CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI1_SPI_ReceiveData_8(void)
{
  return US1CON2;
}

/*****************************************************
*��������:void USCI1_SPI_SendData_16(uint16_t Data)
*��������:US1CON2 SPI��������
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI1_SPI_SendData_16(uint16_t Data)
{
  US1CON3 = (uint8_t)(Data >> 8);
  US1CON2 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI1_SPI_ReceiveData_16(void)
*��������:���US1CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI1_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  SPI_data = (uint16_t)((US1CON3 << 8) | US1CON2);
  return SPI_data;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
*��������:SPI0 �жϴ���
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ����� SPI0 ��Ϣ�� USCI1_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
void
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  if(USCI1_HANDLE->RxState == USCI1_STATE_BUSY)
  {
    /* ������û�н������ */
    if(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize)
    {
      /* SPI����16λͨ��ģʽ */
      if((US1CON1 & USCI1_SPI_DATA16) != 0)
      {
        *(USCI1_HANDLE->pRxBuffPtr.Size_u16 + USCI1_HANDLE->RxXferCount) = USCI1_SPI_ReceiveData_16();			//��ȡ16λ����
      }
      /* SPI����8λͨ��ģʽ */
      else
      {
        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = USCI1_SPI_ReceiveData_8();			//��ȡ16λ����
      }

      TempStatus = Status_BUSY;
      USCI1_HANDLE->RxXferCount++;			//���յ����ݣ�����ֵ��1

      /* ���ݽ�����ɣ�״̬λ�����޸� */
      if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
      {
        USCI1_HANDLE->RxState = USCI1_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI1_HANDLE->TxState == USCI1_STATE_BUSY)
    {

      USCI1_HANDLE->TxXferCount++;			//���ݷ�����ɣ�����ֵ��1
      /* ������û�з������ */
      if(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize)
      {
        /* SPI����16λͨ��ģʽ */
        if((US1CON1 & USCI1_SPI_DATA16) != 0)
        {
          USCI1_SPI_SendData_16(*(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount));			//����16λ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          USCI1_SPI_SendData_8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount));			//����16λ����
        }
        TempStatus = Status_BUSY;
      }
      /* ���ݷ�����ɣ�״̬λ�����޸�  */
      else if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
      {
        USCI1_HANDLE->TxState = USCI1_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* ��SPI��������ʱ��SPI��Ҫ�������ݣ�����ͬ�����յ����� */
    else if(USCI1_HANDLE->RxXferSize)
    {
      USCI1_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}


/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�½��մ�������
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI1 ״̬
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) != 0) || (Size == 0U))
    {
      return USCI1_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US1CON0 & USCI1_SPI_MODE_MASTER) != 0)
    {
      return USCI1_SPI_TransmitReceive(USCI1_HANDLE, pData, pData, Size, Timeout);			//��ת������ģʽ�����շ�����
    }

    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);			 //����ǰ��������жϱ�־
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize)
    {
      /* �ȴ�SPI�жϱ�־λ���� */
      if(USCI1_GetFlagStatus(USCI1_SPI_FLAG_SPIF))
      {
        /* SPI����16λͨ��ģʽ */
        if((US1CON1 & USCI1_SPI_DATA16) != 0)
        {
          *(USCI1_HANDLE->pRxBuffPtr.Size_u16 + USCI1_HANDLE->RxXferCount) = USCI1_SPI_ReceiveData_16();			//��ȡ16λ����,���ҵ�ַ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = USCI1_SPI_ReceiveData_8();			//��ȡ16λ����,���ҵ�ַ����
        }
        USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);				//�����־λ
        USCI1_HANDLE->RxXferCount++;	//����������������1
        TimeoutCnt = 0;			//��ʱ����ֵ����
      }
      else
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
            return USCI1_STATE_TIMEOUT;				//���س�ʱ����
        }
        WDTCON |= 0x10;          //ι����������ֹ���Ź���λ
      }
    }

    if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
    {
      USCI1_HANDLE->RxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;//���մ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:USCI1 ״̬
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return USCI1_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US1CON0 & USCI1_SPI_MODE_MASTER) != 0)
    {
      return USCI1_SPI_TransmitReceive_IT(USCI1_HANDLE, pData, pData, Size);			//��ת���ж�ģʽ�����շ�����
    }

    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//״̬����Ϊ����æµ��
    USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
uint32_t Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI1 ״̬
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->TxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);			 //����ǰ�����־λ
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize)//�ж��Ƿ������������
    {
      if((US1CON1 & USCI1_SPI_DATA16) != 0)/* SPI����16λͨ��ģʽ */
      {
        USCI1_SPI_SendData_16(*(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount));			//����16λ���ݲ������ݵ�ַ����
      }
      else/* SPI����8λͨ��ģʽ */
      {
        USCI1_SPI_SendData_8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount));			//����8λ���ݲ������ݵ�ַ����
      }

      while(!USCI1_GetFlagStatus(USCI1_SPI_FLAG_SPIF))			//�ȴ��������
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if(TimeoutCnt++ > Timeout)
        {
          USCI1_HANDLE->TxState = USCI1_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }

      USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);				//����ǰ�������־λ
      TimeoutCnt = 0;			//��ʱ����������
      USCI1_HANDLE->TxXferCount ++;	//��������������
    }
    if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
    {
      USCI1_HANDLE->TxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI1_HandleInfoDef *USCI1_HANDLE:ָ����� SPI0 ��Ϣ�� USCI1_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI1 ״̬
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* û�п���USCI1�жϻ�����������СΪ0�����ش��� */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->TxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = 0;			//���ͼ�������0

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US1CON1 & USCI1_SPI_DATA16) != 0)
    {
      USCI1_SPI_SendData_16(*(USCI1_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI1_SPI_SendData_8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//״̬λ�޸�Ϊæµ״̬
  }
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_TransmitReceive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*��������:����ģʽ�շ�һ��������
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ����� SPI0 ��Ϣ�� USCI1_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
StatusTypeDef:USCI1 ״̬
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_TransmitReceive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if((USCI1_HANDLE->TxState == USCI1_STATE_READY) && (USCI1_HANDLE->RxState == USCI1_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI1_HANDLE->TxState = USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //ָ����������ݵĵ�ַ
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI1_HANDLE->TxXferSize = USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize);
    {

      if((US1CON1 & USCI1_SPI_DATA16) != 0)
      {
        USCI1_SPI_SendData_16(*(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI1_GetFlagStatus(USCI1_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            return USCI1_STATE_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount) = USCI1_SPI_ReceiveData_16();
      }
      else
      {
        USCI1_SPI_SendData_8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI1_GetFlagStatus(USCI1_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            return USCI1_STATE_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount) = USCI1_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//��ʱ����ֵ����
      USCI1_HANDLE->TxXferCount++;			//����������ֵ��1
      USCI1_HANDLE->TxXferCount++;			//����������ֵ��1
    }
    if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
    {
      USCI1_HANDLE->TxState = USCI1_HANDLE->RxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }

}
/**********************************************************************************************************
*��������:StatusTypeDef USCI1_SPI_TransmitReceive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
*��������:SPI���ж�ģʽ�·��ͺͽ��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI1_HandleInfoDef *USCI1_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:���ͺͽ������ݵĴ����
uint16_t:Size:�����ͺͽ��յ�������
*���ڲ���:
StatusTypeDef:USCI1 ״̬
**********************************************************************************************************/
StatusTypeDef USCI1_SPI_TransmitReceive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
  if((USCI1_HANDLE->TxState == USCI1_STATE_READY) && (USCI1_HANDLE->RxState == USCI1_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }
    USCI1_SPI_Cmd(DISABLE);
    USCI1_HANDLE->TxState = USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->TxXferSize = USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI1_SPI_Cmd(ENABLE);

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US1CON1 & USCI1_SPI_DATA16) != 0)
    {
      USCI1_SPI_SendData_16(*(USCI1_HANDLE->pTxBuffPtr.Size_u16));			//����16λ����
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI1_SPI_SendData_8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8));			//����8λ����
    }
    /* ״̬����æµ��״̬ */

    return  Status_OK;
  }
  else
  {
    return Status_ERROR;
  }
}

/**************************************************
*��������:void USCI1_TWI_Slave_Init(uint8_t TWI_Address)
*��������:USCI1 TWI�ӻ���ʼ�����ú���
*��ڲ���:
uint8_t:TWI_Address:TWI��Ϊ�ӻ�ʱ��7λ�ӻ���ַ
*���ڲ���:void
**************************************************/
void USCI1_TWI_Slave_Init(uint8_t TWI_Address)
{
  OTCON = OTCON & 0X3F | 0X80;
  US1CON2 = TWI_Address << 1;
}

/**************************************************
*��������:void USCI1_TWI_MasterCommunicationRate(USCI1_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*��������:USCI1 TWI����ģʽ��ͨѶ�����趨
*��ڲ���:
USCI1_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI����ģʽ��ͨѶ����
*���ڲ���:void
**************************************************/
void USCI1_TWI_MasterCommunicationRate(USCI1_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  OTCON = OTCON & 0X3F | 0X80;
  US1CON1 |= TWI_MasterCommunicationRate;
}

/**************************************************
*��������:void USCI1_TWI_Start(void)
*��������:USCI1 TWI ��ʼλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI1_TWI_Start(void)
{
  US1CON1 |= 0x20;
}

/**************************************************
*��������:void USCI1_TWI_MasterModeStop(void)
*��������:USCI1 TWI����ģʽֹͣλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI1_TWI_MasterModeStop(void)
{
  US1CON1 |= 0x10;
}

/**************************************************
*��������:void USCI1_TWI_SlaveClockExtension(FunctionalState NewState)
*��������:USCI1 TWI�ӻ�ģʽʱ���ӳ�����λ
*��ڲ���:
FunctionalState:NewState:ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI1_TWI_SlaveClockExtension(FunctionalState NewState)
{
  OTCON = OTCON & 0X3F | 0X80;

  if(NewState != DISABLE)
  {
    US1CON1 |= 0x40;
  }
  else
  {
    US1CON1 &= 0XBF;
  }
}

/**************************************************
*��������:void USCI1_TWI_AcknowledgeConfig(FunctionalState NewState)
*��������:TWI����Ӧ��ʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI1_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  OTCON = OTCON & 0X3F | 0X80;

  if(NewState != DISABLE)
  {
    US1CON0 |= 0X08;
  }
  else
  {
    US1CON0 &= 0XF7;
  }
}

/**************************************************
*��������:void USCI1_TWI_GeneralCallCmd(FunctionalState NewState)
*��������:TWIͨ�õ�ַ��Ӧʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI1_TWI_GeneralCallCmd(FunctionalState NewState)
{
  OTCON = OTCON & 0X3F | 0X80;

  if(NewState != DISABLE)
  {
    US1CON2 |= 0X01;
  }
  else
  {
    US1CON2 &= 0XFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI1_GetTWIStatus(USCI1_TWIState_TypeDef USCI1_TWIState)
*��������:��ȡTWI״̬
*��ڲ���:
USCI1_TWIState_TypeDef:USCI1_TWIState:TWI״̬����
*���ڲ���:
FlagStatus:USCI1_TWI��־״̬
*****************************************************/
FlagStatus USCI1_GetTWIStatus(USCI1_TWIState_TypeDef USCI1_TWIState)
{
  if((US1CON0 & 0x07) == USCI1_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*��������:void USCI1_TWI_Cmd(FunctionalState NewState)
*��������:TWI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI1_TWI_Cmd(FunctionalState NewState)
{
  OTCON = OTCON & 0X3F | 0X80;

  if(NewState != DISABLE)
  {
    US1CON0 |= 0X80;
  }
  else
  {
    US1CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI1_TWI_SendAddr(uint8_t Addr,USCI1_TWI_RWType RW)
*��������:TWI���͵�ַ����д����
*��ڲ���:
uint8_t:Addr:���͵ĵ�ַ 0~127
USCI1_TWI_RWType:RW:��д����
*���ڲ���:void
*****************************************************/
void USCI1_TWI_SendAddr(uint8_t Addr, USCI1_TWI_RWType RW)
{
  US1CON3 = (Addr << 1) | RW;
}

/*****************************************************
*��������:void USCI1_TWI_SendData(uint8_t Data)
*��������:TWI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI1_TWI_SendData(uint8_t Data)
{
  US1CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI1_TWI_SendData(void)
*��������:���US1CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI1_TWI_ReceiveData(void)
{
  return US1CON3;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI1_TWI_Wait_TWIF(USCI1_HandleInfoDef* USCI1_HANDLE,uint32_t Timeout)
*�������ܣ��ȴ�TWIF����
*��ڲ�����
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Wait_TWIF(USCI1_HandleInfoDef* USCI1_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  while(!(USCI1_GetFlagStatus(USCI1_TWI_FLAG_TWIF)))			//�ȴ������źŷ������
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* ��ʱ����״̬ */
      if(USCI1_HANDLE->TxState == USCI1_STATE_BUSY)
        USCI1_HANDLE->TxState = USCI1_STATE_TIMEOUT;
      if(USCI1_HANDLE->RxState == USCI1_STATE_BUSY)
        USCI1_HANDLE->RxState = USCI1_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			//�����־λ
  return Status_OK;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI1_TWI_Master_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ܣ�����8λ����ʱ��������ѯģʽ�·��ʹ�������
*��ڲ�����
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*��pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Master_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->TxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI1_TWI_Start();			//���������ź�
    if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI1_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI1_GetTWIStatus(USCI1_TWI_MasterSendAddress) == RESET)          //���״̬��״̬
    {
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI1_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US1CON3 = (slaveAddr << 1) & 0xFE;			//���͵�ַ�Ͷ�дλ
    if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI1_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI1_GetTWIStatus(USCI1_TWI_MasterSendData) == RESET)          //���״̬��״̬
      {
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI1_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US1CON3 = *(USCI1_HANDLE->pTxBuffPtr.Size_u8  + USCI1_HANDLE->TxXferCount);             //TWI��������
      if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI1_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI1_HANDLE->TxXferCount++;
    }
    while(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize);

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI1_TWI_MasterModeStop();

    if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
    {
      USCI1_HANDLE->TxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }

  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Slave_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�·��ʹ�������
* ע��Ĭ�Ͽ���ʱ���ӳ�ģʽ
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Slave_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      USCI1_HANDLE->TxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
      USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI1_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI1_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
      if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ���ַƥ��
      {
        USCI1_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
        return Status_TIMEOUT;
      }

      do
      {
        if(USCI1_GetTWIStatus(USCI1_TWI_SlaveSendData) == SET)           //���״̬��״̬
        {

          US1CON3 = *(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount);             //TWI��������

          if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)             //�ȴ������źŷ������
          {
            USCI1_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
            return Status_TIMEOUT;
          }
          USCI1_HANDLE->TxXferCount++;

          if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize - 1)

            USCI1_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��

        }
        else if(USCI1_GetTWIStatus(USCI1_TWI_SlaveDisableACK) == SET)
        {

          USCI1_HANDLE->TxXferCount++;
        }
        else
        {
          USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
          return Status_ERROR;
        }
      }
      while(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize);

      if(USCI1_GetTWIStatus(USCI1_TWI_SlaveDisableACK) == RESET)           //���״̬��״̬
      {
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        return Status_ERROR;
      }

      if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
      {
        USCI1_HANDLE->TxState = USCI1_STATE_READY;	//�������
        return Status_OK;
      }
      else
      {
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Master_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
*���ڲ���:
StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Master_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      
      IE2 &= ~0x01;			//�ر��ж�

      USCI1_HANDLE->TxState = USCI1_STATE_WAIT;			//���ͽ���æµ��
      USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI1_TWI_Start();			//���������ź�

      if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI1_TWI_MasterModeStop();
        IE2 |= 0x01;	//�����ж�
        return Status_TIMEOUT;
      }

      if(USCI1_GetTWIStatus(USCI1_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
      {
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI1_TWI_MasterModeStop();
        IE2 |= 0x01;	//�����ж�
        return Status_ERROR;
      }

      IE2 |= 0x01;						//�����ж�
      US1CON3 = (slaveAddr << 1) & 0xFE;//���͵�ַ�Ͷ�дλ

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Master_Transmit_IRQHandler(USCI1_HandleInfoDef *USCI1_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Master_Transmit_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  if(USCI1_HANDLE->TxState == USCI1_STATE_BUSY)
  {
    if(USCI1_GetTWIStatus(USCI1_TWI_MasterSendData) == SET)
    {
      if(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize)
      {
        USCI1_HANDLE->TxXferCount++;			//��ַ֡Ҳ��Ϊ���ݼ���
        US1CON3 = *(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount);             //TWI��������
        return Status_BUSY;
      }
      else if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI1_TWI_MasterModeStop();
        USCI1_HANDLE->TxState = USCI1_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI1_TWI_MasterModeStop();
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI1_GetTWIStatus(USCI1_TWI_MasterReceivedaUACK) == SET)
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI1_HANDLE->TxXferCount++;
      USCI1_TWI_MasterModeStop();
      if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
      {
        USCI1_HANDLE->TxState = USCI1_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI1_TWI_MasterModeStop();
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI1_HANDLE->TxState == USCI1_STATE_WAIT)
  {
    //��ַ�ɹ���Ӧ
    USCI1_HANDLE->TxState = USCI1_STATE_BUSY;
    US1CON3 = *(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Slave_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ӻ�ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
* ע
*��ڲ���:*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI1_TWI_Slave_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      

      USCI1_TWI_AcknowledgeConfig(DISABLE);         //����AA
      USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI1_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI1_HANDLE->TxState = USCI1_STATE_WAIT;			//���ͽ���æµ��
      USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI1_TWI_AcknowledgeConfig(ENABLE);         //����AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Slave_Transmit_IRQHandler(USCI1_HandleInfoDef *USCI1_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�·��ʹ�������
*��ڲ���:*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Slave_Transmit_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  if(USCI1_HANDLE->TxState == USCI1_STATE_BUSY)
  {

    if(USCI1_GetTWIStatus(USCI1_TWI_SlaveSendData) == SET)
    {
      if(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize)
      {
        USCI1_HANDLE->TxXferCount++;			//��һ���ж��ǽ��ܵ���ַ��
        US1CON3 = *(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount);
        if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize - 1)
        {
          USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        }
        return Status_BUSY;
      }
      else
      {
        USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.�����ݴ�������У����������޸���AA����λ
    2.���յ�����������UACK */
    else if((USCI1_GetTWIStatus(USCI1_TWI_SlaveDisableACK) == SET) || (USCI1_GetTWIStatus(USCI1_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
		 USCI1_HANDLE->TxXferCount++;
      if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)			//���������ѷ���
      {
        USCI1_HANDLE->TxState = USCI1_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �������δ���͹��̳���
      1.���ݷ��ͺ���ܵ�UACK
      2.TWI���ǹ����ڴӻ�����״̬ */
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI1_HANDLE->TxState == USCI1_STATE_WAIT)
  {
    if(USCI1_GetTWIStatus(USCI1_TWI_SlaveSendData) == SET)
    {
      US1CON3 = *(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount);
      USCI1_HANDLE->TxState = USCI1_STATE_BUSY;
    }
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Master_Receive(USCI1_HandleInfoDef* USCI1_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*��������:����8λ����ʱ��������ѯģʽ�½��մ�������
*��ڲ���:*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
					 Timeout    ��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Master_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI1_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    USCI1_TWI_Start();			//���������ź�
    if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      USCI1_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI1_GetTWIStatus(USCI1_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
      USCI1_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    US1CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ
    if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI1_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI1_GetTWIStatus(USCI1_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI1_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* ���ݽ��չ��� */
    do
    {
      if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI1_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI1_GetTWIStatus(USCI1_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
      {
        USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI1_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;             //TWI��������
      USCI1_HANDLE->RxXferCount++;
      if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize - 1)
      {
        USCI1_TWI_AcknowledgeConfig(DISABLE);                                     //�ر�AA
        if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			  USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI1_TWI_MasterModeStop();
          return Status_TIMEOUT;
        }
        if(USCI1_GetTWIStatus(USCI1_TWI_MasterReceivedaUACK) == RESET)           //���״̬��״̬
        {
          USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			  USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI1_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;             //TWI��������
        USCI1_HANDLE->RxXferCount++;
      }
    }
    while(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize);
    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
	 USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI1_TWI_MasterModeStop();

    if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
    {
      USCI1_HANDLE->RxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Slave_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�½��մ�������
*��ڲ���:
USCI1_HandleInfoDef:*USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Slave_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI1_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    /* ���յ�ַ���� */
    if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_TIMEOUT;
    }

    if(USCI1_GetTWIStatus(USCI1_TWI_SlaveReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }

    /* �������ݹ��� */
    do
    {
      if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        return Status_TIMEOUT;
      }

      if(USCI1_GetTWIStatus(USCI1_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
      {
        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;             //TWI��������
        USCI1_HANDLE->RxXferCount++;
        if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize - 1)
          USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UAC
      }
      else if(USCI1_GetTWIStatus(USCI1_TWI_SlaveIdle) == SET)
      {
        if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize - 1)
        {
          *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;
          USCI1_HANDLE->RxXferCount++;
        }
        else
        {
          USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
          USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_ERROR;
        }
      }
      else
      {
        USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
        USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        return Status_ERROR;
      }

    }
    while(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize);

    if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
    {
      USCI1_HANDLE->RxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Master_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI1_TWI_Master_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    
    IE2 &= ~0x01;	//�ر��ж�
    USCI1_HANDLE->RxState = USCI1_STATE_WAIT;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI1_TWI_AcknowledgeConfig(ENABLE);         //����AA
    USCI1_TWI_Start();			//���������ź�

    if(USCI1_TWI_Wait_TWIF(USCI1_HANDLE, 0xFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI1_TWI_MasterModeStop();
      IE2 |= 0x01;	//�����ж�
      return Status_TIMEOUT;
    }

    if(USCI1_GetTWIStatus(USCI1_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
		 USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI1_TWI_MasterModeStop();
      IE2 |= 0x01;	//�����ж�
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    IE2 |= 0x01;	//�����ж�
    US1CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Master_Receive_IRQHandler(USCI1_HandleInfoDef *USCI1_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�½��մ�������
*��ڲ���:*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Master_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  if(USCI1_HANDLE->RxState == USCI1_STATE_BUSY)
  {
    if(USCI1_GetTWIStatus(USCI1_TWI_MasterReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize)
      {
        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;             //TWI��������
        USCI1_HANDLE->RxXferCount++;
        if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			  USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI1_TWI_MasterModeStop();
          USCI1_HANDLE->RxState = USCI1_STATE_READY;	//�������
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
			USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI1_TWI_MasterModeStop();
        USCI1_HANDLE->RxState = USCI1_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else if(USCI1_HANDLE->RxState == USCI1_STATE_WAIT)
  {
    if(USCI1_GetTWIStatus(USCI1_TWI_MasterReceivedaData) == SET)
    {
      USCI1_HANDLE->RxState = USCI1_STATE_BUSY;
    }
    return Status_BUSY;
  }
  return Status_ERROR;
}


/*****************************************************
*��������:void USCI1_TWI_Slave_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size))
*��������:�ӻ�ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
		   *pData	ָ��ָ��洢��
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI1_TWI_Slave_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    
    USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI1_ClearFlag(USCI1_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI1_HANDLE->RxState = USCI1_STATE_WAIT;			//���ͽ���æµ��
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI1_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_TWI_Slave_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�½��մ�������
*��ڲ���:
USCI1_HandleInfoDef:*USCI1_HANDLE ָ��ָ��USCI1������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI1_TWI_Slave_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  if(USCI1_HANDLE->RxState == USCI1_STATE_BUSY)
  {
    if(USCI1_GetTWIStatus(USCI1_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize)
      {
        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;             //TWI��������
        USCI1_HANDLE->RxXferCount++;
        if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize - 1)
        {
          USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        }
      }
      else
      {
        USCI1_TWI_AcknowledgeConfig(DISABLE);
        return Status_ERROR;
      }
      return Status_BUSY;
    }
    else if(USCI1_GetTWIStatus(USCI1_TWI_SlaveIdle) == SET)
    {
      if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize - 1)
      {
        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = US1CON3;
        USCI1_HANDLE->RxXferCount++;
        USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        USCI1_HANDLE->RxState = USCI1_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
      USCI1_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }
  }
  else if(USCI1_HANDLE->RxState == USCI1_STATE_WAIT)
  {
    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;
  }
}

/**************************************************
*��������:void USCI1_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI1_UART_Mode_TypeDef Mode, USCI1_UART_RX_TypeDef RxMode)
*��������:UART��ʼ�����ú���
*��ڲ���:
uint32_t:UARTFsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
USCI1_UART_Mode_TypeDef:Mode:UART1����ģʽ
USCI1_UART_RX_TypeDef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void USCI1_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI1_UART_Mode_TypeDef Mode,
                     USCI1_UART_RX_TypeDef RxMode)
{
  OTCON |= 0xC0;
  US1CON0 = US1CON0 & 0X0F | Mode | RxMode;

  if(Mode == USCI1_UART_Mode_8B)
  {
    if(BaudRate == USCI1_UART_BaudRate_FsysDIV12)
    {
      US1CON0 &= 0XDF;
    }
    else if(BaudRate == USCI1_UART_BaudRate_FsysDIV4)
    {
      US1CON0 |= 0X20;
    }
  }
  else
  {
    US1CON2 = UARTFsys / BaudRate / 256;
    US1CON1 = UARTFsys / BaudRate % 256;
  }
}

/*****************************************************
*��������:void USCI1_UART_SendData8(uint8_t Data)
*��������:USCI1 UART1����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI1_UART_SendData8(uint8_t Data)
{
  US1CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI1_UART_ReceiveData8(void)
*��������:���US1CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI1_UART_ReceiveData8(void)
{
  return US1CON3;
}

/*****************************************************
*��������:void USCI1_UART_SendData9(uint16_t Data)
*��������:UART1����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI1_UART_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if(Data_9Bit)
  {
    US1CON0 |= 0x08;
  }
  else
  {
    US1CON0 &= 0xf7;
  }

  US1CON3 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI1_UART_ReceiveData9(void)
*��������:���US1CON3�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI1_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US1CON3 + ((uint16_t)(US1CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*��������:StatusTypeDef  USCI1_UART_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI1_UART_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* ��鷢���������Ƿ�Ϊ0 */
    if(((IE2 & 0x01) != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI1_HANDLE->TxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_UART_FLAG_TI); //����ǰ��������жϱ�־��д1/д0����
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize)//�ж��Ƿ������������
    {
      if((US1CON0 & 0xC0) == USCI1_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI1_UART_SendData9(*(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI1_UART_SendData8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount));
      }

      /* �ȴ�������� */
      delaytime = 0;
      while(!(US1CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI1_HANDLE->TxState = USCI1_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
      }

      USCI1_ClearFlag(USCI1_UART_FLAG_TI); //�����жϱ�־�����д1/д0����
      USCI1_HANDLE->TxXferCount ++;	//��������������
    }

    if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
    {
      USCI1_HANDLE->TxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}


/*****************************************************
*��������:StatusTypeDef USCI1_UART_Transmit_IT(USCI1_HandleInfoDef *USCI1_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI1_UART_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_READY)
  {
    /* û�п���USCI1�жϻ�����������СΪ0�����ش��� */
    if(((IE2 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->TxState = USCI1_STATE_BUSY;
    USCI1_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->TxXferCount = 0;			//���ͼ���������

    /* ���͵�1֡���� */
    if((US1CON0 & 0xC0) == USCI1_UART_Mode_11B)
    {
      /* ����9λ���� */
      USCI1_UART_SendData9(*(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount));
    }
    else
    {
      /* ����8λ���� */
      USCI1_UART_SendData8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef  USCI1_UART_Transmit_IRQHandler(USCI1_HandleInfoDef *USCI1_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:*USCI1_HANDLE ָ��ָ��UART������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI1_UART_Transmit_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  /* ���ڷ����߳� */
  if(USCI1_HANDLE->TxState == USCI1_STATE_BUSY)
  {
    USCI1_HANDLE->TxXferCount++;
    /* �ж���һ�η����Ƿ������һ�� */
    if(USCI1_HANDLE->TxXferCount < USCI1_HANDLE->TxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US1CON0 & 0xC0) == USCI1_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI1_UART_SendData9(*(USCI1_HANDLE->pTxBuffPtr.Size_u16 + USCI1_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI1_UART_SendData8(*(USCI1_HANDLE->pTxBuffPtr.Size_u8 + USCI1_HANDLE->TxXferCount));
      }
      return Status_BUSY;
    }
    else if(USCI1_HANDLE->TxXferCount == USCI1_HANDLE->TxXferSize)
    {
      /* ������� */
      USCI1_HANDLE->TxState = USCI1_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI1_HANDLE->TxState = USCI1_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_UART_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
USCI1_USCI1_HANDLEInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1_UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI1_UART_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  /* �����ս����������� */
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if(((IE2 & 0x01) != 0) || (Size == 0U) || ((US1CON0 & 0x10) != USCI1_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;			//���ͽ���æµ��
    USCI1_ClearFlag(USCI1_UART_FLAG_RI);			 //����ǰ��������жϱ�־
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize) //�ж��Ƿ������������
    {
      if(USCI1_GetFlagStatus(USCI1_UART_FLAG_RI)) //�жϽ��ձ�־λ
      {
        USCI1_ClearFlag(USCI1_UART_FLAG_RI);			//���ձ�־λ����

        /* ������������δΪ0���������� */
        if((US1CON0 & 0xC0) == USCI1_UART_Mode_11B)
        {
          /* ����9λ���� */
          *(USCI1_HANDLE->pRxBuffPtr.Size_u16 + USCI1_HANDLE->RxXferCount) = USCI1_UART_ReceiveData9();
        }
        else
        {
          /* ����8λ���� */
          *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = USCI1_UART_ReceiveData8();
        }

        USCI1_HANDLE->RxXferCount++;	//��������������
        delaytime = 0;//�յ����ݣ���ʱ��������
      }
      if(delaytime++ > Timeout)//��ʱ���
      {
        USCI1_HANDLE->RxState = USCI1_STATE_TIMEOUT;//���ͳ�ʱ
        return Status_TIMEOUT;
      }
    }

    if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
    {
      USCI1_HANDLE->RxState = USCI1_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_UART_Receive_IT(USCI1_HandleInfoDef *USCI1_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI1_USCI1_HANDLEInfoDef*:USCI1_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI1_UART_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI1_HANDLE->RxState == USCI1_STATE_READY)
  {
    /* ����Ƿ���USCI1_UART�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
    if(((IE2 & 0x01) == 0) || (Size == 0U) || ((US1CON0 & 0x10) != USCI1_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI1_HANDLE->RxState = USCI1_STATE_BUSY;
    USCI1_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
    USCI1_HANDLE->RxXferSize = Size;			//���´����յ�������
    USCI1_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI1_UART_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI1_HandleInfoDef*:USCI1_HANDLE:ָ��ָ��USCI1������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI1_UART_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI1_HANDLE->RxState == USCI1_STATE_BUSY)
  {
    /* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
    if(USCI1_HANDLE->RxXferCount < USCI1_HANDLE->RxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US1CON0 & 0xC0) == USCI1_UART_Mode_11B)
      {
        /* ����9λ���� */
        *(USCI1_HANDLE->pRxBuffPtr.Size_u16 + USCI1_HANDLE->RxXferCount) = USCI1_UART_ReceiveData9();
      }
      else
      {
        /* ����8λ���� */
        *(USCI1_HANDLE->pRxBuffPtr.Size_u8 + USCI1_HANDLE->RxXferCount) = USCI1_UART_ReceiveData8();
      }
      USCI1_HANDLE->RxXferCount ++;
      /* �ж���һ�η����Ƿ������һ�� */
      if(USCI1_HANDLE->RxXferCount == USCI1_HANDLE->RxXferSize)
      {
        /* ������� */
        USCI1_HANDLE->RxState = USCI1_STATE_READY;
				return Status_OK;
      }
       return Status_ERROR;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI1_HANDLE->RxState = USCI1_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/********************************* �ж���� **************************************************/
/*****************************************************
*��������:void USCI1_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:USCI1�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void USCI1_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if(NewState != DISABLE)
  {
    IE2 |= 0x01;
  }
  else
  {
    IE2 &= 0xFE;
  }

  /************************************************************/
  if(Priority != LOW)
  {
    IP2 |= 0x01;
  }
  else
  {
    IP2 &= 0xFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI1_GetFlagStatus(USCI1_Flag_TypeDef USCI1_FLAG)
*��������:���USCI1��־״̬
*��ڲ���:
USCI1_Flag_Typedef:USCI1_Flag:�жϱ�־λѡ��
*���ڲ���:
FlagStatus:USCI1��־״̬
*****************************************************/
FlagStatus USCI1_GetFlagStatus(USCI1_Flag_TypeDef USCI1_FLAG)
{
  FlagStatus bitstatus = RESET;
	if((OTCON & 0xC0)==0x40)      //SPIģʽ
	{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)		
		
		if((USCI1_FLAG == USCI1_SPI_FLAG_SPIF) || (USCI1_FLAG == USCI1_SPI_FLAG_WCOL))
			
#elif	defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
	|| defined (SC95F8x1xB) || defined (SC95F7x1xB)	|| defined (SC95F7619B) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605) \
	|| defined	(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
		
		if((USCI1_FLAG == USCI1_SPI_FLAG_SPIF) || (USCI1_FLAG == USCI1_SPI_FLAG_WCOL) || (USCI1_FLAG == USCI1_SPI_FLAG_TXE))
#endif		
		{
			if((USCI1_FLAG & US1CON1) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }		
		}	
	}else if((OTCON & 0xC0)==0x80)  //TWIģʽ
  {
		if(USCI1_FLAG == USCI1_TWI_FLAG_TXRXnE)
		{
			if((USCI1_FLAG & US1CON1) != (uint8_t)RESET)
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
			if((USCI1_FLAG & US1CON0) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    }
	}else if((OTCON & 0xC0)==0xC0)  //UARTģʽ
  {
    if((USCI1_FLAG & US1CON0) != (uint8_t)RESET)
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
*��������:void USCI1_ClearFlag(USCI1_Flag_TypeDef USCI1_FLAG)
*��������:���USCI1��־״̬
*��ڲ���:
USCI1_Flag_TypeDef:USCI1_FLAG:��������ı�־λ
*���ڲ���:void
*****************************************************/
void USCI1_ClearFlag(USCI1_Flag_TypeDef USCI1_FLAG)
{
	if((OTCON & 0xC0)==0x40)      //SPIģʽ
	{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)
		
		if((USCI1_FLAG == USCI1_SPI_FLAG_SPIF) || (USCI1_FLAG == USCI1_SPI_FLAG_WCOL))

#elif	defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
	|| defined (SC95F8x1xB) || defined (SC95F7x1xB)	|| defined (SC95F7619B) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605) \
	|| defined	(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
		
		if((USCI1_FLAG == USCI1_SPI_FLAG_SPIF) || (USCI1_FLAG == USCI1_SPI_FLAG_WCOL) || (USCI1_FLAG == USCI1_SPI_FLAG_TXE))
#endif
    {
		  US1CON1 &= (~USCI1_FLAG); //�����Ĵ���US1CON1	
		}		
	}else if((OTCON & 0xC0)==0x80)  //TWIģʽ
	{
		if(USCI1_FLAG == USCI1_TWI_FLAG_TXRXnE)
	  {
       US1CON1 &= (~USCI1_FLAG); //�����Ĵ���US1CON1
    }
		else
    {
       US1CON0 &= (~USCI1_FLAG); //�����Ĵ���US1CON0
    }	
	}else if((OTCON & 0xC0)==0xC0)  //UARTģʽ
	{
	 if((USCI1_FLAG == USCI1_UART_FLAG_TI) || (USCI1_FLAG == USCI1_UART_FLAG_RI))
   {
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605) || defined (SC95F8x7x) || defined (SC95F7x7x)|| defined(SC95R503)
     US1CON0 = US1CON0  & 0xFC | USCI1_FLAG; //д1����
#else
     US1CON0 &= (~USCI1_FLAG); //д0����
#endif
   }
	}
}


/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/
