//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_usci2.c
//	����		:
//	ģ�鹦��: 		USCI2�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
// ˵��:���ļ���������SC95Fϵ��оƬ
//*************************************************************

#include "sc95f_usci2.h"

#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
 || defined (SC95R751) || defined (SC95F7610B) || defined (SC95F7619B) || defined (SC95R602)  || defined (SC95R605)
#define Select_USCI2()          \
  do {                       \
    USXINX &= 0xF8;          \
    USXINX |= 0X02;          \
  } while(0)

#define USCI2_SetNull() TMCON = (TMCON & 0X3F)
#define USCI2_SetSPI()	TMCON = (TMCON & 0X3F) | 0X40
#define USCI2_SetTWI()	TMCON = (TMCON & 0X3F) | 0X80
#define USCI2_SetUART()	TMCON = (TMCON & 0X3F) | 0XC0

#define US2CON0 USXCON0
#define US2CON1 USXCON1
#define US2CON2 USXCON2
#define US2CON3 USXCON3

#else

#define Select_USCI2()

#define USCI2_SetNull() TMCON = (TMCON & 0X3F)
#define USCI2_SetSPI() TMCON = (TMCON & 0X3F) | 0X40
#define USCI2_SetTWI() TMCON = (TMCON & 0X3F) | 0X80
#define USCI2_SetUART() TMCON = (TMCON & 0X3F) | 0XC0

#endif

