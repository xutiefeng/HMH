//************************************************************
//  Copyright (c) 深圳市赛元微电子股份有限公司
//	文件名称:			sc95f_uart0.c
//	作者:					赛元应用团队
//	模块功能:			BTM固件库函数C文件
// 	最后更正日期:	2024年1月18日
// 	版本:				V1.0002
//  说明:
//*************************************************************

#include "sc95f_uart0.h"

#if !defined (SC95R751)
/**************************************************
*函数名称:void UART0_DeInit(void)
*函数功能:UART0相关寄存器复位至缺省值
*入口参数:void
*出口参数:void
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
*函数名称:void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
						   UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode)
*函数功能:UART0初始化配置函数
*入口参数:
uint32_t:Uart0Fsys:系统时钟频率
uint32_t:BaudRate:波特率
UART0_Mode_Typedef:Mode:UART0工作模式
UART0_Clock_Typedef:ClockMode:波特率时钟源（TIMER1/TIMER2）
UART0_RX_Typedef:RxMode:接收允许选择
*出口参数:void
**************************************************/
void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
				UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode)
{
  SCON = SCON & 0X2F | Mode | RxMode; //设置UART工作模式,设置接收允许位

  if(Mode == UART0_Mode_8B)			//UART工作模式为模式0:8位半双工同步通信模式
  {
	//模式0时钟来源于系统时钟或系统时钟12分频
	if(BaudRate == UART0_BaudRate_FsysDIV12)
	{
	  SCON &= 0XDF;
	}
	else if(BaudRate == UART0_BaudRate_FsysDIV4)
	{
	  SCON |= 0X20;
	}
  }
  else//UART工作模式为模式1或者模式3
  {
	TXCON = (TXCON & 0xCF) | ClockMode; //设置波特率时钟源

	/* 使用定时器1作为波特率发生器 */
	if(ClockMode == UART0_CLOCK_TIMER1)
	{
	  TH1 = (Uart0Fsys / BaudRate) / 256;
	  TL1 = (Uart0Fsys / BaudRate) % 256;
	  TR1 = 0;
	}
	/* 使用定时器2作为波特率发生器 */
	else if(ClockMode == UART0_CLOCK_TIMER2)
	{
	  TXINX = 0X02;			//TIMx寄存器组指向定时器2
	  RCAPXH = (Uart0Fsys / BaudRate) / 256;
	  RCAPXL = (Uart0Fsys / BaudRate) % 256;
	  TRX = 1;
	}
  }
		EUART = 1;     //开启Uart中断
}

/*****************************************************
*函数名称:void UART0_SendData8(uint8_t Data)
*函数功能:UART0发送8位数据
*入口参数:
uint8_t:Data:发送的数据
*出口参数:void
*****************************************************/
void UART0_SendData8(uint8_t Data)
{
  SBUF = Data;
}

/**************************************************
*函数名称:uint8_t UART0_ReceiveData8(void)
*函数功能:获得SBUF中的值
*入口参数:void
*出口参数:
uint8_t:接收的数据
**************************************************/
uint8_t UART0_ReceiveData8(void)
{
  return SBUF;
}

/*****************************************************
*函数名称:void UART0_SendData9(uint16_t Data)
*函数功能:UART0发送9位数据
*入口参数:
uint16_t:Data:发送的数据
*出口参数:void
*****************************************************/
void UART0_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  /* 第9位数据放在SCON寄存器的第3位 */
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
*函数名称:uint16_t UART0_ReceiveData9(void)
*函数功能:获得SBUF中的值及第九位的值
*入口参数:void
*出口参数:
uint16_t:SBUF中的值及第九位的值
**************************************************/
uint16_t UART0_ReceiveData9(void)
{
  uint16_t Data9;
  /* 第9位数据放在SCON寄存器的第3位 */
  Data9 = SBUF + ((uint16_t)(SCON & 0X04) << 6);
  SCON &= 0XFB;
  return Data9;
}

