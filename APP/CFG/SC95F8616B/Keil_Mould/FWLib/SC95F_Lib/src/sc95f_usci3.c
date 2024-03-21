//************************************************************
//  Copyright (c) 深圳市赛元微电子股份有限公司
//	文件名称: 		sc95f_USCI3.c
//	作者:
//	模块功能: 		USCI3固件库函数C文件
// 	最后更正日期:	2024年1月18日
// 	版本:				V1.0002
// 说明:				该文件仅适用于SC95F系列芯片
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

/******************************* SPI函数 *************************************/
/**************************************************
*函数名称:void USCI3_SPI_Init(USCI3_SPI_FirstBit_TypeDef FirstBit, USCI3_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler,USCI3_SPI_Mode_TypeDef Mode,
							 USCI3_SPI_ClockPolarity_TypeDef ClockPolarity, USCI3_SPI_ClockPhase_TypeDef ClockPhase,USCI3_SPI_TXE_INT_TypeDef SPI_TXE_INT,USCI3_TransmissionMode_TypeDef TransmissionMode)
*函数功能:SPI初始化配置函数
*入口参数:
USCI3_SPI_FirstBit_TypeDef:FirstBit:优先传送位选择（MSB/LSB）
USCI3_SPI_BaudRatePrescaler_TypeDef:BaudRatePrescaler:SPI时钟频率选择
USCI3_SPI_Mode_TypeDef:Mode:SPI工作模式选择
USCI3_SPI_ClockPolarity_TypeDef:ClockPolarity:SPI时钟极性选择
USCI3_SPI_ClockPhase_TypeDef:ClockPhase:SPI时钟相位选择
USCI3_SPI_TXE_INT_TypeDef:SPI_TXE_INT:发送缓存器中断允许选择,该功能在USCI3芯片上无效
USCI3_TransmissionMode_TypeDef:TransmissionMode:SPI传输模式选择 8/16位
*出口参数:void
**************************************************/
void USCI3_SPI_Init(USCI3_SPI_FirstBit_TypeDef FirstBit,
                    USCI3_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI3_SPI_Mode_TypeDef Mode,
                    USCI3_SPI_ClockPolarity_TypeDef ClockPolarity, USCI3_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI3_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI3_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetSPI();//USCI3配置为SPI模式
  SPI_TXE_INT = USCI3_SPI_TXE_DISINT; //SPI_TXE_INT该功能在USCI3芯片上无效
  US3CON1 = US3CON1 & (~0X05) | FirstBit | TransmissionMode;
  US3CON0 = US3CON0 & 0X80 | BaudRatePrescaler | Mode | ClockPolarity | ClockPhase;
}

/**************************************************
*函数名称:void USCI3_TransmissionMode(USCI3_TransmissionMode_TypeDef TransmissionMode)
*函数功能:SPI 传输模式配置函数
*入口参数:
USCI3_TransmissionMode_TypeDef:TransmissionMode:SPI传输模式选择 8/16e位
*出口参数:void
**************************************************/
void USCI3_TransmissionMode(USCI3_TransmissionMode_TypeDef TransmissionMode)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetSPI();//USCI3配置为SPI模式
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
*函数名称:void USCI3_SPI_Cmd(FunctionalState NewState)
*函数功能:SPI功能开关函数
*入口参数:
FunctionalState:NewState:功能启动/关闭选择
*出口参数:void
*****************************************************/
void USCI3_SPI_Cmd(FunctionalState NewState)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetSPI();//USCI3配置为SPI模式

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
*函数名称:void USCI3_SPI_SendData_8(uint8_t Data)
*函数功能:USCI3 SPI发送数据
*入口参数:
uint8_t:Data:发送的数据
*出口参数:void
*****************************************************/
void USCI3_SPI_SendData_8(uint8_t Data)
{
  Select_USCI3();//选择控制USCI3
  US3CON2 = Data;
}

/*****************************************************
*函数名称:uint8_t USCI3_SPI_ReceiveData_8(void)
*函数功能:获得US3CON2中的值
*入口参数:void
*出口参数:
uint8_t:接收的数据
*****************************************************/
uint8_t USCI3_SPI_ReceiveData_8(void)
{
  Select_USCI3();//选择控制USCI3
  return US3CON2;
}

/*****************************************************
*函数名称:void USCI3_SPI_SendData_16(uint16_t Data)
*函数功能:US3CON2 SPI发送数据
*入口参数:
uint16_t:Data:发送的数据
*出口参数:void
*****************************************************/
void USCI3_SPI_SendData_16(uint16_t Data)
{
  Select_USCI3();//选择控制USCI3
  US3CON3 = (uint8_t)(Data >> 8);
  US3CON2 = (uint8_t)Data;
}

