//*************************************************************************************************
//  Copyright (c) 	��������Ԫ΢�������޹�˾
//	�ļ�����	:  SensorMethod.h
//	����		: 
//	ģ�鹦��	:  �����㷨ͷ�ļ�
// 	�汾	    :
// 	���ļ�¼	:
//	ע������	:    
//************************************************************************************************
#ifndef	_TKDriver_H
#define	_TKDriver_H


//<<<Use SPTML>>>
#define SCD_TK_Type 1 //SCD_TK_Type 
#define SCD_Wheel 0 //SCD_Wheel 
#define SCD_Slider 0 //SCD_Slider 
#define SCD_Key 1 //SCD_Key 
extern unsigned char xdata SOCAPI_TouchKeyStatus;

extern unsigned long int TouchKeyScan();
extern void TouchKeyInit();
extern void TouchKeyRestart();
//<<<end of SPTML>>>

//�ⲿ����
extern	unsigned	char	xdata	SOCAPI_TouchKeyStatus;			//API?��?�����䨬?��obit7-��???����?������3������??	
extern 	unsigned 	char	xdata	ConfirmTouchCnt;														
extern 	unsigned 	int		xdata 	RawData[];			 				
extern 	unsigned 	int		xdata	BaseLine[];	
extern	unsigned  	int	 	xdata  	FilterData[];	
extern	unsigned 	int 	xdata 	CurrentSensorKey;		 				
extern 	unsigned	char  	xdata 	RestAreaCnt[]; 					 
extern 	unsigned	char  	xdata 	TouchCnt[];
extern 	unsigned	char  	xdata 	NoTouchCnt[];  				 			  
extern  unsigned	char 	xdata 	CurrentChannel[];
extern  unsigned    char    xdata   LowFingerDataCnt[]; 			 
extern 				int     xdata 	DifferAccum[];
extern  unsigned	char  	xdata	FloatAreaCnt[]; 
extern 				char	xdata	SetNoiseThreshold;
extern  unsigned 	char	xdata	MultipleDealTpye;	
extern  unsigned 	char    xdata   ICHG_RawData_Ratio;

				

#endif 
