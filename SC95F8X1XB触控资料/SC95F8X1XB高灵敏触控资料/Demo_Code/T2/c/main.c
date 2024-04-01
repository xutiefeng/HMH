 //************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: main.c
//	作者		: 
//	模块功能	: 
//  最后更正日期:
// 	版本		: 
//              :  
//*************************************************************
#include "h\SC95F861xB_C.h"
#include "h\data_type.h"
#include "lib\SensorMethod.h"

//*****************全局变量区***************************************************
INT32U exKeyValueFlag = 0;		//当前轮按键标志
INT8U  Timercount = 0;			//定时器计数
BOOL   TimerFlag_1ms = 0;		//定时器1MS标志

 /**************************************************
*函数名称：void TimerInit(void) 
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void TimerInit(void)
{
	TMCON = (TMCON&0xfe)|(0<<0);		//bit0: 0为FOSC/12,1为FOSO

	TMOD = TMOD&0xf0;		  			//设置定时0，工作方式0
	TMOD = TMOD|0x00;
	TH0=(8192-1000)/32;       			//1000*1=1000us	,1MS
	TL0=(8192-1000)%32;
	TF0 = 0;						    //清中断标志
	TR0=0;								//关定时器0
   	ET0=1;								//使能定时器0中断
	TR0=1;
}

/**************************************************
*函数名称：void timer0()interrupt 1 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
void timer0()interrupt 1
{
	TH0 = (8192-1000)/32;       			//2000*1/4us=500us
	TL0 = (8192-1000)%32;	
	TimerFlag_1ms = 1;	
}
 /**************************************************
*函数名称：void  Sys_Init(void) 
*函数功能：系统初始化
*入口参数：void
*出口参数：void  
**************************************************/
void  Sys_Init(void)
{	
	WDTCON  = 0x10;				    //1--1 -- 00    开WDT,WDT清0,WDT 524.288ms溢出;烧录时，可Code Option选择ENWDT
	//TK对应的IO设置为强推挽输出1
	P0CON = 0xFF;
	P0PH  = 0xFF;	
	P1CON = 0xFF;
	P1PH  = 0xFF;
	P2CON = 0xFF;
	P2PH  = 0xFF;
	P5CON = 0xFF;
	P5PH  = 0xFF;
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P5 = 0x00; 	
		   
	EA = 1;						//开总中断	
    TimerInit(); 				//定时器初始化
}

/**************************************************
*函数名称：void  Sys_Scan(void) 
*函数功能：扫描TK和显示
*入口参数：void
*出口参数：void  
**************************************************/
void Sys_Scan(void)
{      				
	if(SOCAPI_TouchKeyStatus&0x40)		//当按键个数大于8个时，要判断半轮标志是否被置起，然后启动下半轮
	{
		SOCAPI_TouchKeyStatus &= 0xBf;	// 清除标志位， 需要外部清除。
		TouchKeyRestart();				//启动下一轮转换
	}
	if(SOCAPI_TouchKeyStatus&0x80)	    //重要步骤2:  触摸键扫描一轮标志，是否调用TouchKeyScan()一定要根据此标志位置起后
	 {	   																	
		SOCAPI_TouchKeyStatus &= 0x7f;	//重要步骤3: 清除标志位， 需要外部清除。													    
		exKeyValueFlag = TouchKeyScan();//按键数据处理函数   
		
		TouchKeyRestart();				//启动下一轮转换																														 			
	}		  	   
}
/**************************************************
*函数名称：void main(void)								  
*函数功能：主函数
*入口参数：void
*出口参数：void  
**************************************************/
void main(void)
{					
	Sys_Init();
	
	//触控按键初始化
	TouchKeyInit();

	while(1)
	{
	   WDTCON  = 0x10;	   	   	   
	   if(TimerFlag_1ms==1)
	   {
			TimerFlag_1ms=0;	
			Timercount++;
			if(Timercount>10)
			{
				Timercount=0;
				Sys_Scan();
			}	  		    		   		   	  			  
	   }
	     	   	   
	} 
}