//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_usci0.h
//	����		:
//	ģ�鹦��: 		USCI0�̼��⺯��ͷ�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��:���ļ���������SC95Fϵ��оƬ
//*************************************************************

#ifndef _sc95f_USCI0_H_
#define _sc95f_USCI0_H_

#include "sc95f.h"

#define USCI0_UART_BaudRate_FsysDIV12 0X00 //��ģʽ0���ã����ж˿���ϵͳʱ�ӵ�1/12������
#define USCI0_UART_BaudRate_FsysDIV4 0X01  //��ģʽ0���ã����ж˿���ϵͳʱ�ӵ�1/4������

#if defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
	|| defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605)
typedef enum
{
  USCI0_Mode_SC = (uint8_t)0x00,  //SmartCard
  USCI0_Mode_SPI = (uint8_t)0x01, //SPI
  USCI0_Mode_TWI = (uint8_t)0x02, //TWI
  USCI0_Mode_UART = (uint8_t)0x03 //UART
} USCI0_CommunicationMode_TypeDef;
#else
typedef enum
{
  USCI0_Mode_SPI = (uint8_t)0x01, //SPI
  USCI0_Mode_TWI = (uint8_t)0x02, //TWI
  USCI0_Mode_UART = (uint8_t)0x03 //UART
} USCI0_CommunicationMode_TypeDef;
#endif

typedef enum
{
  USCI0_SPI_FIRSTBIT_MSB = (uint8_t)0x00, //MSB���ȷ���
  USCI0_SPI_FIRSTBIT_LSB = (uint8_t)0x04  //LSB���ȷ���
} USCI0_SPI_FirstBit_TypeDef;

typedef enum
{
  USCI0_SPI_BAUDRATEPRESCALER_1 = (uint8_t)0x00,  //SPIʱ������Ϊϵͳʱ�ӳ���1
  USCI0_SPI_BAUDRATEPRESCALER_2 = (uint8_t)0x01,  //SPIʱ������Ϊϵͳʱ�ӳ���2
  USCI0_SPI_BAUDRATEPRESCALER_4 = (uint8_t)0x02,  //SPIʱ������Ϊϵͳʱ�ӳ���4
  USCI0_SPI_BAUDRATEPRESCALER_8 = (uint8_t)0x03,  //SPIʱ������Ϊϵͳʱ�ӳ���8
  USCI0_SPI_BAUDRATEPRESCALER_16 = (uint8_t)0x04, //SPIʱ������Ϊϵͳʱ�ӳ���16
  USCI0_SPI_BAUDRATEPRESCALER_32 = (uint8_t)0x05, //SPIʱ������Ϊϵͳʱ�ӳ���32
  USCI0_SPI_BAUDRATEPRESCALER_64 = (uint8_t)0x06, //SPIʱ������Ϊϵͳʱ�ӳ���64
  USCI0_SPI_BAUDRATEPRESCALER_128 = (uint8_t)0x07 //SPIʱ������Ϊϵͳʱ�ӳ���128
} USCI0_SPI_BaudRatePrescaler_TypeDef;

typedef enum
{
  USCI0_SPI_MODE_MASTER = (uint8_t)0x20, //SPIΪ���豸
  USCI0_SPI_MODE_SLAVE = (uint8_t)0x00   //SPIΪ���豸
} USCI0_SPI_Mode_TypeDef;

typedef enum
{
  USCI0_SPI_CLOCKPOLARITY_LOW = (uint8_t)0x00, //SCK�ڿ���״̬��Ϊ�͵�ƽ
  USCI0_SPI_CLOCKPOLARITY_HIGH = (uint8_t)0x10 //SCK�ڿ���״̬��Ϊ�ߵ�ƽ
} USCI0_SPI_ClockPolarity_TypeDef;

typedef enum
{
  USCI0_SPI_CLOCKPHASE_1EDGE = (uint8_t)0x00, //SCK�ĵ�һ�زɼ�����
  USCI0_SPI_CLOCKPHASE_2EDGE = (uint8_t)0x08  //SCK�ĵڶ��زɼ�����
} USCI0_SPI_ClockPhase_TypeDef;