/*****************************************************
*函数名称:void UART0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*函数功能:UART0中断初始化
*入口参数:
FunctionalState:NewState:中断使能/关闭选择
PriorityStatus:Priority:中断优先级选择
*出口参数:void
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

  //设置中断优先级
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
*函数名称:StatusTypeDef UART0_Transmit8(UART0_HandleInfoDef* UART_HANDLE, uint8_t *pData, uint8_t Size,uint32_t Timeout)
*函数功能:UART在轮询模式下发送大量数据
*入口参数:
UART0_HandleInfoDef*:UART_HANDLE:指向包含指定UART模块的配置信息结构体的指针
uint8_t *:pData:待发送数据
uint16_t:Size:待发送的数据量
uint32_t:Timeout:超时时间设置
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef UART0_Transmit(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt;
  /* 检查一个发送进程是否正在进行 */
  if(UART_HANDLE->TxState == UART0_STATE_READY)
  {
	/* 待发送数据长度必须大于0，否则返回错误状态 */
	if(Size == 0U)
	{
	  return  Status_ERROR;
	}

	UART_HANDLE->TxState = UART0_STATE_BUSY;			//发送进程忙碌中
	TI = 0; //发送前清除发送标志
	UART_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
	UART_HANDLE->TxXferSize = Size;			//更新待发送的数据量
	UART_HANDLE->TxXferCount = 0;			//发送计数器清零

    while(UART_HANDLE->TxXferCount < UART_HANDLE->TxXferSize)//判断是否接收所有数据
    {
      if((((SCON & 0xC0))) == UART0_Mode_11B)
      {
        SCON &= ~0x08;
        SCON |= (*(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount) >> 5);//设置第9位校验位
        SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount);
      }
      else
      {
        SBUF = *(UART_HANDLE->pTxBuffPtr.Size_u8 + UART_HANDLE->TxXferCount);
      }
      
      while(!TI) //等待发送完成
      {
        if(TimeoutCnt++ > Timeout)
        {
          UART_HANDLE->TxState = UART0_STATE_TIMEOUT;//发送超时
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//喂狗，防止等待时间过长，导致WDT复位
      }
			
      UART0_ClearFlag(UART0_FLAG_TI); //发送中断标志清除，写1/写0清零
			TimeoutCnt = 0;
	  UART_HANDLE->TxXferCount ++;	//发送数据量计数
	}

	if(UART_HANDLE->TxXferCount == UART_HANDLE->TxXferSize)
	{
	  UART_HANDLE->TxState = UART0_STATE_READY;	//发送完成
	  return Status_OK;
	}
	else
	{
	  UART_HANDLE->TxState = UART0_STATE_ERROR;//发生发送错误
	  return Status_ERROR;
	}
  }
  else
  {
	return Status_BUSY;//返回忙碌状态
  }
}

