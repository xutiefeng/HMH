//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����:			sc95f_uart0.c
//	����:					��ԪӦ���Ŷ�
//	ģ�鹦��:			BTM�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��:
//*************************************************************

#include "sc95f_uart0.h"

#if !defined (SC95R751)
/**************************************************
*��������:void UART0_DeInit(void)
*��������:UART0��ؼĴ�����λ��ȱʡֵ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void UART0_DeInit(void)
{
  SCON = 0X00;
  SBUF = 0X00;
  PCON &= 0X7F;
  IE &= 0XEF;
  IP &= 0XEF;
}

/**************************************************
*��������:void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
						   UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode)
*��������:UART0��ʼ�����ú���
*��ڲ���:
uint32_t:Uart0Fsys:ϵͳʱ��Ƶ��
uint32_t:BaudRate:������
UART0_Mode_Typedef:Mode:UART0����ģʽ
UART0_Clock_Typedef:ClockMode:������ʱ��Դ��TIMER1/TIMER2��
UART0_RX_Typedef:RxMode:��������ѡ��
*���ڲ���:void
**************************************************/
void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
				UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode)
{
  SCON = SCON & 0X2F | Mode | RxMode; //����UART����ģʽ,���ý�������λ

  if(Mode == UART0_Mode_8B)			//UART����ģʽΪģʽ0:8λ��˫��ͬ��ͨ��ģʽ
  {
	//ģʽ0ʱ����Դ��ϵͳʱ�ӻ�ϵͳʱ��12��Ƶ
	if(BaudRate == UART0_BaudRate_FsysDIV12)
	{
	  SCON &= 0XDF;
	}
	else if(BaudRate == UART0_BaudRate_FsysDIV4)
	{
	  SCON |= 0X20;
	}
  }
  else//UART����ģʽΪģʽ1����ģʽ3
  {
	TXCON = (TXCON & 0xCF) | ClockMode; //���ò�����ʱ��Դ

	/* ʹ�ö�ʱ��1��Ϊ�����ʷ����� */
	if(ClockMode == UART0_CLOCK_TIMER1)
	{
	  TH1 = (Uart0Fsys / BaudRate) / 256;
	  TL1 = (Uart0Fsys / BaudRate) % 256;
	  TR1 = 0;
	}
	/* ʹ�ö�ʱ��2��Ϊ�����ʷ����� */
	else if(ClockMode == UART0_CLOCK_TIMER2)
	{
	  TXINX = 0X02;			//TIMx�Ĵ�����ָ��ʱ��2
	  RCAPXH = (Uart0Fsys / BaudRate) / 256;
	  RCAPXL = (Uart0Fsys / BaudRate) % 256;
	  TRX = 1;
	}
  }
		EUART = 1;     //����Uart�ж�
}

/*****************************************************
*��������:void UART0_SendData8(uint8_t Data)
*��������:UART0����8λ����
*��ڲ���:
uint8_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void UART0_SendData8(uint8_t Data)
{
  SBUF = Data;
}

/**************************************************
*��������:uint8_t UART0_ReceiveData8(void)
*��������:���SBUF�е�ֵ
*��ڲ���:void
*���ڲ���:
uint8_t:���յ�����
**************************************************/
uint8_t UART0_ReceiveData8(void)
{
  return SBUF;
}

/*****************************************************
*��������:void UART0_SendData9(uint16_t Data)
*��������:UART0����9λ����
*��ڲ���:
uint16_t:Data:���͵�����
*���ڲ���:void
*****************************************************/
void UART0_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  /* ��9λ���ݷ���SCON�Ĵ����ĵ�3λ */
  if(Data_9Bit)
  {
	SCON |= 0X08;
  }
  else
  {
	SCON &= 0XF7;
  }

  SBUF = (uint8_t)Data;
}

/**************************************************
*��������:uint16_t UART0_ReceiveData9(void)
*��������:���SBUF�е�ֵ���ھ�λ��ֵ
*��ڲ���:void
*���ڲ���:
uint16_t:SBUF�е�ֵ���ھ�λ��ֵ
**************************************************/
uint16_t UART0_ReceiveData9(void)
{
  uint16_t Data9;
  /* ��9λ���ݷ���SCON�Ĵ����ĵ�3λ */
  Data9 = SBUF + ((uint16_t)(SCON & 0X04) << 6);
  SCON &= 0XFB;
  return Data9;
}

