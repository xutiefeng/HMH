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
*�������ƣ�void TC_LCD_GUI_GetIapData(void)
*�������ܣ���EEPROM�м�������
*��ڲ�����void	 
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_WriteIapData(void)
*�������ܣ������ݼ��ص�EEPROM
*��ڲ�����void	 
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_TimeGetLoad(void *fun)
*�������ܣ�ʱ���ȡ��������
*��ڲ�����void *fun	 
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_TimeGetLoad(void *fun)
{
	SysTimeTempGet.TimeGet = fun;
}

/**************************************************
*�������ƣ�void TC_LCD_GUI_RoomTempGetLoad(void *fun)
*�������ܣ��¶Ȼ�ȡ��������
*��ڲ�����void *fun	 
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_RoomTempGetLoad(void *fun)
{
	SysTimeTempGet.RoomTemperatureGet = fun;
}

/**************************************************
*�������ƣ�void TC_LCD_GUI_EventSend(TC_LCD_GUI_EVENT event)
*�������ܣ������¼��������¼�����
*��ڲ�����void	 
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_TimedTaskEx()
*�������ܣ�ִ�ж�ʱ����
*��ڲ�����void	 
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_CycleTaskEx()
*�������ܣ�ִ����������
*��ڲ�����void	 
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_En()
*�������ܣ�LCD��ʾ����
*��ڲ�����state �� ֧�ֹ���ģʽ���͹���ģʽ��˯��ģʽ	 
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_TimeTaskEx()
*�������ܣ�ִ����ʱ����ص�����
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_TimeTaskEx()
{
	static xdata unsigned long old_time1;
	static xdata unsigned long old_time2;
	static xdata unsigned long old_time4;
	static xdata char flag = 0;
	long timingTime;
	
	//ִ���û���������
	TC_LCD_GUI_CycleTaskEx();
	TC_LCD_GUI_TimedTaskEx();
	
	if(GUI_TIME > old_time1 + 30)
	{
		//����ʱ��
		if(SysTimeTempGet.TimeGet != 0)	
			Time = SysTimeTempGet.TimeGet();

		//��������
		if(MENU_STATE != TEMP_SET)	
		{
			if(SysTimeTempGet.RoomTemperatureGet != 0)
				RoomTemperature = SysTimeTempGet.RoomTemperatureGet();

			LCD_TempNumAndSign(RoomTemperature / 100 % 10,	//������ʾ
			RoomTemperature / 10 % 10, 1, RoomTemperature % 10, 1);
		}
		
		old_time1 = GUI_TIME;
	}
	
	//��ʾ��˸
	if(WorkState == WORK_STATE)
	{
		if(GUI_TIME > old_time2 + 20 )
		{
			flag = !flag;
			if(MENU_STATE != TIMING_SET)
			{
				//ʱ�䣺��˸
				LCD_TimeNumAndSign(Time / 1000 % 10, Time / 100 % 10, flag, 
				Time / 10 % 10, Time % 10);
			}
			else
			{
				//��ʱʱ��������˸ָ��λ
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
			//�ٶ���������˸
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
			//ģʽ��ť��˸
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
	
	//��ʱ��ʱ
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
			TC_LCD_GUI_WriteIapData();	//��¼��ʱʱ��
			
			if(TC_LCD_ConData.TimingTime == 0)
			{
				TC_LCD_GUI_EventSend(TIMING_OVERTIME_EVENT); //���Ͷ�ʱ��ʱʱ�䣬�����������
				old_time4 = 0;
			}
			
			old_time4 = GUI_TIME;
		}
	}
	
	//3s�޲����˻�
	if(RecoveryTime + 300 <= GUI_TIME && RecoveryTime > 0)
	{
		MENU_STATE = MAIN_MENU_SET;
		
		LCD_SpeedBar(0x00);
		LCD_ConMode(TC_LCD_ConData.Mode);
		LCD_TimeNumAndSign(Time / 1000 % 10, Time / 100 % 10, 1, 
			Time / 10 % 10, Time % 10);
		
		RecoveryTime = 0;
	}
	
	//8s�޲����͹���ʡ��
	if(WorkTime + 1200 <= GUI_TIME && WorkState != 0 && WorkTime > 0)
	{
		TC_LCD_GUI_En(DOWM_POWR_STATE);
		
		WorkTime = 0;
	}
}

/**************************************************
*�������ƣ�void TC_LCD_GUI_MainMenuFun()
*�������ܣ����˵�ҳ�湦��
*��ڲ�����key ������ֵ
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_ModeFun()
*�������ܣ����˵�ҳ�湦��
*��ڲ�����key ������ֵ
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_TempSetFun()
*�������ܣ��¶�����ҳ�湦��
*��ڲ�����key ������ֵ
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_SpeedSetFun()
*�������ܣ��ٶ�����ҳ�湦��
*��ڲ�����key ������ֵ
*���ڲ�����void
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
	TC_LCD_GUI_WriteIapData();	//����EEPROM
	LCD_SpeedBar(TC_LCD_ConData.Speed);
	
	if(MENU_STATE != SPEED_SET)
		LCD_SpeedBar(0x00);
	
	RecoveryTime = GUI_TIME;	//����ʱ�䣬���ڼ�¼����ʱ��
	
	TC_LCD_GUI_EventSend(SPEED_CHANGE_EVENT);	//��ʼ�¼�����
}

/**************************************************
*�������ƣ�void TC_LCD_GUI_TimingSetFun()
*�������ܣ���ʱ����ҳ�湦��
*��ڲ�����key ������ֵ
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_Main()
*�������ܣ�TC_LCD_GUI������
*��ڲ�����key ������ֵ
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_EventTaskLoad()
*�������ܣ��¼��������
*��ڲ�����task ���� event �¼�
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_TimedTaskLoad()
*�������ܣ���ʱ�������
*��ڲ�����task ���� time ʱ��
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_CycleTaskLoad()
*�������ܣ���ʱ�������
*��ڲ�����task ���� time ʱ��
*���ڲ�����void
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
*�������ƣ�void TC_LCD_GUI_DataInit()
*�������ܣ����ݳ�ʼ����û�г�ʼ������EEPROM����������
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_DataInit()
{
	TC_LCD_GUI_GetIapData(); //��ȡEEPROM�е����ݼ�¼
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
*�������ƣ�void TC_LCD_GUI_Timer0Init()
*�������ܣ���ʱ����ʼ��,10ms,��Ϊ�������Ĺ���ʱ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_Timer0Init()
{
	TIM0_TimeBaseInit(TIM0_PRESSEL_FSYS_D12,TIM0_MODE_TIMER);	//Timer0 12��Ƶ,����ʱ��
	TIM0_WorkMode1Config(0xffff - 26667);						//����Timer0����ģʽ0����ֵΪ4192����ʱ3ms
	TIM0_ITConfig(ENABLE, LOW);						//ʹ��Timer0�ж�
	TIM0_Cmd(ENABLE);								//ʹ��Timer0����
}

/**************************************************
*�������ƣ�void TC_LCD_GUI_TimerCount()
*�������ܣ�ʱ������ɶ�ʱ���жϵ���
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_TimerCount()
{
	TIM0_Mode1SetReloadCounter(0xffff - 26667);
	GUI_TIME ++;
}

/**************************************************
*�������ƣ�void TC_LCD_GUI_Init()
*�������ܣ���ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TC_LCD_GUI_Init()
{
	TC_LCD_TK_Init();
	TC_LCD_GUI_DataInit();
	TC_LCD_GUI_Timer0Init();
	TC_LCD_GUI_En(SPEEL_STATE);
}