/*****************************************************
*函数名称:uint16_t USCI3_SPI_ReceiveData_16(void)
*函数功能:获得US3CON2中的值
*入口参数:void
*出口参数:
uint16_t:接收的数据
*****************************************************/
uint16_t USCI3_SPI_ReceiveData_16(void)
{
  uint16_t SPI_data;
  Select_USCI3();//选择控制USCI3
  SPI_data = (uint16_t)((US3CON3 << 8) | US3CON2);
  return SPI_data;
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
*函数功能:SPI0 中断处理
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指向包含 SPI0 信息的 USCI3_HandleInfoDef 结构体的指针。
*出口参数:
void
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  StatusTypeDef TempStatus = Status_ERROR;

  Select_USCI3();//选择控制USCI3

  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    /* 当数据没有接收完成 */
    if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
    {
      /* SPI处于16位通信模式 */
      if((US3CON1 & USCI3_SPI_DATA16) != 0)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_16();			//读取16位数据
      }
      /* SPI处于8位通信模式 */
      else
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_8();			//读取16位数据
      }

      TempStatus = Status_BUSY;
      USCI3_HANDLE->RxXferCount++;			//接收到数据，计数值加1

      /* 数据接收完成，状态位进行修改 */
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
      {
        USCI3_HANDLE->RxState = USCI3_STATE_READY;
        TempStatus =  Status_OK;
      }
    }

    if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
    {

      USCI3_HANDLE->TxXferCount++;			//数据发送完成，计数值加1
      /* 当数据没有发送完成 */
      if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
      {
        /* SPI处于16位通信模式 */
        if((US3CON1 & USCI3_SPI_DATA16) != 0)
        {
          USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));			//发送16位数据
        }
        /* SPI处于8位通信模式 */
        else
        {
          USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));			//发送16位数据
        }
        TempStatus = Status_BUSY;
      }
      /* 数据发送完成，状态位进行修改  */
      else if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxState = USCI3_STATE_READY;
        TempStatus = Status_OK;
      }
    }
    /* 当SPI处于主机时，SPI需要发送数据，才能同步接收到数据 */
    else if(USCI3_HANDLE->RxXferSize)
    {
      USCI3_SPI_SendData_8(0x00);
    }
  }
  return TempStatus;
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*函数功能:SPI在轮询模式下接收大量数据
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指向包含指定SPI模块的配置信息结构体的指针
uint8_t *:pData:接收数据的存放区
uint16_t:Size:待接收的数据量
uint32_t:Timeout:超时时间
*出口参数:
StatusTypeDef:USCI3 状态
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI3();//选择控制USCI3

  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return USCI3_STATE_ERROR;
    }

    /* SPI作为主机时，接收数据必定跟随发送数据 */
    if((USXCON0 & USCI3_SPI_MODE_MASTER) != 0)
    {
      return USCI3_SPI_TransmitReceive(USCI3_HANDLE, pData, pData, Size, Timeout);			//跳转到阻塞模式数据收发函数
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			 //接收前清除接收中断标志
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
    {
      /* 等待SPI中断标志位置起 */
      if(USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))
      {
        /* SPI处于16位通信模式 */
        if((US3CON1 & USCI3_SPI_DATA16) != 0)
        {
          *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_16();			//读取16位数据,并且地址自增
        }
        /* SPI处于8位通信模式 */
        else
        {
          *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_SPI_ReceiveData_8();			//读取16位数据,并且地址自增
        }
        USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//清除标志位
        USCI3_HANDLE->RxXferCount++;	//接收数据量计数加1
        TimeoutCnt = 0;			//超时计数值清零
      }
      else
      {
        /* 超时计数器不断累加，直到超出设定的超时时间 */
        if((TimeoutCnt++) > Timeout)
        {
          if(Timeout == 0)
            return USCI3_STATE_TIMEOUT;				//返回超时错误
        }
        WDTCON |= 0x10;          //喂狗函数，防止看门狗复位
      }
    }
    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//接收完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//接收错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//返回忙碌状态
  }
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size)
*函数功能:中断模式接收一定量数据
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指向包含指定SPI模块的配置信息结构体的指针
uint8_t *:pData:接收数据的存放
uint16_t:Size:待接收的数据量
*出口参数:
StatusTypeDef:USCI3 状态
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI3();//选择控制USCI3

  /* 检查一个接收进程是否正在进行 */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x04 == 0) || (Size == 0U))
    {
      return USCI3_STATE_ERROR;
    }

    /* SPI作为主机时，接收数据必定跟随发送数据 */
    if((US3CON0 & USCI3_SPI_MODE_MASTER) != 0)
    {
      return USCI3_SPI_TransmitReceive_IT(USCI3_HANDLE, pData, pData, Size);			//跳转到中断模式数据收发函数
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//状态更新为接收忙碌中
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//清除中断标志位
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//返回标志位
  }
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*函数功能:SPI在轮询模式下发送大量数据
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指向包含指定SPI模块的配置信息结构体的指针
uint8_t *pData:指向数据缓存的指针。
uint16_t Size:发送数据的大小
uint32_t Timeout:超时时间
*出口参数:
StatusTypeDef:USCI3 状态
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI3();//选择控制USCI3

  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			 //发送前清除标志位
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize) //判断是否接收所有数据
    {
      if((US3CON1 & USCI3_SPI_DATA16) != 0) /* SPI处于16位通信模式 */
      {
        USCI3_SPI_SendData_16(*USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount);			//发送16位数据并且数据地址增加
      }
      else/* SPI处于8位通信模式 */
      {
        USCI3_SPI_SendData_8(*USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);			//发送8位数据并且数据地址增加
      }
      while(!USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))			//等待发送完成
      {
        /* 超时计数器不断累加，直到超出设定的超时时间 */
        if(TimeoutCnt++ > Timeout)
        {
          USCI3_HANDLE->TxState = USCI3_STATE_TIMEOUT;//发送超时
          return Status_TIMEOUT;
        }
        WDTCON |= 0x10;			//喂狗，防止等待时间过长，导致WDT复位
      }
      USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//发送前先清除标志位
      TimeoutCnt = 0;			//超时计数器清零
      USCI3_HANDLE->TxXferCount++;	//发送数据量计数
    }
    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//返回忙碌状态
  }
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
*函数功能:中断模式发送一定量数据
*入口参数:
USCI3_HandleInfoDef *USCI3_HANDLE:指向包含 SPI0 信息的 USCI3_HandleInfoDef 结构体的指针。
uint8_t *pData:指向数据缓存的指针。
uint16_t Size:发送数据的大小
*出口参数:
StatusTypeDef:USCI3 状态
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{

  Select_USCI3();//选择控制USCI3

  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 没有开启USCI3中断或发送数据量大小为0，返回错误 */
    if((IE2 & 0x04 == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//清除中断标志位
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = 0;			//发送计数器置0

    /* 发送第1byte数据 */
    /* SPI处于16位通信模式 */
    if((US3CON1 & USCI3_SPI_DATA16) != 0)
    {
      USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16));
    }
    /* SPI处于8位通信模式 */
    else
    {
      USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8));
    }

    return Status_OK;
  }
  else
  {
    return  Status_BUSY;			//状态位修改为忙碌状态
  }
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_TransmitReceive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
*函数功能:阻塞模式收发一定量数据
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指向包含 SPI0 信息的 USCI3_HandleInfoDef 结构体的指针
uint8_t*:pTxData:指向发送数据缓存的指针
uint8_t*:pRxData:指向接收数据缓存的指针
uint16_t:Size:发送数据的大小
uint32_t:Timeout:超时时间
*出口参数:
StatusTypeDef:函数最新状态
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_TransmitReceive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;

  Select_USCI3();//选择控制USCI3

  /* 检查一个发送进程是否正在进行 */
  if((USCI3_HANDLE->TxState == USCI3_STATE_READY) && (USCI3_HANDLE->RxState == USCI3_STATE_READY))
  {

    /* 待发送和接收数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//清除中断标志位
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pTxData;       //指向待发送数据的地址
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pRxData;
    USCI3_HANDLE->TxXferSize = USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize);
    {

      if((US0CON1 & USCI3_SPI_DATA16) != 0)
      {
        USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));			//发送16位数据并且地址增加
        while(!USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))			//等待发送完成
        {
          /* 等待时间是否发生了超时 */
          if(TimeoutCnt++ > Timeout)
          {
            USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;//接收错误
            return Status_TIMEOUT;				//返回超时错误
          }
          WDTCON |= 0x10;			//喂狗，防止等待时间过长，导致WDT复位
        }
        USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//发送前先清除标志位
        *(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount) = USCI3_SPI_ReceiveData_16();
      }
      else
      {
        USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));			//发送16位数据并且地址增加
        while(!USCI3_GetFlagStatus(USCI3_SPI_FLAG_SPIF))			//等待发送完成
        {
          /* 等待时间是否发生了超时 */
          if(TimeoutCnt++ > Timeout)
          {
            USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;//接收错误
            return Status_TIMEOUT;				//返回超时错误
          }
          WDTCON |= 0x10;			//喂狗，防止等待时间过长，导致WDT复位
        }
        USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);				//发送前先清除标志位
        *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount) = USCI3_SPI_ReceiveData_8();
      }
      TimeoutCnt = 0;			//超时计数值清零
      USCI3_HANDLE->TxXferCount++;			//数据量计数值加1
      USCI3_HANDLE->TxXferCount++;			//数据量计数值加1
    }
    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_HANDLE->RxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/**********************************************************************************************************
*函数名称:StatusTypeDef USCI3_SPI_TransmitReceive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
*函数功能:SPI在中断模式下发送和接收大量数据时，先在main函数中调用此函数
*入口参数:
USCI3_HandleInfoDef *USCI3_HANDLE:指向包含指定SPI模块的配置信息结构体的指针
uint8_t *:pData:发送和接收数据的存放区
uint16_t:Size:待发送和接收的数据量
*出口参数:
StatusTypeDef:USCI3 状态
**********************************************************************************************************/
StatusTypeDef USCI3_SPI_TransmitReceive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size)
{
  /* 待发送数据长度必须大于0，否则返回错误状态 */
  if((USCI3_HANDLE->TxState == USCI3_STATE_READY) && (USCI3_HANDLE->RxState == USCI3_STATE_READY))
  {
    Select_USCI3();//选择控制USCI3

    /* 待发送和接收数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x04 == 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI3_SPI_Cmd(DISABLE);
    USCI3_HANDLE->TxState = USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_SPI_FLAG_SPIF);			//清除中断标志位
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pTxData;
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pRxData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零
    USCI3_SPI_Cmd(ENABLE);

    /* 发送第1byte数据 */
    /* SPI处于16位通信模式 */
    if((US3CON1 & USCI3_SPI_DATA16) != 0)
    {
      USCI3_SPI_SendData_16(*(USCI3_HANDLE->pTxBuffPtr.Size_u16));			//发送16位数据
    }
    /* SPI处于8位通信模式 */
    else
    {
      USCI3_SPI_SendData_8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8));			//发送8位数据
    }
    /* 状态处于忙碌的状态 */

    return  Status_OK;
  }
  else
  {
    return Status_ERROR;
  }
}

