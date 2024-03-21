//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 			sc95f_usci0.c
//	����:					��ԪӦ���Ŷ�
//	ģ�鹦��:			USCI0�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��:���ļ���������SC95Fϵ��оƬ
//*************************************************************

#include "sc95f_usci0.h"

#if !defined (SC95R751) && !defined (SC95F7610B)
/**************************************************
*��������:void USCI0_DeInit(void)
*��������:USCI0��ؼĴ�����λ��ȱʡֵ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI0_DeInit(void)
{
  OTCON &= 0XCF;
  US0CON0 = 0X00;
  US0CON1 = 0X00;
  US0CON2 = 0X00;
  US0CON3 = 0X00;
  IE1 &= (~0X01);
  IP1 &= (~0X01);
}

/**************************************************
*��������:void USCI0_SPI_Init(USCI0_SPI_FirstBit_TypeDef FirstBit, USCI0_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI0_SPI_Mode_TypeDef Mode,
							 USCI0_SPI_ClockPolarity_TypeDef ClockPolarity, USCI0_SPI_ClockPhase_TypeDef ClockPhase,USCI0_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI0_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI��ʼ�����ú���
*��ڲ���:
USCI0_SPI_FirstBit_TypeDef:FirstBit:���ȴ���λѡ��MSB/LSB��
USCI0_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPIʱ��Ƶ��ѡ��
USCI0_SPI_Mode_TypeDef:Mode:SPI����ģʽѡ��
USCI0_SPI_ClockPolarity_TypeDef:ClockPolarity:SPIʱ�Ӽ���ѡ��
USCI0_SPI_ClockPhase_TypeDef:ClockPhase:SPIʱ����λѡ��
USCI0_SPI_TXE_INT_TypeDef:SPI_TXE_INT:���ͻ������ж�����ѡ��,�ù�����SC95FXX1XоƬ����Ч
USCI0_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16λ
*���ڲ���:void
**************************************************/
void USCI0_SPI_Init(USCI0_SPI_FirstBit_TypeDef FirstBit,
                    USCI0_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI0_SPI_Mode_TypeDef Mode,
                    USCI0_SPI_ClockPolarity_TypeDef ClockPolarity, USCI0_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI0_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI0_TransmissionMode_TypeDef TransmissionMode)
{

  OTCON = (OTCON & 0XCF) | 0X10;
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)
  SPI_TXE_INT = USCI0_SPI_TXE_DISINT; //SPI_TXE_INT�ù�����SC95FXX1XоƬ����Ч
  US0CON1 = US0CON1 & (~0X05) | FirstBit | TransmissionMode;
#else
  US0CON1 = US0CON1 & (~0X05) | FirstBit | SPI_TXE_INT | TransmissionMode;
