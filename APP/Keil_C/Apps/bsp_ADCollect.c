#include "globe.h"
#include  "bsp_ADCollect.h"



#define AD_TO_SELECT_TOP  {}//sADcnt= 0; \
                          //ADC0_ChannelSelect(ADC_POSINPUT_AIN0,ADC_NEGINPUT_AIN0)


#define ADCHANAL_GetValue()   ADC_GetConversionValue()
#define ADCHANAL_Select(X)    ADC_ChannelConfig(ADC_CHANNEL_##X,ENABLE);ADC_StartConversion();//ADC0_ChannelSelect(ADC_POSINPUT_AIN##X,ADC_NEGINPUT_AIN##X); 
	
	
#define AD_COLLECT_ENABLE  {}//ADC0_Enable() 
#define AD_COLLECT_DIASBLE {}//ADC0_Disable()	
	
#define AD_CallBACK        {}//ADC0_CallbackRegister(ADC0_CallBack, adc_context)
#define AD_StartConver     {}//ADC0_ConversionStart()
	
#define ADXiShu 0.056	

	
 
/************************************************************************* 
* 函数名称:	sysRuning
* 功能说明:	(gAD_Value[0]/4096)*5.2*10 = 52/4096 = 0.0126953125
* 功能说明:	
* 输    入: 无	
* 输    出: 无
*************************************************************************/


static u8 sADCnt,gTempAD_Cnt;


//unsigned int adc_sum = 0;

#define ADAverage_CH_Num ADMax

typedef struct
{
		u16 adc_value_max[ADAverage_CH_Num];
		u16 adc_value_min[ADAverage_CH_Num];
		u32 adc_sum[ADAverage_CH_Num];

}ST_AD_Aversage;
 
ST_AD_Aversage  gstAD_Aversage;



unsigned int adc_get_average(unsigned short ch,BSP_ADChangeEnum num)            
{
  
  u8 i;
  unsigned int  adc_current = 0;
 
  
  if(gTempAD_Cnt < 10*ADAverage_CH_Num)//for(gTempAD_Cnt = 0;gTempAD_Cnt<10;gTempAD_Cnt++)
  {
		gTempAD_Cnt++;
    adc_current = ch;
    if(adc_current > gstAD_Aversage.adc_value_max[num])
    {
      gstAD_Aversage.adc_value_max[num] = adc_current;
    }
    if(adc_current < gstAD_Aversage.adc_value_min[num])
    {
      gstAD_Aversage.adc_value_min[num] = adc_current;
    }
    gstAD_Aversage.adc_sum[num] += adc_current;
  }
	else
	{
			for(i= 0; i < ADAverage_CH_Num;i++)
			{
					gstAD_Aversage.adc_sum[i] -=  gstAD_Aversage.adc_value_min[i];
					gstAD_Aversage.adc_sum[i] -=  gstAD_Aversage.adc_value_max[i];
					gstAD_Aversage.adc_sum[i] = gstAD_Aversage.adc_sum[i]>>3;	
					adc_current = gstAD_Aversage.adc_sum[i];
					gstAD_Aversage.adc_sum[i] = 0;
					gstAD_Aversage.adc_value_min[i] = 0XFFFFUL;
					gstAD_Aversage.adc_value_max[i] = 0;
					gTempAD_Cnt = 0;
			}
	}
  return adc_current;
}





void ChunShuiTDS(void)
{
	
}

void JieShuiTDS(void)
{
		
}

void YuanShui_IO_Flip(void)
{
		//P04 = !P04;
		//P02 = !P02;
}

void ChunShui_IO_Flip(void)
{
		P14 = !P14;
		P25 = !P25;
}

void JieShui_IO_Flip(void)
{
	//	P26 = !P26;
	//	P27 = !P27;
}


void LouShui_IO_Flip(void)
{
	//	P00 = !P00;
	//	P01 = !P01;
}