typedef enum
{
  USCI0_SPI_TXE_DISINT = (uint8_t)0x00, //TBIEΪ0ʱ���������ж�
  USCI0_SPI_TXE_ENINT = (uint8_t)0x01   //TBIEΪ1ʱ�������ж�
} USCI0_SPI_TXE_INT_TypeDef;

typedef enum
{
  USCI0_SPI_DATA8 = (uint8_t)0x00, //SPI 8λ����ģʽ
  USCI0_SPI_DATA16 = (uint8_t)0x02 //SPI 16λ����ģʽ
} USCI0_TransmissionMode_TypeDef;

typedef enum
{
  USCI0_TWI_1024 = (uint8_t)0x00, //TWIͨ������ Fhrc/1024
  USCI0_TWI_512 = (uint8_t)0x01,  //TWIͨ������ Fhrc/512
  USCI0_TWI_256 = (uint8_t)0x02,  //TWIͨ������ Fhrc/256
  USCI0_TWI_128 = (uint8_t)0x03,  //TWIͨ������ Fhrc/128
  USCI0_TWI_64 = (uint8_t)0x04,   //TWIͨ������ Fhrc/64
  USCI0_TWI_32 = (uint8_t)0x05,   //TWIͨ������ Fhrc/32
  USCI0_TWI_16 = (uint8_t)0x06,   //TWIͨ������ Fhrc/16
} USCI0_TWI_MasterCommunicationRate_TypeDef;

typedef enum
{
  /* ��Ϊ�ӻ� */
  USCI0_TWI_SlaveIdle = 0x00,           //�ӻ����ڿ���״̬
  USCI0_TWI_SlaveReceivedaAddress = 0x01,
  USCI0_TWI_SlaveReceivedaData = 0x02,
  USCI0_TWI_SlaveSendData = 0x03,
  USCI0_TWI_SlaveReceivedaUACK = 0x04,
  USCI0_TWI_SlaveDisableACK = 0x05,
  USCI0_TWI_SlaveAddressError = 0x06,
  /* ��Ϊ���� */
  USCI0_TWI_MasterIdle = 0x00,          //�������ڿ���״̬
  USCI0_TWI_MasterSendAddress = 0x01,
  USCI0_TWI_MasterSendData = 0x02,
  USCI0_TWI_MasterReceivedaData = 0x03,
  USCI0_TWI_MasterReceivedaUACK = 0x04,
} USCI0_TWIState_TypeDef;

typedef enum
{
  USCI0_UART_Mode_8B = 0X00,  //UART����Ϊ8λģʽ
  USCI0_UART_Mode_10B = 0X40, //UART����Ϊ10λģʽ
  USCI0_UART_Mode_11B = 0X80  //UART����Ϊ11λģʽ
} USCI0_UART_Mode_TypeDef;

typedef enum
{
  USCI0_UART_RX_ENABLE = 0X10, //UART�������
  USCI0_UART_RX_DISABLE = 0X00 //UART��ֹ����
} USCI0_UART_RX_TypeDef;

typedef enum
{
  USCI0_SPI_FLAG_SPIF = (uint8_t)0x80, //SPI���ݴ��ͱ�־λSPIF
  USCI0_SPI_FLAG_WCOL = (uint8_t)0x40, //SPIд���ͻ��־λWCOL
  USCI0_SPI_FLAG_TXE = (uint8_t)0x08,  //SPI���ͻ������ձ�־TXE
  USCI0_TWI_FLAG_TWIF = (uint8_t)0x40, //TWI�жϱ�־λTWIF
  USCI0_TWI_FLAG_GCA = (uint8_t)0x10,  //TWIͨ�õ�ַ��Ӧ��־λGCA
  USCI0_TWI_FLAG_MSTR = (uint8_t)0x20, //TWI���ӱ�־λMSTR
  USCI0_TWI_FLAG_TXRXnE = (uint8_t)0x80,
  USCI0_UART_FLAG_RI = (uint8_t)0x01, //UART�����жϱ�־λRI
  USCI0_UART_FLAG_TI = (uint8_t)0x02, //UART�����жϱ�־λTI
} USCI0_Flag_TypeDef;

typedef enum
{
  USCI0_TWI_Write = 0x00, //д����
  USCI0_TWI_Read = 0x01,  //������
} USCI0_TWI_RWType;

