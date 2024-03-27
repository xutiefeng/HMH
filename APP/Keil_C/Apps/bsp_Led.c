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

	
void LED_Process(void)
{

	
//	if(SysRunFlag)
	//	return;
	
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
