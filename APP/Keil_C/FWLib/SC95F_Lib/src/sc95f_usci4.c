//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_USCI4.c
//	����:
//	ģ�鹦��: 		USCI4�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
// ˵��:				���ļ���������SC95Fϵ��оƬ
//*************************************************************

#include "sc95f_USCI4.h"

#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x) || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
|| defined (SC95R602)  || defined (SC95R605)
#if defined(SC95F8x3x) || defined(SC95F7x3x)


#define Select_USCI4()          \
  do {                       \
    USXINX &= 0xF8;          \
    USXINX |= 0X04;          \
  } while(0)

#define USCI4_SetNull()	TMCON = (TMCON & 0XF3)
#define USCI4_SetSPI()	TMCON = (TMCON & 0XF3) | 0X04
#define USCI4_SetTWI()	TMCON = (TMCON & 0XF3) | 0X08
#define USCI4_SetUART()	TMCON = (TMCON & 0XF3) | 0X0C

#define US4CON0 USXCON0
#define US4CON1 USXCON1
#define US4CON2 USXCON2
#define US4CON3 USXCON3

#else

#define Select_USCI4()          \
  do {                       \
    USXINX &= 0xF8;          \
    USXINX |= 0X04;          \
  } while(0)

#define USCI4_SetNull()	TMCON = (TMCON & 0X3F)
#define USCI4_SetSPI()	TMCON = (TMCON & 0X3F) | 0X40
#define USCI4_SetTWI()	TMCON = (TMCON & 0X3F) | 0X80
#define USCI4_SetUART()	TMCON = (TMCON & 0X3F) | 0XC0

#define US4CON0 USXCON0
#define US4CON1 USXCON1
#define US4CON2 USXCON2
#define US4CON3 USXCON3

#endif

void USCI4_DeInit(void)
{
  Select_USCI4();
  USCI4_SetNull();
  US4CON0 = 0X00;
  US4CON1 = 0X00;
  US4CON2 = 0X00;
  US4CON3 = 0X00;
  IE2 &= (~0X04);
  IP2 &= (~0X04);
}