/*****************************************************
*��������:void UART0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*��������:UART0�жϳ�ʼ��
*��ڲ���:
FunctionalState:NewState:�ж�ʹ��/�ر�ѡ��
PriorityStatus:Priority:�ж����ȼ�ѡ��
*���ڲ���:void
*****************************************************/
void UART0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if(NewState == DISABLE)
  {
	EUART = 0;
  }
  else
  {
	EUART = 1;
  }

  //�����ж����ȼ�
  if(Priority == LOW)
  {
	IPUART = 0;
  }
  else
  {
	IPUART = 1;
  }
}

/*****************************************************
*��������:StatusTypeDef UART0_Transmit8(UART0_HandleInfoDef* UART_HANDLE, uint8_t *pData, uint8_t Size,uint32_t Timeout)
*��������:UART����ѯģʽ�·��ʹ�������
*��ڲ���:
UART0_HandleInfoDef*:UART_HANDLE:ָ�����ָ��UARTģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:����������
uint16_t:Size:�����͵�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef UART0_Transmit(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt;
  /* ���һ�����ͽ����Ƿ����ڽ��� */
  if(UART_HANDLE->TxState == UART0_STATE_READY)
  {
	/* ���������ݳ��ȱ������0�����򷵻ش���״̬ */
	if(Size == 0U)
	{
	  return  Status_ERROR;
	}

	UART_HANDLE->TxState = UART0_STATE_BUSY;			//���ͽ���æµ��
	TI = 0; //����ǰ������ͱ�־
	UART_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
	UART_HANDLE->TxXferSize = Size;			//���´����͵�������
	UART_HANDLE->TxXferCount = 0;			//���ͼ���������

    while(UART_HANDLE->TxXferCount < UART_HANDLE->TxXferSize)//�ж��Ƿ������������
    {
      if((((SCON & 0xC0))) == UART0_Mode_11B)
      {
        SCON &= ~0x08;
        SCON |= (*(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount) >> 5);//���õ�9λУ��λ
        SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount);
      }
      else
      {
        SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u8 + UART_HANDLE->TxXferCount);
      }
      
      while(!TI) //�ȴ��������
      {
        if(TimeoutCnt++ > Timeout)
        {
          UART_HANDLE->TxState = UART0_STATE_TIMEOUT;//���ͳ�ʱ
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
      }
			
      UART0_ClearFlag(UART0_FLAG_TI); //�����жϱ�־�����д1/д0����
			TimeoutCnt = 0;
	  UART_HANDLE->TxXferCount ++;	//��������������
	}

	if(UART_HANDLE->TxXferCount == UART_HANDLE->TxXferSize)
	{
	  UART_HANDLE->TxState = UART0_STATE_READY;	//�������
	  return Status_OK;
	}
	else
	{
	  UART_HANDLE->TxState = UART0_STATE_ERROR;//�������ʹ���
	  return Status_ERROR;
	}
  }
  else
  {
	return Status_BUSY;//����æµ״̬
  }
}