typedef enum
{
  USCI0_STATE_READY = 0x00,   //׼����־
  USCI0_STATE_BUSY = 0x01,    //æµ��־
  USCI0_STATE_ERROR = 0x02,   //�����־
  USCI0_STATE_TIMEOUT = 0x03,  //��ʱ��־
  USCI0_STATE_WAIT = 0x04,   //�ȴ���־
} USCI0_StatusTypeDef;

typedef struct __USCI0_HandleInfoDef
{
  union
  {
    uint8_t* Size_u8;              //8λ���ݷ��ͻ�����
    uint16_t* Size_u16;           //16λ���ݷ��ͻ�����
  } pTxBuffPtr;
  uint8_t TxXferSize;              //���ݷ�����
  uint8_t TxXferCount;            //���ݷ��ͼ�����
  union
  {
    uint8_t* Size_u8;              //8λ���ݽ��ջ�����
    uint16_t* Size_u16;           //16λ���ݽ��ջ�����
  } pRxBuffPtr;
  uint8_t RxXferSize;              //���ݽ�����
  uint8_t RxXferCount;              //���ݽ��ռ�������С
  USCI0_StatusTypeDef TxState;      //����״̬
  USCI0_StatusTypeDef RxState;      //����״̬
} USCI0_HandleInfoDef;

#if defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95FS52x)

typedef enum
{
  USCI0_SC_FLAG_TC = (uint8_t)0x40,    //������ɱ�־λ���������Ӳ����1�����������
  USCI0_SC_FLAG_RC = (uint8_t)0x20,    //������ɱ�־λ���������Ӳ����1�����������
  USCI0_SC_FLAG_WTRT = (uint8_t)0x10,  //�ȴ������ط���־λ
  USCI0_SC_FLAG_TBUSY = (uint8_t)0x08, //���ݷ���æ��־λ
  USCI0_SC_FLAG_RBUSY = (uint8_t)0x04, //���ݽ���æ��־λ
  USCI0_SC_FLAG_WTER = (uint8_t)0x02,  //�ȴ���ʱ��־λ
  USCI0_SC_FLAG_FER = (uint8_t)0x01,   //����֡�����־λ
  USCI0_SC_FLAG_TPER = (uint8_t)0x88,  //����������żУ������־λ
  USCI0_SC_FLAG_RPER = (uint8_t)0x84,  //����������żУ������־λ
  USCI0_SC_FLAG_ROVF = (uint8_t)0x81   //���������־λ
} USCI0_SC_Flag_TypeDef;

typedef enum
{
  USCI0_SC_RECE = (uint8_t)0x00, //����ʹ�ܣ����ͽ�ֹ
  USCI0_SC_SEND = (uint8_t)0x20, //����ʹ�ܣ����ս�ֹ
} USCI0_SC_TREN_TypeDef;

typedef enum
{
  USCI0_SC_Cons_Forward = (uint8_t)0x00, //����Լ����ʽ���߼���ƽ��ʵ��һ��
  USCI0_SC_Cons_Reverse = (uint8_t)0x04  //����Լ����ʽ���߼���ƽ��ʵ���෴
} USCI0_SC_CONS_TypeDef;