/**************************************************
*��������:void USCI2_DeInit(void)
*��������:USCI2��ؼĴ�����λ��ȱʡֵ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI2_DeInit(void)
{
  Select_USCI2();
  USCI2_SetNull();
  US2CON0 = 0X00;
  US2CON1 = 0X00;
  US2CON2 = 0X00;
  US2CON3 = 0X00;
  IE2 &= (~0X02);
  IP2 &= (~0X02);
}


/******************************* SPI���� *************************************/
/**************************************************
*��������:void USCI2_SPI_Init(USCI2_SPI_FirstBit_TypeDef FirstBit, USCI2_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI2_SPI_Mode_TypeDef Mode,
							 USCI2_SPI_ClockPolarity_TypeDef ClockPolarity, USCI2_SPI_ClockPhase_TypeDef ClockPhase,USCI2_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI2_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI��ʼ�����ú���
*��ڲ���:
USCI2_SPI_FirstBit_TypeDef:FirstBit:���ȴ���λѡ��MSB/LSB��
USCI2_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPIʱ��Ƶ��ѡ��
USCI2_SPI_Mode_TypeDef:Mode:SPI����ģʽѡ��
USCI2_SPI_ClockPolarity_TypeDef:ClockPolarity:SPIʱ�Ӽ���ѡ��
USCI2_SPI_ClockPhase_TypeDef:ClockPhase:SPIʱ����λѡ��
USCI2_SPI_TXE_INT_TypeDef:SPI_TXE_INT:���ͻ������ж�����ѡ��,�ù�����USCI2оƬ����Ч
USCI2_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16λ
*���ڲ���:void
**************************************************/
void USCI2_SPI_Init(USCI2_SPI_FirstBit_TypeDef FirstBit,
                    USCI2_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI2_SPI_Mode_TypeDef Mode,
                    USCI2_SPI_ClockPolarity_TypeDef ClockPolarity, USCI2_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI2_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI2_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetSPI();//USCI2����ΪSPIģʽ
  SPI_TXE_INT = USCI2_SPI_TXE_DISINT; //SPI_TXE_INT�ù�����USCI2оƬ����Ч
  US2CON1 = US2CON1 & (~0X05) | FirstBit | TransmissionMode;
  US2CON0 = US2CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*��������:void USCI2_TransmissionMode(USCI2_TransmissionMode_TypeDef TransmissionMode)
*��������:SPI ����ģʽ���ú���
*��ڲ���:
USCI2_TransmissionMode_TypeDef:TransmissionMode:SPI����ģʽѡ�� 8/16eλ
*���ڲ���:void
**************************************************/
void USCI2_TransmissionMode(USCI2_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetSPI();//USCI2����ΪSPIģʽ
  if(TransmissionMode == USCI2_SPI_DATA8)
  {
    US2CON1 &= 0xFD;
  }
  else
  {
    US2CON1 |= 0x02;
  }
}

/*****************************************************
*��������:void USCI2_SPI_Cmd(FunctionalState NewState)
*��������:SPI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI2_SPI_Cmd(FunctionalState NewState)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetSPI();//USCI2����ΪSPIģʽ

  if(NewState != DISABLE)
  {
    US2CON0 |= 0X80;
  }
  else
  {
    US2CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI2_SPI_SendData_8(uint8_t Data)
*��������:USCI2 SPI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI2_SPI_SendData_8(uint8_t Data)
{
  Select_USCI2();//ѡ�����USCI2
  US2CON2 = Data;
}

/*****************************************************
*��������:uint8_t USCI2_SPI_ReceiveData_8(void)
*��������:���US2CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI2_SPI_ReceiveData_8(void)
{
  Select_USCI2();//ѡ�����USCI2
  return US2CON2;
}

/*****************************************************
*��������:void USCI2_SPI_SendData_16(uint16_t Data)
*��������:US2CON2 SPI��������
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI2_SPI_SendData_16(uint16_t Data)
{
  Select_USCI2();//ѡ�����USCI2
  US2CON3 = (uint8_t)(Data >> 8);
  US2CON2 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI2_SPI_ReceiveData_16(void)
*��������:���US2CON2�е�ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI2_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  Select_USCI2();//ѡ�����USCI2
  SPI_data = (uint16_t)((US2CON3 << 8) | US2CON2);
  return SPI_data;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
*��������:SPI0 �жϴ���
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ����� SPI0 ��Ϣ�� USCI2_HandleInfoDef �ṹ���ָ�롣
*���ڲ���:
void
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  Select_USCI2();//ѡ�����USCI2

  if(USCI2_HANDLE->RxState == USCI2_STATE_BUSY)
  {
    /* ������û�н������ */
    if(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize)
    {
      /* SPI����16λͨ��ģʽ */
      if((US2CON1 & USCI2_SPI_DATA16) != 0)
      {
        *(USCI2_HANDLE->pRxBuffPtr.Size_u16 + USCI2_HANDLE->RxXferCount) = USCI2_SPI_ReceiveData_16();			//��ȡ16λ����
      }
      /* SPI����8λͨ��ģʽ */
      else
      {
        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = USCI2_SPI_ReceiveData_8();			//��ȡ16λ����
      }

      TempStatus = Status_BUSY;
      USCI2_HANDLE->RxXferCount++;			//���յ����ݣ�����ֵ��1

      /* ���ݽ�����ɣ�״̬λ�����޸� */
      if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
      {
        USCI2_HANDLE->RxState = USCI2_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI2_HANDLE->TxState == USCI2_STATE_BUSY)
    {

      USCI2_HANDLE->TxXferCount++;			//���ݷ�����ɣ�����ֵ��1
      /* ������û�з������ */
      if(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize)
      {
        /* SPI����16λͨ��ģʽ */
        if((US2CON1 & USCI2_SPI_DATA16) != 0)
        {
          USCI2_SPI_SendData_16(*(USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount));			//����16λ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          USCI2_SPI_SendData_8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount));			//����16λ����
        }
        TempStatus = Status_BUSY;
      }
      /* ���ݷ�����ɣ�״̬λ�����޸�  */
      else if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)
      {
        USCI2_HANDLE->TxState = USCI2_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* ��SPI��������ʱ��SPI��Ҫ�������ݣ�����ͬ�����յ����� */
    else if(USCI2_HANDLE->RxXferSize)
    {
      USCI2_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�½��մ�������
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI2 ״̬
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI2();//ѡ�����USCI2

  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 != 0) || (Size == 0U))
    {
      return USCI2_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US2CON0 & USCI2_SPI_MODE_MASTER) != 0)
    {
      return USCI2_SPI_TransmitReceive(USCI2_HANDLE, pData, pData, Size, Timeout);			//��ת������ģʽ�����շ�����
    }

    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);			 //����ǰ��������жϱ�־
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize)
    {
      /* �ȴ�SPI�жϱ�־λ���� */
      if(USCI2_GetFlagStatus(USCI2_SPI_FLAG_SPIF))
      {
        /* SPI����16λͨ��ģʽ */
        if((US2CON1 & USCI2_SPI_DATA16) != 0)
        {
          *(USCI2_HANDLE->pRxBuffPtr.Size_u16 + USCI2_HANDLE->RxXferCount) = USCI2_SPI_ReceiveData_16();			//��ȡ16λ����,���ҵ�ַ����
        }
        /* SPI����8λͨ��ģʽ */
        else
        {
          *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = USCI2_SPI_ReceiveData_8();			//��ȡ16λ����,���ҵ�ַ����
        }
        USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);				//�����־λ
        USCI2_HANDLE->RxXferCount++;	//����������������1
        TimeoutCnt = 0;			//��ʱ����ֵ����
      }
      else
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
            return USCI2_STATE_TIMEOUT;				//���س�ʱ����
        }
        WDTCON |= 0x10;          //ι����������ֹ���Ź���λ
      }
    }
    if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
    {
      USCI2_HANDLE->RxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;//���մ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t *pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:USCI2 ״̬
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 == 0) || (Size == 0U))
    {
      return USCI2_STATE_ERROR;
    }

    /* SPI��Ϊ����ʱ���������ݱض����淢������ */
    if((US2CON0 & USCI2_SPI_MODE_MASTER) != 0)
    {
      return USCI2_SPI_TransmitReceive_IT(USCI2_HANDLE, pData, pData, Size);			//��ת���ж�ģʽ�����շ�����
    }

    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//״̬����Ϊ����æµ��
    USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_Transmit(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:SPI����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
uint32_t Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:USCI2 ״̬
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_Transmit(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI2_HANDLE->TxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);			 //����ǰ�����־λ
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US2CON1 & USCI2_SPI_DATA16) != 0) /* SPI����16λͨ��ģʽ */
      {
        USCI2_SPI_SendData_16(*USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount);			//����16λ���ݲ������ݵ�ַ����
      }
      else/* SPI����8λͨ��ģʽ */
      {
        USCI2_SPI_SendData_8(*USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount);			//����8λ���ݲ������ݵ�ַ����
      }
      while(!USCI2_GetFlagStatus(USCI2_SPI_FLAG_SPIF))			//�ȴ��������
      {
        /* ��ʱ�����������ۼӣ�ֱ�������趨�ĳ�ʱʱ�� */
        if(TimeoutCnt++ > Timeout)
        {
          USCI2_HANDLE->TxState = USCI2_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }
      USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);				//����ǰ�������־λ
      TimeoutCnt = 0;			//��ʱ����������
      USCI2_HANDLE->TxXferCount++;	//��������������
    }
    if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)
    {
      USCI2_HANDLE->TxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//����æµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ж�ģʽ����һ��������
*��ڲ���:
USCI2_HandleInfoDef *USCI2_HANDLE:ָ����� SPI0 ��Ϣ�� USCI2_HandleInfoDef �ṹ���ָ�롣
uint8_t *pData:ָ�����ݻ����ָ�롣
uint16_t Size:�������ݵĴ�С
*���ڲ���:
StatusTypeDef:USCI2 ״̬
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
{

  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* û�п���USCI2�жϻ�����������СΪ0�����ش��� */
    if((IE2 & 0x02 == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI2_HANDLE->TxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = 0;			//���ͼ�������0

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US2CON1 & USCI2_SPI_DATA16) != 0)
    {
      USCI2_SPI_SendData_16(*(USCI2_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI2_SPI_SendData_8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//״̬λ�޸�Ϊæµ״̬
  }
}

/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_TransmitReceive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*��������:����ģʽ�շ�һ��������
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ����� SPI0 ��Ϣ�� USCI2_HandleInfoDef �ṹ���ָ��
uint8_t*:pTxData:ָ�������ݻ����ָ��
uint8_t*:pRxData:ָ��������ݻ����ָ��
uint16_t:Size:�������ݵĴ�С
uint32_t:Timeout:��ʱʱ��
*���ڲ���:
StatusTypeDef:��������״̬
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_TransmitReceive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if((USCI2_HANDLE->TxState == USCI2_STATE_READY) && (USCI2_HANDLE->RxState == USCI2_STATE_READY))
  {

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI2_HANDLE->TxState = USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //ָ����������ݵĵ�ַ
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI2_HANDLE->TxXferSize = USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize);
    {

      if((US2CON1 & USCI2_SPI_DATA16) != 0)
      {
        USCI2_SPI_SendData_16(*(USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI2_GetFlagStatus(USCI2_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI2_HANDLE->RxState = USCI2_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount) = USCI2_SPI_ReceiveData_16();
      }
      else
      {
        USCI2_SPI_SendData_8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount));			//����16λ���ݲ��ҵ�ַ����
        while(!USCI2_GetFlagStatus(USCI2_SPI_FLAG_SPIF))			//�ȴ��������
        {
          /* �ȴ�ʱ���Ƿ����˳�ʱ */
          if(TimeoutCnt++ > Timeout)
          {
            USCI2_HANDLE->RxState = USCI2_STATE_TIMEOUT;//���մ���
            return Status_TIMEOUT;				//���س�ʱ����
          }
          WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
        }
        USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);				//����ǰ�������־λ
        *(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount) = USCI2_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//��ʱ����ֵ����
      USCI2_HANDLE->TxXferCount++;			//����������ֵ��1
      USCI2_HANDLE->TxXferCount++;			//����������ֵ��1
    }
    if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)
    {
      USCI2_HANDLE->TxState = USCI2_HANDLE->RxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }
}


/**********************************************************************************************************
*��������:StatusTypeDef USCI2_SPI_TransmitReceive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
*��������:SPI���ж�ģʽ�·��ͺͽ��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI2_HandleInfoDef *USCI2_HANDLE:ָ�����ָ��SPIģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:���ͺͽ������ݵĴ����
uint16_t:Size:�����ͺͽ��յ�������
*���ڲ���:
StatusTypeDef:USCI2 ״̬
**********************************************************************************************************/
StatusTypeDef USCI2_SPI_TransmitReceive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
  if((USCI2_HANDLE->TxState == USCI2_STATE_READY) && (USCI2_HANDLE->RxState == USCI2_STATE_READY))
  {
    Select_USCI2();//ѡ�����USCI2

    /* �����ͺͽ������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI2_SPI_Cmd(DISABLE);
    USCI2_HANDLE->TxState = USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_SPI_FLAG_SPIF);			//����жϱ�־λ
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->TxXferSize = USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI2_SPI_Cmd(ENABLE);

    /* ���͵�1byte���� */
    /* SPI����16λͨ��ģʽ */
    if((US2CON1 & USCI2_SPI_DATA16) != 0)
    {
      USCI2_SPI_SendData_16(*(USCI2_HANDLE->pTxBuffPtr.Size_u16));			//����16λ����
    }
    /* SPI����8λͨ��ģʽ */
    else
    {
      USCI2_SPI_SendData_8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8));			//����8λ����
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
*��������:void USCI2_TWI_Slave_Init(uint8_t TWI_Address)
*��������:USCI2 TWI �ӻ���ʼ�����ú���
*��ڲ���:
uint8_t:TWI_Address:TWI��Ϊ�ӻ�ʱ��7λ�ӻ���ַ
*���ڲ���:void
**************************************************/
void USCI2_TWI_Slave_Init(uint8_t TWI_Address)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetTWI();//USCI2����ΪTWIģʽ
  US2CON2 = TWI_Address << 1;
}

