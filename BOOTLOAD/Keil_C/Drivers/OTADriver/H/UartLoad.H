//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: UARTtoBOOT.h
//	模块功能	: 赛元BootLoader 示例BootLoader例程 H文件
//  更正日期	: 2019/9/11
// 	版本		: V0.1
//************************************************************
#ifndef _UARTLOAD_H_
#define _UARTLOAD_H_

#include "sc95f.h"
#include "OTADriver.h"
#if false
/********************************用户配置开始*********************************************/
#define UartSelect     0               //选择UART:0:UART0,1:USCI0,2:USCI1,3:USCI2
#define UartBaud       115200          //波特率写入
#define	Fsoc           32000000        //主频选择
/********************************用户配置结束*********************************************/
#endif

#if(UartSelect==0)
#define UART_SFR		SBUF
#define	READ_RI			RI
#define CLEAR_RI		RI=0
#define	READ_TI			TI
#define CLEAR_TI		TI=0
#define	UART_INT_NUMBER	4
#endif
#if(UartSelect==1)
#define UART_SFR		US0CON3
#define	READ_RI			US0CON0&0X01
#define CLEAR_RI		US0CON0 |= 0X01 << 0
#define	READ_TI			US0CON0&0X02
#define CLEAR_TI		US0CON0 |= 0X01 << 1
#define	UART_INT_NUMBER	7
#endif
#if(UartSelect==2)
#define UART_SFR		US1CON3
#define	READ_RI			US1CON0&0X01
#define CLEAR_RI		US1CON0 |= 0X01 << 0
#define	READ_TI			US1CON0&0X02
#define CLEAR_TI		US1CON0 |= 0X01 << 1
#define	UART_INT_NUMBER	15
#endif
#if(UartSelect==3)
#define UART_SFR		USXCON3
#define	READ_RI			USXCON0 & 0X01
#define CLEAR_RI		USXCON0 |= 0X01 << 0;
#define	READ_TI			USXCON0 & 0X02
#define CLEAR_TI		USXCON0 |= 0X01 << 1;
#define	UART_INT_NUMBER	16
#endif
#if(UartSelect==4)
#define UART_SFR		USXCON3
#define	READ_RI			USXCON0&0X01
#define CLEAR_RI		USXCON0 |= 0X01 << 0;
#define	READ_TI			USXCON0&0X02
#define CLEAR_TI		USXCON0 |= 0X01 << 1;
#define	UART_INT_NUMBER	17
#endif
#if(UartSelect==5)
#define UART_SFR		USXCON3
#define	READ_RI			USXCON0&0X01
#define CLEAR_RI		USXCON0 |= 0X01 << 0;
#define	READ_TI			USXCON0&0X02
#define CLEAR_TI		USXCON0 |= 0X01 << 1;
#define	UART_INT_NUMBER	18
#endif

#if (UartSelect == 0)
#define CommunicationIOInit	    {P2PH|=0X03;}		//通讯口加上拉电阻
#endif

#if (UartSelect == 1)
#define CommunicationIOInit	    {P0PH|=0X30;}		//通讯口加上拉电阻
#endif

#if (UartSelect == 2)
#define CommunicationIOInit	    {P1PH|=0X06;}		//通讯口加上拉电阻
#endif

#if (UartSelect == 3)
#define CommunicationIOInit	    {P4PH|=0X30;}		//通讯口加上拉电阻
#endif

#if (UartSelect == 4)
#define CommunicationIOInit	    {P2PH|=0XC0;}		//通讯口加上拉电阻
#endif

#if (UartSelect == 5)
#define CommunicationIOInit	    {P4PH|=0XC0;}		//通讯口加上拉电阻
#endif

/*************************通讯协议相关宏定义*******************************/
//命令帧格式：帧头+命令位+(起始地址+)帧尾
//数据帧格式：帧头+长度+数据+校验
/**************************************************************************/
#define CommunicationLength0   		 3			//通讯一帧长度 用于命令帧
#define CommunicationDataHeader      0X55		//数据帧头
#define CommunicationCommandHeader   0Xaa		//命令帧头
#define CommunicationCommandEnd		 0xEF		//命令帧尾

#define IAPMODEENTER 				 0X01		//进入IAP操作模式命令宏
#define IAPBAUDSET					 0x04		//更新通讯波特率命令
#define IAPBAUDREALLY				 0x14		//回应波特率设置成功
#define IAPMODEEXIT					 0xff		//退出IAP操作模式命令宏
#define IAPSUCCESS 				     0X02		//IAP操作成功
#define IAPFAIL						 0x03		//IAP操作失败
#define IAPIDLE						 0x00       //待机
#define IAPWRITE					 0X10		//接收到IAP写命令
#define IAPREALLY					 0x11		//准备接收数据
#define IAPADRESSERROR				 0X44		//操作地址不在用户区

/****************************状态宏定义**********************************/
#define WORKMODE    	0			//等待进入BOOT模式
#define IAPMODE 		1			//BootLoader模式

#define RECECMD     0				//接收命令帧
#define RECEDATA 	1			  	//接收数据帧
/***********************************************************************/
extern unsigned char xdata UpdataComplete;
extern unsigned char xdata UpdataSate;

void UartLoadInit();		  //串口初始化
void CheckCMDtoBOOT(); //检测是否有进入BOOT的命令，将该函数放在串口中断服务函数内
void UartLoad(unsigned char UartData);

#endif