#endif
  US0CON0 = US0CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*��������:void USCI0_TransmissionMode(USCI0_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI ����ģʽ���ú���
*��ڲ���:
USCI0_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16eλ
*���ڲ���:void
**************************************************/
void USCI0_TransmissionMode(USCI0_TransmissionMode_TypeDef TransmissionMode)
{
  if(TransmissionMode == USCI0_SPI_DATA8)
  {
    US0CON1 &= 0xFD;
  }
  else
  {
    US0CON1 |= 0x02;
  }
}

/*****************************************************
*��������:void USCI0_SPI_Cmd(FunctionalState NewState)
*��������:SPI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI0_SPI_Cmd(FunctionalState NewState)
{
  OTCON = (OTCON & 0XCF) | 0X10;

  if(NewState != DISABLE)
  {
    US0CON0 |= 0X80;
  }
  else
  {
    US0CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI0_SPI_SendData_8(uint8_t Data)
*��������:USCI0 SPI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI0_SPI_SendData_8(uint8_t Data)
{
  US0CON2 = Data;
}

/*****************************************************
*��������:uint8_t USCI0_SPI_ReceiveData_8(void)
*��������:���US0CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�������
*****************************************************/
uint8_t USCI0_SPI_ReceiveData_8(void)
{
  return US0CON2;
}

/*****************************************************
*��������:void USCI0_SPI_SendData_16(uint16_t Data)
*��������:US0CON2 SPI��������
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI0_SPI_SendData_16(uint16_t Data)
{
  US0CON3 = (uint8_t)(Data >> 8);
  US0CON2 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI0_SPI_ReceiveData_16(void)
*��������:���US0CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI0_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  SPI_data = (uint16_t)((US0CON3 << 8) | US0CON2);
  return SPI_data;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
*��������:SPI0 �жϴ���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ��
*���ڲ���:
StatusTypeDef:����ִ��״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  if(USCI0_HANDLE->RxState == USCI0_STATE_BUSY)
  {
    /* ������û�н������ */
    if(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize)
    {
      /* SPI����16λͨ��ģʽ */
      if((US0CON1 & USCI0_SPI_DATA16) != 0)
      {
        *(USCI0_HANDLE->pRxBuffPtr.Size_u16 + USCI0_HANDLE->RxXferCount) = USCI0_SPI_ReceiveData_16();			//��ȡ16λ����
      }
      /* SPI����8λͨ��ģʽ */
      else
      {
        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = USCI0_SPI_ReceiveData_8();			//��ȡ16λ����
      }

      TempStatus = Status_BUSY;
      USCI0_HANDLE->RxXferCount++;			//���յ����ݣ�����ֵ��1

      /* ���ݽ�����ɣ�״̬λ�����޸� */
      if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
      {
        USCI0_HANDLE->RxState = USCI0_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI0_HANDLE->TxState == USCI0_STATE_BUSY)
    {

      USCI0_HANDLE->TxXferCount++;			//���ݷ�����ɣ�����ֵ��1
      /* ������û�з������ */
      if(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize)
      {
        /* SPI����16λͨ��ģʽ */
        if((US0CON1 & USCI0_SPI_DATA16) != 0)
        {
          USCI0_SPI_SendData_16(*(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount));			//����16λ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          USCI0_SPI_SendData_8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount));			//����16λ����
        }
        TempStatus = Status_BUSY;
      }
      /* ���ݷ�����ɣ�״̬λ�����޸�  */
      else if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
      {
        USCI0_HANDLE->TxState = USCI0_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* ��SPI��������ʱ��SPI��Ҫ�������ݣ�����ͬ�����յ����� */
    else if(USCI0_HANDLE->RxXferSize)
    {
      USCI0_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�½��մ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return USCI0_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US0CON0 & USCI0_SPI_MODE_MASTER) != 0)
    {
      return USCI0_SPI_TransmitReceive(USCI0_HANDLE, pData, pData, Size, Timeout);			//��ת������ģʽ�����շ�����
    }

    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);			 //����ǰ��������жϱ�־
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize)
    {
      /* �ȴ�SPI�жϱ�־λ���� */
      if(USCI0_GetFlagStatus(USCI0_SPI_FLAG_SPIF))
      {
        /* SPI����16λͨ��ģʽ */
        if((US0CON1 & USCI0_SPI_DATA16) != 0)
        {
          *(USCI0_HANDLE->pRxBuffPtr.Size_u16 + USCI0_HANDLE->RxXferCount) = USCI0_SPI_ReceiveData_16();			//��ȡ16λ����,���ҵ�ַ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = USCI0_SPI_ReceiveData_8();			//��ȡ16λ����,���ҵ�ַ����
        }
        USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);				//�����־λ
        USCI0_HANDLE->RxXferCount++;	//����������������1
        TimeoutCnt = 0;			//��ʱ����ֵ����
      }
      else
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
          {
            USCI0_HANDLE->RxState = USCI0_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
        }
        WDTCON |= 0x10;          //ι����������ֹ���Ź���λ
      }
    }

    if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
    {
      USCI0_HANDLE->RxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;//���մ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t *pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 == 0) || (Size == 0U))
    {
      return USCI0_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US0CON0 & USCI0_SPI_MODE_MASTER) != 0)
    {
      return USCI0_SPI_TransmitReceive_IT(USCI0_HANDLE, pData, pData, Size);			//��ת���ж�ģʽ�����շ�����
    }

    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//״̬����Ϊ����æµ��
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
uint32_t Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->TxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);			 //����ǰ�����־λ
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize)//�ж��Ƿ������������
    {
      if((US0CON1 & USCI0_SPI_DATA16) != 0)/* SPI����16λͨ��ģʽ */
      {
        USCI0_SPI_SendData_16(*(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount));			//����16λ���ݲ������ݵ�ַ����
      }
      else/* SPI����8λͨ��ģʽ */
      {
        USCI0_SPI_SendData_8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount));			//����8λ���ݲ������ݵ�ַ����
      }

      while(!USCI0_GetFlagStatus(USCI0_SPI_FLAG_SPIF))			//�ȴ��������
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if(TimeoutCnt++ > Timeout)
        {
          USCI0_HANDLE->TxState = USCI0_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }

      USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);				//����ǰ�������־λ
      TimeoutCnt = 0;			//��ʱ����������
      USCI0_HANDLE->TxXferCount ++;	//��������������
    }
    if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
    {
      USCI0_HANDLE->TxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI0_HandleInfoDef *USCI0_HANDLE:ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* û�п���USCI0�жϻ�����������СΪ0�����ش��� */
    if((IE1 & 0x01 == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->TxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = 0;			//���ͼ�������0

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US0CON1 & USCI0_SPI_DATA16) != 0)
    {
      USCI0_SPI_SendData_16(*(USCI0_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI0_SPI_SendData_8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//״̬λ�޸�Ϊæµ״̬
  }
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_TransmitReceive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*��������:����ģʽ�շ�һ��������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ��
uint8_t*:pTxData:ָ�������ݻ����ָ��
uint8_t*:pRxData:ָ��������ݻ����ָ��
uint16_t:Size:�������ݵĴ�С
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:��������״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_TransmitReceive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if((USCI0_HANDLE->TxState == USCI0_STATE_READY) && (USCI0_HANDLE->RxState == USCI0_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI0_HANDLE->TxState = USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //ָ����������ݵĵ�ַ
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI0_HANDLE->TxXferSize = USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize);
    {

      if((US0CON1 & USCI0_SPI_DATA16) != 0)
      {
        USCI0_SPI_SendData_16(*(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI0_GetFlagStatus(USCI0_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI0_HANDLE->RxState = USCI0_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount) = USCI0_SPI_ReceiveData_16();
      }
      else
      {
        USCI0_SPI_SendData_8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI0_GetFlagStatus(USCI0_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI0_HANDLE->RxState = USCI0_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount) = USCI0_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//��ʱ����ֵ����
      USCI0_HANDLE->TxXferCount++;			//����������ֵ��1
      USCI0_HANDLE->TxXferCount++;			//����������ֵ��1
    }
    if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
    {
      USCI0_HANDLE->TxState = USCI0_HANDLE->RxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }

}
/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_TransmitReceive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
*��������:SPI���ж�ģʽ�·��ͺͽ��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ��
uint8_t*:pTxData:ָ�������ݻ����ָ��
uint8_t*:pRxData:ָ��������ݻ����ָ��
uint16_t:Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_TransmitReceive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
  if((USCI0_HANDLE->TxState == USCI0_STATE_READY) && (USCI0_HANDLE->RxState == USCI0_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }
    USCI0_SPI_Cmd(DISABLE);
    USCI0_HANDLE->TxState = USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->TxXferSize = USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI0_SPI_Cmd(ENABLE);

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US0CON1 & USCI0_SPI_DATA16) != 0)
    {
      USCI0_SPI_SendData_16(*(USCI0_HANDLE->pTxBuffPtr.Size_u16));			//����16λ����
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI0_SPI_SendData_8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8));			//����8λ����
    }
    /* ״̬����æµ��״̬ */

    return  Status_OK;
  }
  else
  {
    return Status_ERROR;
  }
}


#if defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
	|| defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605)\
  || defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
/**********************************************************************************************************
*��������:uint8_t USCI0_SPI_WriteFIFO(uint16_t *sbuf,uint8_t length)
*��������:SPI0д��FIFO
*��ڲ���:
uint16_t*:sbuf:����buffer
uint8_t:length:�������ݳ���
*���ڲ���:
ErrorStatus:(0:�Ƿ�������1:д��FIFO����ɹ�)
**********************************************************************************************************/
ErrorStatus USCI0_SPI_WriteFIFO(uint16_t* sbuf, uint8_t length)
{
  /* FIFO��СΪ8�� */
  if(length > 8 || length < 1)
  {
    return ERROR;
  }
  else
  {
    uint8_t i = 0;
    US0CON1 &= ~0x01; //�ر�TBIE
    US0CON1 &= ~USCI0_SPI_FLAG_WCOL; //��WCOL

    for(i = 0; i < length; i++)
    {
      US0CON3 = *(sbuf + i) >> 8; //д����������ݵĸ��ֽ�
      US0CON2 = *(sbuf + i);      //д����������ݵĵ��ֽ�
    }

    if(length != 0x01)  //���ǵ���д�룬�����TXE
    {
      US0CON1 &= (~0x08); //��TXE
    }

    /* FIFO�ձ�־λ���� */
    if((US0CON1 & USCI0_SPI_FLAG_WCOL) != 0)
    {
      return ERROR;
    }

    US0CON1 |= SUCCESS; //����TBIE
    return SUCCESS;
  }
}