/**************************************************
*��������:void USCI2_TWI_MasterCommunicationRate(USCI2_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*��������:USCI2 TWI����ģʽ��ͨѶ�����趨
*��ڲ���:
USCI2_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI����ģʽ��ͨѶ����
*���ڲ���:void
**************************************************/
void USCI2_TWI_MasterCommunicationRate(USCI2_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetTWI();//USCI2����ΪTWIģʽ

  US2CON1 = TWI_MasterCommunicationRate;
}

/**************************************************
*��������:void USCI2_TWI_Start(void)
*��������:USCI2 TWI ��ʼλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI2_TWI_Start(void)
{
  Select_USCI2();//ѡ�����USCI2
  US2CON1 |= 0x20;
}

/**************************************************
*��������:void USCI2_TWI_MasterModeStop(void)
*��������:USCI2 TWI����ģʽֹͣλ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI2_TWI_MasterModeStop(void)
{
  Select_USCI2();//ѡ�����USCI2
  US2CON1 |= 0x10;
}

/**************************************************
*��������:void USCI2_TWI_SlaveClockExtension(void)
*��������:USCI2 TWI�ӻ�ģʽʱ���ӳ�����λ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void USCI2_TWI_SlaveClockExtension(FunctionalState NewState)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetTWI();//USCI2����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US2CON1 |= 0x40;
  }
  else
  {
    US2CON1 &= 0XBF;
  }
}

/**************************************************
*��������:void USCI2_TWI_AcknowledgeConfig(FunctionalState NewState)
*��������:TWI����Ӧ��ʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI2_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetTWI();//USCI2����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US2CON0 |= 0X08;
  }
  else
  {
    US2CON0 &= 0XF7;
  }
}

/**************************************************
*��������:void USCI2_TWI_GeneralCallCmd(FunctionalState NewState)
*��������:TWIͨ�õ�ַ��Ӧʹ�ܺ���
*��ڲ���:
FunctionalState:NewState:����Ӧ��ʹ��/ʧ��ѡ��
*���ڲ���:void
**************************************************/
void USCI2_TWI_GeneralCallCmd(FunctionalState NewState)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetTWI();//USCI2����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US2CON2 |= 0X01;
  }
  else
  {
    US2CON2 &= 0XFE;
  }
}