/******************************* SPI���� *************************************/
/**************************************************
*��������:void USCI4_SPI_Init(USCI4_SPI_FirstBit_TypeDef FirstBit, USCI4_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI4_SPI_Mode_TypeDef Mode,
							 USCI4_SPI_ClockPolarity_TypeDef ClockPolarity, USCI4_SPI_ClockPhase_TypeDef ClockPhase,USCI4_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI4_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI��ʼ�����ú���
*��ڲ���:
USCI4_SPI_FirstBit_TypeDef:FirstBit:���ȴ���λѡ��MSB/LSB��
USCI4_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPIʱ��Ƶ��ѡ��
USCI4_SPI_Mode_TypeDef:Mode:SPI����ģʽѡ��
USCI4_SPI_ClockPolarity_TypeDef:ClockPolarity:SPIʱ�Ӽ���ѡ��
USCI4_SPI_ClockPhase_TypeDef:ClockPhase:SPIʱ����λѡ��
USCI4_SPI_TXE_INT_TypeDef:SPI_TXE_INT:���ͻ������ж�����ѡ��,�ù�����USCI4оƬ����Ч
USCI4_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16λ
*���ڲ���:void
**************************************************/
void USCI4_SPI_Init(USCI4_SPI_FirstBit_TypeDef FirstBit,
                    USCI4_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI4_SPI_Mode_TypeDef Mode,
                    USCI4_SPI_ClockPolarity_TypeDef ClockPolarity, USCI4_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI4_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI4_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetSPI();//USCI4����ΪSPIģʽ
  SPI_TXE_INT = USCI4_SPI_TXE_DISINT; //SPI_TXE_INT�ù�����USCI4оƬ����Ч
  US4CON1 = US4CON1 & (~0X05) | FirstBit | TransmissionMode;
  US4CON0 = US4CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*��������:void USCI4_TransmissionMode(USCI4_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI ����ģʽ���ú���
*��ڲ���:
USCI4_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16eλ
*���ڲ���:void
**************************************************/
void USCI4_TransmissionMode(USCI4_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetSPI();//USCI4����ΪSPIģʽ
  if(TransmissionMode == USCI4_SPI_DATA8)
  {
    US4CON1 &= 0xFD;
  }
  else
  {
    US4CON1 |= 0x02;
  }
}

/*****************************************************
*��������:void USCI4_SPI_Cmd(FunctionalState NewState)
*��������:SPI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI4_SPI_Cmd(FunctionalState NewState)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetSPI();//USCI4����ΪSPIģʽ

  if(NewState != DISABLE)
  {
    US4CON0 |= 0X80;
  }
  else
  {
    US4CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI4_SPI_SendData_8(uint8_t Data)
*��������:USCI4 SPI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI4_SPI_SendData_8(uint8_t Data)
{
  Select_USCI4();//ѡ�����USCI4
  US4CON2 = Data;
}

/*****************************************************
*��������:uint8_t USCI4_SPI_ReceiveData_8(void)
*��������:���US4CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI4_SPI_ReceiveData_8(void)
{
  Select_USCI4();//ѡ�����USCI4
  return US4CON2;
}

/*****************************************************
*��������:void USCI4_SPI_SendData_16(uint16_t Data)
*��������:US4CON2 SPI��������
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI4_SPI_SendData_16(uint16_t Data)
{
  Select_USCI4();//ѡ�����USCI4
  US4CON3 = (uint8_t)(Data >> 8);
  US4CON2 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI4_SPI_ReceiveData_16(void)
*��������:���US4CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI4_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  Select_USCI4();//ѡ�����USCI4
  SPI_data = (uint16_t)((US4CON3 << 8) | US4CON2);
  return SPI_data;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
*��������:SPI0 �жϴ���
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ����� SPI0 ��Ϣ�� USCI4_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
void
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  Select_USCI4();//ѡ�����USCI4

  if(USCI4_HANDLE->RxState == USCI4_STATE_BUSY)
  {
    /* ������û�н������ */
    if(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize)
    {
      /* SPI����16λͨ��ģʽ */
      if((US4CON1 & USCI4_SPI_DATA16) != 0)
      {
        *(USCI4_HANDLE->pRxBuffPtr.Size_u16 + USCI4_HANDLE->RxXferCount) = USCI4_SPI_ReceiveData_16();			//��ȡ16λ����
      }
      /* SPI����8λͨ��ģʽ */
      else
      {
        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = USCI4_SPI_ReceiveData_8();			//��ȡ16λ����
      }

      TempStatus = Status_BUSY;
      USCI4_HANDLE->RxXferCount++;			//���յ����ݣ�����ֵ��1

      /* ���ݽ�����ɣ�״̬λ�����޸� */
      if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
      {
        USCI4_HANDLE->RxState = USCI4_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI4_HANDLE->TxState == USCI4_STATE_BUSY)
    {

      USCI4_HANDLE->TxXferCount++;			//���ݷ�����ɣ�����ֵ��1
      /* ������û�з������ */
      if(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize)
      {
        /* SPI����16λͨ��ģʽ */
        if((US4CON1 & USCI4_SPI_DATA16) != 0)
        {
          USCI4_SPI_SendData_16(*(USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount));			//����16λ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          USCI4_SPI_SendData_8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount));			//����16λ����
        }
        TempStatus = Status_BUSY;
      }
      /* ���ݷ�����ɣ�״̬λ�����޸�  */
      else if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
      {
        USCI4_HANDLE->TxState = USCI4_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* ��SPI��������ʱ��SPI��Ҫ�������ݣ�����ͬ�����յ����� */
    else if(USCI4_HANDLE->RxXferSize)
    {
      USCI4_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�½��մ�������
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI4 ״̬
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI4();//ѡ�����USCI4

  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return USCI4_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((USXCON0 & USCI4_SPI_MODE_MASTER) != 0)
    {
      return USCI4_SPI_TransmitReceive(USCI4_HANDLE, pData, pData, Size, Timeout);			//��ת������ģʽ�����շ�����
    }

    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);			 //����ǰ��������жϱ�־
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize)
    {
      /* �ȴ�SPI�жϱ�־λ���� */
      if(USCI4_GetFlagStatus(USCI4_SPI_FLAG_SPIF))
      {
        /* SPI����16λͨ��ģʽ */
        if((US4CON1 & USCI4_SPI_DATA16) != 0)
        {
          *(USCI4_HANDLE->pRxBuffPtr.Size_u16 + USCI4_HANDLE->RxXferCount) = USCI4_SPI_ReceiveData_16();			//��ȡ16λ����,���ҵ�ַ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = USCI4_SPI_ReceiveData_8();			//��ȡ16λ����,���ҵ�ַ����
        }
        USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);				//�����־λ
        USCI4_HANDLE->RxXferCount++;	//����������������1
        TimeoutCnt = 0;			//��ʱ����ֵ����
      }
      else
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
            return USCI4_STATE_TIMEOUT;				//���س�ʱ����
        }
        WDTCON |= 0x10;          //ι����������ֹ���Ź���λ
      }
    }
    if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
    {
      USCI4_HANDLE->RxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;//���մ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t *pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:USCI4 ״̬
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 == 0) || (Size == 0U))
    {
      return USCI4_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US4CON0 & USCI4_SPI_MODE_MASTER) != 0)
    {
      return USCI4_SPI_TransmitReceive_IT(USCI4_HANDLE, pData, pData, Size);			//��ת���ж�ģʽ�����շ�����
    }

    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//״̬����Ϊ����æµ��
    USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