/**********************************************************************************************************
*��������:void USCI0_SPIFIFO_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
*��������:SPI0 FIFOģʽ �жϴ���
*��ڲ���:
USCI0_HandleInfoDef *USCI0_HANDLE��ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
void
**********************************************************************************************************/
void USCI0_SPIFIFO_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  unsigned char i;
  /* �жϷ��ͻ������Ƿ�Ϊ�� */
  if(US0CON1 & 0x08)
  {
    unsigned int m, n;
    US0CON1 &= ~0x08;				//������ͻ�����Ϊ�ձ�־λ
    //��ȡʣ�෢��/����������
    m = USCI0_HANDLE->TxXferSize;
    n = USCI0_HANDLE->RxXferSize;
    i = 8;
    /* �ж��Ƿ��д��������ݺ�д���ͻ��־λ */
    if((m > 0) && ((US0CON1 & 0x40) == 0))
    {

      while((m > 0) && (i != 0))
      {
        /* 16λģʽ */
        if(US0CON1 & USCI0_SPI_DATA16)
        {
          US0CON3 = (uint8_t)((*USCI0_HANDLE->pTxBuffPtr.Size_u16) >> 8);
          US0CON2 = (uint8_t)(*USCI0_HANDLE->pTxBuffPtr.Size_u16);
          if(US0CON1 & 0X40)
          {
            US0CON1 &= ~0X40;
            break;
          }
          USCI0_HANDLE->pTxBuffPtr.Size_u16++;
          /* ���������ݲ�Ϊ0 */
          if(n != 0)
          {
            (*USCI0_HANDLE->pRxBuffPtr.Size_u16) = (uint16_t)((US0CON3 << 8) | US0CON2);
            USCI0_HANDLE->pRxBuffPtr.Size_u16++;
            n--;
          }
        }
        /* 8λģʽ */
        else
        {
          US0CON3 = (*USCI0_HANDLE->pTxBuffPtr.Size_u8);
          if(US0CON1 & 0X40)
          {
            US0CON1 &= ~0X40;
            break;
          }
          USCI0_HANDLE->pTxBuffPtr.Size_u8++;
          /* ���������ݲ�Ϊ0 */
          if(n != 0)
          {
            (*USCI0_HANDLE->pRxBuffPtr.Size_u8) = US0CON2;
            USCI0_HANDLE->pRxBuffPtr.Size_u8--;
            n--;
          }
        }
        m--;
        i--;
      }
      USCI0_HANDLE->TxXferSize = m;
      USCI0_HANDLE->RxXferSize = n;
    }
  }
  else if(US0CON1 & 0x80)
  {
    USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF);

    if(USCI0_HANDLE->RxXferSize != 0)
    {
      /* ���������Ի�ȡ8������ */
      for(i = 0; i < 8; i++)
      {
        if((US0CON1 & USCI0_SPI_DATA16) != 0)
        {
          (*USCI0_HANDLE->pRxBuffPtr.Size_u16) = USCI0_SPI_ReceiveData_16();
          USCI0_HANDLE->pRxBuffPtr.Size_u16++;
        }
        else
        {
          (*USCI0_HANDLE->pRxBuffPtr.Size_u8) = USCI0_SPI_ReceiveData_8();
          USCI0_HANDLE->pRxBuffPtr.Size_u8++;
        }
        USCI0_HANDLE->RxXferSize--;
        if(USCI0_HANDLE->RxXferSize == 0)
        {
          break;
        }
      }
    }
    /* ������ɣ��޸�״̬ */
    if(USCI0_HANDLE->TxXferSize == 0)
    {
      USCI0_HANDLE->TxState = USCI0_STATE_READY;
    }
    if(USCI0_HANDLE->RxXferSize == 0)
    {
      USCI0_HANDLE->RxState = USCI0_STATE_READY;
    }
  }
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_Receive_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE)
*��������:FIFOģʽ����һ��������
*��ڲ���:
USCI0_HandleInfoDef *USCI0_HANDLE��ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData��ָ�����ݻ����ָ�롣
uint16_t Size���������ݵĴ�С
*���ڲ���:
StatusTypeDef��USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_Receive_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  return USCI0_SPI_TransmitReceive_FIFO(USCI0_HANDLE);
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_Transmit_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE)
*��������:FIFO ģʽ����һ��������
*��ڲ���:
USCI0_HandleInfoDef *USCI0_HANDLE��ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
StatusTypeDef��USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_Transmit_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  uint8_t i, length;
  /* ����/�������ݵĳ��Ȳ���Ϊ0 */
  if(USCI0_HANDLE->TxXferSize == 0)
  {
    USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
    USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
    return USCI0_STATE_ERROR;
  }

  US0CON1 &= ~0x01; //�ر�TBIE
  USCI0_ClearFlag(USCI0_SPI_FLAG_WCOL); //��WCOL
  if(USCI0_HANDLE->TxXferSize)
  {
    length = USCI0_HANDLE->TxXferSize;
  }
  else
  {
    length = 8;
  }
  /* ���1�ε�fifo */
  for(i = 0; i < length; i++)
  {
    if((US0CON1 & USCI0_SPI_DATA16) != 0)
    {
      USCI0_SPI_SendData_16(*USCI0_HANDLE->pTxBuffPtr.Size_u16);
      USCI0_HANDLE->pTxBuffPtr.Size_u16 ++;
    }
    else
    {
      USCI0_SPI_SendData_8(*USCI0_HANDLE->pTxBuffPtr.Size_u8);
      USCI0_HANDLE->pTxBuffPtr.Size_u8 ++;
    }
    USCI0_HANDLE->TxXferSize--;
  }
  /* ��������ʱ�����ش��� */
  if((US0CON1 & USCI0_SPI_FLAG_WCOL) != 0)
  {
    USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
    USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
    return USCI0_STATE_ERROR;
  }

  USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF); //��SPIF
  USCI0_ClearFlag(USCI0_SPI_FLAG_TXE);  //��TXE
  US0CON1 |= 0x01; //����TBIE

  //״̬����Ϊæµ
  USCI0_HANDLE->TxState = USCI0_STATE_BUSY;
  USCI0_HANDLE->RxState = USCI0_STATE_BUSY;
  return USCI0_STATE_BUSY;
}
/**********************************************************************************************************
*��������:StatusTypeDef USCI0_SPI_TransmitReceive_FIFO(USCI0_HandleInfoDef *USCI0_HANDLE)
*��������:FIFOģʽ�շ�һ��������
*��ڲ���:
USCI0_HandleInfoDef *USCI0_HANDLE��ָ����� SPI0 ��Ϣ�� USCI0_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
StatusTypeDef��USCI0 ״̬
**********************************************************************************************************/
StatusTypeDef USCI0_SPI_TransmitReceive_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  uint8_t i, length;


  IE1 &= 0xFE;
  if((USCI0_HANDLE->RxXferSize == 0) || (USCI0_HANDLE->TxXferSize == 0))
  {
    USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
    USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
    return USCI0_STATE_ERROR;
  }
  US0CON1 &= ~0x01; //�ر�TBIE

  USCI0_ClearFlag(USCI0_SPI_FLAG_WCOL); //��WCOL
  USCI0_ClearFlag(USCI0_SPI_FLAG_SPIF); //��SPIF
  USCI0_ClearFlag(USCI0_SPI_FLAG_TXE);  //��TXE
  if(USCI0_HANDLE->TxXferSize <= 8)
  {
    length = USCI0_HANDLE->TxXferSize;
  }
  else
  {
    length = 8;
  }


  if((US0CON1 & USCI0_SPI_DATA16) != 0)
  {
    for(i = 0; i < length; i++)
    {
      USCI0_SPI_SendData_16(*USCI0_HANDLE->pTxBuffPtr.Size_u16);
      USCI0_HANDLE->pTxBuffPtr.Size_u16 ++;
    }
  }
  else
  {
    for(i = 0; i < length; i++)
    {
      USCI0_SPI_SendData_8(*USCI0_HANDLE->pTxBuffPtr.Size_u8);
      USCI0_HANDLE->pTxBuffPtr.Size_u8 ++;
    }
  }
  USCI0_HANDLE->TxXferSize -= length;

  if((US0CON1 & USCI0_SPI_FLAG_WCOL) != 0)
  {
    USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
    USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
    return USCI0_STATE_ERROR;
  }

  US0CON1 |= 0x01; //����TBIE
  USCI0_HANDLE->TxState = USCI0_STATE_BUSY;
  USCI0_HANDLE->RxState = USCI0_STATE_BUSY;
  IE1 |= 0x01;
  return USCI0_STATE_BUSY;
}

