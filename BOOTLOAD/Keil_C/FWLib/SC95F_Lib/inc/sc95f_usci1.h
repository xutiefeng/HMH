//************************************************************
//  Copyright (c) 深圳市赛元微电子股份有限公司
//	文件名称: 		sc95f_usci1.h
//	作者		:
//	模块功能: 		USCI1固件库函数头文件
// 	最后更正日期:	2024年1月18日
// 	版本:				V1.0002
//  说明:该文件仅适用于SC95F系列芯片
//*************************************************************

#ifndef _sc95f_USCI1_H_
#define _sc95f_USCI1_H_

#include "sc95f.h"


#define USCI1_UART_BaudRate_FsysDIV12 0X00 //仅模式0可用，串行端口在系统时钟的1/12下运行
#define USCI1_UART_BaudRate_FsysDIV4 0X01  //仅模式0可用，串行端口在系统时钟的1/4下运行

typedef enum
{
  USCI1_Mode_SPI = (uint8_t)0x01, //SPI
  USCI1_Mode_TWI = (uint8_t)0x02, //TWI
  USCI1_Mode_UART = (uint8_t)0x03 //UART
} USCI1_CommunicationMode_TypeDef;

typedef enum
{
  USCI1_SPI_FIRSTBIT_MSB = (uint8_t)0x00, //MSB优先发送
  USCI1_SPI_FIRSTBIT_LSB = (uint8_t)0x04  //LSB优先发送
} USCI1_SPI_FirstBit_TypeDef;

typedef enum
{
  USCI1_SPI_BAUDRATEPRESCALER_1 = (uint8_t)0x00,  //SPI时钟速率为系统时钟除以1
  USCI1_SPI_BAUDRATEPRESCALER_2 = (uint8_t)0x01,  //SPI时钟速率为系统时钟除以2
  USCI1_SPI_BAUDRATEPRESCALER_4 = (uint8_t)0x02,  //SPI时钟速率为系统时钟除以4
  USCI1_SPI_BAUDRATEPRESCALER_8 = (uint8_t)0x03,  //SPI时钟速率为系统时钟除以8
  USCI1_SPI_BAUDRATEPRESCALER_16 = (uint8_t)0x04, //SPI时钟速率为系统时钟除以16
  USCI1_SPI_BAUDRATEPRESCALER_32 = (uint8_t)0x05, //SPI时钟速率为系统时钟除以32
  USCI1_SPI_BAUDRATEPRESCALER_64 = (uint8_t)0x06, //SPI时钟速率为系统时钟除以64
  USCI1_SPI_BAUDRATEPRESCALER_128 = (uint8_t)0x07 //SPI时钟速率为系统时钟除以128
} USCI1_SPI_BaudRatePrescaler_TypeDef;

typedef enum
{
  USCI1_SPI_MODE_MASTER = (uint8_t)0x20, //SPI为主设备
  USCI1_SPI_MODE_SLAVE = (uint8_t)0x00   //SPI为从设备
} USCI1_SPI_Mode_TypeDef;

typedef enum
{
  USCI1_SPI_CLOCKPOLARITY_LOW = (uint8_t)0x00, //SCK在空闲状态下为低电平
  USCI1_SPI_CLOCKPOLARITY_HIGH = (uint8_t)0x10 //SCK在空闲状态下为高电平
} USCI1_SPI_ClockPolarity_TypeDef;

typedef enum
{
  USCI1_SPI_CLOCKPHASE_1EDGE = (uint8_t)0x00, //SCK的第一沿采集数据
  USCI1_SPI_CLOCKPHASE_2EDGE = (uint8_t)0x08  //SCK的第二沿采集数据
} USCI1_SPI_ClockPhase_TypeDef;

typedef enum
{
  USCI1_SPI_TXE_DISINT = (uint8_t)0x00, //TBIE为0时不允许发送中断
  USCI1_SPI_TXE_ENINT = (uint8_t)0x01   //TBIE为1时允许发送中断
} USCI1_SPI_TXE_INT_TypeDef;

typedef enum
{
  USCI1_SPI_DATA8 = (uint8_t)0x00, //SPI 8位传输模式
  USCI1_SPI_DATA16 = (uint8_t)0x02 //SPI 16位传输模式
} USCI1_TransmissionMode_TypeDef;