/*****************************************************
*��������:FlagStatus USCI2_GetTWIStatus(USCI2_TWIState_TypeDef USCI2_TWIState)
*��������:��ȡTWI״̬
*��ڲ���:
USCI2_TWIState_TypeDef:USCI2_TWIState:TWI״̬����
*���ڲ���:
FlagStatus:USCI2_TWI��־״̬
*****************************************************/
FlagStatus USCI2_GetTWIStatus(USCI2_TWIState_TypeDef USCI2_TWIState)
{
  if((US2CON0 & 0x07) == USCI2_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*��������:void USCI2_TWI_Cmd(FunctionalState NewState)
*��������:TWI���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void USCI2_TWI_Cmd(FunctionalState NewState)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetTWI();//USCI2����ΪTWIģʽ

  if(NewState != DISABLE)
  {
    US2CON0 |= 0X80;
  }
  else
  {
    US2CON0 &= (~0X80);
  }
}

/*****************************************************
*��������:void USCI2_TWI_SendAddr(uint8_t Addr,USCI2_TWI_RWType RW)
*��������:TWI���͵�ַ����д����
*��ڲ���:
uint8_t:Addr:���͵ĵ�ַ
USCI2_TWI_RWType:RW:��д����
*���ڲ���:void
*****************************************************/
void USCI2_TWI_SendAddr(uint8_t Addr, USCI2_TWI_RWType RW)
{
  US2CON3 = (Addr << 1) | RW;
}


/*****************************************************
*��������:void USCI2_TWI_SendData(uint8_t Data)
*��������:TWI��������
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI2_TWI_SendData(uint8_t Data)
{
  US2CON3 = Data;
}


/*****************************************************
*��������:uint8_t USCI2_TWI_ReceiveData(void)
*��������:���US2CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI2_TWI_ReceiveData(void)
{
  return US2CON3;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI2_TWI_Wait_TWIF(USCI2_HandleInfoDef* USCI2_HANDLE,uint32_t Timeout)
*�������ܣ��ȴ�TWIF����
*��ڲ�����
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��USCI2������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Wait_TWIF(USCI2_HandleInfoDef* USCI2_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  while(!(USCI2_GetFlagStatus(USCI2_TWI_FLAG_TWIF)))			//�ȴ������źŷ������
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* ��ʱ����״̬ */
      if(USCI2_HANDLE->TxState == USCI2_STATE_BUSY)
        USCI2_HANDLE->TxState = USCI2_STATE_TIMEOUT;
      if(USCI2_HANDLE->RxState == USCI2_STATE_BUSY)
        USCI2_HANDLE->RxState = USCI2_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			//�����־λ
  return Status_OK;
}