/******************************* TWI函数 *************************************/
/**************************************************
*函数名称:void USCI3_TWI_Slave_Init(uint8_t TWI_Address)
*函数功能:USCI3 TWI 从机初始化配置函数
*入口参数:
uint8_t:TWI_Address:TWI作为从机时的7位从机地址
*出口参数:void
**************************************************/
void USCI3_TWI_Slave_Init(uint8_t TWI_Address)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetTWI();//USCI3配置为TWI模式
  US3CON2 = TWI_Address << 1;
}

/**************************************************
*函数名称:void USCI3_TWI_MasterCommunicationRate(USCI3_TWI_MasterCommunicationRate_TypeDef TWI_MasterCommunicationRate)
*函数功能:USCI3 TWI主机模式下通讯速率设定
*入口参数:
USCI3_TWI_MasterCommunicationRate_TypeDef:TWI_MasterCommunicationRate:TWI主机模式下通讯速率
*出口参数:void
**************************************************/
void USCI3_TWI_MasterCommunicationRate(USCI3_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetTWI();//USCI3配置为TWI模式

  US3CON1 = TWI_MasterCommunicationRate;
}

/**************************************************
*函数名称:void USCI3_TWI_Start(void)
*函数功能:USCI3 TWI 起始位
*入口参数:void
*出口参数:void
**************************************************/
void USCI3_TWI_Start(void)
{
  Select_USCI3();//选择控制USCI3
  US3CON1 |= 0x20;
}

/**************************************************
*函数名称:void USCI3_TWI_MasterModeStop(void)
*函数功能:USCI3 TWI主机模式停止位
*入口参数:void
*出口参数:void
**************************************************/
void USCI3_TWI_MasterModeStop(void)
{
  Select_USCI3();//选择控制USCI3
  US3CON1 |= 0x10;
}