uint32_t Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI4 ״̬
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);			 //����ǰ�����־λ
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US4CON1 & USCI4_SPI_DATA16) != 0) /* SPI����16λͨ��ģʽ */
      {
        USCI4_SPI_SendData_16(*USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount);			//����16λ���ݲ������ݵ�ַ����
      }
      else/* SPI����8λͨ��ģʽ */
      {
        USCI4_SPI_SendData_8(*USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount);			//����8λ���ݲ������ݵ�ַ����
      }
      while(!USCI4_GetFlagStatus(USCI4_SPI_FLAG_SPIF))			//�ȴ��������
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if(TimeoutCnt++ > Timeout)
        {
          USCI4_HANDLE->TxState = USCI4_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }
      USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);				//����ǰ�������־λ
      TimeoutCnt = 0;			//��ʱ����������
      USCI4_HANDLE->TxXferCount++;	//��������������
    }
    if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
    {
      USCI4_HANDLE->TxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI4_HandleInfoDef *USCI4_HANDLE:ָ����� SPI0 ��Ϣ�� USCI4_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI4 ״̬
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
{

  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* û�п���USCI4�жϻ�����������СΪ0�����ش��� */
    if((IE2 & 0x08 == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = 0;			//���ͼ�������0

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US4CON1 & USCI4_SPI_DATA16) != 0)
    {
      USCI4_SPI_SendData_16(*(USCI4_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI4_SPI_SendData_8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//״̬λ�޸�Ϊæµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_TransmitReceive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*��������:����ģʽ�շ�һ��������
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ����� SPI0 ��Ϣ�� USCI4_HandleInfoDef �ṹ���ָ��
uint8_t*:pTxData:ָ�������ݻ����ָ��
uint8_t*:pRxData:ָ��������ݻ����ָ��
uint16_t:Size:�������ݵĴ�С
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:��������״̬
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_TransmitReceive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if((USCI4_HANDLE->TxState == USCI4_STATE_READY) && (USCI4_HANDLE->RxState == USCI4_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI4_HANDLE->TxState = USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //ָ����������ݵĵ�ַ
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI4_HANDLE->TxXferSize = USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize);
    {

      if((US0CON1 & USCI4_SPI_DATA16) != 0)
      {
        USCI4_SPI_SendData_16(*(USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI4_GetFlagStatus(USCI4_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI4_HANDLE->RxState = USCI4_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount) = USCI4_SPI_ReceiveData_16();
      }
      else
      {
        USCI4_SPI_SendData_8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI4_GetFlagStatus(USCI4_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI4_HANDLE->RxState = USCI4_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount) = USCI4_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//��ʱ����ֵ����
      USCI4_HANDLE->TxXferCount++;			//����������ֵ��1
      USCI4_HANDLE->TxXferCount++;			//����������ֵ��1
    }
    if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
    {
      USCI4_HANDLE->TxState = USCI4_HANDLE->RxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI4_SPI_TransmitReceive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
*��������:SPI���ж�ģʽ�·��ͺͽ��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI4_HandleInfoDef *USCI4_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:���ͺͽ������ݵĴ����
uint16_t:Size:�����ͺͽ��յ�������
*���ڲ���:
StatusTypeDef:USCI4 ״̬
**********************************************************************************************************/
StatusTypeDef USCI4_SPI_TransmitReceive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
  if((USCI4_HANDLE->TxState == USCI4_STATE_READY) && (USCI4_HANDLE->RxState == USCI4_STATE_READY))
  {
    Select_USCI4();//ѡ�����USCI4

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI4_SPI_Cmd(DISABLE);
    USCI4_HANDLE->TxState = USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI4_SPI_Cmd(ENABLE);

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US4CON1 & USCI4_SPI_DATA16) != 0)
    {
      USCI4_SPI_SendData_16(*(USCI4_HANDLE->pTxBuffPtr.Size_u16));			//����16λ����
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI4_SPI_SendData_8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8));			//����8λ����
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
*��������:void USCI4_TWI_Slave_Init(uint8_t TWI_Address)
*��������:USCI4 TWI �ӻ���ʼ�����ú���
*��ڲ���:
uint8_t:TWI_Address:TWI��Ϊ�ӻ�ʱ��7λ�ӻ���ַ
*���ڲ���:void
**************************************************/
void USCI4_TWI_Slave_Init(uint8_t TWI_Address)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetTWI();//USCI4����ΪTWIģʽ
  US4CON2 = TWI_Address << 1;
}

/**************************************************
*��������:void USCI4_TWI_MasterCommunicationRate(USCI4_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*��������:USCI4 TWI����ģʽ��ͨѶ�����趨
*��ڲ���:
USCI4_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI����ģʽ��ͨѶ����
*���ڲ���:void
**************************************************/
void USCI4_TWI_MasterCommunicationRate(USCI4_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetTWI();//USCI4����ΪTWIģʽ

  US4CON1 = TWI_MasterCommunicationRate;
}

/**************************************************
*��������:void USCI4_TWI_Start(void)
*��������:USCI4 TWI ��ʼλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI4_TWI_Start(void)
{
  Select_USCI4();//ѡ�����USCI4
  US4CON1 |= 0x20;
}

/**************************************************
*��������:void USCI4_TWI_MasterModeStop(void)
*��������:USCI4 TWI����ģʽֹͣλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI4_TWI_MasterModeStop(void)
{
  Select_USCI4();//ѡ�����USCI4
  US4CON1 |= 0x10;
}

/**************************************************
*��������:void USCI4_TWI_SlaveClockExtension(void)
*��������:USCI4 TWI�ӻ�ģʽʱ���ӳ�����λ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI4_TWI_SlaveClockExtension(FunctionalState NewState)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetTWI();//USCI4����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US4CON1 |= 0x40;
  }
  else
  {
    US4CON1 &= 0XBF;
  }
}

/**************************************************
*��������:void USCI4_TWI_AcknowledgeConfig(FunctionalState NewState)
*��������:TWI����Ӧ��ʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI4_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetTWI();//USCI4����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US4CON0 |= 0X08;
  }
  else
  {
    US4CON0 &= 0XF7;
  }
}

/**************************************************
*��������:void USCI4_TWI_GeneralCallCmd(FunctionalState NewState)
*��������:TWIͨ�õ�ַ��Ӧʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI4_TWI_GeneralCallCmd(FunctionalState NewState)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetTWI();//USCI4����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US4CON2 |= 0X01;
  }
  else
  {
    US4CON2 &= 0XFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI4_GetTWIStatus(USCI4_TWIState_TypeDef USCI4_TWIState)
*��������:��ȡTWI״̬
*��ڲ���:
USCI4_TWIState_TypeDef:USCI4_TWIState:TWI״̬����
*���ڲ���:
FlagStatus:USCI4_TWI��־״̬
*****************************************************/
FlagStatus USCI4_GetTWIStatus(USCI4_TWIState_TypeDef USCI4_TWIState)
{
  if((US4CON0 & 0x07) == USCI4_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*��������:void USCI4_TWI_Cmd(FunctionalState NewState)
*��������:TWI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI4_TWI_Cmd(FunctionalState NewState)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetTWI();//USCI4����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US4CON0 |= 0X80;
  }
  else
  {
    US4CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI4_TWI_SendAddr(uint8_t Addr,USCI4_TWI_RWType RW)
*��������:TWI���͵�ַ����д����
*��ڲ���:
uint8_t:Addr:���͵ĵ�ַ
USCI4_TWI_RWType:RW:��д����
*���ڲ���:void
*****************************************************/
void USCI4_TWI_SendAddr(uint8_t Addr, USCI4_TWI_RWType RW)
{
  US4CON3 = (Addr << 1) | RW;
}


/*****************************************************
*��������:void USCI4_TWI_SendData(uint8_t Data)
*��������:TWI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI4_TWI_SendData(uint8_t Data)
{
  US4CON3 = Data;
}


/*****************************************************
*��������:uint8_t USCI4_TWI_ReceiveData(void)
*��������:���US4CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI4_TWI_ReceiveData(void)
{
  return US4CON3;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI4_TWI_Wait_TWIF(USCI4_HandleInfoDef* USCI4_HANDLE,uint32_t Timeout)
*�������ܣ��ȴ�TWIF����
*��ڲ�����
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Wait_TWIF(USCI4_HandleInfoDef* USCI4_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  while(!(USCI4_GetFlagStatus(USCI4_TWI_FLAG_TWIF)))			//�ȴ������źŷ������
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* ��ʱ����״̬ */
      if(USCI4_HANDLE->TxState == USCI4_STATE_BUSY)
        USCI4_HANDLE->TxState = USCI4_STATE_TIMEOUT;
      if(USCI4_HANDLE->RxState == USCI4_STATE_BUSY)
        USCI4_HANDLE->RxState = USCI4_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			//�����־λ
  return Status_OK;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI4_TWI_Master_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ܣ�����8λ����ʱ��������ѯģʽ�·��ʹ�������
*��ڲ�����
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*��pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Master_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI4();
    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI4_TWI_Start();			//���������ź�
    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI4_GetTWIStatus(USCI4_TWI_MasterSendAddress) == RESET)          //���״̬��״̬
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US4CON3 = (slaveAddr << 1) & 0xFE;			//���͵�ַ�Ͷ�дλ
    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI4_GetTWIStatus(USCI4_TWI_MasterSendData) == RESET)          //���״̬��״̬
      {
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US4CON3 = *(USCI4_HANDLE->pTxBuffPtr.Size_u8  + USCI4_HANDLE->TxXferCount);             //TWI��������
      if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI4_HANDLE->TxXferCount++;
    }
    while(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize);

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI4_TWI_MasterModeStop();

    if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
    {
      USCI4_HANDLE->TxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }

  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Slave_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�·��ʹ�������
* ע��Ĭ�Ͽ���ʱ���ӳ�ģʽ
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Slave_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI4();
    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI4_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
    USCI4_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ���ַƥ��
    {
      USCI4_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI4_GetTWIStatus(USCI4_TWI_SlaveSendData) == SET)           //���״̬��״̬
      {

        US4CON3 = *(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount);             //TWI��������

        if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)             //�ȴ������źŷ������
        {
          USCI4_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��
          return Status_TIMEOUT;
        }
        USCI4_HANDLE->TxXferCount++;

        if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize - 1)

          USCI4_TWI_AcknowledgeConfig(DISABLE);            //�ر�AAʹ��

      }
      else if(USCI4_GetTWIStatus(USCI4_TWI_SlaveDisableACK) == SET)
      {

        USCI4_HANDLE->TxXferCount++;
      }
      else
      {
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        return Status_ERROR;
      }
    }
    while(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize);

    if(USCI4_GetTWIStatus(USCI4_TWI_SlaveDisableACK) == RESET)           //���״̬��״̬
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
      return Status_ERROR;
    }

    if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
    {
      USCI4_HANDLE->TxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Master_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
*���ڲ���:
StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Master_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x08) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI4();
      IE2 &= ~(0x04);			//�ر��ж�

      USCI4_HANDLE->TxState = USCI4_STATE_WAIT;			//���ͽ���æµ��
      USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI4_TWI_Start();			//���������ź�

      if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_MasterModeStop();
        IE2 |= 0x04;	//�����ж�
        return Status_TIMEOUT;
      }

      if(USCI4_GetTWIStatus(USCI4_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
      {
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_MasterModeStop();
        IE2 |= 0x04;	//�����ж�
        return Status_ERROR;
      }

      IE2 |= 0x04;						//�����ж�
      US4CON3 = (slaveAddr << 1) & 0xFE;//���͵�ַ�Ͷ�дλ

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Master_Transmit_IRQHandler(USCI4_HandleInfoDef *USCI4_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Master_Transmit_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  if(USCI4_HANDLE->TxState == USCI4_STATE_BUSY)
  {
    if(USCI4_GetTWIStatus(USCI4_TWI_MasterSendData) == SET)
    {
      if(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize)
      {
        USCI4_HANDLE->TxXferCount++;			//��ַ֡Ҳ��Ϊ���ݼ���
        US4CON3 = *(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount);             //TWI��������
        return Status_BUSY;
      }
      else if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_MasterModeStop();
        USCI4_HANDLE->TxState = USCI4_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_MasterModeStop();
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI4_GetTWIStatus(USCI4_TWI_MasterReceivedaUACK) == SET)
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_HANDLE->TxXferCount++;
      USCI4_TWI_MasterModeStop();
      if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
      {
        USCI4_HANDLE->TxState = USCI4_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI4_TWI_MasterModeStop();
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI4_HANDLE->TxState == USCI4_STATE_WAIT)
  {
    //��ַ�ɹ���Ӧ
    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;
    US4CON3 = *(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}
/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Slave_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ӻ�ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
* ע
*��ڲ���:*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI4_TWI_Slave_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x08) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI4();

      USCI4_TWI_AcknowledgeConfig(DISABLE);         //����AA
      USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI4_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI4_HANDLE->TxState = USCI4_STATE_WAIT;			//���ͽ���æµ��
      USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI4_TWI_AcknowledgeConfig(ENABLE);         //����AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Slave_Transmit_IRQHandler(USCI4_HandleInfoDef *USCI4_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�·��ʹ�������
*��ڲ���:*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Slave_Transmit_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  if(USCI4_HANDLE->TxState == USCI4_STATE_BUSY)
  {

    if(USCI4_GetTWIStatus(USCI4_TWI_SlaveSendData) == SET)
    {
      if(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize)
      {
        USCI4_HANDLE->TxXferCount++;
        US4CON3 = *(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount);
        if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize - 1)
        {
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        }
        return Status_BUSY;
      }
      else
      {
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.�����ݴ�������У����������޸���AA����λ
    2.���յ�����������UACK */
    else if((USCI4_GetTWIStatus(USCI4_TWI_SlaveDisableACK) == SET) || (USCI4_GetTWIStatus(USCI4_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI4_HANDLE->TxXferCount++;
      if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)			//���������ѷ���
      {
        USCI4_HANDLE->TxState = USCI4_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �������δ���͹��̳���
      1.���ݷ��ͺ���ܵ�UACK
      2.TWI���ǹ����ڴӻ�����״̬ */
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI4_HANDLE->TxState == USCI4_STATE_WAIT)
  {
    if(USCI4_GetTWIStatus(USCI4_TWI_SlaveSendData) == SET)
    {
      US4CON3 = *(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount);
      USCI4_HANDLE->TxState = USCI4_STATE_BUSY;
    }
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}
/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Master_Receive(USCI4_HandleInfoDef* USCI4_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*��������:����8λ����ʱ��������ѯģʽ�½��մ�������
*��ڲ���:*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
					 Timeout    ��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Master_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI4();
    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI4_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    USCI4_TWI_Start();			//���������ź�
    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      USCI4_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI4_GetTWIStatus(USCI4_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
      USCI4_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    US4CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ
    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI4_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI4_GetTWIStatus(USCI4_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI4_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* ���ݽ��չ��� */
    do
    {
      if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI4_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI4_GetTWIStatus(USCI4_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
      {
        USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI4_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;             //TWI��������
      USCI4_HANDLE->RxXferCount++;
      if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize - 1)
      {
        USCI4_TWI_AcknowledgeConfig(DISABLE);                                     //�ر�AA
        if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI4_TWI_MasterModeStop();
          return Status_TIMEOUT;
        }
        if(USCI4_GetTWIStatus(USCI4_TWI_MasterReceivedaUACK) == RESET)           //���״̬��״̬
        {
          USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI4_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;             //TWI��������
        USCI4_HANDLE->RxXferCount++;
      }
    }
    while(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize);
    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI4_TWI_MasterModeStop();

    if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
    {
      USCI4_HANDLE->RxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Slave_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�½��մ�������
*��ڲ���:
USCI4_HandleInfoDef:*USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Slave_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI4();
    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI4_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    /* ���յ�ַ���� */
    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_TIMEOUT;
    }

    if(USCI4_GetTWIStatus(USCI4_TWI_SlaveReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }

    /* �������ݹ��� */
    do
    {
      if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        return Status_TIMEOUT;
      }

      if(USCI4_GetTWIStatus(USCI4_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
      {
        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;             //TWI��������
        USCI4_HANDLE->RxXferCount++;
        if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize - 1)
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UAC
      }
      else if(USCI4_GetTWIStatus(USCI4_TWI_SlaveIdle) == SET)
      {
        if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize - 1)
        {
          *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;
          USCI4_HANDLE->RxXferCount++;
        }
        else
        {
          USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_ERROR;
        }
      }
      else
      {
        USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        return Status_ERROR;
      }

    }
    while(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize);

    if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
    {
      USCI4_HANDLE->RxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Master_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI4_TWI_Master_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x08) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI4();
    IE2 &= ~(0x04);	//�ر��ж�
    USCI4_HANDLE->RxState = USCI4_STATE_WAIT;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI4_TWI_AcknowledgeConfig(ENABLE);         //����AA
    USCI4_TWI_Start();			//���������ź�

    if(USCI4_TWI_Wait_TWIF(USCI4_HANDLE, 0xFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI4_TWI_MasterModeStop();
      IE2 |= 0x04;	//�����ж�
      return Status_TIMEOUT;
    }

    if(USCI4_GetTWIStatus(USCI4_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI4_TWI_MasterModeStop();
      IE2 |= 0x04;	//�����ж�
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    IE2 |= 0x04;	//�����ж�
    US4CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Master_Receive_IRQHandler(USCI4_HandleInfoDef *USCI4_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�½��մ�������
*��ڲ���:*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Master_Receive_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  if(USCI4_HANDLE->RxState == USCI4_STATE_BUSY)
  {
    if(USCI4_GetTWIStatus(USCI4_TWI_MasterReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize)
      {
        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;             //TWI��������
        USCI4_HANDLE->RxXferCount++;
        if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI4_TWI_MasterModeStop();
          USCI4_HANDLE->RxState = USCI4_STATE_READY;	//�������
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI4_TWI_MasterModeStop();
        USCI4_HANDLE->RxState = USCI4_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else if(USCI4_HANDLE->RxState == USCI4_STATE_WAIT)
  {
    if(USCI4_GetTWIStatus(USCI4_TWI_MasterReceivedaData) == SET)
    {
      USCI4_HANDLE->RxState = USCI4_STATE_BUSY;
    }
    return Status_BUSY;
  }
  return Status_ERROR;
}

/*****************************************************
*��������:void USCI4_TWI_Slave_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size))
*��������:�ӻ�ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
		   *pData	ָ��ָ��洢��
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI4_TWI_Slave_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x08) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI4();
    USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI4_ClearFlag(USCI4_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI4_HANDLE->RxState = USCI4_STATE_WAIT;			//���ͽ���æµ��
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI4_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_TWI_Slave_Receive_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�½��մ�������
*��ڲ���:
USCI4_HandleInfoDef:*USCI4_HANDLE ָ��ָ��USCI4������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI4_TWI_Slave_Receive_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  if(USCI4_HANDLE->RxState == USCI4_STATE_BUSY)
  {
    if(USCI4_GetTWIStatus(USCI4_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize)
      {
        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;             //TWI��������
        USCI4_HANDLE->RxXferCount++;
        if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize - 1)
        {
          USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        }
      }
      else
      {
        USCI4_TWI_AcknowledgeConfig(DISABLE);
        return Status_ERROR;
      }
      return Status_BUSY;
    }
    else if(USCI4_GetTWIStatus(USCI4_TWI_SlaveIdle) == SET)
    {
      if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize - 1)
      {
        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = US4CON3;
        USCI4_HANDLE->RxXferCount++;
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        USCI4_HANDLE->RxState = USCI4_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
      USCI4_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }
  }
  else if(USCI4_HANDLE->RxState == USCI4_STATE_WAIT)
  {
    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;
  }
}

/******************************* UART���� *************************************/
/**************************************************
*��������:void USCI4_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI4_UART_Mode_TypeDef Mode, USCI4_UART_RX_TypeDef RxMode)
*��������:UART��ʼ�����ú���
*��ڲ���:
uint32_t:UARTFsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
USCI4_UART_Mode_TypeDef:Mode:UART1����ģʽ
USCI4_UART_RX_TypeDef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void USCI4_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI4_UART_Mode_TypeDef Mode,
                     USCI4_UART_RX_TypeDef RxMode)
{
  Select_USCI4();//ѡ�����USCI4
  USCI4_SetUART();//USCI4����ΪUARTģʽ
  US4CON0 = US4CON0 & 0X0F | Mode | RxMode;

  if(Mode == USCI4_UART_Mode_8B)
  {
    if(BaudRate == USCI4_UART_BaudRate_FsysDIV12)
    {
      US4CON0 &= 0XDF;
    }
    else if(BaudRate == USCI4_UART_BaudRate_FsysDIV4)
    {
      US4CON0 |= 0X20;
    }
  }
  else
  {
    US4CON2 = UARTFsys / BaudRate / 256;
    US4CON1 = UARTFsys / BaudRate % 256;
  }
}

/*****************************************************
*��������:void USCI4_UART_SendData8(uint8_t Data)
*��������:USCI4 UART1����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI4_UART_SendData8(uint8_t Data)
{
  US4CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI4_UART_ReceiveData8(void)
*��������:���US4CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI4_UART_ReceiveData8(void)
{
  return US4CON3;
}

/*****************************************************
*��������:void USCI4_UART_SendData9(uint16_t Data)
*��������:UART����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI4_UART_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if(Data_9Bit)
  {
    US4CON0 |= 0x08;
  }
  else
  {
    US4CON0 &= 0xf7;
  }

  US4CON3 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI4_UART_ReceiveData9(void)
*��������:���US4CON3�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI4_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US4CON3 + ((uint16_t)(US4CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*��������:StatusTypeDef  USCI4_UART_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI4_UART_Transmit(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  uint32_t delaytime = 0;
  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* ��鷢���������Ƿ�Ϊ0 */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_UART_FLAG_TI); //����ǰ��������жϱ�־��д1/д0����
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US4CON0 & 0xC0) == USCI4_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI4_UART_SendData9(*(USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI4_UART_SendData8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount));
      }

      /* �ȴ�������� */
      delaytime = 0;
      while(!(US4CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI4_HANDLE->TxState = USCI4_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
      }

      USCI4_ClearFlag(USCI4_UART_FLAG_TI); //�����жϱ�־�����д1/д0����
      USCI4_HANDLE->TxXferCount++;	//��������������
    }

    if(USCI4_HANDLE->TxXferCount == USCI4_HANDLE->TxXferSize)
    {
      USCI4_HANDLE->TxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_UART_Transmit_IT(USCI4_HandleInfoDef *USCI4_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI4_UART_Transmit_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_READY)
  {
    /* û�п���USCI4�жϻ�����������СΪ0�����ش��� */
    if(((IE2 & 0x08) == 0) || (USCI4_HANDLE->TxXferSize == 0U))
    {
      return Status_ERROR;
    }

    USCI4_HANDLE->TxState = USCI4_STATE_BUSY;
    USCI4_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->TxXferCount = 0;			//���ͼ���������

    /* ���͵�1֡���� */
    if((US4CON0 & 0xC0) == USCI4_UART_Mode_11B)
    {
      /* ����9λ���� */
      USCI4_UART_SendData9(*(USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount));
    }
    else
    {
      /* ����8λ���� */
      USCI4_UART_SendData8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef  USCI4_UART_Transmit_IRQHandler(USCI4_HandleInfoDef *USCI4_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:*USCI4_HANDLE ָ��ָ��UART������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI4_UART_Transmit_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  Select_USCI4();//ѡ�����USCI4

  /* ���ڷ����߳� */
  if(USCI4_HANDLE->TxState == USCI4_STATE_BUSY)
  {
    USCI4_HANDLE->TxXferCount++;
    /* �ж���һ�η����Ƿ������һ�� */
    if(USCI4_HANDLE->TxXferCount < USCI4_HANDLE->TxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US4CON0 & 0xC0) == USCI4_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI4_UART_SendData9(*(USCI4_HANDLE->pTxBuffPtr.Size_u16 + USCI4_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI4_UART_SendData8(*(USCI4_HANDLE->pTxBuffPtr.Size_u8 + USCI4_HANDLE->TxXferCount));
      }
      return Status_OK;
    }
    else if(USCI4_HANDLE->TxXferCount)
    {
      /* ������� */
      USCI4_HANDLE->TxState = USCI4_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI4_HANDLE->TxState = USCI4_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_UART_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
USCI4_USCI4_HANDLEInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4_UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI4_UART_Receive(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  Select_USCI4();//ѡ�����USCI4

  /* �����ս����������� */
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if((IE2 & 0x08 != 0) || (Size == 0U) || ((US4CON0 & 0x10) != USCI4_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;			//���ͽ���æµ��
    USCI4_ClearFlag(USCI4_UART_FLAG_RI);			 //����ǰ��������жϱ�־
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize)  //�ж��Ƿ������������
    {
      if(USCI4_GetFlagStatus(USCI4_UART_FLAG_RI))  //�жϽ��ձ�־λ
      {
        USCI4_ClearFlag(USCI4_UART_FLAG_RI);			//���ձ�־λ����

        /* ������������δΪ0���������� */
        if((US4CON0 & 0xC0) == USCI4_UART_Mode_11B)
        {
          /* ����9λ���� */
          *(USCI4_HANDLE->pRxBuffPtr.Size_u16 + USCI4_HANDLE->RxXferCount) = USCI4_UART_ReceiveData9();
        }
        else
        {
          /* ����8λ���� */
          *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = USCI4_UART_ReceiveData8();
        }

        USCI4_HANDLE->RxXferCount++;	//��������������
        delaytime = 0;//�յ����ݣ���ʱ��������
      }
      if(delaytime++ > Timeout) //��ʱ���
      {
        USCI4_HANDLE->RxState = USCI4_STATE_TIMEOUT;//���ͳ�ʱ
        return Status_TIMEOUT;
      }
    }

    if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
    {
      USCI4_HANDLE->RxState = USCI4_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_UART_Receive_IT(USCI4_HandleInfoDef *USCI4_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI4_USCI4_HANDLEInfoDef*:USCI4_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI4_UART_Receive_IT(USCI4_HandleInfoDef* USCI4_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI4_HANDLE->RxState == USCI4_STATE_READY)
  {
    /* ����Ƿ���USCI4_UART�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
    if(((IE2 & 0x08) == 0) || (Size == 0U) || ((US4CON0 & 0x10) != USCI4_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI4_HANDLE->RxState = USCI4_STATE_BUSY;
    USCI4_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
    USCI4_HANDLE->RxXferSize = Size;			//���´����յ�������
    USCI4_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI4_UART_Receive_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI4_HandleInfoDef*:USCI4_HANDLE:ָ��ָ��USCI4������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI4_UART_Receive_IRQHandler(USCI4_HandleInfoDef* USCI4_HANDLE)
{
  Select_USCI4();//ѡ�����USCI4

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI4_HANDLE->RxState == USCI4_STATE_BUSY)
  {
    /* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
    if(USCI4_HANDLE->RxXferCount < USCI4_HANDLE->RxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US4CON0 & 0xC0) == USCI4_UART_Mode_11B)
      {
        /* ����9λ���� */
        *(USCI4_HANDLE->pRxBuffPtr.Size_u16 + USCI4_HANDLE->RxXferCount) = USCI4_UART_ReceiveData9();
      }
      else
      {
        /* ����8λ���� */
        *(USCI4_HANDLE->pRxBuffPtr.Size_u8 + USCI4_HANDLE->RxXferCount) = USCI4_UART_ReceiveData8();
      }
      USCI4_HANDLE->RxXferCount++;
      /* �ж���һ�η����Ƿ������һ�� */
      if(USCI4_HANDLE->RxXferCount == USCI4_HANDLE->RxXferSize)
      {
        /* ������� */
        USCI4_HANDLE->RxState = USCI4_STATE_READY;
        return Status_OK;
			}
       return Status_BUSY;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI4_HANDLE->RxState = USCI4_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:FlagStatus USCI4_GetFlagStatus(USCI4_Flag_TypeDef USCI4_FLAG)
*��������:���USCI4��־״̬
*��ڲ���:
USCI4_Flag_TypeDef:USCI4_FLAG:�����ȡ�ı�־λ
*���ڲ���:
FlagStatus:USCI4��־״̬
*****************************************************/
FlagStatus USCI4_GetFlagStatus(USCI4_Flag_TypeDef USCI4_FLAG)
{
  FlagStatus bitstatus = RESET;
  Select_USCI4();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
	  if((USCI4_FLAG == USCI4_SPI_FLAG_SPIF) || (USCI4_FLAG == USCI4_SPI_FLAG_WCOL) || (USCI4_FLAG == USCI4_SPI_FLAG_TXE))
		{
			if((USCI4_FLAG & US4CON1) != (uint8_t)RESET)
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
		if(USCI4_FLAG == USCI4_TWI_FLAG_TXRXnE)
		{
			if((USCI4_FLAG & US4CON1) != (uint8_t)RESET)
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
			if((USCI4_FLAG & US4CON0) != (uint8_t)RESET)
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
    if((USCI4_FLAG & US4CON0) != (uint8_t)RESET)
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
*��������:void USCI4_ClearFlag(USCI4_Flag_TypeDef USCI4_FLAG)
*��������:���USCI4��־״̬
*��ڲ���:
USCI4_Flag_TypeDef:USCI4_FLAG:��������ı�־λ
*���ڲ���:void
*****************************************************/
void USCI4_ClearFlag(USCI4_Flag_TypeDef USCI4_FLAG)
{
  Select_USCI4();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
	  if((USCI4_FLAG == USCI4_SPI_FLAG_SPIF) || (USCI4_FLAG == USCI4_SPI_FLAG_WCOL) || (USCI4_FLAG == USCI4_SPI_FLAG_TXE))
		{
		  US4CON1 &= (~USCI4_FLAG); //�����Ĵ���US4CON1	
		}
	}else if((TMCON & 0xC0)==0x80)  //TWIģʽ
	{
	  if(USCI4_FLAG == USCI4_TWI_FLAG_TXRXnE)
    {
      US4CON1 &= (~USCI4_FLAG); //�����Ĵ���US4CON1	
    }
		else
    {
       US4CON0 &= (~USCI4_FLAG); //�����Ĵ���US4CON0
    }	
	}else if((TMCON & 0xC0)==0xC0)  //UARTģʽ
	{
	  if((USCI4_FLAG == USCI4_UART_FLAG_TI) || (USCI4_FLAG == USCI4_UART_FLAG_RI))
    {
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605)
     US4CON0 = US4CON0  & 0xFC | USCI4_FLAG;//д1����
#else
     US4CON0 &= (~USCI4_FLAG); //д0����
#endif
    }	
	}
}

/*****************************************************
*��������:void USCI4_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:USCI4�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void USCI4_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{

  if(NewState != DISABLE)
  {
    IE2 |= 0x08;
  }
  else
  {
    IE2 &= 0xF7;
  }

  /************************************************************/
  if(Priority != LOW)
  {
    IP2 |= 0x08;
  }
  else
  {
    IP2 &= 0xF7;
  }
}

#endif
/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/