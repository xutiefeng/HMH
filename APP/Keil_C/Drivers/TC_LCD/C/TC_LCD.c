#include "..\H\TC_LCD.h"
#include "SC_Init.h"

extern unsigned char LCDRAM[];
//LCD��ʾ���
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
*��������: void LCD_BLSet(char state)
*��������: �������
*��ڲ�����bit 7 ������� bit 6�α����
*���ڲ�����void
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
*��������: LCD_SpeedBar
*��������: �ٶ�������
*��ڲ�����char bit7 �ٶ���3�� bit6 �ٶ���2�� bit5 �ٶ���1��
*���ڲ�����char ��������ֵ
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
*��������: LCD_SelectMenu
*��������: ѡ��˵�
*��ڲ�����char bit7 ʱ��˵� bit6 M�˵� bit5 �� bit4�� bit3 �ٶ� bit2 ���İ���
*���ڲ�����char ��������ֵ
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
*��������: LCD_ConMode
*��������: �����¿���modeѡ����ʾ
*��ڲ�����charbit7 �Զ� bit6 �� bit5 �� bit4���⹦��
*���ڲ�����char ��������ֵ
*****************************************************/ 
char LCD_ConMode(char state)
{
	LCDRAM[19] = state;
	return state;
}


/*****************************************************
*��������: LCD_ConMode
*��������: ����ʱ��״̬
*��ڲ�����char bit7 ʱ��״̬1 bit6ʱ��״̬2 bit5ʱ��״̬3 bit4ʱ��״̬4
*���ڲ�����char ��������ֵ
*****************************************************/ 
char LCD_TimeState(char state)
{
	
	LCDRAM[18] = state;
	
	return state;
}

/*****************************************************
*��������: LCD_TimeNumAndSign
*��������: ����ʱ��
*��ڲ�����char num0 ��һλ���� num1�ڶ�λ���� signʱ�䣺 num2����λ���� num3����λ����
*���ڲ�����char ��������ֵ
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
*��������: LCD_Week
*��������: ����������ʾ
*��ڲ�����char bit7 ����һ ~bit1 ������
*���ڲ�����char ��������ֵ
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
*��������: LCD_TimeNumAndSign
*��������: �����¶�
*��ڲ�����char num0 ��һλ���� num1�ڶ�λ���� signС���� num2����λ���� num3 ��c����
*���ڲ�����char ��������ֵ
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
		LCDRAM[3] |= 0x01 << 7; //С������ʾ
	}
	else
	{
		LCDRAM[3] &= ~(0x01 << 7); //С����hide

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
*��������: LCD_AutoWind
*��������: �����Զ�������ʾ
*��ڲ�����char ��0��
*���ڲ�����char ��������ֵ
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
*��������: LCD_RoomTemperature
*��������: �����¶���ʾ�������¶���ʾ
*��ڲ�����char bit1 �� bit2 �¶�
*���ڲ�����char ��������ֵ
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
*��������: LCD_Lock
*��������: ��������ʾ
*��ڲ�������0��ʾ
*���ڲ�����char ��������ֵ
*****************************************************/
char LCD_Lock(char state)
{
	if(state)
		LCDRAM[0] |= 0x01 << 7;
	else
		LCDRAM[0] &= ~(0x01 << 7);
	return state;
}