/*****************************************************
*��������:StatusTypeDef UART0_Transmit8_IT(UART0_HandleInfoDef *UART_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ������main�����е��ô˺���
*��ڲ���:
UART0_HandleInfoDef*:UART_HANDLE:ָ�����ָ��UARTģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:���������ݵ��׵�ַ
uint16_t:Size:�����͵�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef UART0_Transmit_IT(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size)
{
	/* ���һ�����ͽ����Ƿ����ڽ��� */
	if (UART_HANDLE->TxState == UART0_STATE_READY)
	{
		/* û�п���UART0�жϻ�����������СΪ0�����ش��� */
		if ((EUART == 0) || (Size == 0U))
		{
			return Status_ERROR;
		}

	UART_HANDLE->TxState = UART0_STATE_BUSY;			//���ͽ���æµ��
	TI = 0; //����ǰ������ͱ�־
	UART_HANDLE->pTxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
	UART_HANDLE->TxXferSize = Size;			//���´����͵�������
	UART_HANDLE->TxXferCount = 0;			//���ͼ���������


	/* ���͵�1֡���� */
	if((SCON & 0xC0) == UART0_Mode_11B)
	{
	  SCON |= (*(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount) >> 5) & 0x08;//���õ�9λУ��λ
	  SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount);
	}
	else
	{
	  SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u8 + UART_HANDLE->TxXferCount);
	}
	return Status_OK;
  }
  else
  {
	return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef UART0_Transmit_IRQHandler(UART0_HandleInfoDef *UART_HANDLE)
*��������:UART���ж�ģʽ�·��ʹ�������ʱ�����жϷ������е���
*	ע���ú�������ǰ��Ҫ�ж��жϱ�־λ�Ƿ�����
*��ڲ���:
UART0_HandleInfoDef*:UART_HANDLE:ָ�����ָ��UARTģ���������Ϣ�ṹ���ָ��
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef UART0_Transmit_IRQHandler(UART0_HandleInfoDef* UART_HANDLE)
{
	/* ���ڷ����߳� */
	if (UART_HANDLE->TxState == UART0_STATE_BUSY)
	{
		UART_HANDLE->TxXferCount++;
		/* �ж���һ�η����Ƿ������һ�� */
		if (UART_HANDLE->TxXferCount < UART_HANDLE->TxXferSize)
		{
			/* ������������δΪ0���������� */
			if((SCON & 0xC0) == UART0_Mode_11B)
			{
				SCON |= (*(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount) >> 5) & 0x08;//���õ�9λУ��λ
				SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount);
			}
			else
			{
				SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u8 + UART_HANDLE->TxXferCount);
			}
			return Status_BUSY;
		}
		else if (UART_HANDLE->TxXferCount == UART_HANDLE->TxXferSize)
		{
			/* ������� */
			UART_HANDLE->TxState = UART0_STATE_READY;
			return Status_OK;
		}
		else
		{
			/* ����������Ϊ0ʱ���������ݣ����ش��� */
			UART_HANDLE->TxState = UART0_STATE_ERROR;
			return Status_ERROR;
		}
	}
	else
	{
		return Status_ERROR;
	}
}