typedef enum
{
  USCI0_SC_PRESSEL_FSYS_D2 = (uint8_t)0x00,  //���ܿ�ʱ��Դ����ϵͳʱ��2��Ƶ
  USCI0_SC_PRESSEL_FSYS_D4 = (uint8_t)0x01,  //���ܿ�ʱ��Դ����ϵͳʱ��4��Ƶ
  USCI0_SC_PRESSEL_FSYS_D6 = (uint8_t)0x02,  //���ܿ�ʱ��Դ����ϵͳʱ��6��Ƶ
  USCI0_SC_PRESSEL_FSYS_D8 = (uint8_t)0x03,  //���ܿ�ʱ��Դ����ϵͳʱ��8��Ƶ
  USCI0_SC_PRESSEL_FSYS_D10 = (uint8_t)0x04, //���ܿ�ʱ��Դ����ϵͳʱ��10��Ƶ
  USCI0_SC_PRESSEL_FSYS_D12 = (uint8_t)0x05, //���ܿ�ʱ��Դ����ϵͳʱ��12��Ƶ
  USCI0_SC_PRESSEL_FSYS_D14 = (uint8_t)0x06, //���ܿ�ʱ��Դ����ϵͳʱ��14��Ƶ
  USCI0_SC_PRESSEL_FSYS_D16 = (uint8_t)0x07, //���ܿ�ʱ��Դ����ϵͳʱ��16��Ƶ
  USCI0_SC_PRESSEL_FSYS_D18 = (uint8_t)0x08, //���ܿ�ʱ��Դ����ϵͳʱ��18��Ƶ
  USCI0_SC_PRESSEL_FSYS_D20 = (uint8_t)0x09, //���ܿ�ʱ��Դ����ϵͳʱ��20��Ƶ
  USCI0_SC_PRESSEL_FSYS_D22 = (uint8_t)0x0a, //���ܿ�ʱ��Դ����ϵͳʱ��22��Ƶ
  USCI0_SC_PRESSEL_FSYS_D24 = (uint8_t)0x0b, //���ܿ�ʱ��Դ����ϵͳʱ��24��Ƶ
  USCI0_SC_PRESSEL_FSYS_D26 = (uint8_t)0x0c, //���ܿ�ʱ��Դ����ϵͳʱ��26��Ƶ
  USCI0_SC_PRESSEL_FSYS_D28 = (uint8_t)0x0d, //���ܿ�ʱ��Դ����ϵͳʱ��28��Ƶ
  USCI0_SC_PRESSEL_FSYS_D30 = (uint8_t)0x0e, //���ܿ�ʱ��Դ����ϵͳʱ��30��Ƶ
  USCI0_SC_PRESSEL_FSYS_D32 = (uint8_t)0x0f  //���ܿ�ʱ��Դ����ϵͳʱ��32��Ƶ
} USCI0_SC_PresSel_TypeDef;

typedef enum
{
  USCI0_SC_ERS_2_ETU = 0x00,   //Stop��Error Signal���Ⱦ�Ϊ2��ETU
  USCI0_SC_ERS_1P5_ETU = 0x02, //Stop��Error Signal���Ⱦ�Ϊ1.5��ETU
  USCI0_SC_ERS_1_ETU = 0x03    //Stop��Error Signal���Ⱦ�Ϊ1��ETU
} USCI0_SC_ERS_TypeDef;

typedef enum
{
  USCI0_SC_NoCheck = 0x00,  //��У��
  USCI0_SC_EvenCheck = 0x01 //żУ��
} USCI0_SC_Check_TypeDef;

#endif

void USCI0_DeInit(void);

/********************************* SPI **************************************************/
void USCI0_SPI_Init(USCI0_SPI_FirstBit_TypeDef FirstBit,
                    USCI0_SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, USCI0_SPI_Mode_TypeDef Mode,
                    USCI0_SPI_ClockPolarity_TypeDef ClockPolarity, USCI0_SPI_ClockPhase_TypeDef ClockPhase,
                    USCI0_SPI_TXE_INT_TypeDef SPI_TXE_INT, USCI0_TransmissionMode_TypeDef TransmissionMode);
void USCI0_TransmissionMode(USCI0_TransmissionMode_TypeDef TransmissionMode);
void USCI0_SPI_Cmd(FunctionalState NewState);
void USCI0_SPI_SendData_8(uint8_t Data);
uint8_t USCI0_SPI_ReceiveData_8(void);
void USCI0_SPI_SendData_16(uint16_t Data);
uint16_t USCI0_SPI_ReceiveData_16(void);

StatusTypeDef USCI0_SPI_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI0_SPI_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI0_SPI_TransmitReceive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size, uint32_t Timeout);

StatusTypeDef USCI0_SPI_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);
StatusTypeDef USCI0_SPI_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_SPI_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_SPI_TransmitReceive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pTxData, uint8_t* pRxData, uint8_t Size);

#if defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95F8x3x) || defined(SC95F7x3x) || defined (SC95F8x6x) || defined (SC95F7x6x)\
	|| defined (SC95F8x1xB) || defined (SC95F7x1xB) || defined(SC95FS52x) || defined (SC95R602)  || defined (SC95R605)\
	|| defined(SC95F8x7x) || defined(SC95F7x7x)|| defined(SC95R503)
