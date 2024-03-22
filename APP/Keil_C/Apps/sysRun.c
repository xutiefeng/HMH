
//////min sys//////
#include "globe.h"
#include "clib.h"
#include "sysRun.h"
//////min sys//////

//#include "key.h"
//#include "bsp_Led.h"
//#include "factory.h"
//#include "FuncTab.h"
#include "..\Apps\bsp_Led.h"
#include "..\Apps\ioConfig.h"
#include "string.h"

#define CLOSE_ALL_LED()  gbFlagData[3].all=0;BlueLedFlag =0;RedLedtFlag =0
#define OPEN_ALL_LED()   gbFlagData[3].all=0xff;BlueLedFlag =1;RedLedtFlag =1

/*****************************************************
*函数名称：void CMP_Init(uchar CMPIS, uchar CMPRF)
*函数功能：模拟比较器初始化
*入口参数：CMPIS=正端通道，CMPRF=负端电压
*出口参数：void
*****************************************************/
void CMP_Init(void)
{
	CMPCON = 0x86;   //开启模拟比较器电源 选择负端电压 vdd/3
	CMPCFG = 0x18;//正端选择1.5v   ，从正端电压小于负端电压到正端电压大于负端电压 产生中断  
    IE1 |= 0x20;
}


void sysRest(void)
{
 	 memset(gbFlagData,0,5);
 	 memset(&gstFilte,0,sizeof(gstFilte));
	 gstAM901.stFilter = &gstFilte;
}

typedef enum
{
	ROFilter,
	MixFilter
}emFilter;




/************************************************************************* 
* 函数名称:	FilterlifeTime
* 功能说明:	用于计算滤芯寿命剩余时间
* 输    入: 无	
* 输    出: 无
*************************************************************************/
void FilterlifeTime(emFilter type)
{
	if( type  == ROFilter)
	{
		
		gstFilte.RO_FlowRateCnt++;
	}
	else
	{
		gstFilte.Mix_FlowRate++;
	}

}

void TimeReminder(void)
{
	static u8 sTimeReminderCnt =0;
	u8  temp =gstAM901.Run.Bit.DayResidualPercent;
	
	if(gstAM901.Run.Bit.DayResidualPercent < temp)
		temp = gstAM901.Run.Bit.DayResidualPercent;
		
	if( gstFilte.type  == ROFilter)
	{
		if(temp <1)//寿命到期
		{
				LED2_R =1;
				LED2_L =0;
		}
	
		else if(temp <3)//寿命快到期
		{
				if(sTimeReminderCnt < 5)
				{
						LED2_R =1;
						LED2_L =0;
				}
				else
				{
						LED2_R =0;
						LED2_L =0;
				}
				
				sTimeReminderCnt++;
				
				if(sTimeReminderCnt >= 10)
					sTimeReminderCnt = 0;
		}
		else
		{
				LED2_R =0;
				LED2_L =1;
		}
	}
	else
	{
		if(temp < 3)//寿命到期
		{
				LED1_R =1;
				LED1_L =0;

		}
	
		else if(temp < 10)//寿命快到期
		{
				if(sTimeReminderCnt < 5)
				{
						LED1_R =1;
						LED1_L =0;
				}
				else
				{
						LED1_R =0;
						LED1_L =0;
				}
				
				sTimeReminderCnt++;
				
				if(sTimeReminderCnt >= 10)
					sTimeReminderCnt = 0;
		}
		else
		{
				LED1_R =0;
				LED1_L =1;
		}
	}
		
	
}