#endif


/* USCI_TWI */
/**************************************************
*��������:void USCI0_TWI_Slave_Init(uint8_t TWI_Address)
*��������:USCI0 TWI�ӻ���ʼ�����ú���
*��ڲ���:
uint8_t:TWI_Address:TWI��Ϊ�ӻ�ʱ��7λ�ӻ���ַ
*���ڲ���:void
**************************************************/
void USCI0_TWI_Slave_Init(uint8_t TWI_Address)
{
  OTCON = OTCON & 0XCF | 0X20;
  US0CON2 = TWI_Address << 1;
}

/**************************************************
*��������:void USCI0_TWI_MasterCommunicationRate(USCI0_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*��������:USCI0 TWI����ģʽ��ͨѶ�����趨
*��ڲ���:
USCI0_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI����ģʽ��ͨѶ����
*���ڲ���:void
**************************************************/
void USCI0_TWI_MasterCommunicationRate(USCI0_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  OTCON = OTCON & 0XCF | 0X20;
  US0CON1 |= TWI_MasterCommunicationRate;
}

/**************************************************
*��������:void USCI0_TWI_Start(void)
*��������:USCI0 TWI ��ʼλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI0_TWI_Start(void)
{
  US0CON1 |= 0x20;
}

/**************************************************
*��������:void USCI0_TWI_MasterModeStop(void)
*��������:USCI0 TWI����ģʽֹͣλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI0_TWI_MasterModeStop(void)
{
  US0CON1 |= 0x10;
}

/**************************************************
*��������:void USCI0_TWI_SlaveClockExtension(void)
*��������:USCI0 TWI�ӻ�ģʽʱ���ӳ�����λ
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
**************************************************/
void USCI0_TWI_SlaveClockExtension(FunctionalState NewState)
{
  OTCON = OTCON & 0XCF | 0X20;

  if(NewState != DISABLE)
  {
    US0CON1 |= 0x40;
  }
  else
  {
    US0CON1 &= 0XBF;
  }
}

