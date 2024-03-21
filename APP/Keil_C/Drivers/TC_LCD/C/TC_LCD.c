#include "..\H\TC_LCD.h"
#include "SC_Init.h"

extern unsigned char LCDRAM[];
//LCD显示码表
static const unsigned char Lcd_Code[] =
{
	245,96,182,242,		//0   0123 
	99,211,215,112,  	//4   4567
	247,243,119,		//8   89a
	199,149,230,151,	//11  bcde
	23,103,70,55,	//15  fhnp	
	227,2, 133, 198,		//19  y-Lo
  135,
};

/*****************************************************
*函数名称: void LCD_BLSet(char state)
*函数功能: 背光控制
*入口参数：bit 7 主背光灯 bit 6次背光灯
*出口参数：void
*****************************************************/
void LCD_BLSet(char state)
{
	GPIO_Init(BL_M_INIT / 0xff, BL_M_INIT % 0xff,GPIO_MODE_OUT_PP);
	GPIO_Init(BL_0_INIT / 0xff, BL_0_INIT % 0xff,GPIO_MODE_OUT_PP);
	
	if(state & 0x80)
		BL_M = 1;
	else
		BL_M = 0;
	
	if(state & 0x40)
		BL_0 = 1;
	else
		BL_0 = 0;
}

/*****************************************************
*函数名称: LCD_SpeedBar
*函数功能: 速度条设置
*入口参数：char bit7 速度条3级 bit6 速度条2级 bit5 速度条1级
*出口参数：char 返回输入值
*****************************************************/
char LCD_SpeedBar(char state)
{
	if(state & (0x01 << 7))
		LCDRAM[5] |= 0x01 << 7;
	else
		LCDRAM[5] &= ~(0x01 << 7);
	
	if(state & (0x01 << 6))
		LCDRAM[1] |= 0x01 << 7;
	else
		LCDRAM[1] &= ~(0x01 << 7);

	if(state & (0x01 << 5))
		LCDRAM[10] |= 0x01 << 7;
	else
		LCDRAM[10] &= ~(0x01 << 7);
	return state;
}


/*****************************************************
*函数名称: LCD_SelectMenu
*函数功能: 选择菜单
*入口参数：char bit7 时间菜单 bit6 M菜单 bit5 左 bit4右 bit3 速度 bit2 中心按键
*出口参数：char 返回输入值
*****************************************************/
void LCD_SelectMenu(char state)
{
	if(state & (0x01 << 7))
		LCDRAM[12] |= 0x80;
	else
		LCDRAM[12] &= ~0x80;
	
	if(state & (0x01 << 6))
		LCDRAM[16] |= 0x80;
	else
		LCDRAM[16] &= ~0x80;
	
	if(state & (0x01 << 5))
		LCDRAM[20] |= 0x40;
	else
		LCDRAM[20] &= ~0x40;
	
	if(state & (0x01 << 4))
		LCDRAM[20] |= 0x20;
	else
		LCDRAM[20] &= ~0x20;
	
	if(state & (0x01 << 3))
		LCDRAM[20] |= 0x10;
	else
		LCDRAM[20] &= ~0x10;
	
	if(state & (0x01 << 2))
		LCDRAM[20] |= 0x80;
	else
		LCDRAM[20] &= ~0x80;
}

/*****************************************************
*函数名称: LCD_ConMode
*函数功能: 设置温控器mode选项显示
*入口参数：charbit7 自动 bit6 冷 bit5 热 bit4特殊功能
*出口参数：char 返回输入值
*****************************************************/ 
char LCD_ConMode(char state)
{
	LCDRAM[19] = state;
	return state;
}


/*****************************************************
*函数名称: LCD_ConMode
*函数功能: 设置时间状态
*入口参数：char bit7 时间状态1 bit6时间状态2 bit5时间状态3 bit4时间状态4
*出口参数：char 返回输入值
*****************************************************/ 
char LCD_TimeState(char state)
{
	
	LCDRAM[18] = state;
	
	return state;
}

/*****************************************************
*函数名称: LCD_TimeNumAndSign
*函数功能: 设置时间
*入口参数：char num0 第一位数字 num1第二位数字 sign时间： num2第三位数字 num3第四位数字
*出口参数：char 返回输入值
*****************************************************/
void LCD_TimeNumAndSign(char num0, char num1, char sign, char num2, char num3)
{
	if(sign)
		LCDRAM[14] |= 0x01 << 7;
	else
		LCDRAM[14] &= ~(0x01 << 7);
	
	if(num0 >= 0)
	{
		LCDRAM[10] = (LCDRAM[10] & ~0x70) | 0x70 & Lcd_Code[num0] << 4;
		LCDRAM[11] = Lcd_Code[num0];
	}
	else
	{
		LCDRAM[10] &= ~0x70;
		LCDRAM[11] &= 0x0f;
	}
	
	if(num1 >= 0)
	{
		LCDRAM[12] = (LCDRAM[12] & ~0x70) | 0x70 & Lcd_Code[num1] << 4;
		LCDRAM[13] = Lcd_Code[num1];
	}
	else
	{
		LCDRAM[12] &= ~0x70;
		LCDRAM[13] &= 0x0f;
	}
	
	if(num2 >= 0)
	{
		LCDRAM[14] = (LCDRAM[14] & ~0x70) | 0x70 & Lcd_Code[num2] << 4;
		LCDRAM[15] = Lcd_Code[num2];
	}
	else
	{
		LCDRAM[14] &= ~0x70;
		LCDRAM[15] &= 0x0f;
	}
	
	if(num3 >= 0)
	{
		LCDRAM[16] = (LCDRAM[16] & ~0x70) | 0x70 & Lcd_Code[num3] << 4;
		LCDRAM[17] = Lcd_Code[num3];
	}
	else
	{
		LCDRAM[16] &= ~0x70;
		LCDRAM[17] &= 0x0f;
	}
}