/************************************************************************* 
* 函数名称:	FilterSysRunTime
* 功能说明:	用于滤芯寿命計數.单位秒
* 输    入: 无	
* 输    出: 无
*************************************************************************/
void FilterSysRunTime(void)
{
	u16 temp;
	if(gstFilte.type == ROFilter)	
	{
	//	if(gstFilte.RO_DayCnt < 0x058FD400?)//3600*24*365
		if(gstFilte.RO_DayCnt <0x01E13380 )	
			gstFilte.RO_DayCnt++;
			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 ：(10/2.63)*60*365
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.RO_DayCnt/0x00015180;//gstFilte.RO_DayCnt*2.63/60
			
		gstAM901.Run.Bit.DayResidualPercent = 100*( (1080-temp)/1080);
	}
	else
	{
		
		//if(gstFilte.Mix_DayCnt < ??0x01E13380)//83270(10/2.63)*60*1080)
		if(gstFilte.Mix_DayCnt <0x01E13380 )
			gstFilte.Mix_DayCnt++;
			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 ：(10/2.63)*60*365
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.Mix_DayCnt/0x00015180;//gstFilte.RO_DayCnt*2.63/60
			
		gstAM901.Run.Bit.DayResidualPercent = 100*( (365-temp)/365);
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	if(gstFilte.type == ROFilter && PunpFlag)	
	{
		if(gstFilte.RO_FlowRateCnt < 246388ul)//108000/(2.63/60)
			gstFilte.RO_FlowRateCnt++;

			
			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 2.63*60*10800 
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.RO_FlowRateCnt*0.04384;//2.63/60
		
		if(temp >10800ul )
			temp = 10800ul;

		gstAM901.Run.Bit.DayResidualPercent = 100*( (10800-temp)/10800);
	}
	else if(gstFilte.type == MixFilter && PunpFlag)
	{
		if(gstFilte.Mix_FlowRate < 82117ul)//3600/(2.63/60)
			gstFilte.Mix_FlowRate++;
			
		if(temp >3600ul )
			temp = 3600ul;

			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 ：(10/2.63)*60*365
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.Mix_FlowRate*0.04384;//gstFilte.RO_DayCnt*2.63/60
			
		gstAM901.Run.Bit.DayResidualPercent = 100*( (3600-temp)/3600);
	}
}



/************************************************************************* 
* 函数名称:	DevicePowerOn
* 功能说明:	设备上电初始化
* 输    入: 无	
* 输    出: 无
*************************************************************************/
volatile  u8 sDPOCnt =0;nt =0;
void DevicePowerOnInit(void)
{
	
	Blueled_io = 1;
	Redled_io = 1;
	Chanel_IO1 = 1;
	Chanel_IO2 = 1;
	led1_io = 0;
	led2_io = 0;
	led3_io = 0;
	led4_io = 0;
	//Buzzer2Flag = 1;
	//BuzzerProcess();
	PWM_IndependentModeConfig(PWM02,2000);
	while(1)
	{
		if(Ev100MSFlag)
		{
			WDTCON |=0x10;
			sDPOCnt++;
			Ev100MSFlag =0;
			
			  
		    if(sDPOCnt>= 30)
			{
				break;
			}
			else if(sDPOCnt>= 2)
			{
				PWM_IndependentModeConfig(PWM02,0);
			}				
		}
	}
	Chanel_IO1 = 0;
	Chanel_IO2 = 0;
	led1_io = 0;
	led2_io = 0;
	led3_io = 0;
	led4_io = 0;
	Blueled_io = 0;
	Redled_io = 0;
}

/************************************************************************* 
* 函数名称:	FirstPowerOnProcess
* 功能说明:	首次上电对设备清洗处理
* 输    入: 无	
* 输    出: 无
*************************************************************************/
void FirstPowerOnProcess(void)
{
		static u16 sTimeCnt = 0;
		if(!FirstPownOnFlag)
		{
				return;
		}	
		sTimeCnt++;
		JinShuiFa_io = 1;
		Pump_io = 1;
		FeiShuiFa_IO = 1;
		KongShuiFa_IO = 1;
		
		if(sTimeCnt > _15Min_Per1S)
		{
				JinShuiFa_io = 0;
				Pump_io = 0;
				FeiShuiFa_IO = 0;
				KongShuiFa_IO = 0;
				FirstPownOnFlag =  1;
		}
				
		
}
/************************************************************************* 
* 函数名称:	makeWaterProcess
* 功能说明:	制作纯水
* 输    入: 无	
* 输    出: 无
*************************************************************************/


void MakeWaterProcess(void)
{		
		static u8 sMWaterStep = 5;
		static u16 sMWaterStepCnt = 0;
	
		if(FirstPownOnFlag)//和首次上电初始化是互斥关系
		{
				return;
		}	
		
		if(KeySwitchFlag && !MakeWaterFlag)//检测到高压开关打开,若设备没有制水，则开始制水
		{
			sMWaterStep =   0;
			MakeWaterFlag = 1;	
			HuiLiuFa_io = 0;
		}

		
	
		
		switch(sMWaterStep)
		{
				case 0:
					JinShuiFa_io =1;
					sMWaterStep =1;
					LED4_L =1;
					PunpFlag = 1;
					
				break;
				
				case 1://0.5秒后打开增压泵
					sMWaterStepCnt++;
					if(sMWaterStepCnt > _500MS_Per50MS)
					{
						sMWaterStepCnt = 0;
						Pump_io = 1;
						sMWaterStep++;
					}			
				break;
				
				case 2:
					if(!KeySwitchFlag)//水满
					{
							sMWaterStep++;
							LED4_L =0;
					}
				break;
				
				case 3:
					JinShuiFa_io = 0;//关闭进水阀
					sMWaterStepCnt = 0;
					sMWaterStep++;
					MakeWaterFlag = 0;
				break;
				
				case 4://0.5秒后关闭氣泵
					sMWaterStepCnt++;
					if(sMWaterStepCnt > _500MS_Per50MS)
					{
						sMWaterStepCnt = 0;
						Pump_io = 0;
						PunpFlag = 0;
						sMWaterStep++;							
					}
				break;
				
				case 5://连续五分钟检测到水满，打开回流阀
					  
					  sMWaterStepCnt++;
						if(sMWaterStepCnt >_5Min_Per50MS)
						{
								sMWaterStepCnt = 0;
								sMWaterStep++;		
								
						}
				break;
						
				case 6:
						if(sMWaterStepCnt >_1Min_Per50MS)//一分钟后关闭回流阀
						{
								sMWaterStepCnt = 0;
								sMWaterStep = 5;		
								HuiLiuFa_io = 0;
								Pump_io = 0;
						}
						else
						{
								HuiLiuFa_io = 1;
								Pump_io = 1;//打开回流阀
						}
						if(KeySwitchFlag )
							HuiLiuFa_io = 0;
				break;
						
				default:
				break;
				
				
			}
}


void JinShuiWorkProcess(void)
{
		
}

/************************************************************************* 
* 函数名称:	sysRuning
* 功能说明:	程序运行主体
* 输    入: 无	
* 输    出: 无
*************************************************************************/
void sysRuning(void)
{	

	EventCollect();
    switch(PopEvent())
    {    

		case ev3MS:
			 LED_sCan();
		break; 
				
        case ev5MS:       
					BuzzerProcess();
			 //ADCollectProcess();	//	
        break;  
    
        case ev20MS:
        {
						
        }				
        break;
				
        case ev50MS:
        {
						KeySelect();
						KeyRest();
						KeyGaoYaSwitch();
						MakeWaterProcess();						
        }
        break;
				
        case ev100MS:
        {
						LED_Process();	
						TimeReminder();
            ////EepromProcess();   
        }
        break;
				
        case ev1S:
        {
						FilterSysRunTime();//滤芯寿命计算
						FirstPowerOnProcess();//首次上电冲洗15分钟
        }
        break;
   
        default:
        break;
    }
}