/**************************************************
*函数名称:void USCI3_TWI_SlaveClockExtension(void)
*函数功能:USCI3 TWI从机模式时钟延长功能位
*入口参数:void
*出口参数:void
**************************************************/
void USCI3_TWI_SlaveClockExtension(FunctionalState NewState)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetTWI();//USCI3配置为TWI模式

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
*函数名称:void USCI3_TWI_AcknowledgeConfig(FunctionalState NewState)
*函数功能:TWI接收应答使能函数
*入口参数:
FunctionalState:NewState:接收应答使能/失能选择
*出口参数:void
**************************************************/
void USCI3_TWI_AcknowledgeConfig(FunctionalState NewState)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetTWI();//USCI3配置为TWI模式

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
*函数名称:void USCI3_TWI_GeneralCallCmd(FunctionalState NewState)
*函数功能:TWI通用地址响应使能函数
*入口参数:
FunctionalState:NewState:接收应答使能/失能选择
*出口参数:void
**************************************************/
void USCI3_TWI_GeneralCallCmd(FunctionalState NewState)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetTWI();//USCI3配置为TWI模式

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
*函数名称:FlagStatus USCI3_GetTWIStatus(USCI3_TWIState_TypeDef USCI3_TWIState)
*函数功能:读取TWI状态
*入口参数:
USCI3_TWIState_TypeDef:USCI3_TWIState:TWI状态类型
*出口参数:
FlagStatus:USCI3_TWI标志状态
*****************************************************/
FlagStatus USCI3_GetTWIStatus(USCI3_TWIState_TypeDef USCI3_TWIState)
{
  if((US3CON0 & 0x07) == USCI3_TWIState)
    return SET;
  else
    return RESET;
}

/*****************************************************
*函数名称:void USCI3_TWI_Cmd(FunctionalState NewState)
*函数功能:TWI功能开关函数
*入口参数:
FunctionalState:NewState:功能启动/关闭选择
*出口参数:void
*****************************************************/
void USCI3_TWI_Cmd(FunctionalState NewState)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetTWI();//USCI3配置为TWI模式

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
*函数名称:void USCI3_TWI_SendAddr(uint8_t Addr,USCI3_TWI_RWType RW)
*函数功能:TWI发送地址，读写类型
*入口参数:
uint8_t:Addr:发送的地址
USCI3_TWI_RWType:RW:读写类型
*出口参数:void
*****************************************************/
void USCI3_TWI_SendAddr(uint8_t Addr, USCI3_TWI_RWType RW)
{
  US3CON3 = (Addr << 1) | RW;
}


/*****************************************************
*函数名称:void USCI3_TWI_SendData(uint8_t Data)
*函数功能:TWI发送数据
*入口参数:
uint8_t:Data:发送的数据
*出口参数:void
*****************************************************/
void USCI3_TWI_SendData(uint8_t Data)
{
  US3CON3 = Data;
}


/*****************************************************
*函数名称:uint8_t USCI3_TWI_ReceiveData(void)
*函数功能:获得US3CON3中的值
*入口参数:void
*出口参数:
uint8_t:接收的数据
*****************************************************/
uint8_t USCI3_TWI_ReceiveData(void)
{
  return US3CON3;
}
/*****************************************************
*函数名称：StatusTypeDef USCI3_TWI_Wait_TWIF(USCI3_HandleInfoDef* USCI3_HANDLE,uint32_t Timeout)
*函数功能：等待TWIF置起
*入口参数：
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向USCI3数据信息存放区
uint32_t:Timeout:超时时间设置
*出口参数：StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Wait_TWIF(USCI3_HandleInfoDef* USCI3_HANDLE, uint32_t Timeout)
{
  uint32_t TimeoutCnt = 0;
  Select_USCI3();//选择控制USCI3
  while(!(USCI3_GetFlagStatus(USCI3_TWI_FLAG_TWIF)))			//等待启动信号发送完毕
  {
    TimeoutCnt++;
    if(TimeoutCnt > Timeout)
    {
      /* 超时更新状态 */
      if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
        USCI3_HANDLE->TxState = USCI3_STATE_TIMEOUT;
      if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
        USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;
      return Status_TIMEOUT;
    }
  }
  USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			//清除标志位
  return Status_OK;
}

