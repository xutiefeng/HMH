#include "..\H\TC_LCD_GUI.h"
#include "..\H\TC_LCD_GUI_DEF.h"
#include "..\Drivers\TC_LCD_TK\H\TC_LCD_TK.h"
#include "..\Drivers\TC_LCD\H\TC_LCD.h"
#include "SC_Init.h"
#include <string.h>
#include <math.h>

xdata TC_LCD_GUI_DATA TC_LCD_ConData;
xdata volatile unsigned long GUI_TIME = 0;
xdata TC_LCD_GUI_EVENT_TASK EventTask[10];
xdata TC_LCD_GUI_TIMED_TASK TimedTask[10];
xdata TC_LCD_GUI_CYCLE_TASK CycleTask[10];
xdata TC_LCD_GUI_MAIN_MENU_STATE MENU_STATE;
xdata TC_LCD_GUI_TIME_TEMP SysTimeTempGet;
xdata TC_LCD_GUI_WORK_STATE WorkState;
xdata long WorkTime;
xdata char TimingTimePointer;
xdata long Time;
xdata long RecoveryTime;
xdata long RoomTemperature;


/**************************************************
*函数名称：void TC_LCD_GUI_GetIapData(void)
*函数功能：从EEPROM中加载数据
*入口参数：void	 
*出口参数：void
**************************************************/
void  TC_LCD_GUI_GetIapData()
{
	unsigned int addr;
	for(addr = 0; addr < sizeof(TC_LCD_GUI_DATA); addr ++)
	{
		*((char*)&TC_LCD_ConData + addr) = IAP_ReadByte(addr, IAP_MEMTYPE_EEPROM);
	}
	
}

