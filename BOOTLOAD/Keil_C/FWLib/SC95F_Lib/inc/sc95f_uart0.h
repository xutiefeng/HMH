//************************************************************
//  Copyright (c) 深圳市赛元微电子股份有限公司
//	文件名称:			sc95f_uart0.h
//	作者:					赛元应用团队
//	模块功能:			UART0固件库函数C文件
// 	最后更正日期:	2024年1月18日
// 	版本:				V1.0002
//  说明:
//*************************************************************
#ifndef _sc95f_UART0_H_
#define _sc95f_UART0_H_

#include "sc95f.h"

#if !defined (SC95R751)

#define UART0_BaudRate_FsysDIV12 0X00 //仅模式0可用，串行端口在系统时钟的1/12下运行
#define UART0_BaudRate_FsysDIV4 0X01  //仅模式0可用，串行端口在系统时钟的1/4下运行

typedef enum
{
  UART0_CLOCK_TIMER1 = (uint8_t)0X00, //TIMER1做波特率发生器
  UART0_CLOCK_TIMER2 = (uint8_t)0X30  //TIMER2做波特率发生器
} UART0_Clock_Typedef;

typedef enum
{
  UART0_Mode_8B = 0X00,  //UART工作模式:8位半双工
  UART0_Mode_10B = 0X40, //UART工作模式:10位全双工
  UART0_Mode_11B = 0XC0  //UART工作模式:11位全双工
} UART0_Mode_Typedef;

typedef enum
{
  UART0_RX_ENABLE = 0x10, //允许接收数据
  UART0_RX_DISABLE = 0x00 //不允许接收数据
} UART0_RX_Typedef;

typedef enum
{
  UART0_FLAG_RI = 0X01, //接收中断标志位RI
  UART0_FLAG_TI = 0X02  //发送中断标志位TI
} UART0_Flag_Typedef;

typedef enum
{
  UART0_STATE_READY = 0x00,   //准备标志
  UART0_STATE_BUSY = 0x01,    //忙碌标志
  UART0_STATE_ERROR = 0x02,   //错误标志
  UART0_STATE_TIMEOUT = 0x03  //超时标志
} UART0_StatusTypeDef;

typedef struct __UART0_HandleInfoDef
{
  union
  {
    uint8_t* Size_u8;             //8位数据发送缓存区
    uint16_t* Size_u16;           //16位数据发送缓存区
  } pTxBuffPtr;										//指向UART0 Tx传输缓冲区的指针
  uint8_t TxXferSize;             //数据发送量大小
  uint8_t TxXferCount;            //数据发送计数器
  union
  {
    uint8_t* Size_u8;              //8位数据接收缓存区
    uint16_t* Size_u16;           //16位数据接收缓存区
  } pRxBuffPtr;										//指向UART0 Rx传输缓冲区的指针
  uint8_t RxXferSize;              //数据接收量
  uint8_t RxXferCount;              //数据接收计数器大小
  UART0_StatusTypeDef TxState;      //发送状态
  UART0_StatusTypeDef RxState;      //接收状态
} UART0_HandleInfoDef;

/*******************************宏函数*******************************/
/*****************************************************
*函数名称:FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
*函数功能:获得UART0中断标志状态
*入口参数:
UART0_GetFlagStatus:UART0_Flag:中断标志位选择
*出口参数:
FlagStatus:UART0中断标志位置起状态
*****************************************************/
#define UART0_GetFlagStatus(UART0_Flag) ((UART0_Flag == UART0_FLAG_TI) ? (TI):(RI))

/*****************************************************
*函数名称:void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
*函数功能:清除UART0中断标志状态
*入口参数:
UART0_Flag_Typedef;UART0_Flag:中断标志位选择
*出口参数:void
*****************************************************/
#define UART0_ClearFlag(UART0_Flag) CLEAR_BIT(SCON,UART0_Flag)

void UART0_DeInit(void);
void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
                UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode);
void UART0_SendData8(uint8_t Data);
uint8_t UART0_ReceiveData8(void);
void UART0_SendData9(uint16_t Data);
uint16_t UART0_ReceiveData9(void);
void UART0_ITConfig(FunctionalState NewState, PriorityStatus Priority);

StatusTypeDef UART0_Transmit(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef UART0_Transmit_IT(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef UART0_Transmit_IRQHandler(UART0_HandleInfoDef* UART_HANDLE);

StatusTypeDef UART0_Receive(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef UART0_Receive_IT(UART0_HandleInfoDef* UART_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef UART0_Receive_IRQHandler(UART0_HandleInfoDef* UART_HANDLE);

#endif
#endif
/******************* (C) COPYRIGHT 2018 SinOne Microelectronics *****END OF FILE****/