/*****************************************************
*��������:UART0_StatusTypeDef UART0_Receive8(UART0_HandleInfoDef* UART_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*��������:UART����ѯģʽ�½��մ�������
*��ڲ���:
UART0_HandleInfoDef*:UART_HANDLE:ָ�����ָ��UARTģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ��
uint16_t:Size:�����յ�������
uint32_t:Timeout:��ʱʱ������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef UART0_Receive(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  /* �����ս����������� */
  if(UART_HANDLE->RxState == UART0_STATE_READY)
  {
    /* ��������������С���Ƿ�������ʹ�� */
    if((Size == 0U) || ((SCON & 0x10) != UART0_RX_ENABLE))
    {
      return  Status_ERROR;
    }

	UART_HANDLE->RxState = UART0_STATE_BUSY;			//״̬����Ϊ����æµ��
	RI = 0;			 //����ǰ��������жϱ�־
	UART_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ����������ݵĵ�ַ
	UART_HANDLE->RxXferSize = Size;			//���´����͵�������
	UART_HANDLE->RxXferCount = 0;			//���ͼ���������

    while(UART_HANDLE->RxXferCount < UART_HANDLE->RxXferSize) //�ж��Ƿ������������
    {
      if(RI) //�жϽ��ձ�־λ
      {
        if((SCON & 0xC0) == UART0_Mode_11B)
        {
          *(UART_HANDLE->pRxBuffPtr.Size_u16 + UART_HANDLE->RxXferCount) = SBUF + ((uint16_t)(SCON & 0X04) << 6); //��������
        }
        else
        {
          *(UART_HANDLE->pRxBuffPtr.Size_u8 + UART_HANDLE->RxXferCount) = SBUF;			//��������
        }
				RI = 0;			//���ձ�־λ����
		UART_HANDLE->RxXferCount++;	//����������������1
		TimeoutCnt = 0;//�յ����ݣ���ʱ��������
	  }
	  else
	  {
		if(TimeoutCnt++ > Timeout)//��ʱ���
		{
		  UART_HANDLE->RxState = UART0_STATE_TIMEOUT;//���ͳ�ʱ
		  return Status_TIMEOUT;
		}
				WDTCON |= 0x10;			//ι������ֹ�ȴ�ʱ�����������WDT��λ
	  }
	}

	if(UART_HANDLE->RxXferCount == UART_HANDLE->RxXferSize)
	{
	  UART_HANDLE->RxState = UART0_STATE_READY;	//�������
	  return Status_OK;
	}
	else
	{
	  UART_HANDLE->RxState = UART0_STATE_ERROR;//���մ���
	  return Status_ERROR;
	}
  }
  else
  {
	return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef UART0_Transmit_IT(UART0_HandleInfoDef *UART_HANDLE, uint8_t *pData, uint8_t Size)
*��������:UART���ж�ģʽ�½��մ�������ʱ������main�����е��ô˺���
*��ڲ���:
UART0_HandleInfoDef*:UART_HANDLE:ָ�����ָ��UARTģ���������Ϣ�ṹ���ָ��
uint8_t *:pData:�������ݵĴ����
uint16_t:Size:�����յ�������
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef UART0_Receive_IT(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size)
{
	/* ���һ�����ս����Ƿ����ڽ��� */
	if (UART_HANDLE->RxState == UART0_STATE_READY)
	{
		/* ����Ƿ���UART0�жϻ�����������СΪ0���Ƿ�������ʹ�ܣ����ش��� */
		if ((EUART == 0) || (Size == 0U) || ((SCON & 0x10) != UART0_RX_ENABLE))
		{
			return UART0_STATE_ERROR;
		}

	UART_HANDLE->RxState = UART0_STATE_BUSY;
	UART_HANDLE->pRxBuffPtr.Size_u8 = pData;			//ָ��������ݵĴ�ŵ�ַ
	UART_HANDLE->RxXferSize = Size;			//���´����յ�������
	UART_HANDLE->RxXferCount = 0;			//���ͼ���������

	return Status_OK;
  }
  else
  {
	return Status_BUSY;//���ر�־λ
  }
}

/*****************************************************
*��������:StatusTypeDef UART0_Receive8_IRQHandler(UART0_HandleInfoDef* UART_HANDLE)
*��������:UART���ж�ģʽ�½��մ�������ʱ�����жϷ������е���
*��ڲ���:
UART0_HandleInfoDef*:UART_HANDLE:ָ�����ָ��UARTģ���������Ϣ�ṹ���ָ��
*���ڲ���:
StatusTypeDef:����ִ��״̬
*****************************************************/
StatusTypeDef UART0_Receive_IRQHandler(UART0_HandleInfoDef* UART_HANDLE)
{
	/* ���һ�����ս����Ƿ����ڽ��� */
	if (UART_HANDLE->RxState == UART0_STATE_BUSY)
	{
		/* ���ڽ����̣߳����������������Ƿ�Ϊ0 */
		if (UART_HANDLE->RxXferCount < UART_HANDLE->RxXferSize)
		{
			if((SCON & 0xC0) == UART0_Mode_11B)
			{
				*(UART_HANDLE->pRxBuffPtr.Size_u16 + UART_HANDLE->RxXferCount) = SBUF + ((uint16_t)(SCON & 0X04) << 6); //��������
			}
			else
			{
				*(UART_HANDLE->pRxBuffPtr.Size_u8 + UART_HANDLE->RxXferCount) = SBUF;			//��������
			}
			UART_HANDLE->RxXferCount++;
			/* �ж���һ�η����Ƿ������һ�� */
			if (UART_HANDLE->RxXferCount == UART_HANDLE->RxXferSize)
			{
				/* ������� */
				UART_HANDLE->RxState = UART0_STATE_READY;
				return Status_OK;
			}
			return Status_BUSY;
			
		}
		else
		{
			/* ����������Ϊ0ʱ���������ݣ����ش��� */
			UART_HANDLE->RxState = UART0_STATE_ERROR;
			return Status_ERROR;
		}
	}
	else
	{
		return Status_BUSY;
	}
}

#endif

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/