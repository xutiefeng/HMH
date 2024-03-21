//************************************************************
//  Copyright (c) 
//	文件名称	: sc92f854x_ddic.c
//	作者		: 
//	模块功能	: TC_LCD_GUI_DEF H文件
//	局部函数列表:
//  最后更正日期: 2020/7/10
// 	版本		: V1.0
//  说明        ：  
//*************************************************************
#ifndef __TC_LCD_GUI_DEF_H_
#define __TC_LCD_GUI_DEF_H_

#define KEY_SWITCH 	6
#define KEY_SPEED 	1
#define KEY_RIGHT 	2
#define KEY_LEFT 	3
#define KEY_MENU   	4
#define KEY_TIMING 	5

typedef void (TC_LCD_GUI_TASK)(void);
typedef long (TC_LCD_GUI_GET_DATA)(void);

typedef enum
{
	MAIN_MENU_SET,
	TIMING_SET,
	TEMP_SET,
	MODE_SET,
	SPEED_SET
}TC_LCD_GUI_MAIN_MENU_STATE;

typedef enum
{
	SPEEL_STATE,
	WORK_STATE,
	DOWM_POWR_STATE
}TC_LCD_GUI_WORK_STATE;

typedef struct
{
	unsigned int DefaultSetFlag;
	unsigned char Speed; 			
	unsigned char Mode;
	long Temperature;		
	long TimingTime;
	long TimingStart;
}TC_LCD_GUI_DATA;

typedef struct
{
	TC_LCD_GUI_GET_DATA *TimeGet;
	TC_LCD_GUI_GET_DATA *RoomTemperatureGet;
}TC_LCD_GUI_TIME_TEMP;

	

typedef enum
{
	TIMING_CHANGE_EVENT = 0,
	MODE_CHANGE_EVENT = 1,
	TEMP_CHANGE_EVENT = 2,
	SPEED_CHANGE_EVENT = 3,
	SPEEL_EVENT = 4,
	DOWM_POWR_EVENT = 5,
	WORK_EVENT = 6,
	TIMING_OVERTIME_EVENT = 7
}TC_LCD_GUI_EVENT;

typedef struct 
{
	TC_LCD_GUI_TASK *task;
	TC_LCD_GUI_EVENT event;
}TC_LCD_GUI_EVENT_TASK;

typedef struct 
{
	TC_LCD_GUI_TASK *task;
	unsigned long time;
}TC_LCD_GUI_TIMED_TASK;

typedef struct 
{
	TC_LCD_GUI_TASK *task;
	unsigned long time;
	unsigned long lastTime;
}TC_LCD_GUI_CYCLE_TASK;



#endif