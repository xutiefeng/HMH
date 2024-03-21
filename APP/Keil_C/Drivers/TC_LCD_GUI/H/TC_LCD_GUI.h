#ifndef _TC_LCD_GUI_H_
#define _TC_LCD_GUI_H_
//<<<Use SPTML>>>
#define invalid P00//invalid 
#define invalid_INIT 0x0001//invalid_INIT 
extern void  TC_LCD_GUI_Main();
extern void  TC_LCD_GUI_CycleTaskLoad(void * task,unsigned int time);
extern void TC_LCD_GUI_TimeGetLoad(void * fun);
extern void  TC_LCD_GUI_TimerCount();
extern void TC_LCD_GUI_TimedTaskLoad(void * task,unsigned int time);
extern void  TC_LCD_GUI_RoomTempGetLoad(void * fun);
extern void TC_LCD_GUI_Init();
extern void  TC_LCD_GUI_EventTaskLoad(void * task,char event);
//<<<end of SPTML>>>
#endif