typedef enum
{
  USCI1_TWI_1024 = (uint8_t)0x00, //TWI通信速率 Fhrc/1024
  USCI1_TWI_512 = (uint8_t)0x01,  //TWI通信速率 Fhrc/512
  USCI1_TWI_256 = (uint8_t)0x02,  //TWI通信速率 Fhrc/256
  USCI1_TWI_128 = (uint8_t)0x03,  //TWI通信速率 Fhrc/128
  USCI1_TWI_64 = (uint8_t)0x04,   //TWI通信速率 Fhrc/64
  USCI1_TWI_32 = (uint8_t)0x05,   //TWI通信速率 Fhrc/32
  USCI1_TWI_16 = (uint8_t)0x06,   //TWI通信速率 Fhrc/16
} USCI1_TWI_MasterCommunicationRate_TypeDef;

typedef enum
{
  /* 作为从机 */
  USCI1_TWI_SlaveIdle = 0x00,           //从机处于空闲状态
  USCI1_TWI_SlaveReceivedaAddress = 0x01,
  USCI1_TWI_SlaveReceivedaData = 0x02,
  USCI1_TWI_SlaveSendData = 0x03,
  USCI1_TWI_SlaveReceivedaUACK = 0x04,
  USCI1_TWI_SlaveDisableACK = 0x05,
  USCI1_TWI_SlaveAddressError = 0x06,
  /* 作为主机 */
  USCI1_TWI_MasterIdle = 0x00,          //主机处于空闲状态
  USCI1_TWI_MasterSendAddress = 0x01,
  USCI1_TWI_MasterSendData = 0x02,
  USCI1_TWI_MasterReceivedaData = 0x03,
  USCI1_TWI_MasterReceivedaUACK = 0x04,
} USCI1_TWIState_TypeDef;

typedef enum
{
  USCI1_UART_Mode_8B = 0X00,  //UART设置为8位模式
  USCI1_UART_Mode_10B = 0X40, //UART设置为10位模式
  USCI1_UART_Mode_11B = 0X80  //UART设置为11位模式
} USCI1_UART_Mode_TypeDef;

typedef enum
{
  USCI1_UART_RX_ENABLE = 0X10, //UART允许接收
  USCI1_UART_RX_DISABLE = 0X00 //UART禁止接收
} USCI1_UART_RX_TypeDef;

typedef enum
{
  USCI1_SPI_FLAG_SPIF = (uint8_t)0x80, //SPI数据传送标志位SPIF
  USCI1_SPI_FLAG_WCOL = (uint8_t)0x40, //SPI写入冲突标志位WCOL
  USCI1_SPI_FLAG_TXE = (uint8_t)0x08,  //SPI发送缓存器空标志TXE
  USCI1_TWI_FLAG_TWIF = (uint8_t)0x40, //TWI中断标志位TWIF
  USCI1_TWI_FLAG_GCA = (uint8_t)0x10,  //TWI通用地址响应标志位GCA
  USCI1_TWI_FLAG_MSTR = (uint8_t)0x20, //TWI主从标志位MSTR
  USCI1_TWI_FLAG_TXRXnE = (uint8_t)0x80,
  USCI1_UART_FLAG_RI = (uint8_t)0x01, //UART接收中断标志位RI
  USCI1_UART_FLAG_TI = (uint8_t)0x02, //UART发送中断标志位TI
} USCI1_Flag_TypeDef;

typedef enum
{
  USCI1_TWI_Write = 0x00, //写操作
  USCI1_TWI_Read = 0x01,  //读操作
} USCI1_TWI_RWType;

typedef enum
{
  USCI1_STATE_READY = 0x00,   //准备标志
  USCI1_STATE_BUSY = 0x01,    //忙碌标志
  USCI1_STATE_ERROR = 0x02,   //错误标志      
  USCI1_STATE_TIMEOUT = 0x03,  //超时标志
	USCI1_STATE_WAIT = 0x04,   //等待标志  
} USCI1_StatusTypeDef;

typedef struct __USCI1_HandleInfoDef
{
  union 
  {
    uint8_t* Size_u8;              //8位数据发送缓存区
    uint16_t* Size_u16;           //16位数据发送缓存区
  }pTxBuffPtr;
  uint8_t TxXferSize;              //数据发送量
	uint8_t TxXferCount;            //数据发送计数器
  union 
  {
    uint8_t* Size_u8;              //8位数据接收缓存区
    uint16_t* Size_u16;           //16位数据接收缓存区
  }pRxBuffPtr;
  uint8_t RxXferSize;              //数据接收量
	uint8_t RxXferCount;              //数据接收计数器大小
  USCI1_StatusTypeDef TxState;      //发送状态
  USCI1_StatusTypeDef RxState;      //接收状态
} USCI1_HandleInfoDef;


