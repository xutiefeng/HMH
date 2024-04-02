#include "..\Apps\bsp_Led.h"
#include "..\Apps\includeall.h"


void RedLed(u8 x)
{
	
}

void LED_sCan(void)
{	
	if(gbFlagData[3].all ==0 )
	{
		Chanel_IO1 = 0;
		Chanel_IO2 = 0;
		
		return;
	}
	led1_io = 1;
	led2_io = 1;
	led3_io = 1;
	led4_io = 1;
	if(LedChangeFlag)
	{
		LedChangeFlag = 0;
		Chanel_IO1 = 1;
		Chanel_IO2 = 0;
		if(LED1_L)
		{
			led1_io = 0;
		}
		
		if(LED2_L)
		{
			led2_io = 0;
		}

		if(LED3_L)
		{
			led3_io = 0;
		}
		
		if(LED4_L)
		{
			led4_io = 0;
		}
	}
	else
	{
		LedChangeFlag = 1;

		Chanel_IO1 = 0;
		Chanel_IO2 = 1;
		
	
		if(LED1_R)
		{
			led1_io = 0;
		}
		if(LED2_R)
		{
			led2_io = 0;
		}
		
		 if(LED3_R)
		{
			led3_io = 0;
		}
		
		 if(LED4_R)
		{
			led4_io = 0;
		}
	}	
}

typedef struct
{
	  union
		{
				u8 all;
				struct
				{
					u8 lednum :3;
					u8 time		:5;
				}Bit;
		}run;
		u8 lednum;//哪个灯
		emColor color;	//什么颜色
		U_LED action;//需要执行什么状态，常亮？或者闪烁	
}ST_LED;

ST_LED gstLed;

void setLED(u8 num,emColor color ,U_LED state,u8 time)
{
		if(gstLed.action == blink)
			return;
		
		gstLed.run.Bit.lednum = num-1;
		gstLed.run.Bit.time   = time;
		gstLed.color =  color;
		gstLed.action = state;
}

	
void LED_Process(void)
{

	u8 temp_bit = 0;
	u8 temp_bit2;
	
	
	
	if(gstLed.action == blink)
	{
			
		if(gstLed.run.Bit.time > 0)
		{
				gstLed.run.Bit.time--;
				
				if(gstLed.run.Bit.time %5 == 0)
				{
					if(gstLed.color == BlueColor)
					{
						temp_bit =1<<(gstLed.run.Bit.lednum+4);
					}
					else
					{
						temp_bit =1<<(gstLed.run.Bit.lednum);
					}
				
					 if(gbFlagData[3].all && temp_bit)
					 {
						gbFlagData[3].all &=~temp_bit;
					 }
					else
				  	{
						gbFlagData[3].all |=temp_bit;
					}			 
				}
		}			
		
			
		
	}
	else if( gstLed.action == lightOn)
	{
		if(gstLed.color == BlueColor)
		{
			temp_bit2 =1<<(gstLed.run.Bit.lednum+4);
			temp_bit = 1<<(gstLed.run.Bit.lednum);
		}
		else
		{
			temp_bit2 =	1<<(gstLed.run.Bit.lednum);
			temp_bit = 1<<(gstLed.run.Bit.lednum+4);
			
		}
		gbFlagData[3].all |=temp_bit2;
		//gbFlagData[3].all &=~temp_bit;
	}

	else if( gstLed.action == lightOff)
	{
		if(gstLed.color == BlueColor)
		{
			temp_bit2 =1<<(gstLed.run.Bit.lednum+4);
			temp_bit = 1<<(gstLed.run.Bit.lednum);
		}
		else
		{
			temp_bit2 =	1<<(gstLed.run.Bit.lednum);
			temp_bit = 1<<(gstLed.run.Bit.lednum+4);
			
		}
		gbFlagData[3].all &=~temp_bit2;
	}
	
	if(BlueLedFlag)
	{
		Blueled_io = 1;
	}
	else
	{
		Blueled_io = 0;
	}
	
	if(RedLedtFlag)
	{
		Redled_io = 1;
	}
	else
	{
		Redled_io = 0;
	}
		
}