/*****************************************************
*函数名称:StatusTypeDef UART0_Transmit8_IT(UART0_HandleInfoDef *UART_HANDLE, uint8_t *pData, uint8_t Size)
*函数功能:UART在中断模式下发送大量数据时，先在main函数中调用此函数
*入口参数:
UART0_HandleInfoDef*:UART_HANDLE:指向包含指定UART模块的配置信息结构体的指针
uint8_t *:pData:待发送数据的首地址
uint16_t:Size:待发送的数据量
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef UART0_Transmit_IT(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size)
{
	/* 检查一个发送进程是否正在进行 */
	if (UART_HANDLE->TxState == UART0_STATE_READY)
	{
		/* 没有开启UART0中断或发送数据量大小为0，返回错误 */
		if ((EUART == 0) || (Size == 0U))
		{
			return Status_ERROR;
		}

	UART_HANDLE->TxState = UART0_STATE_BUSY;			//发送进程忙碌中
	TI = 0; //发送前清除发送标志
	UART_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
	UART_HANDLE->TxXferSize = Size;			//更新待发送的数据量
	UART_HANDLE->TxXferCount = 0;			//发送计数器清零


	/* 发送第1帧数据 */
	if((SCON & 0xC0) == UART0_Mode_11B)
	{
	  SCON |= (*(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount) >> 5) & 0x08;//设置第9位校验位
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
	return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef UART0_Transmit_IRQHandler(UART0_HandleInfoDef *UART_HANDLE)
*函数功能:UART在中断模式下发送大量数据时，在中断服务函数中调用
*	注：该函数调用前需要判断中断标志位是否置起
*入口参数:
UART0_HandleInfoDef*:UART_HANDLE:指向包含指定UART模块的配置信息结构体的指针
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef UART0_Transmit_IRQHandler(UART0_HandleInfoDef* UART_HANDLE)
{
	/* 处于发送线程 */
	if (UART_HANDLE->TxState == UART0_STATE_BUSY)
	{
		UART_HANDLE->TxXferCount++;
		/* 判断上一次发送是否是最后一次 */
		if (UART_HANDLE->TxXferCount < UART_HANDLE->TxXferSize)
		{
			/* 待发送数据量未为0，继续发送 */
			if((SCON & 0xC0) == UART0_Mode_11B)
			{
				SCON |= (*(UART_HANDLE->pTxBuffPtr.Size_u16 + UART_HANDLE->TxXferCount) >> 5) & 0x08;//设置第9位校验位
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
			/* 发送完成 */
			UART_HANDLE->TxState = UART0_STATE_READY;
			return Status_OK;
		}
		else
		{
			/* 发送数据量为0时还发送数据，返回错误 */
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
*函数名称:UART0_StatusTypeDef UART0_Receive8(UART0_HandleInfoDef* UART_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*函数功能:UART在轮询模式下接收大量数据
*入口参数:
UART0_HandleInfoDef*:UART_HANDLE:指向包含指定UART模块的配置信息结构体的指针
uint8_t *:pData:接收数据的存放
uint16_t:Size:待接收的数据量
uint32_t:Timeout:超时时间设置
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef UART0_Receive(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  /* 检查接收进程正在运行 */
  if(UART_HANDLE->RxState == UART0_STATE_READY)
  {
    /* 检查接收数据量大小和是否开启接收使能 */
    if((Size == 0U) || ((SCON & 0x10) != UART0_RX_ENABLE))
    {
      return  Status_ERROR;
    }

	UART_HANDLE->RxState = UART0_STATE_BUSY;			//状态更新为接收忙碌中
	RI = 0;			 //接收前清除接收中断标志
	UART_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
	UART_HANDLE->RxXferSize = Size;			//更新待发送的数据量
	UART_HANDLE->RxXferCount = 0;			//发送计数器清零

    while(UART_HANDLE->RxXferCount < UART_HANDLE->RxXferSize) //判断是否接收所有数据
    {
      if(RI) //判断接收标志位
      {
        if((SCON & 0xC0) == UART0_Mode_11B)
        {
          *(UART_HANDLE->pRxBuffPtr.Size_u16 + UART_HANDLE->RxXferCount) = SBUF + ((uint16_t)(SCON & 0X04) << 6); //接收数据
        }
        else
        {
          *(UART_HANDLE->pRxBuffPtr.Size_u8 + UART_HANDLE->RxXferCount) = SBUF;			//接收数据
        }
				RI = 0;			//接收标志位清零
		UART_HANDLE->RxXferCount++;	//接收数据量计数加1
		TimeoutCnt = 0;//收到数据，超时计数清零
	  }
	  else
	  {
		if(TimeoutCnt++ > Timeout)//计时溢出
		{
		  UART_HANDLE->RxState = UART0_STATE_TIMEOUT;//发送超时
		  return Status_TIMEOUT;
		}
				WDTCON |= 0x10;			//喂狗，防止等待时间过长，导致WDT复位
	  }
	}

	if(UART_HANDLE->RxXferCount == UART_HANDLE->RxXferSize)
	{
	  UART_HANDLE->RxState = UART0_STATE_READY;	//接收完成
	  return Status_OK;
	}
	else
	{
	  UART_HANDLE->RxState = UART0_STATE_ERROR;//接收错误
	  return Status_ERROR;
	}
  }
  else
  {
	return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef UART0_Transmit_IT(UART0_HandleInfoDef *UART_HANDLE, uint8_t *pData, uint8_t Size)
*函数功能:UART在中断模式下接收大量数据时，先在main函数中调用此函数
*入口参数:
UART0_HandleInfoDef*:UART_HANDLE:指向包含指定UART模块的配置信息结构体的指针
uint8_t *:pData:接收数据的存放区
uint16_t:Size:待接收的数据量
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef UART0_Receive_IT(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size)
{
	/* 检查一个接收进程是否正在进行 */
	if (UART_HANDLE->RxState == UART0_STATE_READY)
	{
		/* 检查是否开启UART0中断或发送数据量大小为0或是否开启接收使能，返回错误 */
		if ((EUART == 0) || (Size == 0U) || ((SCON & 0x10) != UART0_RX_ENABLE))
		{
			return UART0_STATE_ERROR;
		}

	UART_HANDLE->RxState = UART0_STATE_BUSY;
	UART_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向接收数据的存放地址
	UART_HANDLE->RxXferSize = Size;			//更新待接收的数据量
	UART_HANDLE->RxXferCount = 0;			//发送计数器清零

	return Status_OK;
  }
  else
  {
	return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef UART0_Receive8_IRQHandler(UART0_HandleInfoDef* UART_HANDLE)
*函数功能:UART在中断模式下接收大量数据时，在中断服务函数中调用
*入口参数:
UART0_HandleInfoDef*:UART_HANDLE:指向包含指定UART模块的配置信息结构体的指针
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef UART0_Receive_IRQHandler(UART0_HandleInfoDef* UART_HANDLE)
{
	/* 检查一个接收进程是否正在进行 */
	if (UART_HANDLE->RxState == UART0_STATE_BUSY)
	{
		/* 处于接收线程，检查待接收数据量是否为0 */
		if (UART_HANDLE->RxXferCount < UART_HANDLE->RxXferSize)
		{
			if((SCON & 0xC0) == UART0_Mode_11B)
			{
				*(UART_HANDLE->pRxBuffPtr.Size_u16 + UART_HANDLE->RxXferCount) = SBUF + ((uint16_t)(SCON & 0X04) << 6); //接收数据
			}
			else
			{
				*(UART_HANDLE->pRxBuffPtr.Size_u8 + UART_HANDLE->RxXferCount) = SBUF;			//接收数据
			}
			UART_HANDLE->RxXferCount++;
			/* 判断上一次发送是否是最后一次 */
			if (UART_HANDLE->RxXferCount == UART_HANDLE->RxXferSize)
			{
				/* 接收完成 */
				UART_HANDLE->RxState = UART0_STATE_READY;
				return Status_OK;
			}
			return Status_BUSY;
			
		}
		else
		{
			/* 接收数据量为0时还接收数据，返回错误 */
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