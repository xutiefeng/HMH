#ifndef _TC_LCD_H_
#define _TC_LCD_H_
//<<<Use SPTML>>>
#define BL_M P50//BL_M 
#define BL_M_INIT 0x0501//BL_M_INIT 
#define BL_0 P51//BL_0 
#define BL_0_INIT 0x0502//BL_0_INIT 
extern char LCD_AutoWind(char state);
extern char LCD_RoomTemperature(char state);
extern char LCD_SpeedBar(char state);
extern void LCD_BLSet(char state);
extern char LCD_Lock(char state);
extern void LCD_TempNumAndSign(char num0,char num1,char sign_num,char num2,char sign_temp);
extern char LCD_TimeState(char state);
extern void LCD_SelectMenu(char state);
extern void LCD_TimeNumAndSign(char num0,char num1,char sign,char num2,char num3);
extern char LCD_Week(char week);
extern char LCD_ConMode(char state);
//<<<end of SPTML>>>
#endif
