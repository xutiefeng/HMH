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



#define AD_Cycle 10
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
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(1);				
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.ChunShui[gstADCollect.ADAvergeCnt] = ADCHANAL_GetValue();	
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step++;
				sADChanalCnt =0;
				ChunShui_IO_Flip_2();
				YuanShui_IO_Flip_1();
			}
			
			 
		break;	

		case 1:
			
			if(sADChanalCnt == 1)
			{
				JieShui_IO_Flip_1();
				// ChunShui_IO_Flip_1();
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(3);
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.JieShui[gstADCollect.ADAvergeCnt]  = ADCHANAL_GetValue();
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step++;
				sADChanalCnt =0;
				JieShui_IO_Flip_2();
			  //ChunShui_IO_Flip_2();
				YuanShui_IO_Flip_2();
			}
			
		break;
#if 1
		case 2:
			if(sADChanalCnt == 1)
			{
				LouShui_IO_Flip_1();
				//ChunShui_IO_Flip_1();
			}
			else if(sADChanalCnt == 3)
			{
				 ADCHANAL_Select(6);
			}

			else if(sADChanalCnt == 5)
			{
				gstADCollect.LouShui[gstADCollect.ADAvergeCnt]  = ADCHANAL_GetValue();// YuanShui
				
			}


			else if(sADChanalCnt >= AD_Cycle)
			{
				sAD_Step++;
				sADChanalCnt =0;
				LouShui_IO_Flip_2();
				// ChunShui_IO_Flip_2();
			}
		break;
#endif
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
				YuanShui_IO_Flip_2();
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





void TDS_Calulate(void)
{
	static u8 sTDS_Calulate =0;

	
	
	
	if(gstADCollect.ADAvergeCnt < 10)
	{
		return;
	}

	

		

	if(sTDS_Calulate < 5)//TDS每隔500ms检测一次,前面3秒钟AD数据不计算
	{
			sTDS_Calulate++;
			if(sTDS_Calulate < 0)
			{
				gstADCollect.ADAvergeCnt = 0;
			}
			return;
	}
		
	else
	{	
			sTDS_Calulate = 15;
	}
		

	gstADCollect.ADAvergeCnt = 0;


	gstADCollect.fYuanShui= adc_get_average(gstADCollect.YuanShui);

	gstADCollect.fChunShui=  adc_get_average(gstADCollect.ChunShui);

	gstADCollect.fLouShui=    adc_get_average(gstADCollect.LouShui);

	gstADCollect.fJieShui=     adc_get_average(gstADCollect.JieShui);

}



// 计算TDS值的函数

#include <math.h>
#define ADC_MAX_VALUE 0x0fff
#define ADC_MIN_VALUE 0



float adc_to_tds(u16 adc_value, int a,int b) 
{
	
    // 将ADC值标准化到0到1之间
    float normalized_adc = (adc_value  - ADC_MIN_VALUE) / (float)(ADC_MAX_VALUE - ADC_MIN_VALUE);
    // 应用一次线性拟合
    float  linear_tds= a * normalized_adc + b;

		return linear_tds;
}

void test_TDS(u16 D, float *pp)
{
		//gstADCollect.tds_ChunShui =adc_to_tds(gstADCollect.fChunShui);
		u16 v = D;
		
		float *p =pp;	
		
		if(p == &gstADCollect.tds_ChunShui)
		{
				if( v <= 377 )//ppm 6 357
				{
							*p = adc_to_tds(v,70,0);
				}
				else if(v <= 688)//ppm 668 20
				{
						*p = adc_to_tds(v,182,-9.7);
				}

				else if(v<= 1086)						   //1066 50ppm
				{
						*p  = adc_to_tds(v,309,-31);//40
				}
				else if(v <= 1156)//50ppm
				{
						//*p   = adc_to_tds(v,200,-9.5);//50
						*p= adc_to_tds(v,372,-54);
				}
				
				else if(v <= 1356)//100ppm 1226
				{
						//*p   = adc_to_tds(v,200,-9.5);//50
						*p= adc_to_tds(v,1137,-268);
				}
				
				else if(v <= 1610)//200ppm
				{
						*p  = adc_to_tds(v,1479,-390);//85
				}
				
				else if(v <= 1820)
				{
						
						*p   = adc_to_tds(v,819,-174);//110
				}
				
				else if(v <= 1820)
				{
						
						*p  = adc_to_tds(v,897,-225);//110
				}
		}

		else if(p == &gstADCollect.tds_YuanShui)
		{
			if(v< 2225)//ppm 108 ad:2225
			{
						*p  = adc_to_tds(v,205,0);//110
			}
			
			
			else if(v< 3090)//ppm 299 3087
			{
					*p  = adc_to_tds(v,907,-385);//110
			
			}
			
			else if(v< 3545)//ppm 496 3535
			{
					*p  = adc_to_tds(v,1880,-1117);//110
			
			}
			
			else//ppm 1000 3835
			{
					*p  = adc_to_tds(v,6880,-5443);//110
			}
		}

		else
		{
				if(v<= 880)						   //1146 50ppm
				{
						*p  = adc_to_tds(v,140,-5);//40
				}
				else if(v <= 1156)//50ppm
				{
						//*p   = adc_to_tds(v,200,-9.5);//50
						*p= adc_to_tds(v,372,-54);
				}
				
				else if(v <= 1356)//100ppm 1226
				{
						//*p   = adc_to_tds(v,200,-9.5);//50
						*p= adc_to_tds(v,1137,-268);
				}
				
				else if(v <= 1610)//200ppm
				{
						*p  = adc_to_tds(v,1479,-390);//85
				}
				
				else if(v <= 1820)
				{
						
						*p   = adc_to_tds(v,819,-174);//110
				}
				
				else if(v <= 1820)
				{
						
						*p  = adc_to_tds(v,897,-225);//110
				}
		}
		
	}

	void TDS_Select(u16 D, float *pp)
{
		//gstADCollect.tds_ChunShui =adc_to_tds(gstADCollect.fChunShui);
		u16 v = D;
		
		float *p =pp;	
		
	#if 0
	  if(v <= 600)//20
		{
				*p = adc_to_tds(v,80,-3);
		}

		else if(v<= 880)						   //1146 50ppm
		{
				*p  = adc_to_tds(v,140,-5);//40
		}
		else if(v <= 1156)//50ppm
		{
				//*p   = adc_to_tds(v,200,-9.5);//50
				*p= adc_to_tds(v,372,-54);
		}
		
		else if(v <= 1356)//100ppm 1226
		{
				//*p   = adc_to_tds(v,200,-9.5);//50
				*p= adc_to_tds(v,1137,-268);
		}
		
		else if(v <= 1610)//200ppm
		{
				*p  = adc_to_tds(v,1479,-390);//85
		}
		
		else if(v <= 1820)
		{
				
				*p   = adc_to_tds(v,819,-174);//110
		}
		
		else if(v <= 1820)
		{
				
				*p  = adc_to_tds(v,897,-225);//110
		}
		#endif
		
				*p  = adc_to_tds(v,205,0);//110

	}
		