void ADCollectProcess(void)
{
	static u8 sADChanalCnt = 0;

	sADChanalCnt++;
#if 1
	switch(sADChanalCnt)
	{
		case 1:
			 ChunShui_IO_Flip();
		break;
		
		case 2:
			  ADCHANAL_Select(1);
		break;
		
		case 3:
				gstADCollect.ChunShui = ADCHANAL_GetValue();		
		break;
		
		
		case 8:
			ChunShui_IO_Flip();
		break;
		
		
#if 0	
		case 4:
				JieShui_IO_Flip();
		break;
		
		case 5:
				 ADCHANAL_Select(3);
		break;
		
		case 6:
			
		break;
				gstADCollect.JieShui = ADCHANAL_GetValue();
				JieShui_IO_Flip();
		case 7:
				LouShui_IO_Flip();
		break;
		
		case 8:
				ADCHANAL_Select(6);
		break;
		
		case 9:
				gstADCollect.LouShui = ADCHANAL_GetValue();// YuanShui
				LouShui_IO_Flip();
		break;
		
		case 10:
				YuanShui_IO_Flip();
		break;
		
		case 11:
				ADCHANAL_Select(7);
		break;
		
		case 12:
				gstADCollect.YuanShui = ADCHANAL_GetValue();// YuanShui
				YuanShui_IO_Flip();
		break;
		
#endif		
		
		default:
			if(sADChanalCnt >= 10)
				sADChanalCnt = 0;
			break;
	}
#elif 1
	if( == 1)
	{
			gstADCollect.YuanShui = ADCHANAL_GetValue();
		  gstADCollect.YuanShui  *= 0.00122;
		  ChunShui_IO_Flip();
			ADCHANAL_Select(1);
			
			sADChanalCnt = 3;
	}
	
	else if(sADChanalCnt == 3)
	{
			gstADCollect.ChunShui = ADCHANAL_GetValue();//
			gstADCollect.ChunShui *= 0.00122;
		
			JieShui_IO_Flip();
			ADCHANAL_Select(3);
			sADChanalCnt = 6;
	}
	
	else if(sADChanalCnt == 6)
	{
			gstADCollect.JieShui = ADCHANAL_GetValue();//
			gstADCollect.JieShui  *= 0.00122;
		
			LouShui_IO_Flip();
			ADCHANAL_Select(6);
			sADChanalCnt = 7;
	}
	else if(sADChanalCnt == 7)
	{
			gstADCollect.LouShui = ADCHANAL_GetValue();// YuanShui
		  gstADCollect.LouShui  *= 0.00122;
		
			ChunShui_IO_Flip();
			YuanShui_IO_Flip();
			ADCHANAL_Select(7);
			sADChanalCnt = 1;
	}
	else
	{
			ADCHANAL_Select(1);
	}

#else
	
	if(sADChanalCnt == 1)
	{
			ChunShui_IO_Flip();
			ADCHANAL_Select(1);
			gstADCollect.ChunShui = ADC_GetConversionAverageValue(3);//YuanShui
		  gstADCollect.ChunShui  *= 0.00122;
			sADChanalCnt = 3;
	}

	else if(sADChanalCnt == 3)
	{
			ADCHANAL_Select(3);
			gstADCollect.JieShui = ADC_GetConversionAverageValue(3);//ChunShui
			gstADCollect.JieShui *= 0.00122;
		
			JieShui_IO_Flip();
			
			sADChanalCnt = 6;
	}
	
	else if(sADChanalCnt == 6)
	{
			ChunShui_IO_Flip();
			ADCHANAL_Select(6);
			gstADCollect.LouShui = ADC_GetConversionAverageValue(3);//JieShui
			gstADCollect.LouShui  *= 0.00122;
		
			LouShui_IO_Flip();
			
			sADChanalCnt = 7;
	}
	else if(sADChanalCnt == 7)
	{
			ADCHANAL_Select(7);
			gstADCollect.YuanShui = ADC_GetConversionAverageValue(3);// YuanShui LouShui
		  gstADCollect.YuanShui  *= 0.00122;
		
			YuanShui_IO_Flip();
			
			sADChanalCnt = 1;
	}
	else
	{
			ADCHANAL_Select(1);
	}
#endif
	
	
	
}