/**************************************************
*函数名称：void TC_LCD_GUI_WriteIapData(void)
*函数功能：将数据加载到EEPROM
*入口参数：void	 
*出口参数：void
**************************************************/
void  TC_LCD_GUI_WriteIapData()
{
	unsigned int addr;
	for(addr = 0; addr < sizeof(TC_LCD_GUI_DATA); addr ++)
	{
		IAP_ProgramByte(addr, *((char*)&TC_LCD_ConData + addr), IAP_MEMTYPE_EEPROM, 100);
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_TimeGetLoad(void *fun)
*函数功能：时间获取函数加载
*入口参数：void *fun	 
*出口参数：void
**************************************************/
void TC_LCD_GUI_TimeGetLoad(void *fun)
{
	SysTimeTempGet.TimeGet = fun;
}

/**************************************************
*函数名称：void TC_LCD_GUI_RoomTempGetLoad(void *fun)
*函数功能：温度获取函数加载
*入口参数：void *fun	 
*出口参数：void
**************************************************/
void TC_LCD_GUI_RoomTempGetLoad(void *fun)
{
	SysTimeTempGet.RoomTemperatureGet = fun;
}

/**************************************************
*函数名称：void TC_LCD_GUI_EventSend(TC_LCD_GUI_EVENT event)
*函数功能：发送事件，触发事件任务
*入口参数：void	 
*出口参数：void
**************************************************/
void TC_LCD_GUI_EventSend(TC_LCD_GUI_EVENT event)
{
	int i;
	for(i = 0; i < sizeof(EventTask) / sizeof(TC_LCD_GUI_EVENT_TASK); i ++)
	{
		if(EventTask[i].event == event && EventTask[i].task != (void *)0)
			EventTask[i].task();
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_TimedTaskEx()
*函数功能：执行定时任务
*入口参数：void	 
*出口参数：void
**************************************************/
void  TC_LCD_GUI_TimedTaskEx()
{
	int i;
	for(i = 0; i < sizeof(TimedTask) / sizeof(TC_LCD_GUI_TIMED_TASK); i ++)
	{
		if(TimedTask[i].task != 0 && TimedTask[i].time >= GUI_TIME)
		{	
			TimedTask[i].task();
			TimedTask[i].task = (void *)0;
		}
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_CycleTaskEx()
*函数功能：执行周期任务
*入口参数：void	 
*出口参数：void
**************************************************/
void  TC_LCD_GUI_CycleTaskEx()
{
	int i;
	for(i = 0; i < sizeof(CycleTask) / sizeof(TC_LCD_GUI_CYCLE_TASK); i ++)
	{
		if(CycleTask[i].task != 0 && CycleTask[i].lastTime +  CycleTask[i].time >= GUI_TIME)
		{	
			CycleTask[i].task();
			CycleTask[i].lastTime = GUI_TIME;
		}
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_En()
*函数功能：LCD显示控制
*入口参数：state ， 支持工作模式、低功耗模式、睡眠模式	 
*出口参数：void
**************************************************/
void TC_LCD_GUI_En(TC_LCD_GUI_WORK_STATE state)
{
	MENU_STATE = MAIN_MENU_SET;
	if(state == WORK_STATE)
	{
		LCD_BLSet(0xff);
		LCD_SelectMenu(0xff);
		LCD_RoomTemperature(0x03);
		LCD_ConMode(TC_LCD_ConData.Mode);
		TimingTimePointer = 0;
		
		WorkState = WORK_STATE;
		
		TC_LCD_GUI_EventSend(WORK_EVENT);
	}
	
	if(state == DOWM_POWR_STATE)
	{
		LCD_Week(0x00);
		LCD_ConMode(0x00);
		LCD_SelectMenu(0x04);
		LCD_TimeState(0x00);
		LCD_SpeedBar(0x00);
		LCD_RoomTemperature(0x00);
		LCD_AutoWind(0x00);
		LCD_TimeNumAndSign(-1, -1, 0, -1, -1);
		TimingTimePointer = 0;
		
		WorkState = DOWM_POWR_STATE;
		
		TC_LCD_GUI_EventSend(DOWM_POWR_EVENT);
	}
	
	if(state == SPEEL_STATE)
	{
		LCD_BLSet(0x70);
		LCD_Week(0x00);
		LCD_ConMode(0x00);
		LCD_SelectMenu(0x00);
		LCD_TimeState(0x00);
		LCD_SpeedBar(0x00);
		LCD_RoomTemperature(0x00);
		LCD_AutoWind(0x00);
		LCD_TimeNumAndSign(-1, -1, 0, -1, -1);
		LCD_TempNumAndSign(-1, -1, 0, -1, 0);
		TimingTimePointer = 0;
		
		WorkState = SPEEL_STATE;
		
		TC_LCD_GUI_EventSend(SPEEL_EVENT);
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_TimeTaskEx()
*函数功能：执行与时间相关的任务
*入口参数：void
*出口参数：void
**************************************************/
void TC_LCD_GUI_TimeTaskEx()
{
	static xdata unsigned long old_time1;
	static xdata unsigned long old_time2;
	static xdata unsigned long old_time4;
	static xdata char flag = 0;
	long timingTime;
	
	//执行用户加载任务
	TC_LCD_GUI_CycleTaskEx();
	TC_LCD_GUI_TimedTaskEx();
	
	if(GUI_TIME > old_time1 + 30)
	{
		//更新时间
		if(SysTimeTempGet.TimeGet != 0)	
			Time = SysTimeTempGet.TimeGet();

		//更新室温
		if(MENU_STATE != TEMP_SET)	
		{
			if(SysTimeTempGet.RoomTemperatureGet != 0)
				RoomTemperature = SysTimeTempGet.RoomTemperatureGet();

			LCD_TempNumAndSign(RoomTemperature / 100 % 10,	//更新显示
			RoomTemperature / 10 % 10, 1, RoomTemperature % 10, 1);
		}
		
		old_time1 = GUI_TIME;
	}
	
	//显示闪烁
	if(WorkState == WORK_STATE)
	{
		if(GUI_TIME > old_time2 + 20 )
		{
			flag = !flag;
			if(MENU_STATE != TIMING_SET)
			{
				//时间：闪烁
				LCD_TimeNumAndSign(Time / 1000 % 10, Time / 100 % 10, flag, 
				Time / 10 % 10, Time % 10);
			}
			else
			{
				//定时时间设置闪烁指定位
				timingTime = TC_LCD_ConData.TimingTime;
		
				switch(TimingTimePointer)
				{
					case 0:
						LCD_TimeNumAndSign(flag ? TC_LCD_ConData.TimingTime / 1000 % 10 : -1, TC_LCD_ConData.TimingTime / 100 % 10, 1, 
							TC_LCD_ConData.TimingTime / 10 % 10, TC_LCD_ConData.TimingTime % 10);
					break;
					
					case 1:
						LCD_TimeNumAndSign(TC_LCD_ConData.TimingTime / 1000 % 10, flag ? TC_LCD_ConData.TimingTime / 100 % 10 : -1, 1, 
							TC_LCD_ConData.TimingTime / 10 % 10, TC_LCD_ConData.TimingTime % 10);
					break;
					
					case 2:
						LCD_TimeNumAndSign(TC_LCD_ConData.TimingTime / 1000 % 10, TC_LCD_ConData.TimingTime / 100 % 10, 1, 
							flag ? TC_LCD_ConData.TimingTime / 10 % 10 : -1, TC_LCD_ConData.TimingTime % 10);
					break;
					
					case 3:
						LCD_TimeNumAndSign(TC_LCD_ConData.TimingTime / 1000 % 10, TC_LCD_ConData.TimingTime / 100 % 10, 1, 
							TC_LCD_ConData.TimingTime / 10 % 10, flag ? TC_LCD_ConData.TimingTime % 10 : -1);
					break;
				}
			}
			//速度设置条闪烁
			if(MENU_STATE == SPEED_SET)
			{
				if(flag)
				{
					LCD_SpeedBar(TC_LCD_ConData.Speed);
				}
				else
				{
					LCD_SpeedBar(0x00);
				}
			}
			//模式按钮闪烁
			if(MENU_STATE == MODE_SET)
			{
				if(flag)
				{
					LCD_ConMode(TC_LCD_ConData.Mode);
				}
				else
				{
					LCD_ConMode(0x00);
				}
			}
			
			old_time2 = GUI_TIME;
		}
	}
	
	//定时计时
	if(TC_LCD_ConData.TimingTime > 0 && MENU_STATE != TIMING_SET)
	{
		if(old_time4 == 0)
		{
			old_time4 = GUI_TIME;
		}
		
		if(old_time4 + 100 <= GUI_TIME)
		{
			if(TC_LCD_ConData.TimingTime % 100 == 0)
			{
				TC_LCD_ConData.TimingTime = TC_LCD_ConData.TimingTime + 60 - 100;
			}
			TC_LCD_ConData.TimingTime --;
			TC_LCD_GUI_WriteIapData();	//记录定时时间
			
			if(TC_LCD_ConData.TimingTime == 0)
			{
				TC_LCD_GUI_EventSend(TIMING_OVERTIME_EVENT); //发送定时超时时间，触发相关任务
				old_time4 = 0;
			}
			
			old_time4 = GUI_TIME;
		}
	}
	
	//3s无操作退回
	if(RecoveryTime + 300 <= GUI_TIME && RecoveryTime > 0)
	{
		MENU_STATE = MAIN_MENU_SET;
		
		LCD_SpeedBar(0x00);
		LCD_ConMode(TC_LCD_ConData.Mode);
		LCD_TimeNumAndSign(Time / 1000 % 10, Time / 100 % 10, 1, 
			Time / 10 % 10, Time % 10);
		
		RecoveryTime = 0;
	}
	
	//8s无操作低功耗省电
	if(WorkTime + 1200 <= GUI_TIME && WorkState != 0 && WorkTime > 0)
	{
		TC_LCD_GUI_En(DOWM_POWR_STATE);
		
		WorkTime = 0;
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_MainMenuFun()
*函数功能：主菜单页面功能
*入口参数：key 按键键值
*出口参数：void
**************************************************/
void TC_LCD_GUI_MainMenuFun(unsigned char key)
{
	switch(key)
	{
		case KEY_TIMING:
			MENU_STATE = TIMING_SET;
			LCD_TimeNumAndSign(TC_LCD_ConData.TimingTime / 1000 % 10, TC_LCD_ConData.TimingTime / 100 % 10, 1, 
			TC_LCD_ConData.TimingTime / 10 % 10, TC_LCD_ConData.TimingTime % 10);
			break;
		case KEY_RIGHT:
		case KEY_LEFT:
			MENU_STATE = TEMP_SET;
			LCD_TempNumAndSign(TC_LCD_ConData.Temperature / 100 % 10,
			TC_LCD_ConData.Temperature / 10 % 10, 1, TC_LCD_ConData.Temperature % 10, 1);
			LCD_RoomTemperature(0x02);
			break;
		case KEY_MENU:
			MENU_STATE = MODE_SET;
			break;
		case KEY_SPEED:
			MENU_STATE = SPEED_SET;
			break;
	}
	
	RecoveryTime = GUI_TIME;
}

/**************************************************
*函数名称：void TC_LCD_GUI_ModeFun()
*函数功能：主菜单页面功能
*入口参数：key 按键键值
*出口参数：void
**************************************************/
void TC_LCD_GUI_ModeFun(unsigned char key)
{
	int i,flag = 0;
	switch(key)
	{
		case KEY_TIMING:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_RIGHT:
		case KEY_LEFT:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_MENU:
			for(i = 0; i < 4; i ++)
			{
				if(TC_LCD_ConData.Mode == 0x80 >> i)
					flag = 1;
			}
			if(flag != 1)
				TC_LCD_ConData.Mode = 0x80;
			
			if(TC_LCD_ConData.Mode > 0x10)
				TC_LCD_ConData.Mode = TC_LCD_ConData.Mode >> 1;
			else
				TC_LCD_ConData.Mode = 0x80;
			break;
		case KEY_SPEED:
			MENU_STATE = MAIN_MENU_SET;
			break;
	}
	TC_LCD_GUI_WriteIapData();
	LCD_ConMode(TC_LCD_ConData.Mode);
	
	RecoveryTime = GUI_TIME;
	
	TC_LCD_GUI_EventSend(MODE_CHANGE_EVENT);
}

/**************************************************
*函数名称：void TC_LCD_GUI_TempSetFun()
*函数功能：温度设置页面功能
*入口参数：key 按键键值
*出口参数：void
**************************************************/
void TC_LCD_GUI_TempSetFun(unsigned char key)
{
	switch(key)
	{
		case KEY_TIMING:
			MENU_STATE = TIMING_SET;
			break;
		case KEY_RIGHT:
			TC_LCD_ConData.Temperature += 5; 
			break;
		case KEY_LEFT:
			TC_LCD_ConData.Temperature -= 5; 
			break;
		case KEY_MENU:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_SPEED:
			MENU_STATE = MAIN_MENU_SET;
			break;
	}
	
	if(TC_LCD_ConData.Temperature < 0)
	{
		TC_LCD_ConData.Temperature = 0;
	}
	if(TC_LCD_ConData.Temperature > 995)
	{
		TC_LCD_ConData.Temperature = 995;
	}
	
	TC_LCD_GUI_WriteIapData();
	
	LCD_TempNumAndSign(TC_LCD_ConData.Temperature / 100 % 10,
			TC_LCD_ConData.Temperature / 10 % 10, 1, TC_LCD_ConData.Temperature % 10, 1);

	if(MENU_STATE != TEMP_SET)
		LCD_RoomTemperature(0x03);
	
	RecoveryTime = GUI_TIME;
	
	TC_LCD_GUI_EventSend(TEMP_CHANGE_EVENT);
}

/**************************************************
*函数名称：void TC_LCD_GUI_SpeedSetFun()
*函数功能：速度设置页面功能
*入口参数：key 按键键值
*出口参数：void
**************************************************/
void TC_LCD_GUI_SpeedSetFun(unsigned char key)
{
	switch(key)
	{
		case KEY_TIMING:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_RIGHT:
		case KEY_LEFT:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_MENU:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_SPEED:
			MENU_STATE = SPEED_SET;
			switch(TC_LCD_ConData.Speed)
			{
				case 0x00:
					TC_LCD_ConData.Speed = 0x20;
					break;
				case 0x20:
					TC_LCD_ConData.Speed = 0x60;
					break;
				case 0x60:
					TC_LCD_ConData.Speed = 0xe0;
					break;
				case 0xe0:
					TC_LCD_ConData.Speed = 0x00;
					break;
				default :TC_LCD_ConData.Speed = 0x00;
			}
			break;
	}
	TC_LCD_GUI_WriteIapData();	//载入EEPROM
	LCD_SpeedBar(TC_LCD_ConData.Speed);
	
	if(MENU_STATE != SPEED_SET)
		LCD_SpeedBar(0x00);
	
	RecoveryTime = GUI_TIME;	//更新时间，用于记录操作时间
	
	TC_LCD_GUI_EventSend(SPEED_CHANGE_EVENT);	//初始事件任务
}

/**************************************************
*函数名称：void TC_LCD_GUI_TimingSetFun()
*函数功能：定时设置页面功能
*入口参数：key 按键键值
*出口参数：void
**************************************************/
void TC_LCD_GUI_TimingSetFun(unsigned char key)
{
	
	switch(key)
	{
		case KEY_TIMING:
			TimingTimePointer ++;
			if(TimingTimePointer > 3)
			{
				MENU_STATE = MAIN_MENU_SET;
				TimingTimePointer = 0;
			}
			break;
		case KEY_RIGHT:
			if(TC_LCD_ConData.TimingTime + pow(10, 3 - TimingTimePointer) < 10000)
			{
				TC_LCD_ConData.TimingTime += pow(10, 3 - TimingTimePointer);
				if(TC_LCD_ConData.TimingTime % 100 >= 60)
				{
					TC_LCD_ConData.TimingTime = TC_LCD_ConData.TimingTime - 60 + 100;
				}
			}
			break;
		case KEY_LEFT:
			if(TC_LCD_ConData.TimingTime - pow(10, 3 - TimingTimePointer) > 0)
			{
				if(TC_LCD_ConData.TimingTime % 100 == 0)
				{
					TC_LCD_ConData.TimingTime = TC_LCD_ConData.TimingTime + 60 - 100;
				}
				
				TC_LCD_ConData.TimingTime -= pow(10, 3 - TimingTimePointer);
			}
			break;
		case KEY_MENU:
			MENU_STATE = MAIN_MENU_SET;
			break;
		case KEY_SPEED:
			MENU_STATE = MAIN_MENU_SET;
	}
	
	if(MENU_STATE != TIMING_SET)
		TimingTimePointer = 0;
	
	TC_LCD_GUI_WriteIapData();
	LCD_TimeNumAndSign(TC_LCD_ConData.TimingTime / 1000 % 10, TC_LCD_ConData.TimingTime / 100 % 10, 1, 
	TC_LCD_ConData.TimingTime / 10 % 10, TC_LCD_ConData.TimingTime % 10);

	RecoveryTime = GUI_TIME;
	
	TC_LCD_GUI_EventSend(TIMING_CHANGE_EVENT);
}

/**************************************************
*函数名称：void TC_LCD_GUI_Main()
*函数功能：TC_LCD_GUI主功能
*入口参数：key 按键键值
*出口参数：void
**************************************************/
void TC_LCD_GUI_Main()
{
	unsigned char key;

	TC_LCD_GUI_TimeTaskEx();
	key = TC_LCD_TK_Scan();
	TC_LCD_TK_KeyValueClear();
	
	if(key != 0)
	{
		WorkTime = GUI_TIME;
		
		if(WorkState != SPEEL_STATE)
		{
			switch(MENU_STATE)
			{
				case MAIN_MENU_SET:
					TC_LCD_GUI_MainMenuFun(key);
					break;
				case MODE_SET:
					TC_LCD_GUI_ModeFun(key);
					break;
				case TEMP_SET:
					TC_LCD_GUI_TempSetFun(key);
					break;
				case SPEED_SET:
					TC_LCD_GUI_SpeedSetFun(key);
					break;
				case TIMING_SET:
					TC_LCD_GUI_TimingSetFun(key);
					break;
			}
		}
		
		if(key == KEY_SWITCH)
		{
			if(WorkState != WORK_STATE)
				TC_LCD_GUI_En(WORK_STATE);
			else
				TC_LCD_GUI_En(SPEEL_STATE);
		}
		else
		{
			if(WorkState == DOWM_POWR_STATE)
				TC_LCD_GUI_En(WORK_STATE);
		}
	}
	
}

/**************************************************
*函数名称：void TC_LCD_GUI_EventTaskLoad()
*函数功能：事件任务加载
*入口参数：task 任务 event 事件
*出口参数：void
**************************************************/
void TC_LCD_GUI_EventTaskLoad(void *task, char event)
{
	int i;
	for(i = 0; i < sizeof(EventTask) / sizeof(TC_LCD_GUI_EVENT_TASK); i ++)
	{
		if(EventTask[i].task == (void *)0)
		{
			EventTask[i].task = task;
			EventTask[i].event = event;
			
			break;
		}
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_TimedTaskLoad()
*函数功能：定时任务加载
*入口参数：task 任务 time 时间
*出口参数：void
**************************************************/
void TC_LCD_GUI_TimedTaskLoad(void *task, unsigned int time)
{
	int i;
	for(i = 0; i < sizeof(TimedTask) / sizeof(TC_LCD_GUI_TIMED_TASK); i ++)
	{
		if(TimedTask[i].task == (void *)0)
		{
			TimedTask[i].task = task;
			TimedTask[i].time = time;
			
			break;
		}
	}
}

/**************************************************
*函数名称：void TC_LCD_GUI_CycleTaskLoad()
*函数功能：定时任务加载
*入口参数：task 任务 time 时间
*出口参数：void
**************************************************/
void TC_LCD_GUI_CycleTaskLoad(void *task, unsigned int time)
{
	int i;
	for(i = 0; i < sizeof(CycleTask) / sizeof(TC_LCD_GUI_CYCLE_TASK); i ++)
	{
		if(CycleTask[i].task == (void *)0)
		{
			CycleTask[i].task = task;
			CycleTask[i].time = time;
			CycleTask[i].lastTime = GUI_TIME;
			break;
		}
	}
}


/**************************************************
*函数名称：void TC_LCD_GUI_DataInit()
*函数功能：数据初始化，没有初始化则往EEPROM中载入数据
*入口参数：void
*出口参数：void
**************************************************/
void TC_LCD_GUI_DataInit()
{
	TC_LCD_GUI_GetIapData(); //获取EEPROM中的数据记录
	if(TC_LCD_ConData.DefaultSetFlag != 0x5555)
	{
		TC_LCD_ConData.DefaultSetFlag = 0x5555;
		TC_LCD_ConData.Speed = 0x20;
		TC_LCD_ConData.Mode = 0x80;
		TC_LCD_ConData.Temperature = 300;
		TC_LCD_ConData.TimingTime = 0;
		TC_LCD_ConData.TimingStart = 0;
		
		TC_LCD_GUI_WriteIapData();
	}
	
	MENU_STATE = MAIN_MENU_SET;
}

/**************************************************
*函数名称：void TC_LCD_GUI_Timer0Init()
*函数功能：定时器初始化,10ms,作为该驱动的工作时钟
*入口参数：void
*出口参数：void
**************************************************/
void TC_LCD_GUI_Timer0Init()
{
	TIM0_TimeBaseInit(TIM0_PRESSEL_FSYS_D12,TIM0_MODE_TIMER);	//Timer0 12分频,做定时器
	TIM0_WorkMode1Config(0xffff - 26667);						//设置Timer0工作模式0，初值为4192，定时3ms
	TIM0_ITConfig(ENABLE, LOW);						//使能Timer0中断
	TIM0_Cmd(ENABLE);								//使能Timer0计数
}

/**************************************************
*函数名称：void TC_LCD_GUI_TimerCount()
*函数功能：时间计数由定时器中断调用
*入口参数：void
*出口参数：void
**************************************************/
void TC_LCD_GUI_TimerCount()
{
	TIM0_Mode1SetReloadCounter(0xffff - 26667);
	GUI_TIME ++;
}

/**************************************************
*函数名称：void TC_LCD_GUI_Init()
*函数功能：初始化
*入口参数：void
*出口参数：void
**************************************************/
void TC_LCD_GUI_Init()
{
	TC_LCD_TK_Init();
	TC_LCD_GUI_DataInit();
	TC_LCD_GUI_Timer0Init();
	TC_LCD_GUI_En(SPEEL_STATE);
}