void USCI1_DeInit(void);

/********************************* SPI **************************************************/
void USCI1_SPI_Init(USCI1_SPI_FirstBit_TypeDef FirstBit,
                    USCI1_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI1_SPI_Mode_TypeDef Mode,
                    USCI1_SPI_ClockPolarity_TypeDef ClockPolarity, USCI1_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI1_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI1_TransmissionMode_TypeDef TransmissionMode);
void USCI1_TransmissionMode(USCI1_TransmissionMode_TypeDef TransmissionMode);
void USCI1_SPI_Cmd(FunctionalState NewState);
void USCI1_SPI_SendData_8(uint8_t Data);
uint8_t USCI1_SPI_ReceiveData_8(void);
void USCI1_SPI_SendData_16(uint16_t Data);
uint16_t USCI1_SPI_ReceiveData_16(void);

StatusTypeDef USCI1_SPI_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI1_SPI_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI1_SPI_TransmitReceive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout);

StatusTypeDef USCI1_SPI_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE);
StatusTypeDef USCI1_SPI_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size);
StatusTypeDef USCI1_SPI_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size);
StatusTypeDef USCI1_SPI_TransmitReceive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size);

/********************************* TWI **************************************************/
void USCI1_TWI_Slave_Init(uint8_t TWI_Address);
void USCI1_TWI_MasterCommunicationRate(USCI1_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate);
void USCI1_TWI_Start(void);
void USCI1_TWI_MasterModeStop(void);
void USCI1_TWI_SendAddr(uint8_t Addr, USCI1_TWI_RWType RW);
void USCI1_TWI_SlaveClockExtension(FunctionalState NewState);
void USCI1_TWI_AcknowledgeConfig(FunctionalState NewState);
void USCI1_TWI_GeneralCallCmd(FunctionalState NewState);
FlagStatus USCI1_GetTWIStatus(USCI1_TWIState_TypeDef USCI1_TWIState);
void USCI1_TWI_Cmd(FunctionalState NewState);
void USCI1_TWI_SendData(uint8_t Data);
uint8_t USCI1_TWI_ReceiveData(void);

StatusTypeDef USCI1_TWI_Master_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI1_TWI_Slave_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);

StatusTypeDef USCI1_TWI_Master_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI1_TWI_Master_Transmit_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE);

StatusTypeDef USCI1_TWI_Slave_Transmit_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI1_TWI_Slave_Transmit_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE);

StatusTypeDef USCI1_TWI_Master_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI1_TWI_Slave_Receive(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);

StatusTypeDef USCI1_TWI_Master_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI1_TWI_Master_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE);

StatusTypeDef USCI1_TWI_Slave_Receive_IT(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI1_TWI_Slave_Receive_IRQHandler(USCI1_HandleInfoDef* USCI1_HANDLE);

/********************************* UART **************************************************/
void USCI1_UART_Init(uint32_t UART1Fsys, uint32_t BaudRate, USCI1_UART_Mode_TypeDef Mode,
                     USCI1_UART_RX_TypeDef RxMode);
void USCI1_UART_SendData8(uint8_t Data);
uint8_t USCI1_UART_ReceiveData8(void);
void USCI1_UART_SendData9(uint16_t Data);
uint16_t USCI1_UART_ReceiveData9(void);

StatusTypeDef USCI1_UART_Transmit(USCI1_HandleInfoDef* USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI1_UART_Transmit_IT(USCI1_HandleInfoDef *USCI1_HANDLE, uint8_t *pData, uint8_t Size);
StatusTypeDef USCI1_UART_Transmit_IRQHandler(USCI1_HandleInfoDef *USCI1_HANDLE);
StatusTypeDef USCI1_UART_Receive(USCI1_HandleInfoDef *USCI1_HANDLE, uint8_t *pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI1_UART_Receive_IT(USCI1_HandleInfoDef *USCI1_HANDLE, uint8_t *pData, uint8_t Size);
StatusTypeDef USCI1_UART_Receive_IRQHandler(USCI1_HandleInfoDef *USCI1_HANDLE);

/********************************* 标志位相关 **************************************************/
void USCI1_ITConfig(FunctionalState NewState, PriorityStatus Priority);
FlagStatus USCI1_GetFlagStatus(USCI1_Flag_TypeDef USCI1_FLAG);
void USCI1_ClearFlag(USCI1_Flag_TypeDef USCI1_FLAG);

#endif

/******************* (C) COPYRIGHT 2022 SinOne Microelectronics *****END OF FILE****/