/*****************************************************
*函数名称：StatusTypeDef USCI3_TWI_Master_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*函数名称：StatusTypeDef USCI3_TWI_Master_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*函数功能：发送8位数据时，主机轮询模式下发送大量数据
*入口参数：
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向USCI3数据信息存放区
uint8_t:slaveAddr:从机地址
uint8_t*：pData	指针指向存储区
uint32_t:Size:存储数据长度
uint32_t:Timeout:超时时间设置
*出口参数：StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

    USCI3_TWI_Start();			//发送启动信号
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)           //等待启动信号发送完毕
    {
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      USCI3_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)          //检测状态机状态
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      USCI3_TWI_MasterModeStop();
      return Status_ERROR;
    }

    US3CON3 = (slaveAddr << 1) & 0xFE;			//发送地址和读写位
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)           //等待信号发送完毕
    {
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      USCI3_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendData) == RESET)          //检测状态机状态
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
        USCI3_TWI_MasterModeStop();
        return Status_ERROR;
      }
      US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8  + USCI3_HANDLE->TxXferCount);             //TWI发送数据
      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)           //等待信号发送完毕
      {
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
        USCI3_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }
      USCI3_HANDLE->TxXferCount++;
    }
    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize);

    /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
    USCI3_TWI_MasterModeStop();

    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }

  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Slave_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint32_t Timeout)
*函数功能:发送8位数据时，从机轮询模式下发送大量数据
* 注：默认开启时钟延长模式
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向USCI3数据信息存放区
uint32_t:Timeout:超时时间设置
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }

    Select_USCI3();
    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

    USCI3_TWI_SlaveClockExtension(ENABLE);            //开启时钟延长
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //开启AA使能位
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待地址匹配
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);            //关闭AA使能
      return Status_TIMEOUT;
    }

    do
    {
      if(USCI3_GetTWIStatus(USCI3_TWI_SlaveSendData) == SET)           //检测状态机状态
      {

        US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);             //TWI发送数据

        if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)             //等待启动信号发送完毕
        {
          USCI3_TWI_AcknowledgeConfig(DISABLE);            //关闭AA使能
          return Status_TIMEOUT;
        }
        USCI3_HANDLE->TxXferCount++;

        if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize - 1)

          USCI3_TWI_AcknowledgeConfig(DISABLE);            //关闭AA使能

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

    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveDisableACK) == RESET)           //检测状态机状态
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      return Status_ERROR;
    }

    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Master_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*函数功能:主机使用中断模式下发送大量数据时，先在main函数中调用此函数
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向USCI3数据信息存放区
*出口参数:
StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI3();
      IE2 &= ~(0x04);			//关闭中断

      USCI3_HANDLE->TxState = USCI3_STATE_WAIT;			//发送进程忙碌中
      USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
      USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
      USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
      USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

      USCI3_TWI_Start();			//发送启动信号

      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, 0xFFFFFFFF) == Status_TIMEOUT)            //等待启动信号发送完毕
      {
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
        USCI3_TWI_MasterModeStop();
        IE2 |= 0x04;	//开启中断
        return Status_TIMEOUT;
      }

      if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)           //检测状态机状态
      {
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
        USCI3_TWI_MasterModeStop();
        IE2 |= 0x04;	//开启中断
        return Status_ERROR;
      }

      IE2 |= 0x04;						//开启中断
      US3CON3 = (slaveAddr << 1) & 0xFE;//发送地址和读写位

      return Status_OK;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Master_Transmit_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*函数功能:发送8位数据时，主机中断模式下发送大量数据
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向USCI3数据信息存放区
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Transmit_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendData) == SET)
    {
      if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxXferCount++;			//地址帧也作为数据计数
        US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);             //TWI发送数据
        return Status_BUSY;
      }
      else if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
      {
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
        USCI3_TWI_MasterModeStop();
        USCI3_HANDLE->TxState = USCI3_STATE_READY;
        return Status_OK;
      }
      else
      {
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
        USCI3_TWI_MasterModeStop();
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_OK;
      }
    }
    else if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaUACK) == SET)
    {
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
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
    //地址成功响应
    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;
    US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);
    return Status_BUSY;
  }
  else
  {
    return Status_ERROR;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Slave_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
*函数功能:从机使用中断模式下发送大量数据时，先在main函数中调用此函数
* 注
*入口参数:*USCI3_HANDLE 指针指向USCI3数据信息存放区
*出口参数:void
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    else
    {
      Select_USCI3();

      USCI3_TWI_AcknowledgeConfig(DISABLE);         //开启AA
      USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
      USCI3_TWI_SlaveClockExtension(ENABLE);            //开启时钟延长
      USCI3_HANDLE->TxState = USCI3_STATE_WAIT;			//发送进程忙碌中
      USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
      USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
      USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

      USCI3_TWI_AcknowledgeConfig(ENABLE);         //开启AA

      return Status_OK;;
    }
  }
  else
  {
    return Status_ERROR;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Slave_Transmit_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*函数功能:发送8位数据时，从机中断模式下发送大量数据
*入口参数:*USCI3_HANDLE 指针指向USCI3数据信息存放区
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Transmit_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
  {

    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveSendData) == SET)
    {
      if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
      {
        USCI3_HANDLE->TxXferCount++;			//第一次中断是接受到地址了
        US3CON3 = *(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount);
        if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize - 1)
        {
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        }
        return Status_BUSY;
      }
      else
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    /*
    1.但数据传输过程中，其他程序修改了AA控制位
    2.接收到主机回来的UACK */
    else if((USCI3_GetTWIStatus(USCI3_TWI_SlaveDisableACK) == SET) || (USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaUACK) == SET))
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
		 USCI3_HANDLE->TxXferCount++;
      if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)			//所有数据已发送
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
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      /* 以下情况未发送过程出错
      1.数据发送后接受到UACK
      2.TWI不是工作在从机发送状态 */
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
    return Status_ERROR;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Master_Receive(USCI3_HandleInfoDef* USCI3_HANDLE,uint8_t slaveAddr, uint8_t* pData, uint8_t Size,uint32_t Timeout)
*函数功能:接收8位数据时，主机轮询模式下接收大量数据
*入口参数:*USCI3_HANDLE 指针指向USCI3数据信息存放区
					 Timeout    超时时间设置
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  /* 检查一个接收进程是否正在进行 */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 待接收数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //开启AA使能位
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    USCI3_TWI_Start();			//发送启动信号
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待启动信号发送完毕
    {
      USCI3_TWI_AcknowledgeConfig(DISABLE);
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)           //检测状态机状态
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      USCI3_TWI_AcknowledgeConfig(DISABLE);
      return Status_ERROR;
    }

    /* 发送地址帧过程 */
    US3CON3 = (slaveAddr << 1) | 0x01;//发送地址和读写位
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待信号发送完毕
    {
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      USCI3_TWI_MasterModeStop();
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == RESET)           //检测状态机状态
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      USCI3_TWI_MasterModeStop();
      return Status_ERROR;
    }

    /* 数据接收过程 */
    do
    {
      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待信号发送完毕
      {
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
			USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        USCI3_TWI_MasterModeStop();
        return Status_TIMEOUT;
      }

      if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == RESET)           //检测状态机状态
      {
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
        /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
			USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        USCI3_TWI_MasterModeStop();
        return Status_ERROR;
      }

      *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI接收数据
      USCI3_HANDLE->RxXferCount++;
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);                                     //关闭AA
        if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待信号发送完毕
        {
          /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
			  USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
          USCI3_TWI_MasterModeStop();
          return Status_TIMEOUT;
        }
        if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaUACK) == RESET)           //检测状态机状态
        {
          USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
          /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
			  USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
          USCI3_TWI_MasterModeStop();
          return Status_ERROR;
        }

        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI接收数据
        USCI3_HANDLE->RxXferCount++;
      }
    }
    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize);
    /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
	 USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
    USCI3_TWI_MasterModeStop();

    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}


/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Slave_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
*函数功能:接收8位数据时，从机轮询模式下接收大量数据
*入口参数:
USCI3_HandleInfoDef:*USCI3_HANDLE:指针指向USCI3数据信息存放区
uint32_t:Timeout:超时时间设置
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if((IE2 & 0x08 != 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //开启AA使能位
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    /* 接收地址工程 */
    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待信号发送完毕
    {
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaData) == RESET)           //检测状态机状态
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      return Status_ERROR;
    }

    /* 接收数据过程 */
    do
    {
      if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, Timeout) == Status_TIMEOUT)            //等待启动信号发送完毕
      {
        return Status_TIMEOUT;
      }

      if(USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaData) == SET)           //检测状态机状态
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI接收数据
        USCI3_HANDLE->RxXferCount++;
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位,回主机UAC
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
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
          return Status_ERROR;
        }
      }
      else
      {
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        return Status_ERROR;
      }

    }
    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize);

    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Master_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