/*****************************************************
*函数名称: LCD_Week
*函数功能: 设置星期显示
*入口参数：char bit7 星期一 ~bit1 星期日
*出口参数：char 返回输入值
*****************************************************/
char LCD_Week(char week)
{
	int i;
	for(i = 0; i < 7; i ++)
	{
		if(week & (0x80 >> i))
		{
			if(i < 4)
			{
				LCDRAM[7] = (LCDRAM[7] & ~(0x10 << i)) | (0x10 << i);
			}
			else
			{
				LCDRAM[8] = (LCDRAM[8] & ~(0x10 << i)) | (0x10 << (6 - i));
			}
		}
		else
		{
			if(i < 4)
			{
				LCDRAM[7] = (LCDRAM[7] & ~(0x10 << i)) & ~(0x10 << i);
			}
			else
			{
				LCDRAM[8] = (LCDRAM[8] & ~(0x10 << i)) & ~(0x10 << (6 - i));
			}
		}
	}
	return week;
}


/*****************************************************
*函数名称: LCD_TimeNumAndSign
*函数功能: 设置温度
*入口参数：char num0 第一位数字 num1第二位数字 sign小数点 num2第三位数字 num3 °c符号
*出口参数：char 返回输入值
*****************************************************/
void LCD_TempNumAndSign(char num0, char num1, char sign_num, char num2,  char sign_temp)
{
	if(sign_temp)
	{
		LCDRAM[8] |= 0x80;
	}
	else
	{
		LCDRAM[8] &= ~0x80;
	}
	
	if(sign_num)
	{
		LCDRAM[3] |= 0x01 << 7; //小数点显示
	}
	else
	{
		LCDRAM[3] &= ~(0x01 << 7); //小数点hide

	}
	
	if(num0 >= 0)
	{
		LCDRAM[1] = (LCDRAM[1] & ~0x70) | 0x70 & Lcd_Code[num0] << 4;
		LCDRAM[2] = Lcd_Code[num0];
	}
	else
	{
		LCDRAM[1] &= ~0x70;
		LCDRAM[2] &= 0x0f;
	}
	
	if(num1 >= 0)
	{
		LCDRAM[3] = (LCDRAM[3] & ~0x70) | 0x70 & Lcd_Code[num1] << 4;
		LCDRAM[4] = Lcd_Code[num1];
	}
	else
	{
		LCDRAM[3] &= ~0x70;
		LCDRAM[4] &= 0x0f;
	}
	
	if(num2 >= 0)
	{
		LCDRAM[5] = (LCDRAM[5] & ~0x70) | 0x70 & Lcd_Code[num2] << 4;
		LCDRAM[6] = Lcd_Code[num2];
	}
	else
	{
		LCDRAM[5] &= ~0x70;
		LCDRAM[6] &= 0x0f;
	}
}	

/*****************************************************
*函数名称: LCD_AutoWind
*函数功能: 设置自动节能显示
*入口参数：char 非0开
*出口参数：char 返回输入值
*****************************************************/
char LCD_AutoWind(char state)
{
	if(state)
		LCDRAM[0] |= 0x01 << 6;
	else
		LCDRAM[0] &= ~(0x01 << 6);
	return state;
}

/*****************************************************
*函数名称: LCD_RoomTemperature
*函数功能: 设置温度显示和室内温度显示
*入口参数：char bit1 室 bit2 温度
*出口参数：char 返回输入值
*****************************************************/
char LCD_RoomTemperature(char state)
{
	if(state & 0x01)
		LCDRAM[0] |= 0x01 << 4;
	else
		LCDRAM[0] &= ~(0x01 << 4);
	if(state & 0x02)
		LCDRAM[0] |= 0x01 << 5;
	else
		LCDRAM[0] &= ~(0x01 << 5);	
	
	return state;
}

/*****************************************************
*函数名称: LCD_Lock
*函数功能: 设置锁显示
*入口参数：非0显示
*出口参数：char 返回输入值
*****************************************************/
char LCD_Lock(char state)
{
	if(state)
		LCDRAM[0] |= 0x01 << 7;
	else
		LCDRAM[0] &= ~(0x01 << 7);
	return state;
}