/**************************************************
*��������:void USCI0_TWI_AcknowledgeConfig(FunctionalState NewState)
*��������:TWI����Ӧ��ʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI0_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  OTCON = OTCON & 0XCF | 0X20;

  if(NewState != DISABLE)
  {
    US0CON0 |= 0X08;
  }
  else
  {
    US0CON0 &= 0XF7;
  }
}

/**************************************************
*��������:void USCI0_TWI_GeneralCallCmd(FunctionalState NewState)
*��������:TWIͨ�õ�ַ��Ӧʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:ͨ�õ�ַ��Ӧʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI0_TWI_GeneralCallCmd(FunctionalState NewState)
{
  OTCON = OTCON & 0XCF | 0X20;

  if(NewState != DISABLE)
  {
    US0CON2 |= 0X01;
  }
  else
  {
    US0CON2 &= 0XFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI0_GetTWIStatus(USCI0_TWIState_TypeDef USCI0_TWIState)
*��������:��ȡTWI״̬
*��ڲ���:
USCI0_TWIState_TypeDef:USCI0_TWIState:TWI״̬����
*���ڲ���:
FlagStatus:USCI0_SC��־״̬
*****************************************************/
FlagStatus USCI0_GetTWIStatus(USCI0_TWIState_TypeDef USCI0_TWIState)
{
  if((US0CON0 & 0x07) == USCI0_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*��������:void USCI0_TWI_Cmd(FunctionalState NewState)
*��������:TWI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI0_TWI_Cmd(FunctionalState NewState)
{
  OTCON = OTCON & 0XCF | 0X20;

  if(NewState != DISABLE)
  {
    US0CON0 |= 0X80;
  }
  else
  {
    US0CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI0_TWI_SendData(uint8_t Data)
*��������:TWI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI0_TWI_SendData(uint8_t Data)
{
  US0CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI0_TWI_SendData(void)
*��������:���US0CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI0_TWI_ReceiveData(void)
{
  return US0CON3;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI0_TWI_Wait_TWIF(USCI0_HandleInfoDef* USCI0_HANDLE,uint32_t Timeout)
*�������ܣ��ȴ�TWIF����
*��ڲ�����
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Wait_TWIF(USCI0_HandleInfoDef* USCI0_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  while(!(USCI0_GetFlagStatus(USCI0_TWI_FLAG_TWIF)))			//�ȴ������źŷ������
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* ��ʱ����״̬ */
      if(USCI0_HANDLE->TxState == USCI0_STATE_BUSY)
        USCI0_HANDLE->TxState = USCI0_STATE_TIMEOUT;
      if(USCI0_HANDLE->RxState == USCI0_STATE_BUSY)
        USCI0_HANDLE->RxState = USCI0_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			//�����־λ
  return Status_OK;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI0_TWI_Master_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ܣ�����8λ����ʱ��������ѯģʽ�·��ʹ�������
*��ڲ�����
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*:pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ�����
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Master_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->TxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI0_TWI_Start();			//���������ź�
    if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI0_GetTWIStatus(USCI0_TWI_MasterSendAddress) == RESET)          //���״̬��״̬
    {
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US0CON3 = (slaveAddr << 1) & 0xFE;			//���͵�ַ�Ͷ�дλ
    if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI0_GetTWIStatus(USCI0_TWI_MasterSendData) == RESET)          //���״̬��״̬
      {
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US0CON3 = *(USCI0_HANDLE->pTxBuffPtr.Size_u8  + USCI0_HANDLE->TxXferCount);             //TWI��������
      if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI0_HANDLE->TxXferCount++;
    }
    while(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize);

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI0_TWI_MasterModeStop();

    if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
    {
      USCI0_HANDLE->TxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }

  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Slave_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�·��ʹ�������
* ע��Ĭ�Ͽ���ʱ���ӳ�ģʽ
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t*:pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Slave_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      USCI0_HANDLE->TxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
      USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI0_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI0_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
      if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ���ַƥ��
      {
        USCI0_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
        return Status_TIMEOUT;
      }

      do
      {
        if(USCI0_GetTWIStatus(USCI0_TWI_SlaveSendData) == SET)           //���״̬��״̬
        {

          US0CON3 = *(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount);             //TWI��������

          if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)             //�ȴ������źŷ������
          {
            USCI0_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
            return Status_TIMEOUT;
          }
          USCI0_HANDLE->TxXferCount++;

          if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize - 1)

            USCI0_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��

        }
        else if(USCI0_GetTWIStatus(USCI0_TWI_SlaveDisableACK) == SET)
        {

          USCI0_HANDLE->TxXferCount++;
        }
        else
        {
          USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
          return Status_ERROR;
        }
      }
      while(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize);

      if(USCI0_GetTWIStatus(USCI0_TWI_SlaveDisableACK) == RESET)           //���״̬��״̬
      {
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        return Status_ERROR;
      }

      if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
      {
        USCI0_HANDLE->TxState = USCI0_STATE_READY;	//�������
        return Status_OK;
      }
      else
      {
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;//�������ʹ���
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
*��������:StatusTypeDef USCI0_TWI_Master_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*:pData ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Master_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE1 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {

      IE1 &= ~0x01;			//�ر��ж�

      USCI0_HANDLE->TxState = USCI0_STATE_WAIT;			//���ͽ���æµ��
      USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI0_TWI_Start();			//���������ź�

      if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_MasterModeStop();
        IE1 |= 0x01;	//�����ж�
        return Status_TIMEOUT;
      }

      if(USCI0_GetTWIStatus(USCI0_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
      {
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_MasterModeStop();
        IE1 |= 0x01;	//�����ж�
        return Status_ERROR;
      }

      IE1 |= 0x01;						//�����ж�
      US0CON3 = (slaveAddr << 1) & 0xFE;//���͵�ַ�Ͷ�дλ

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Master_Transmit_IRQHandler(USCI0_HandleInfoDef *USCI0_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
*���ڲ���:StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Master_Transmit_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  if(USCI0_HANDLE->TxState == USCI0_STATE_BUSY)
  {
    if(USCI0_GetTWIStatus(USCI0_TWI_MasterSendData) == SET)
    {
      if(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize)
      {
        USCI0_HANDLE->TxXferCount++;			//��ַ֡Ҳ��Ϊ���ݼ���
        US0CON3 = *(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount);             //TWI��������
        return Status_BUSY;
      }
      else if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_MasterModeStop();
        USCI0_HANDLE->TxState = USCI0_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_MasterModeStop();
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI0_GetTWIStatus(USCI0_TWI_MasterReceivedaUACK) == SET)
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_HANDLE->TxXferCount++;
      USCI0_TWI_MasterModeStop();
      if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
      {
        USCI0_HANDLE->TxState = USCI0_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI0_TWI_MasterModeStop();
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI0_HANDLE->TxState == USCI0_STATE_WAIT)
  {
    //��ַ�ɹ���Ӧ
    USCI0_HANDLE->TxState = USCI0_STATE_BUSY;
    US0CON3 = *(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Slave_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ӻ�ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t*:pData ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Slave_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE1 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {

      USCI0_TWI_AcknowledgeConfig(DISABLE);         //����AA
      USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI0_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI0_HANDLE->TxState = USCI0_STATE_WAIT;			//���ͽ���æµ��
      USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI0_TWI_AcknowledgeConfig(ENABLE);         //����AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Slave_Transmit_IRQHandler(USCI0_HandleInfoDef *USCI0_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Slave_Transmit_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  if(USCI0_HANDLE->TxState == USCI0_STATE_BUSY)
  {

    if(USCI0_GetTWIStatus(USCI0_TWI_SlaveSendData) == SET)
    {
      if(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize)
      {
        USCI0_HANDLE->TxXferCount++;			//��һ���ж��ǽ��ܵ���ַ��
        US0CON3 = *(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount);
        if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize - 1)
        {
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        }
        return Status_BUSY;
      }
      else
      {
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.�����ݴ�������У����������޸���AA����λ
    2.���յ�����������UACK */
    else if((USCI0_GetTWIStatus(USCI0_TWI_SlaveDisableACK) == SET) || (USCI0_GetTWIStatus(USCI0_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI0_HANDLE->TxXferCount++;
      if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)			//���������ѷ���
      {
        USCI0_HANDLE->TxState = USCI0_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �������δ���͹��̳���
      1.���ݷ��ͺ���ܵ�UACK
      2.TWI���ǹ����ڴӻ�����״̬ */
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI0_HANDLE->TxState == USCI0_STATE_WAIT)
  {
    if(USCI0_GetTWIStatus(USCI0_TWI_SlaveSendData) == SET)
    {
      US0CON3 = *(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount);
      USCI0_HANDLE->TxState = USCI0_STATE_BUSY;
    }
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Master_Receive(USCI0_HandleInfoDef* USCI0_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*��������:����8λ����ʱ��������ѯģʽ�½��մ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*:pData ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Master_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI0_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    USCI0_TWI_Start();			//���������ź�
    if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      USCI0_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI0_GetTWIStatus(USCI0_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
      USCI0_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    US0CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ
    if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI0_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI0_GetTWIStatus(USCI0_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI0_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* ���ݽ��չ��� */
    do
    {
      if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI0_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI0_GetTWIStatus(USCI0_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
      {
        USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI0_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;             //TWI��������
      USCI0_HANDLE->RxXferCount++;
      if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize - 1)
      {
        USCI0_TWI_AcknowledgeConfig(DISABLE);                                     //�ر�AA
        if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI0_TWI_MasterModeStop();
          return Status_TIMEOUT;
        }
        if(USCI0_GetTWIStatus(USCI0_TWI_MasterReceivedaUACK) == RESET)           //���״̬��״̬
        {
          USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI0_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;             //TWI��������
        USCI0_HANDLE->RxXferCount++;
      }
    }
    while(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize);
    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI0_TWI_MasterModeStop();

    if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
    {
      USCI0_HANDLE->RxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Slave_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�½��մ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t*:pData ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Slave_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI0_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    /* ���յ�ַ���� */
    if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_TIMEOUT;
    }

    if(USCI0_GetTWIStatus(USCI0_TWI_SlaveReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ

      return Status_ERROR;
    }

    /* �������ݹ��� */
    do
    {
      if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        return Status_TIMEOUT;
      }

      if(USCI0_GetTWIStatus(USCI0_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
      {
        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;             //TWI��������
        USCI0_HANDLE->RxXferCount++;
        if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize - 1)
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UAC
      }
      else if(USCI0_GetTWIStatus(USCI0_TWI_SlaveIdle) == SET)
      {
        if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize - 1)
        {
          *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;
          USCI0_HANDLE->RxXferCount++;
        }
        else
        {
          USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_ERROR;
        }
      }
      else
      {
        USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        return Status_ERROR;
      }

    }
    while(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize);

    if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
    {
      USCI0_HANDLE->RxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Master_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*:pData ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Master_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE1 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    IE1 &= ~0x01;	//�ر��ж�
    USCI0_HANDLE->RxState = USCI0_STATE_WAIT;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI0_TWI_AcknowledgeConfig(ENABLE);         //����AA
    USCI0_TWI_Start();			//���������ź�

    if(USCI0_TWI_Wait_TWIF(USCI0_HANDLE, 0xFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI0_TWI_MasterModeStop();
      IE1 |= 0x01;	//�����ж�
      return Status_TIMEOUT;
    }

    if(USCI0_GetTWIStatus(USCI0_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI0_TWI_MasterModeStop();
      IE1 |= 0x01;	//�����ж�
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    IE1 |= 0x01;	//�����ж�
    US0CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Master_Receive_IRQHandler(USCI0_HandleInfoDef *USCI0_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�½��մ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Master_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  if(USCI0_HANDLE->RxState == USCI0_STATE_BUSY)
  {
    if(USCI0_GetTWIStatus(USCI0_TWI_MasterReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize)
      {
        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;             //TWI��������
        USCI0_HANDLE->RxXferCount++;
        if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI0_TWI_MasterModeStop();
          USCI0_HANDLE->RxState = USCI0_STATE_READY;	//�������
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI0_TWI_MasterModeStop();
        USCI0_HANDLE->RxState = USCI0_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else if(USCI0_HANDLE->RxState == USCI0_STATE_WAIT)
  {
    if(USCI0_GetTWIStatus(USCI0_TWI_MasterReceivedaData) == SET)
    {
      USCI0_HANDLE->RxState = USCI0_STATE_BUSY;
    }
    return Status_BUSY;
  }
  return Status_ERROR;
}


/*****************************************************
*��������:void USCI0_TWI_Slave_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size))
*��������:�ӻ�ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
uint8_t*:pData ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Slave_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE1 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI0_ClearFlag(USCI0_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI0_HANDLE->RxState = USCI0_STATE_WAIT;			//���ͽ���æµ��
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI0_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_TWI_Slave_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�½��մ�������
*��ڲ���:
USCI0_HandleInfoDef:*USCI0_HANDLE ָ��ָ��USCI0������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_TWI_Slave_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  if(USCI0_HANDLE->RxState == USCI0_STATE_BUSY)
  {
    if(USCI0_GetTWIStatus(USCI0_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize)
      {
        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;             //TWI��������
        USCI0_HANDLE->RxXferCount++;
        if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize - 1)
        {
          USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        }
      }
      else
      {
        USCI0_TWI_AcknowledgeConfig(DISABLE);
        return Status_ERROR;
      }
      return Status_BUSY;
    }
    else if(USCI0_GetTWIStatus(USCI0_TWI_SlaveIdle) == SET)
    {
      if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize - 1)
      {
        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = US0CON3;
        USCI0_HANDLE->RxXferCount++;
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        USCI0_HANDLE->RxState = USCI0_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
      USCI0_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }
  }
  else if(USCI0_HANDLE->RxState == USCI0_STATE_WAIT)
  {
    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;
  }
}

/**************************************************
*��������:void USCI0_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI0_UART_Mode_TypeDef Mode, USCI0_UART_RX_TypeDef RxMode)
*��������:UART��ʼ�����ú���
*��ڲ���:
uint32_t:UARTFsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
USCI0_UART_Mode_TypeDef:Mode:UART1����ģʽ
USCI0_UART_RX_TypeDef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void USCI0_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI0_UART_Mode_TypeDef Mode,
                     USCI0_UART_RX_TypeDef RxMode)
{
  OTCON |= 0x30;
  US0CON0 = US0CON0 & 0X0F | Mode | RxMode;

  if(Mode == USCI0_UART_Mode_8B)
  {
    if(BaudRate == USCI0_UART_BaudRate_FsysDIV12)
    {
      US0CON0 &= 0XDF;
    }
    else if(BaudRate == USCI0_UART_BaudRate_FsysDIV4)
    {
      US0CON0 |= 0X20;
    }
  }
  else
  {
    US0CON2 = UARTFsys / BaudRate / 256;
    US0CON1 = UARTFsys / BaudRate % 256;
  }
}

/*****************************************************
*��������:void USCI0_UART_SendData8(uint8_t Data)
*��������:USCI0 UART1����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI0_UART_SendData8(uint8_t Data)
{
  US0CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI0_UART_ReceiveData8(void)
*��������:���US0CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI0_UART_ReceiveData8(void)
{
  return US0CON3;
}

/*****************************************************
*��������:void USCI0_UART_SendData9(uint16_t Data)
*��������:UART1����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI0_UART_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if(Data_9Bit)
  {
    US0CON0 |= 0x08;
  }
  else
  {
    US0CON0 &= 0xf7;
  }

  US0CON3 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI0_UART_ReceiveData9(void)
*��������:���US0CON3�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI0_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US0CON3 + ((uint16_t)(US0CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*��������:StatusTypeDef  USCI0_UART_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_UART_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* ��鷢���������Ƿ�Ϊ0 */
    if((IE1 & 0x01 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI0_HANDLE->TxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_UART_FLAG_TI); //����ǰ��������жϱ�־��д1/д0����
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize)//�ж��Ƿ������������
    {
      if((US0CON0 & 0xC0) == USCI0_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI0_UART_SendData9(*(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI0_UART_SendData8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount));
      }

      /* �ȴ�������� */
      delaytime = 0;
      while(!(US0CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI0_HANDLE->TxState = USCI0_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
      }

      USCI0_ClearFlag(USCI0_UART_FLAG_TI); //�����жϱ�־�����д1/д0����
      USCI0_HANDLE->TxXferCount ++;	//��������������
    }

    if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
    {
      USCI0_HANDLE->TxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}


/*****************************************************
*��������:StatusTypeDef USCI0_UART_Transmit_IT(USCI0_HandleInfoDef *USCI0_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_UART_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_READY)
  {
    /* û�п���USCI0�жϻ�����������СΪ0�����ش��� */
    if(((IE1 & 0x01) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->TxState = USCI0_STATE_BUSY;
    USCI0_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->TxXferCount = 0;			//���ͼ���������

    /* ���͵�1֡���� */
    if((US0CON0 & 0xC0) == USCI0_UART_Mode_11B)
    {
      /* ����9λ���� */
      USCI0_UART_SendData9(*(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount));
    }
    else
    {
      /* ����8λ���� */
      USCI0_UART_SendData8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef  USCI0_UART_Transmit_IRQHandler(USCI0_HandleInfoDef *USCI0_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI0_HandleInfoDef:*USCI0_HANDLE:ָ��ָ��UART������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_UART_Transmit_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  /* ���ڷ����߳� */
  if(USCI0_HANDLE->TxState == USCI0_STATE_BUSY)
  {
    USCI0_HANDLE->TxXferCount++;
    /* �ж���һ�η����Ƿ������һ�� */
    if(USCI0_HANDLE->TxXferCount < USCI0_HANDLE->TxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US0CON0 & 0xC0) == USCI0_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI0_UART_SendData9(*(USCI0_HANDLE->pTxBuffPtr.Size_u16 + USCI0_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI0_UART_SendData8(*(USCI0_HANDLE->pTxBuffPtr.Size_u8 + USCI0_HANDLE->TxXferCount));
      }
      return Status_BUSY;
    }
    else if(USCI0_HANDLE->TxXferCount == USCI0_HANDLE->TxXferSize)
    {
      /* ������� */
      USCI0_HANDLE->TxState = USCI0_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI0_HANDLE->TxState = USCI0_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_UART_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
USCI0_USCI0_HANDLEInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0_UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_UART_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  /* �����ս����������� */
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if((IE1 & 0x01 != 0) || (Size == 0U) || ((US0CON0 & 0x10) != USCI0_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;			//���ͽ���æµ��
    USCI0_ClearFlag(USCI0_UART_FLAG_RI);			 //����ǰ��������жϱ�־
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize) //�ж��Ƿ������������
    {
      if(USCI0_GetFlagStatus(USCI0_UART_FLAG_RI)) //�жϽ��ձ�־λ
      {
        USCI0_ClearFlag(USCI0_UART_FLAG_RI);			//���ձ�־λ����

        /* ������������δΪ0���������� */
        if((US0CON0 & 0xC0) == USCI0_UART_Mode_11B)
        {
          /* ����9λ���� */
          *(USCI0_HANDLE->pRxBuffPtr.Size_u16 + USCI0_HANDLE->RxXferCount) = USCI0_UART_ReceiveData9();
        }
        else
        {
          /* ����8λ���� */
          *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = USCI0_UART_ReceiveData8();
        }

        USCI0_HANDLE->RxXferCount++;	//��������������
        delaytime = 0;//�յ����ݣ���ʱ��������
      }
      if(delaytime++ > Timeout)//��ʱ���
      {
        USCI0_HANDLE->RxState = USCI0_STATE_TIMEOUT;//���ͳ�ʱ
        return Status_TIMEOUT;
      }
    }

    if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
    {
      USCI0_HANDLE->RxState = USCI0_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_UART_Receive_IT(USCI0_HandleInfoDef *USCI0_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI0_USCI0_HANDLEInfoDef*:USCI0_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_UART_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI0_HANDLE->RxState == USCI0_STATE_READY)
  {
    /* ����Ƿ���USCI0_UART�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
    if(((IE1 & 0x01) == 0) || (Size == 0U) || ((US0CON0 & 0x10) != USCI0_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI0_HANDLE->RxState = USCI0_STATE_BUSY;
    USCI0_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
    USCI0_HANDLE->RxXferSize = Size;			//���´����յ�������
    USCI0_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI0_UART_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI0_HandleInfoDef*:USCI0_HANDLE:ָ��ָ��USCI0������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI0_UART_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI0_HANDLE->RxState == USCI0_STATE_BUSY)
  {
    /* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
    if(USCI0_HANDLE->RxXferCount < USCI0_HANDLE->RxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US0CON0 & 0xC0) == USCI0_UART_Mode_11B)
      {
        /* ����9λ���� */
        *(USCI0_HANDLE->pRxBuffPtr.Size_u16 + USCI0_HANDLE->RxXferCount) = USCI0_UART_ReceiveData9();
      }
      else
      {
        /* ����8λ���� */
        *(USCI0_HANDLE->pRxBuffPtr.Size_u8 + USCI0_HANDLE->RxXferCount) = USCI0_UART_ReceiveData8();
      }
      USCI0_HANDLE->RxXferCount ++;
      /* �ж���һ�η����Ƿ������һ�� */
      if(USCI0_HANDLE->RxXferCount == USCI0_HANDLE->RxXferSize)
      {
        /* ������� */
        USCI0_HANDLE->RxState = USCI0_STATE_READY;
				return Status_OK;
      }
        return Status_BUSY;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI0_HANDLE->RxState = USCI0_STATE_ERROR;
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
*��������:void USCI0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:USCI0�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void USCI0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if(NewState != DISABLE)
  {
    IE1 |= 0x01;
  }
  else
  {
    IE1 &= 0xFE;
  }

  /************************************************************/
  if(Priority != LOW)
  {
    IP1 |= 0x01;
  }
  else
  {
    IP1 &= 0xFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI0_GetFlagStatus(USCI0_Flag_TypeDef USCI0_FLAG)
*��������:���USCI0��־״̬
*��ڲ���:
USCI0_Flag_TypeDef:USCI0_FLAG:�����ȡ�ı�־λ
*���ڲ���:
FlagStatus:USCI0��־״̬
*****************************************************/
FlagStatus USCI0_GetFlagStatus(USCI0_Flag_TypeDef USCI0_FLAG)
{
  FlagStatus bitstatus = RESET;
	if((OTCON & 0x30)==0x10)      //SPIģʽ
	{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)
		
    if((USCI0_FLAG == USCI0_SPI_FLAG_SPIF) || (USCI0_FLAG == USCI0_SPI_FLAG_WCOL))
			
#elif	defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
	|| defined (SC95F8x1xB) || defined (SC95F7x1xB)	|| defined (SC95F7619B) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605) \
	|| defined	(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
	
		if((USCI0_FLAG == USCI0_SPI_FLAG_SPIF) || (USCI0_FLAG == USCI0_SPI_FLAG_WCOL) || (USCI0_FLAG == USCI0_SPI_FLAG_TXE))
#endif	
		{
			if((USCI0_FLAG & US0CON1) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }		
		}	
	}else if((OTCON & 0x30)==0x20)  //TWIģʽ
	{
		if(USCI0_FLAG == USCI0_TWI_FLAG_TXRXnE)
		{
			if((USCI0_FLAG & US0CON1) != (uint8_t)RESET)
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
			if((USCI0_FLAG & US0CON0) != (uint8_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    }
	}else if((OTCON & 0x30)==0x30)  //UARTģʽ
	{
		if((USCI0_FLAG & US0CON0) != (uint8_t)RESET)
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
*��������:void USCI0_ClearFlag(USCI0_Flag_TypeDef USCI0_FLAG)
*��������:���USCI0��־״̬
*��ڲ���:
USCI0_Flag_TypeDef:USCI0_FLAG:��������ı�־λ
*���ڲ���:void
*****************************************************/
void USCI0_ClearFlag(USCI0_Flag_TypeDef USCI0_FLAG)
{
	if((OTCON & 0x30)==0x10)  //SPIģʽ
	{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)
		
    if((USCI0_FLAG == USCI0_SPI_FLAG_SPIF) || (USCI0_FLAG == USCI0_SPI_FLAG_WCOL))
			
#elif defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
  || defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined (SC95F7619B) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605) \
	|| defined	(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
		
	  if((USCI0_FLAG == USCI0_SPI_FLAG_SPIF) || (USCI0_FLAG == USCI0_SPI_FLAG_WCOL) || (USCI0_FLAG == USCI0_SPI_FLAG_TXE))
#endif
    {
       US0CON1 &= (~USCI0_FLAG); //�����Ĵ���US0CON1
    }

	}else if((OTCON & 0x30)==0x20)  //TWIģʽ
	{	
	  if(USCI0_FLAG == USCI0_TWI_FLAG_TXRXnE)
	  {
       US0CON1 &= (~USCI0_FLAG); //�����Ĵ���US0CON1
    }
		else
    {
       US0CON0 &= (~USCI0_FLAG); //�����Ĵ���US0CON0
    }	
	}else if((OTCON & 0x30)==0x30)  //UARTģʽ
	{
		if((USCI0_FLAG == USCI0_UART_FLAG_TI) || (USCI0_FLAG == USCI0_UART_FLAG_RI))
		{
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605) || defined (SC95F8x7x) || defined (SC95F7x7x)|| defined(SC95R503)
       US0CON0 = US0CON0  & 0xFC | USCI0_FLAG;//д1����		
#else    
			 US0CON0 &= (~USCI0_FLAG); //д0����
#endif
		}	
	}
	
	}

/*****************************************************
*��������:void USCI0_TWI_SendAddr(uint8_t Addr,USCI0_TWI_RWType RW)
*��������:TWI���͵�ַ����д����
*��ڲ���:
uint8_t:Addr:���͵ĵ�ַ 0~127
USCI0_TWI_RWType:RW:��д����
*���ڲ���:void
*****************************************************/
void USCI0_TWI_SendAddr(uint8_t Addr, USCI0_TWI_RWType RW)
{
  US0CON3 = (Addr << 1) | RW;
}

#if defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95FS52x)
/**************************************************
*��������:void USCI0_SC_Init()
*��������:���ܿ���ʼ�����ú���
*��ڲ���:
FunctionalState:ClockEnable:ʱ�����ʹ��λ
USCI0_SC_ERS_TypeDef:ErrorSignal:ErrorSignal����
USCI0_SC_TREN_TypeDef:TrenMode:����/����ʹ�ܣ����ͺͽ���ֻ�ܶ�ѡһ
USCI0_SC_CONS_TypeDef:ConsMod:���뷽ʽѡ��
FunctionalState:ReSend:���ݷ��ͽ���У������ط�ʹ��λ
USCI0_SC_Check_TypeDef:CheckType:У������
*���ڲ���:void
**************************************************/
void USCI0_SC_Init(FunctionalState ClockEnable,
                   USCI0_SC_ERS_TypeDef ErrorSignal,
                   USCI0_SC_TREN_TypeDef TrenMode,
                   USCI0_SC_CONS_TypeDef ConsMode,
                   FunctionalState ReSend,
                   USCI0_SC_Check_TypeDef CheckType)
{
  if(ClockEnable != DISABLE)
  {
    US0CON0 |= 0x40;
  }
  else
  {
    US0CON0 &= ~0x40;
  }

  US0CON0 &= ~0x3d;
  US0CON0 |= TrenMode | ConsMode | CheckType | (ErrorSignal << 3);

  if(ReSend != DISABLE)
  {
    US0CON0 |= 0x02;
  }
  else
  {
    US0CON0 &= ~0x02;
  }
}
/**************************************************
*��������:void USCI0_SC_EGT(uint8_t EgtCount)
*��������:���ܿ���չ����ʱ������
*��ڲ���:
uint8_t:EgtCount:��չ����ʱ����:EgtCount��ETU
*���ڲ���:void
**************************************************/
void USCI0_SC_EGT(uint8_t EgtCount)
{
  US0CON4 = EgtCount;
}

/**************************************************
*��������:void USCI0_SC_ETU_SET(USCI0_SC_Pres	Sel_TypeDef SC_PrescalerSelection,uint8_t EtuCk)
*��������:ETU���ã�һ��ETUʱ�䳤������
*��ڲ���:
USCI0_SC_PresSel_TypeDef:SC_PrescalerSelection:SC Clock ʱ����Դ
uint16_t:EtuCk:һ��ETUΪ(EtuCk+1)��SC Clock���������0x004
*���ڲ���:void
**************************************************/
void USCI0_SC_ETU_SET(USCI0_SC_PresSel_TypeDef SC_PrescalerSelection, uint16_t EtuCk)
{
  US0CON2 = (EtuCk >> 8);
  US0CON1 = (uint8_t)EtuCk;
  US0CON2 = (SC_PrescalerSelection << 4);
}
/*****************************************************
*��������:void USCI0_SC_Cmd(FunctionalState NewState)
*��������:���ܿ����ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI0_SC_Cmd(FunctionalState NewState)
{
  if(NewState == DISABLE)
  {
    US0CON0 &= 0X7F;
  }
  else
  {
    US0CON0 |= 0x80;
  }
}
/*****************************************************
*��������:FlagStatus USCI0_SC_GetFlagStatus(USCI0_SC_Flag_TypeDef USCI0_SC_FLAG)
*��������:���SC��־״̬
*��ڲ���:
USCI0_SC_Flag_TypeDef:USCI0_SC_FLAG:�����ȡ�ı�־λ
*���ڲ���:
FlagStatus:USCI0_SC��־״̬
*****************************************************/
FlagStatus USCI0_SC_GetFlagStatus(USCI0_SC_Flag_TypeDef USCI0_SC_FLAG)
{
  //	  �Ĵ����л�����λ:
  //	  0 :US0CON5 (B7H)Ϊ��д״̬�Ĵ���
  //	  1 :US0CON5 (B7H)Ϊ��д������Ϣ�Ĵ���
  US0CON5 |= (USCI0_SC_FLAG & 0X80);
  return USCI0_SC_FLAG & US0CON5;
}
/*****************************************************
*��������:void USCI0_UART_SendData(uint8_t Data)
*��������:USCI0 SC����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI0_SC_SendData(uint8_t Data)
{
  US0CON3 = Data;
}
/*****************************************************
*��������:uint8_t USCI0_SC_ReceiveData(void)
*��������:��ý��ջ����е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI0_SC_ReceiveData(void)
{
  return US0CON3;
}

#endif


#endif
/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/