*函数功能:主机使用中断模式下接收大量数据时，先在main函数中调用此函数
*入口参数:*USCI3_HANDLE 指针指向USCI3数据信息存放区
		   Size        存储数据长度
*出口参数:void
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size)
{
  /* 检查一个接收进程是否正在进行 */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 待接收数据长度必须大于0，否则返回错误状态 */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    IE2 &= ~(0x04);	//关闭中断
    USCI3_HANDLE->RxState = USCI3_STATE_WAIT;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //开启AA
    USCI3_TWI_Start();			//发送启动信号

    if(USCI3_TWI_Wait_TWIF(USCI3_HANDLE, 0xFFFF) == Status_TIMEOUT)            //等待启动信号发送完毕
    {
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      USCI3_TWI_MasterModeStop();
      IE2 |= 0x04;	//开启中断
      return Status_TIMEOUT;
    }

    if(USCI3_GetTWIStatus(USCI3_TWI_MasterSendAddress) == RESET)           //检测状态机状态
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;
      /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
		 USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
      USCI3_TWI_MasterModeStop();
      IE2 |= 0x04;	//开启中断
      return Status_ERROR;
    }

    /* 发送地址帧过程 */
    IE2 |= 0x04;	//开启中断
    US3CON3 = (slaveAddr << 1) | 0x01;//发送地址和读写位

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Master_Receive_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*函数功能:接收8位数据时，主机中断模式下接收大量数据
*入口参数:*USCI3_HANDLE 指针指向USCI3数据信息存放区
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Master_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_MasterReceivedaData) == SET)           //检测状态机状态
    {
      if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI接收数据
        USCI3_HANDLE->RxXferCount++;
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
        {
          /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
			  USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
          USCI3_TWI_MasterModeStop();
          USCI3_HANDLE->RxState = USCI3_STATE_READY;	//发送完成
          return Status_OK;
        }
        return Status_BUSY;
      }
      else
      {
			USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        USCI3_TWI_MasterModeStop();
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//发生发送错误
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
*函数名称:void USCI3_TWI_Slave_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size))
*函数功能:从机使用中断模式下接收大量数据时，先在main函数中调用此函数
*入口参数:
*USCI3_HANDLE 指针指向USCI3数据信息存放区
		   *pData	指针指向存储区
		   Size        存储数据长度
*出口参数:void
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 待发送数据长度必须大于0，否则返回错误状态 */
    if(((IE2 & 0x04) == 0) || (Size == 0U))
    {
      return Status_ERROR;
    }
    Select_USCI3();
    USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
    USCI3_ClearFlag(USCI3_TWI_FLAG_TWIF);			 //发送前清除标志位
    USCI3_HANDLE->RxState = USCI3_STATE_WAIT;			//发送进程忙碌中
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零
    USCI3_TWI_AcknowledgeConfig(ENABLE);         //开启AA使能位

    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_TWI_Slave_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