uint8_t USCI0_SPI_WriteFIFO(uint16_t* sbuf, uint8_t length);
void USCI0_SPIFIFO_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);
USCI0_StatusTypeDef USCI0_SPI_Receive_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE);
USCI0_StatusTypeDef USCI0_SPI_TransmitReceive_FIFO(USCI0_HandleInfoDef* USCI0_HANDLE);
#endif

/********************************* TWI **************************************************/
void USCI0_TWI_Slave_Init(uint8_t TWI_Address);
void USCI0_TWI_MasterCommunicationRate(USCI0_TWI_MasterCommunicationRate_TypeDef
                                       TWI_MasterCommunicationRate);
void USCI0_TWI_Start(void);
void USCI0_TWI_MasterModeStop(void);
void USCI0_TWI_SendAddr(uint8_t Addr, USCI0_TWI_RWType RW);
void USCI0_TWI_SlaveClockExtension(FunctionalState NewState);
void USCI0_TWI_AcknowledgeConfig(FunctionalState NewState);
void USCI0_TWI_GeneralCallCmd(FunctionalState NewState);
FlagStatus USCI0_GetTWIStatus(USCI0_TWIState_TypeDef USCI0_TWIState);
void USCI0_TWI_Cmd(FunctionalState NewState);
void USCI0_TWI_SendData(uint8_t Data);
uint8_t USCI0_TWI_ReceiveData(void);

StatusTypeDef USCI0_TWI_Master_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI0_TWI_Slave_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);

StatusTypeDef USCI0_TWI_Master_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_TWI_Master_Transmit_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);

StatusTypeDef USCI0_TWI_Slave_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_TWI_Slave_Transmit_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);

StatusTypeDef USCI0_TWI_Master_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI0_TWI_Slave_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);

StatusTypeDef USCI0_TWI_Master_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t slaveAddr, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_TWI_Master_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);

StatusTypeDef USCI0_TWI_Slave_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_TWI_Slave_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);

/********************************* UART **************************************************/
void USCI0_UART_Init(uint32_t UART1Fsys, uint32_t BaudRate, USCI0_UART_Mode_TypeDef Mode,
                     USCI0_UART_RX_TypeDef RxMode);
void USCI0_UART_SendData8(uint8_t Data);
uint8_t USCI0_UART_ReceiveData8(void);
void USCI0_UART_SendData9(uint16_t Data);
uint16_t USCI0_UART_ReceiveData9(void);

StatusTypeDef USCI0_UART_Transmit(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI0_UART_Transmit_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_UART_Transmit_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);
StatusTypeDef USCI0_UART_Receive(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size, uint32_t Timeout);
StatusTypeDef USCI0_UART_Receive_IT(USCI0_HandleInfoDef* USCI0_HANDLE, uint8_t* pData, uint8_t Size);
StatusTypeDef USCI0_UART_Receive_IRQHandler(USCI0_HandleInfoDef* USCI0_HANDLE);

/********************************* ��־λ��� **************************************************/
void USCI0_ITConfig(FunctionalState NewState, PriorityStatus Priority);
FlagStatus USCI0_GetFlagStatus(USCI0_Flag_TypeDef USCI0_FLAG);
void USCI0_ClearFlag(USCI0_Flag_TypeDef USCI0_FLAG);

/********************************* SC **************************************************/
#if defined(SC95F8x2x) || defined(SC95F7x2x) || defined(SC95FS52x)
void USCI0_SC_Init(FunctionalState ClockEnable,
                   USCI0_SC_ERS_TypeDef ErrorSignal,
                   USCI0_SC_TREN_TypeDef TrenMode,
                   USCI0_SC_CONS_TypeDef ConsMode,
                   FunctionalState ReSend,
                   USCI0_SC_Check_TypeDef CheckType);
void USCI0_SC_EGT(uint8_t EgtCount);
void USCI0_SC_Cmd(FunctionalState NewState);
void USCI0_SC_ETU_SET(USCI0_SC_PresSel_TypeDef SC_PrescalerSelection, uint16_t EtuCk);
FlagStatus USCI0_SC_GetFlagStatus(USCI0_SC_Flag_TypeDef USCI0_SC_FLAG);
void USCI0_SC_SendData(uint8_t Data);
uint8_t USCI0_SC_ReceiveData(void);

#endif

#endif

/******************* (C) COPYRIGHT 2022 SinOne Microelectronics *****END OF FILE****/