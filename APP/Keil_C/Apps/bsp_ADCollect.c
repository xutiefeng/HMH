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







void ChunShuiTDS(void)
{
	
}

void JieShuiTDS(void)
{
		
}

void YuanShui_IO_Flip(void)
{
		P04 = !P04;
		P02 = !P02;
}

void YuanShui_IO_Flip_1(void)
{
		P04 = 0;
		P02 = 1;
}

void YuanShui_IO_Flip_2(void)
{
		P04 = 1;
		P02 = 0;
}

void ChunShui_IO_Flip(void)
{
		P14 = !P14;
		P25 = !P25;
}

void ChunShui_IO_Flip_1(void)
{
		P14 = 0;
		P25 = 1;
}

void ChunShui_IO_Flip_2(void)
{
		P14 = 1;
		P25 = 0;
}

void JieShui_IO_Flip(void)
{
		P26 = !P26;
		P27 = !P27;
}

void JieShui_IO_Flip_1(void)
{
		P26 = 0;
		P27 = 1;
}

void JieShui_IO_Flip_2(void)
{
		P26 = 1;
		P27 = 0;
}

void LouShui_IO_Flip(void)
{
		P00 = !P00;
		P01 = !P01;
}

void LouShui_IO_Flip_1(void)
{
		P00 =  0;
		P01 =  1;
}

void LouShui_IO_Flip_2(void)
{
		P00 = 1;
		P01 = 0;
}

void TDs_IO_Flip(void)
{
	P04 = !P04;
	P02 = !P02;
	
	P14 = !P14;
	P25 = !P25;
	
	P26 = !P26;
	P27 = !P27;
	
	P00 = !P00;
	P01 = !P01;
}

#define AD_Cycle 15
void ADCollectProcess(void)
{
	static u8 sADChanalCnt = 0;
	static u8 sAD_Step = 0;
	
	if(gstADCollect.ADAvergeCnt >=10)
		return;

	sADChanalCnt++;
	
#if 1
	switch(sAD_Step)
	{
		case 0:
			
			if(sADChanalCnt == 1)
			{
				ChunShui_IO_Flip_1();
				//LouShui_IO_Flip();
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(1);
				
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.ChunShui[gstADCollect.ADAvergeCnt] = ADCHANAL_GetValue();	
				//LouShui_IO_Flip();
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step++;
				sADChanalCnt =0;
				ChunShui_IO_Flip_2();
				
			}
			
			 
		break;	

		case 1:
			
			if(sADChanalCnt == 1)
			{
				JieShui_IO_Flip_1();
				//YuanShui_IO_Flip();
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(3);
			  // YuanShui_IO_Flip();
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.JieShui[gstADCollect.ADAvergeCnt]  = ADCHANAL_GetValue();
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step++;
				sADChanalCnt =0;
				JieShui_IO_Flip_1();
				
			}
			
		break;

		case 2:
			if(sADChanalCnt == 1)
			{
				LouShui_IO_Flip_1();
				//ChunShui_IO_Flip();
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(6);;
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.LouShui[gstADCollect.ADAvergeCnt]  = ADCHANAL_GetValue();// YuanShui
				
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step++;
				sADChanalCnt =0;
				LouShui_IO_Flip_1();
			}
		break;

		case 3:
			if(sADChanalCnt == 1)
			{
				YuanShui_IO_Flip_1();
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(7);
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.YuanShui[gstADCollect.ADAvergeCnt]  = ADCHANAL_GetValue();// YuanShui				
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step =0;
				sADChanalCnt =0;
				YuanShui_IO_Flip_1();
				gstADCollect.ADAvergeCnt++;
			}
		break;

		default:
			break;

		

	}
#endif
	
}


unsigned int adc_get_average(u16 *p)            
{
  unsigned int adc_value_max = 0;
  unsigned int adc_value_min = 0xffff;
  unsigned int adc_sum = 0;
  unsigned int adc_current = 0;
  unsigned char i;
  
  for(i = 0;i<10;i++)
  {
    adc_current = p[i];
    if(adc_current > adc_value_max)
    {
      adc_value_max = adc_current;
    }
    if(adc_current < adc_value_min)
    {
      adc_value_min = adc_current;
    }
    adc_sum = adc_sum + adc_current;
  }
  
  adc_sum = adc_sum - adc_value_min;
  adc_sum = adc_sum - adc_value_max;
  adc_sum = adc_sum>>3;
  return adc_sum;
}

#define TDS_100 1
#define TDS_300 1
#define TDS_500 1
#define TDS_1000 1

void TDS_Calulate(void)
{
	static u8 sTDS_Calulate =0;
	
	
	if(gstADCollect.ADAvergeCnt < 10)
	{
		return;
	}

	

		

	if(sTDS_Calulate < 5)//TDS每隔500ms检测一次
		sTDS_Calulate++;
	else
		sTDS_Calulate =0;

	gstADCollect.ADAvergeCnt = 0;

	 gstADCollect.fYuanShui= adc_get_average(gstADCollect.YuanShui);

	gstADCollect.fChunShui=  adc_get_average(gstADCollect.ChunShui);

	gstADCollect.fLouShui=    adc_get_average(gstADCollect.LouShui);

	gstADCollect.fJieShui=     adc_get_average(gstADCollect.JieShui);
	

	

	


	
	
}

void PaiShuiProcess(void)//1S钟运行一次
{
	static u8 sTDS_Time =0;

	if(sTDS_Time < 100)
		sTDS_Time++;

	if(sTDS_Time < 100)
	{
		if(gstADCollect.fYuanShui >= TDS_100)
		{
			KongShuiFa_IO = 1;			
		}

		else if(gstADCollect.fYuanShui >= TDS_300)
		{
			KongShuiFa_IO = 1;	
		}

		else if(gstADCollect.fYuanShui >= TDS_500)
		{
			KongShuiFa_IO = 1;	
		}
		else//上店10秒内TDS <100
		{
			KongShuiFa_IO = 1;	
		}
	}
}


#if 0
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