/*****************************************************
*�������ƣ�StatusTypeDef USCI2_TWI_Master_Transmit(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*�������ܣ�����8λ����ʱ��������ѯģʽ�·��ʹ�������
*��ڲ�����
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��USCI2������Ϣ�����
uint8_t:slaveAddr:�ӻ���ַ
uint8_t*��pData	ָ��ָ��洢��
uint32_t:Size:�洢���ݳ���
uint32_t:Timeout:��ʱʱ������
*���ڲ�����StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Master_Transmit(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x01 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI2_HANDLE->TxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = 0;			//���ͼ���������

    USCI2_TWI_Start();			//���������ź�
    if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI2_GetTWIStatus(USCI2_TWI_MasterSendAddress) == RESET)          //���״̬��״̬
    {
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US2CON3 = (slaveAddr << 1) & 0xFE;			//���͵�ַ�Ͷ�дλ
    if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI2_GetTWIStatus(USCI2_TWI_MasterSendData) == RESET)          //���״̬��״̬
      {
        USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US1CON3 = *(USCI2_HANDLE->pTxBuffPtr.Size_u8  + USCI2_HANDLE->TxXferCount);             //TWI��������
      if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)           //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI2_HANDLE->TxXferCount++;
    }
    while(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize);

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI2_TWI_MasterModeStop();

    if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)
    {
      USCI2_HANDLE->TxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Master_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��USCI2������Ϣ�����
*���ڲ���:
StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Master_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x02) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI2();
      IE2 &= ~0x02;			//�ر��ж�

      USCI2_HANDLE->TxState = USCI2_STATE_WAIT;			//���ͽ���æµ��
      USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI2_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI2_TWI_Start();			//���������ź�

      if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_MasterModeStop();
        IE2 |= 0x02;	//�����ж�
        return Status_TIMEOUT;
      }

      if(USCI2_GetTWIStatus(USCI2_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
      {
        USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_MasterModeStop();
        IE2 |= 0x02;	//�����ж�
        return Status_ERROR;
      }

      IE2 |= 0x02;						//�����ж�
      US2CON3 = (slaveAddr << 1) & 0xFE;//���͵�ַ�Ͷ�дλ

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Master_Transmit_IRQHandler(USCI2_HandleInfoDef *USCI2_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�·��ʹ�������
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��USCI2������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Master_Transmit_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  if(USCI2_HANDLE->TxState == USCI2_STATE_BUSY)
  {
    if(USCI2_GetTWIStatus(USCI2_TWI_MasterSendData) == SET)
    {
      if(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize)
      {
        USCI2_HANDLE->TxXferCount++;			//��ַ֡Ҳ��Ϊ���ݼ���
        US2CON3 = *(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount);             //TWI��������
        return Status_BUSY;
      }
      else if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_MasterModeStop();
        USCI2_HANDLE->TxState = USCI2_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_MasterModeStop();
        USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI2_GetTWIStatus(USCI2_TWI_MasterReceivedaUACK) == SET)
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_HANDLE->TxXferCount++;
      USCI2_TWI_MasterModeStop();
      if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)
      {
        USCI2_HANDLE->TxState = USCI2_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI2_TWI_MasterModeStop();
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI2_HANDLE->TxState == USCI2_STATE_WAIT)
  {
    //��ַ�ɹ���Ӧ
    USCI2_HANDLE->TxState = USCI2_STATE_BUSY;
    US2CON3 = *(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Slave_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
*��������:�ӻ�ʹ���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
* ע
*��ڲ���:*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI2_TWI_Slave_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x02) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI2();

      USCI2_TWI_AcknowledgeConfig(DISABLE);         //����AA
      USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
      USCI2_TWI_SlaveClockExtension(ENABLE);            //����ʱ���ӳ�
      USCI2_HANDLE->TxState = USCI2_STATE_WAIT;			//���ͽ���æµ��
      USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
      USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
      USCI2_HANDLE->TxXferCount = 0;			//���ͼ���������

      USCI2_TWI_AcknowledgeConfig(ENABLE);         //����AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Slave_Transmit_IRQHandler(USCI2_HandleInfoDef *USCI2_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�·��ʹ�������
*��ڲ���:*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Slave_Transmit_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  if(USCI2_HANDLE->TxState == USCI2_STATE_BUSY)
  {

    if(USCI2_GetTWIStatus(USCI2_TWI_SlaveSendData) == SET)
    {
      if(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize)
      {
        USCI2_HANDLE->TxXferCount++;			//��һ���ж��ǽ��ܵ���ַ��
        US2CON3 = *(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount);
        if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize - 1)
        {
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        }
        return Status_BUSY;
      }
      else
      {
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.�����ݴ�������У����������޸���AA����λ
    2.���յ�����������UACK */
    else if((USCI2_GetTWIStatus(USCI2_TWI_SlaveDisableACK) == SET) || (USCI2_GetTWIStatus(USCI2_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI2_HANDLE->TxXferCount++;
      if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)			//���������ѷ���
      {
        USCI2_HANDLE->TxState = USCI2_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      /* �������δ���͹��̳���
      1.���ݷ��ͺ���ܵ�UACK
      2.TWI���ǹ����ڴӻ�����״̬ */
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else if(USCI2_HANDLE->TxState == USCI2_STATE_WAIT)
  {
    if(USCI2_GetTWIStatus(USCI2_TWI_SlaveSendData) == SET)
    {
      US2CON3 = *(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount);
      USCI2_HANDLE->TxState = USCI2_STATE_BUSY;
    }
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Master_Receive(USCI2_HandleInfoDef* USCI2_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*��������:����8λ����ʱ��������ѯģʽ�½��մ�������
*��ڲ���:*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
					 Timeout    ��ʱʱ������
*���ڲ���:
StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Master_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI2();
    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI2_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    USCI2_TWI_Start();			//���������ź�
    if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      USCI2_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI2_GetTWIStatus(USCI2_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
      USCI2_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    US2CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ
    if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI2_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI2_GetTWIStatus(USCI2_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI2_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* ���ݽ��չ��� */
    do
    {
      if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
      {
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI2_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI2_GetTWIStatus(USCI2_TWI_MasterReceivedaData) == RESET)           //���״̬��״̬
      {
        USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
        /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI2_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;             //TWI��������
      USCI2_HANDLE->RxXferCount++;
      if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize - 1)
      {
        USCI2_TWI_AcknowledgeConfig(DISABLE);                                     //�ر�AA
        if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI2_TWI_MasterModeStop();
          return Status_TIMEOUT;
        }
        if(USCI2_GetTWIStatus(USCI2_TWI_MasterReceivedaUACK) == RESET)           //���״̬��״̬
        {
          USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI2_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;             //TWI��������
        USCI2_HANDLE->RxXferCount++;
      }
    }
    while(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize);
    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
    USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI2_TWI_MasterModeStop();

    if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
    {
      USCI2_HANDLE->RxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Slave_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*��������:����8λ����ʱ���ӻ���ѯģʽ�½��մ�������
*��ڲ���:
USCI2_HandleInfoDef:*USCI2_HANDLE:ָ��ָ��USCI2������Ϣ�����
uint32_t:Timeout:��ʱʱ������
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Slave_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if((IE2 & 0x02 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI2();
    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI2_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    /* ���յ�ַ���� */
    if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ��źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_TIMEOUT;
    }

    if(USCI2_GetTWIStatus(USCI2_TWI_SlaveReceivedaData) == RESET)           //���״̬��״̬
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }

    /* �������ݹ��� */
    do
    {
      if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, Timeout) == Status_TIMEOUT)            //�ȴ������źŷ������
      {
        return Status_TIMEOUT;
      }

      if(USCI2_GetTWIStatus(USCI2_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
      {
        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;             //TWI��������
        USCI2_HANDLE->RxXferCount++;
        if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize - 1)
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UAC
      }
      else if(USCI2_GetTWIStatus(USCI2_TWI_SlaveIdle) == SET)
      {
        if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize - 1)
        {
          *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;
          USCI2_HANDLE->RxXferCount++;
        }
        else
        {
          USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          return Status_ERROR;
        }
      }
      else
      {
        USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        return Status_ERROR;
      }

    }
    while(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize);

    if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
    {
      USCI2_HANDLE->RxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Master_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*��������:����ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI2_TWI_Master_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x02) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI2();
    IE2 &= ~0x02;	//�ر��ж�
    USCI2_HANDLE->RxState = USCI2_STATE_WAIT;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI2_TWI_AcknowledgeConfig(ENABLE);         //����AA
    USCI2_TWI_Start();			//���������ź�

    if(USCI2_TWI_Wait_TWIF(USCI2_HANDLE, 0xFFFF) == Status_TIMEOUT)            //�ȴ������źŷ������
    {
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI2_TWI_MasterModeStop();
      IE2 |= 0x02;	//�����ж�
      return Status_TIMEOUT;
    }

    if(USCI2_GetTWIStatus(USCI2_TWI_MasterSendAddress) == RESET)           //���״̬��״̬
    {
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
      /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      USCI2_TWI_MasterModeStop();
      IE2 |= 0x02;	//�����ж�
      return Status_ERROR;
    }

    /* ���͵�ַ֡���� */
    IE2 |= 0x02;	//�����ж�
    US2CON3 = (slaveAddr << 1) | 0x01;//���͵�ַ�Ͷ�дλ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Master_Receive_IRQHandler(USCI2_HandleInfoDef *USCI2_HANDLE)
*��������:����8λ����ʱ�������ж�ģʽ�½��մ�������
*��ڲ���:*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Master_Receive_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  if(USCI2_HANDLE->RxState == USCI2_STATE_BUSY)
  {
    if(USCI2_GetTWIStatus(USCI2_TWI_MasterReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize)
      {
        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;             //TWI��������
        USCI2_HANDLE->RxXferCount++;
        if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
        {
          /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
          USCI2_TWI_MasterModeStop();
          USCI2_HANDLE->RxState = USCI2_STATE_READY;	//�������
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI2_TWI_MasterModeStop();
        USCI2_HANDLE->RxState = USCI2_STATE_ERROR;//�������ʹ���
        return Status_ERROR;
      }
    }
  }
  else if(USCI2_HANDLE->RxState == USCI2_STATE_WAIT)
  {
    if(USCI2_GetTWIStatus(USCI2_TWI_MasterReceivedaData) == SET)
    {
      USCI2_HANDLE->RxState = USCI2_STATE_BUSY;
    }
    return Status_BUSY;
  }
  return Status_ERROR;
}


/*****************************************************
*��������:void USCI2_TWI_Slave_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size))
*��������:�ӻ�ʹ���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
		   *pData	ָ��ָ��洢��
		   Size        �洢���ݳ���
*���ڲ���:void
*****************************************************/
StatusTypeDef USCI2_TWI_Slave_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
    if(((IE2 & 0x02) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI2();
    USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
    USCI2_ClearFlag(USCI2_TWI_FLAG_TWIF);			 //����ǰ�����־λ
    USCI2_HANDLE->RxState = USCI2_STATE_WAIT;			//���ͽ���æµ��
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������
    USCI2_TWI_AcknowledgeConfig(ENABLE);         //����AAʹ��λ

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_TWI_Slave_Receive_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
*��������:����8λ����ʱ���ӻ��ж�ģʽ�½��մ�������
*��ڲ���:
USCI2_HandleInfoDef:*USCI2_HANDLE ָ��ָ��USCI2������Ϣ�����
*���ڲ���:StatusTypeDef״̬ö��
*****************************************************/
StatusTypeDef USCI2_TWI_Slave_Receive_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  if(USCI2_HANDLE->RxState == USCI2_STATE_BUSY)
  {
    if(USCI2_GetTWIStatus(USCI2_TWI_SlaveReceivedaData) == SET)           //���״̬��״̬
    {
      if(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize)
      {
        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;             //TWI��������
        USCI2_HANDLE->RxXferCount++;
        if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize - 1)
        {
          USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        }
      }
      else
      {
        USCI2_TWI_AcknowledgeConfig(DISABLE);
        return Status_ERROR;
      }
      return Status_BUSY;
    }
    else if(USCI2_GetTWIStatus(USCI2_TWI_SlaveIdle) == SET)
    {
      if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize - 1)
      {
        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = US2CON3;
        USCI2_HANDLE->RxXferCount++;
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ,������UACK
        USCI2_HANDLE->RxState = USCI2_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
        USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
      USCI2_TWI_AcknowledgeConfig(DISABLE);         //�ر�AAʹ��λ
      return Status_ERROR;
    }
  }
  else if(USCI2_HANDLE->RxState == USCI2_STATE_WAIT)
  {
    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;
  }
}

/******************************* UART���� *************************************/
/**************************************************
*��������:void USCI2_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI2_UART_Mode_TypeDef Mode, USCI2_UART_RX_TypeDef RxMode)
*��������:UART��ʼ�����ú���
*��ڲ���:
uint32_t:UARTFsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
USCI2_UART_Mode_TypeDef:Mode:UART1����ģʽ
USCI2_UART_RX_TypeDef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void USCI2_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI2_UART_Mode_TypeDef Mode,
                     USCI2_UART_RX_TypeDef RxMode)
{
  Select_USCI2();//ѡ�����USCI2
  USCI2_SetUART();//USCI2����ΪUARTģʽ
  US2CON0 = US2CON0 & 0X0F | Mode | RxMode;

  if(Mode == USCI2_UART_Mode_8B)
  {
    if(BaudRate == USCI2_UART_BaudRate_FsysDIV12)
    {
      US2CON0 &= 0XDF;
    }
    else if(BaudRate == USCI2_UART_BaudRate_FsysDIV4)
    {
      US2CON0 |= 0X20;
    }
  }
  else
  {
    US2CON2 = UARTFsys / BaudRate / 256;
    US2CON1 = UARTFsys / BaudRate % 256;
  }
}

/*****************************************************
*��������:void USCI2_UART_SendData8(uint8_t Data)
*��������:USCI2 UART1����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI2_UART_SendData8(uint8_t Data)
{
  US2CON3 = Data;
}

/*****************************************************
*��������:uint8_t USCI2_UART_ReceiveData8(void)
*��������:���US2CON3�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
*****************************************************/
uint8_t USCI2_UART_ReceiveData8(void)
{
  return US2CON3;
}

/*****************************************************
*��������:void USCI2_UART_SendData9(uint16_t Data)
*��������:UART����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void USCI2_UART_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if(Data_9Bit)
  {
    US2CON0 |= 0x08;
  }
  else
  {
    US2CON0 &= 0xf7;
  }

  US2CON3 = (uint8_t)Data;
}

/*****************************************************
*��������:uint16_t USCI2_UART_ReceiveData9(void)
*��������:���US2CON3�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:���յ�����
*****************************************************/
uint16_t USCI2_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US2CON3 + ((uint16_t)(US2CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*��������:StatusTypeDef  USCI2_UART_Transmit(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI2_UART_Transmit(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  uint32_t delaytime = 0;
  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* ��鷢���������Ƿ�Ϊ0 */
    if((IE2 & 0x02 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI2_HANDLE->TxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_UART_FLAG_TI); //����ǰ��������жϱ�־��д1/д0����
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize) //�ж��Ƿ������������
    {
      if((US2CON0 & 0xC0) == USCI2_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI2_UART_SendData9(*(USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI2_UART_SendData8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount));
      }

      /* �ȴ�������� */
      delaytime = 0;
      while(!(US2CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI2_HANDLE->TxState = USCI2_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
      }

      USCI2_ClearFlag(USCI2_UART_FLAG_TI); //�����жϱ�־�����д1/д0����
      USCI2_HANDLE->TxXferCount++;	//��������������
    }

    if(USCI2_HANDLE->TxXferCount == USCI2_HANDLE->TxXferSize)
    {
      USCI2_HANDLE->TxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_UART_Transmit_IT(USCI2_HandleInfoDef *USCI2_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI2_UART_Transmit_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_READY)
  {
    /* û�п���USCI2�жϻ�����������СΪ0�����ش��� */
    if(((IE2 & 0x02) == 0) || (USCI2_HANDLE->TxXferSize == 0U))
    {
      return Status_ERROR;
    }

    USCI2_HANDLE->TxState = USCI2_STATE_BUSY;
    USCI2_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->TxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->TxXferCount = 0;			//���ͼ���������

    /* ���͵�1֡���� */
    if((US2CON0 & 0xC0) == USCI2_UART_Mode_11B)
    {
      /* ����9λ���� */
      USCI2_UART_SendData9(*(USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount));
    }
    else
    {
      /* ����8λ���� */
      USCI2_UART_SendData8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef  USCI2_UART_Transmit_IRQHandler(USCI2_HandleInfoDef *USCI2_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:*USCI2_HANDLE ָ��ָ��UART������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI2_UART_Transmit_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  Select_USCI2();//ѡ�����USCI2

  /* ���ڷ����߳� */
  if(USCI2_HANDLE->TxState == USCI2_STATE_BUSY)
  {
    USCI2_HANDLE->TxXferCount++;
    /* �ж���һ�η����Ƿ������һ�� */
    if(USCI2_HANDLE->TxXferCount < USCI2_HANDLE->TxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US2CON0 & 0xC0) == USCI2_UART_Mode_11B)
      {
        /* ����9λ���� */
        USCI2_UART_SendData9(*(USCI2_HANDLE->pTxBuffPtr.Size_u16 + USCI2_HANDLE->TxXferCount));
      }
      else
      {
        /* ����8λ���� */
        USCI2_UART_SendData8(*(USCI2_HANDLE->pTxBuffPtr.Size_u8 + USCI2_HANDLE->TxXferCount));
      }
      return Status_OK;
    }
    else if(USCI2_HANDLE->TxXferCount)
    {
      /* ������� */
      USCI2_HANDLE->TxState = USCI2_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI2_HANDLE->TxState = USCI2_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_UART_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
USCI2_USCI2_HANDLEInfoDef*:USCI2_HANDLE:ָ��ָ��USCI2_UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI2_UART_Receive(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  Select_USCI2();//ѡ�����USCI2

  /* �����ս����������� */
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if((IE2 & 0x02 != 0) || (Size == 0U) || ((US2CON0 & 0x10) != USCI2_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;			//���ͽ���æµ��
    USCI2_ClearFlag(USCI2_UART_FLAG_RI);			 //����ǰ��������жϱ�־
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����͵�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize)  //�ж��Ƿ������������
    {
      if(USCI2_GetFlagStatus(USCI2_UART_FLAG_RI))  //�жϽ��ձ�־λ
      {
        USCI2_ClearFlag(USCI2_UART_FLAG_RI);			//���ձ�־λ����

        /* ������������δΪ0���������� */
        if((US2CON0 & 0xC0) == USCI2_UART_Mode_11B)
        {
          /* ����9λ���� */
          *(USCI2_HANDLE->pRxBuffPtr.Size_u16 + USCI2_HANDLE->RxXferCount) = USCI2_UART_ReceiveData9();
        }
        else
        {
          /* ����8λ���� */
          *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = USCI2_UART_ReceiveData8();
        }

        USCI2_HANDLE->RxXferCount++;	//��������������
        delaytime = 0;//�յ����ݣ���ʱ��������
      }
      if(delaytime++ > Timeout) //��ʱ���
      {
        USCI2_HANDLE->RxState = USCI2_STATE_TIMEOUT;//���ͳ�ʱ
        return Status_TIMEOUT;
      }
    }

    if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
    {
      USCI2_HANDLE->RxState = USCI2_STATE_READY;	//�������
      return Status_OK;
    }
    else
    {
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;//�������ʹ���
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_UART_Receive_IT(USCI2_HandleInfoDef *USCI2_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
USCI2_USCI2_HANDLEInfoDef*:USCI2_HANDLE:ָ��ָ��UART������Ϣ�����
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI2_UART_Receive_IT(USCI2_HandleInfoDef* USCI2_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI2_HANDLE->RxState == USCI2_STATE_READY)
  {
    /* ����Ƿ���USCI2_UART�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
    if(((IE2 & 0x02) == 0) || (Size == 0U) || ((US2CON0 & 0x10) != USCI2_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI2_HANDLE->RxState = USCI2_STATE_BUSY;
    USCI2_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
    USCI2_HANDLE->RxXferSize = Size;			//���´����յ�������
    USCI2_HANDLE->RxXferCount = 0;			//���ͼ���������

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef USCI2_UART_Receive_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*	ע���ú���������Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
USCI2_HandleInfoDef*:USCI2_HANDLE:ָ��ָ��USCI2������Ϣ�����
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef USCI2_UART_Receive_IRQHandler(USCI2_HandleInfoDef* USCI2_HANDLE)
{
  Select_USCI2();//ѡ�����USCI2

  /* ���һ�����ս����Ƿ����ڽ��� */
  if(USCI2_HANDLE->RxState == USCI2_STATE_BUSY)
  {
    /* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
    if(USCI2_HANDLE->RxXferCount < USCI2_HANDLE->RxXferSize)
    {
      /* ������������δΪ0���������� */
      if((US2CON0 & 0xC0) == USCI2_UART_Mode_11B)
      {
        /* ����9λ���� */
        *(USCI2_HANDLE->pRxBuffPtr.Size_u16 + USCI2_HANDLE->RxXferCount) = USCI2_UART_ReceiveData9();
      }
      else
      {
        /* ����8λ���� */
        *(USCI2_HANDLE->pRxBuffPtr.Size_u8 + USCI2_HANDLE->RxXferCount) = USCI2_UART_ReceiveData8();
      }
      USCI2_HANDLE->RxXferCount++;
      /* �ж���һ�η����Ƿ������һ�� */
      if(USCI2_HANDLE->RxXferCount == USCI2_HANDLE->RxXferSize)
      {
        /* ������� */
        USCI2_HANDLE->RxState = USCI2_STATE_READY;
        return Status_OK;
			}

       return Status_BUSY;
    }
    else
    {
      /* ����������Ϊ0ʱ���������ݣ����ش��� */
      USCI2_HANDLE->RxState = USCI2_STATE_ERROR;
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*��������:FlagStatus USCI2_GetFlagStatus(USCI2_Flag_TypeDef USCI2_FLAG)
*��������:���USCI2��־״̬
*��ڲ���:
USCI2_Flag_TypeDef:USCI2_FLAG:�����ȡ�ı�־λ
*���ڲ���:
FlagStatus:USCI2��־״̬
*****************************************************/
FlagStatus USCI2_GetFlagStatus(USCI2_Flag_TypeDef USCI2_FLAG)
{
  FlagStatus bitstatus = RESET;
  Select_USCI2();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)	
		
	  if((USCI2_FLAG == USCI2_SPI_FLAG_SPIF) || (USCI2_FLAG == USCI2_SPI_FLAG_WCOL))
#else	
	  if((USCI2_FLAG == USCI2_SPI_FLAG_SPIF) || (USCI2_FLAG == USCI2_SPI_FLAG_WCOL) || (USCI2_FLAG == USCI2_SPI_FLAG_TXE))
#endif
		{
			if((USCI2_FLAG & US2CON1) != (uint8_t)RESET)
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
		if(USCI2_FLAG == USCI2_TWI_FLAG_TXRXnE)
		{
			if((USCI2_FLAG & US2CON1) != (uint8_t)RESET)
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
			if((USCI2_FLAG & US2CON0) != (uint8_t)RESET)
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
    if((USCI2_FLAG & US2CON0) != (uint8_t)RESET)
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
*��������:void USCI2_ClearFlag(USCI2_Flag_TypeDef USCI2_FLAG)
*��������:���USCI2��־״̬
*��ڲ���:
USCI2_Flag_TypeDef:USCI2_FLAG:��������ı�־λ
*���ڲ���:void
*****************************************************/
void USCI2_ClearFlag(USCI2_Flag_TypeDef USCI2_FLAG)
{
  Select_USCI2();
	if((TMCON & 0xC0)==0x40)  //SPIģʽ
	{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx)	
		
	  if((USCI2_FLAG == USCI2_SPI_FLAG_SPIF) || (USCI2_FLAG == USCI2_SPI_FLAG_WCOL))
#else	
	  if((USCI2_FLAG == USCI2_SPI_FLAG_SPIF) || (USCI2_FLAG == USCI2_SPI_FLAG_WCOL) || (USCI2_FLAG == USCI2_SPI_FLAG_TXE))
#endif
		{
		  US2CON1 &= (~USCI2_FLAG); //�����Ĵ���US2CON1	
		}
	}else if((TMCON & 0xC0)==0x80)  //TWIģʽ
	{
	  if(USCI2_FLAG == USCI2_TWI_FLAG_TXRXnE)
    {
      US2CON1 &= (~USCI2_FLAG); //�����Ĵ���US2CON1	
    }
		else
    {
       US2CON0 &= (~USCI2_FLAG); //�����Ĵ���US2CON0
    }	
	}else if((TMCON & 0xC0)==0xC0)  //UARTģʽ
	{
	  if((USCI2_FLAG == USCI2_UART_FLAG_TI) || (USCI2_FLAG == USCI2_UART_FLAG_RI))
    {
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605) || defined (SC95F8x7x) || defined (SC95F7x7x)|| defined(SC95R503)
     US2CON0 = US2CON0  & 0xFC | USCI2_FLAG;//д1����
#else
     US2CON0 &= (~USCI2_FLAG); //д0����
#endif
    }	
	}
}

/*****************************************************
*��������:void USCI2_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:USCI2�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void USCI2_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if(NewState != DISABLE)
  {
    IE2 |= 0x02;
  }
  else
  {
    IE2 &= 0xFD;
  }

  /************************************************************/
  if(Priority != LOW)
  {
    IP2 |= 0x02;
  }
  else
  {
    IP2 &= 0xFD;
  }
}


/******************* (C) COPYRIGHT 2022 SinOne Microelectronics *****END OF FILE****/