*函数功能:接收8位数据时，从机中断模式下接收大量数据
*入口参数:
USCI3_HandleInfoDef:*USCI3_HANDLE 指针指向USCI3数据信息存放区
*出口参数:StatusTypeDef状态枚举
*****************************************************/
StatusTypeDef USCI3_TWI_Slave_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    if(USCI3_GetTWIStatus(USCI3_TWI_SlaveReceivedaData) == SET)           //检测状态机状态
    {
      if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
      {
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = US3CON3;             //TWI发送数据
        USCI3_HANDLE->RxXferCount++;
        if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize - 1)
        {
          USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位,回主机UACK
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
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位,回主机UACK
        USCI3_HANDLE->RxState = USCI3_STATE_READY;
        return Status_OK;
      }
      else
      {
        USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
        USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
        return Status_ERROR;
      }
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;
      USCI3_TWI_AcknowledgeConfig(DISABLE);         //关闭AA使能位
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

/******************************* UART函数 *************************************/
/**************************************************
*函数名称:void USCI3_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI3_UART_Mode_TypeDef Mode, USCI3_UART_RX_TypeDef RxMode)
*函数功能:UART初始化配置函数
*入口参数:
uint32_t:UARTFsys:系统时钟频率
uint32_t:BaudRate:波特率
USCI3_UART_Mode_TypeDef:Mode:UART1工作模式
USCI3_UART_RX_TypeDef:RxMode:接收允许选择
*出口参数:void
**************************************************/
void USCI3_UART_Init(uint32_t UARTFsys, uint32_t BaudRate, USCI3_UART_Mode_TypeDef Mode,
                     USCI3_UART_RX_TypeDef RxMode)
{
  Select_USCI3();//选择控制USCI3
  USCI3_SetUART();//USCI3配置为UART模式
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
*函数名称:void USCI3_UART_SendData8(uint8_t Data)
*函数功能:USCI3 UART1发送8位数据
*入口参数:
uint8_t:Data:发送的数据
*出口参数:void
*****************************************************/
void USCI3_UART_SendData8(uint8_t Data)
{
  US3CON3 = Data;
}

/*****************************************************
*函数名称:uint8_t USCI3_UART_ReceiveData8(void)
*函数功能:获得US3CON3中的值
*入口参数:void
*出口参数:
uint8_t:接收的数据
*****************************************************/
uint8_t USCI3_UART_ReceiveData8(void)
{
  return US3CON3;
}

/*****************************************************
*函数名称:void USCI3_UART_SendData9(uint16_t Data)
*函数功能:UART发送9位数据
*入口参数:
uint16_t:Data:发送的数据
*出口参数:void
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
*函数名称:uint16_t USCI3_UART_ReceiveData9(void)
*函数功能:获得US3CON3中的值及第九位的值
*入口参数:void
*出口参数:
uint16_t:接收的数据
*****************************************************/
uint16_t USCI3_UART_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = US3CON3 + ((uint16_t)(US3CON0 & 0X04) << 6);
  return Data9;
}

/*****************************************************
*函数名称:StatusTypeDef  USCI3_UART_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*函数功能:UART在轮询模式下发送大量数据
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向UART数据信息存放区
uint8_t *:pData:待发送数据
uint16_t:Size:待发送的数据量
uint32_t:Timeout:超时时间设置
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef USCI3_UART_Transmit(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{

  uint32_t delaytime = 0;
  Select_USCI3();//选择控制USCI3

  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 检查发送数据量是否为0 */
    if((IE2 & 0x04 != 0) || (Size == 0U))
    {
      return  Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_UART_FLAG_TI); //发送前清除发送中断标志，写1/写0清零
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

    while(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize) //判断是否接收所有数据
    {
      if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
      {
        /* 发送9位数据 */
        USCI3_UART_SendData9(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));
      }
      else
      {
        /* 发送8位数据 */
        USCI3_UART_SendData8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));
      }

      /* 等待发送完成 */
      delaytime = 0;
      while(!(US3CON0 & 0x02))
      {
        if(delaytime++ > Timeout)
        {
          USCI3_HANDLE->TxState = USCI3_STATE_TIMEOUT;//发送超时
          return Status_TIMEOUT;
        }
      }

      USCI3_ClearFlag(USCI3_UART_FLAG_TI); //发送中断标志清除，写1/写0清零
      USCI3_HANDLE->TxXferCount++;	//发送数据量计数
    }

    if(USCI3_HANDLE->TxXferCount == USCI3_HANDLE->TxXferSize)
    {
      USCI3_HANDLE->TxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->TxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_UART_Transmit_IT(USCI3_HandleInfoDef *USCI3_HANDLE, uint8_t *pData, uint8_t Size)
*函数功能:UART在中断模式下发送大量数据时，先在main函数中调用此函数
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向UART数据信息存放区
uint8_t *:pData:待发送数据
uint16_t:Size:待发送的数据量
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef USCI3_UART_Transmit_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI3();//选择控制USCI3

  /* 检查一个发送进程是否正在进行 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_READY)
  {
    /* 没有开启USCI3中断或发送数据量大小为0，返回错误 */
    if(((IE2 & 0x04) == 0) || (USCI3_HANDLE->TxXferSize == 0U))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->TxState = USCI3_STATE_BUSY;
    USCI3_HANDLE->pTxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->TxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->TxXferCount = 0;			//发送计数器清零

    /* 发送第1帧数据 */
    if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
    {
      /* 发送9位数据 */
      USCI3_UART_SendData9(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));
    }
    else
    {
      /* 发送8位数据 */
      USCI3_UART_SendData8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));
    }
    return Status_OK;
  }
  else
  {
    return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef  USCI3_UART_Transmit_IRQHandler(USCI3_HandleInfoDef *USCI3_HANDLE)
*函数功能:UART在中断模式下发送大量数据时，在中断服务函数中调用
*	注：该函数调用需要判断中断标志位是否置起
*入口参数:*USCI3_HANDLE 指针指向UART数据信息存放区
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef USCI3_UART_Transmit_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  Select_USCI3();//选择控制USCI3

  /* 处于发送线程 */
  if(USCI3_HANDLE->TxState == USCI3_STATE_BUSY)
  {
    USCI3_HANDLE->TxXferCount++;
    /* 判断上一次发送是否是最后一次 */
    if(USCI3_HANDLE->TxXferCount < USCI3_HANDLE->TxXferSize)
    {
      /* 待发送数据量未为0，继续发送 */
      if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
      {
        /* 发送9位数据 */
        USCI3_UART_SendData9(*(USCI3_HANDLE->pTxBuffPtr.Size_u16 + USCI3_HANDLE->TxXferCount));
      }
      else
      {
        /* 发送8位数据 */
        USCI3_UART_SendData8(*(USCI3_HANDLE->pTxBuffPtr.Size_u8 + USCI3_HANDLE->TxXferCount));
      }
      return Status_OK;
    }
    else if(USCI3_HANDLE->TxXferCount)
    {
      /* 发送完成 */
      USCI3_HANDLE->TxState = USCI3_STATE_READY;
      return Status_OK;
    }
    else
    {
      /* 发送数据量为0时还发送数据，返回错误 */
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
*函数名称:StatusTypeDef USCI3_UART_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout)
*函数功能:UART在轮询模式下接收大量数据
*入口参数:
USCI3_USCI3_HANDLEInfoDef*:USCI3_HANDLE:指针指向USCI3_UART数据信息存放区
uint8_t *:pData:接收数据的存放区
uint16_t:Size:待接收的数据量
uint32_t:Timeout:超时时间设置
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef USCI3_UART_Receive(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout)
{
  uint32_t delaytime = 0;
  Select_USCI3();//选择控制USCI3

  /* 检查接收进程正在运行 */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 检查接收数据量大小和是否开启接收使能 */
    if((IE2 & 0x04 != 0) || (Size == 0U) || ((US3CON0 & 0x10) != USCI3_UART_RX_ENABLE))
    {
      return  Status_ERROR;
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;			//发送进程忙碌中
    USCI3_ClearFlag(USCI3_UART_FLAG_RI);			 //接收前清除接收中断标志
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向待发送数据的地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待发送的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    while(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)  //判断是否接收所有数据
    {
      if(USCI3_GetFlagStatus(USCI3_UART_FLAG_RI))  //判断接收标志位
      {
        USCI3_ClearFlag(USCI3_UART_FLAG_RI);			//接收标志位清零

        /* 待接收数据量未为0，继续发送 */
        if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
        {
          /* 接收9位数据 */
          *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData9();
        }
        else
        {
          /* 接收8位数据 */
          *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData8();
        }

        USCI3_HANDLE->RxXferCount++;	//接收数据量计数
        delaytime = 0;//收到数据，超时计数清零
      }
      if(delaytime++ > Timeout) //计时溢出
      {
        USCI3_HANDLE->RxState = USCI3_STATE_TIMEOUT;//发送超时
        return Status_TIMEOUT;
      }
    }

    if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
    {
      USCI3_HANDLE->RxState = USCI3_STATE_READY;	//发送完成
      return Status_OK;
    }
    else
    {
      USCI3_HANDLE->RxState = USCI3_STATE_ERROR;//发生发送错误
      return Status_ERROR;
    }
  }
  else
  {
    return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_UART_Receive_IT(USCI3_HandleInfoDef *USCI3_HANDLE, uint8_t *pData, uint8_t Size)
*函数功能:UART在中断模式下接收大量数据时，先在main函数中调用此函数
*入口参数:
USCI3_USCI3_HANDLEInfoDef*:USCI3_HANDLE:指针指向UART数据信息存放区
uint8_t *:pData:接收数据的存放区
uint16_t:Size:待接收的数据量
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef USCI3_UART_Receive_IT(USCI3_HandleInfoDef* USCI3_HANDLE, uint8_t* pData, uint8_t Size)
{
  Select_USCI3();//选择控制USCI3

  /* 检查一个接收进程是否正在进行 */
  if(USCI3_HANDLE->RxState == USCI3_STATE_READY)
  {
    /* 检查是否开启USCI3_UART中断或发送数据量大小为0或是否开启接收使能，返回错误 */
    if(((IE2 & 0x04) == 0) || (Size == 0U) || ((US3CON0 & 0x10) != USCI3_UART_RX_ENABLE))
    {
      return Status_ERROR;
    }

    USCI3_HANDLE->RxState = USCI3_STATE_BUSY;
    USCI3_HANDLE->pRxBuffPtr.Size_u8 = pData;			//指向接收数据的存放地址
    USCI3_HANDLE->RxXferSize = Size;			//更新待接收的数据量
    USCI3_HANDLE->RxXferCount = 0;			//发送计数器清零

    return Status_OK;
  }
  else
  {
    return Status_BUSY;//返回标志位
  }
}

/*****************************************************
*函数名称:StatusTypeDef USCI3_UART_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
*函数功能:UART在中断模式下接收大量数据时，在中断服务函数中调用
*	注：该函数调用需要判断中断标志位是否置起
*入口参数:
USCI3_HandleInfoDef*:USCI3_HANDLE:指针指向USCI3数据信息存放区
*出口参数:
StatusTypeDef:函数执行状态
*****************************************************/
StatusTypeDef USCI3_UART_Receive_IRQHandler(USCI3_HandleInfoDef* USCI3_HANDLE)
{
  Select_USCI3();//选择控制USCI3

  /* 检查一个接收进程是否正在进行 */
  if(USCI3_HANDLE->RxState == USCI3_STATE_BUSY)
  {
    /* 处于接收线程，检查待接收数据量是否为0 */
    if(USCI3_HANDLE->RxXferCount < USCI3_HANDLE->RxXferSize)
    {
      /* 待接收数据量未为0，继续接收 */
      if((US3CON0 & 0xC0) == USCI3_UART_Mode_11B)
      {
        /* 接收9位数据 */
        *(USCI3_HANDLE->pRxBuffPtr.Size_u16 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData9();
      }
      else
      {
        /* 接收8位数据 */
        *(USCI3_HANDLE->pRxBuffPtr.Size_u8 + USCI3_HANDLE->RxXferCount) = USCI3_UART_ReceiveData8();
      }
      USCI3_HANDLE->RxXferCount++;
      /* 判断上一次发送是否是最后一次 */
      if(USCI3_HANDLE->RxXferCount == USCI3_HANDLE->RxXferSize)
      {
        /* 接收完成 */
        USCI3_HANDLE->RxState = USCI3_STATE_READY;
        return Status_OK;
			}
      return Status_BUSY;
    }
    else
    {
      /* 接收数据量为0时还接收数据，返回错误 */
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
*函数名称:FlagStatus USCI3_GetFlagStatus(USCI3_Flag_TypeDef USCI3_FLAG)
*函数功能:获得USCI3标志状态
*入口参数:
USCI3_Flag_TypeDef:USCI3_FLAG:所需获取的标志位
*出口参数:
FlagStatus:USCI3标志状态
*****************************************************/
FlagStatus USCI3_GetFlagStatus(USCI3_Flag_TypeDef USCI3_FLAG)
{
  FlagStatus bitstatus = RESET;
  Select_USCI3();
	if((TMCON & 0xC0)==0x40)  //SPI模式
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
	}else if((TMCON & 0xC0)==0x80)  //TWI模式
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
  }else if((TMCON & 0xC0)==0xC0)  //UART模式
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
*函数名称:void USCI3_ClearFlag(USCI3_Flag_TypeDef USCI3_FLAG)
*函数功能:清除USCI3标志状态
*入口参数:
USCI3_Flag_TypeDef:USCI3_FLAG:所需清除的标志位
*出口参数:void
*****************************************************/
void USCI3_ClearFlag(USCI3_Flag_TypeDef USCI3_FLAG)
{
  Select_USCI3();
	if((TMCON & 0xC0)==0x40)  //SPI模式
	{
	  if((USCI3_FLAG == USCI3_SPI_FLAG_SPIF) || (USCI3_FLAG == USCI3_SPI_FLAG_WCOL) || (USCI3_FLAG == USCI3_SPI_FLAG_TXE))
		{
		  US3CON1 &= (~USCI3_FLAG); //操作寄存器US3CON1	
		}
	}else if((TMCON & 0xC0)==0x80)  //TWI模式
	{
	  if(USCI3_FLAG == USCI3_TWI_FLAG_TXRXnE)
    {
      US3CON1 &= (~USCI3_FLAG); //操作寄存器US3CON1	
    }
		else
    {
       US3CON0 &= (~USCI3_FLAG); //操作寄存器US3CON0
    }	
	}else if((TMCON & 0xC0)==0xC0)  //UART模式
	{
	  if((USCI3_FLAG == USCI3_UART_FLAG_TI) || (USCI3_FLAG == USCI3_UART_FLAG_RI))
    {
#if defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)  || defined (SC95F8x1xB) || defined (SC95F7x1xB)\
		|| defined (SC95R602)  || defined (SC95R605)
     US3CON0 = US3CON0  & 0xFC | USCI3_FLAG;//写1清零
#else
     US3CON0 &= (~USCI3_FLAG); //写0清零
#endif
    }	
	}
}

/*****************************************************
*函数名称:void USCI3_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*函数功能:USCI3中断初始化
*入口参数:
FunctionalState:NewState:中断使能/关闭选择
PriorityStatus:Priority:中断优先级选择
*出口参数:void
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