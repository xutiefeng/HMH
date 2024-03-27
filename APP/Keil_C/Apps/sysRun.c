
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


#define STEP_1()  Pump_io =1;LED1_L=1;sFactoryStep++
#define STEP_2()  JinShuiFa_io =1;LED2_L=1;sFactoryStep++
#define STEP_3()  FeiShuiFa_IO =1;LED4_L=1;sFactoryStep++
#define STEP_4()  HuiLiuFa_io =1;LED3_L=1;sFactoryStep++
#define STEP_5()  KongShuiFa_IO =1;LED4_R=1;sFactoryStep++

#define STEP_6()  BlueLedFlag =1;RedLedtFlag =1;gbFlagData[3].all =0XFF;Buzzer5Flag =1;sFactoryStep++
#define STEP_7()  sFactoryStep++;


void FactoryProcess(void)
{
	static u16 FactoryPeriodCnt = 0;
		
	static u8 sFactoryStep =0;

	FactoryPeriodCnt++;

	if(!FactoryModeFlag)
	{
		
		return ;
	}

	if(FactoryModeFlag^HisFactoryModeFlag)
	{
		sFactoryStep =0;
		HisFactoryModeFlag = FactoryModeFlag;
		
		gbFlagData[3].all = 0;
		Buzzer5Flag = 0;
		RedLedtFlag = 0;
	}

	if(FactoryPeriodCnt%2 == 0)
	{
		switch(sFactoryStep)
		{
			case 0:
				STEP_1();
			break;

			case 1:
				STEP_2();
			break;

			case 2:
				STEP_3();
			break;

			case 3:
				STEP_4();
			break;

			case 4:
				STEP_5();
			break;

			case 5:
				STEP_6();
			break;

			case 6:
				STEP_7();
			break;

			case 7:
				gbFlagData[3].all = 0;
				BlueLedFlag = 0;
				RedLedtFlag = 0;
				JinShuiFa_io = 0;
				FeiShuiFa_IO = 0;
				HuiLiuFa_io = 0;
				KongShuiFa_IO = 0;
				sFactoryStep++;
			break;

			case 8:
				 FstModeFlag = 1;
				 FactoryModeFlag =0;
			break;
			
			

			default:
				break;
			
		}
	}
	
}

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
 	 memset(gbFlagData,0,10);
 	 memset(&gstFilte,0,sizeof(gstFilte));
	 memset(&gstAM901,0,sizeof(gstAM901));
	 memset(&gstRDsysTick,0,sizeof(gstRDsysTick));
	 
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
	if(FstModeFlag)
	{
		if( type  == ROFilter)
		{			
			gstFilte.RO_FlowRateCnt+=600ul;
		}
		else
		{
			gstFilte.Mix_FlowRate+=600ul;
		}
	}
	else
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
	

}

void FilterlifeDayTime(emFilter type)
{
	if(FstModeFlag)
	{
		if( type  == ROFilter)
		{
			gstFilte.RO_DayCnt+=86400ul;
		}
		else
		{
			gstFilte.Mix_DayCnt+=86400ul;
		}
	}
	else
	{
		if( type  == ROFilter)
		{
			
			gstFilte.RO_DayCnt++;
		}
		else
		{
			gstFilte.Mix_DayCnt++;
		}
	}
	

}


/************************************************************************* 
* 函数名称:	TimeReminder
* 功能说明:	寿命到期处理
* 输    入: 无	
* 输    出: 无
*************************************************************************/



void TimeReminder(void)
{
	static ST_TimeReminder sTimeReminderCnt ={0};
	u8  temp =gstAM901.Run.Bit.DayResidualPercent;

	if(FactoryModeFlag)
	{
		return ;
	}
 
	
	if(gstAM901.Run.Bit.DayResidualPercent < temp)
		temp = gstAM901.Run.Bit.DayResidualPercent;
	
		
	if( gstFilte.type  == ROFilter)
	{
		if(temp <= 1)//寿命到期
		{
				if(!sTimeReminderCnt.Bit.buzeerOn1 && KeySwitchFlag1)
				{
					sTimeReminderCnt.Bit.buzeerOn = 0;
					sTimeReminderCnt.Bit.buzeerOn1 = 1;
					Buzzer1Flag = 1;
				}
				LED2_R =1;
				LED2_L =0; 	
		}
	
		else if(temp <=3)//寿命快到期
		{
		

				if(!sTimeReminderCnt.Bit.buzeerOn && KeySwitchFlag1)
				{
					sTimeReminderCnt.Bit.buzeerOn = 1;
					sTimeReminderCnt.Bit.buzeerOn1 = 0;
					Buzzer4Flag = 1;
				}
				
				if(sTimeReminderCnt.Bit.cnt <= 5)
				{
						LED2_R =1;
						LED2_L =0;
				}
				else
				{
						LED2_R =0;
						LED2_L =0;
				}
				
				sTimeReminderCnt.Bit.cnt++;
				
				if(sTimeReminderCnt.Bit.cnt >= 10)
					sTimeReminderCnt.Bit.cnt = 0;
				
		}
		else
		{
				LED2_R =0;
				LED2_L =1;
		}
	}
	else
	{
		if(temp <= 3)//寿命到期
		{
				LED1_R =1;
				LED1_L =0;
				if(!sTimeReminderCnt.Bit.buzeerOn1 && KeySwitchFlag1)
				{
					sTimeReminderCnt.Bit.buzeerOn = 0;
					sTimeReminderCnt.Bit.buzeerOn1 = 1;
					Buzzer1Flag = 1;
				}

		}
	
		else if(temp <= 10)//寿命快到期
		{

				if(!sTimeReminderCnt.Bit.buzeerOn && KeySwitchFlag1)
				{
					sTimeReminderCnt.Bit.buzeerOn = 1;
					sTimeReminderCnt.Bit.buzeerOn1 = 0;
					Buzzer4Flag = 1;
				}
				
				if(sTimeReminderCnt.Bit.cnt < 5)
				{
						LED1_R =1;
						LED1_L =0;
				}
				else
				{
						LED1_R =0;
						LED1_L =0;
				}
				
				sTimeReminderCnt.Bit.cnt++;
				
				if(sTimeReminderCnt.Bit.cnt >= 10)
					sTimeReminderCnt.Bit.cnt = 0;
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

#define RO_DAY 1080ul
#define MIX_3INI_DAY 360ul
void FilterSysRunTime(void)
{
	 unsigned long  temp;
	 float ftemp;
	if(gstFilte.type == ROFilter)	
	{
	/* 测试使用
		百分之2   90514640
		百分之3   0x5651D00
		百分之10  0x5017200
		
		*/
		if(gstFilte.RO_DayCnt <0x058FD400 )	//3600*24*1080 0x01E13380
			FilterlifeDayTime();
			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 ：(10/2.63)*60*365
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.RO_DayCnt/864; //gstFilte.RO_DayCnt/0x00015180 *100;
			
		if(temp < 108000ul)
		{
			temp = 108000ul-temp;
			ftemp = (float)temp/1080ul;
			gstAM901.Run.Bit.DayResidualPercent =  (u8)ftemp;
		}
		
		
	}
	else
	{
		/* 测试使用
		百分之3   0x01C9CE63
		百分之10   0x1AB2600
		百分之90  3110405
		*/
		if(gstFilte.Mix_DayCnt <0x01DA9C00 )//3600*24*360 
			FilterlifeDayTime();
			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 ：(10/2.63)*60*365
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.Mix_DayCnt/864; //gstFilte.RO_DayCnt/0x00015180 *100;
		temp = 	36000ul-temp;	

		ftemp = (float)temp/MIX_3INI_DAY;
		gstAM901.Run.Bit.DayResidualPercent = (u8)ftemp;
	}
///////////////////////////////////////////////////MIX_RO_DAY///////////////////////////////////////////////////////////

	
	if(gstFilte.type == ROFilter && PunpFlag)	
	{
		if(gstFilte.RO_FlowRateCnt < 246388ul)//108000/(2.63/60)
			FilterlifeTime();

		/* 测试使用
		百分之1   90514640
		百分之3   0x5651D00
		百分之100  246388ul
		
		*/	
			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 2.63*60*10800 
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  (float)gstFilte.RO_FlowRateCnt*0.04384;//2.63/60;//
		if(temp >10800ul )
			temp = 10800ul;
		
		temp = 	10800-temp;	

		ftemp = (float)temp/108;

		gstAM901.Run.Bit.FlowResidualPercent = (u8)ftemp;
	}
	else if(gstFilte.type == MixFilter && PunpFlag)
	{
		if(gstFilte.Mix_FlowRate < 82117ul)//3600/(2.63/60)
			FilterlifeTime();;
			
		

			
		// 气泵运行多少分钟代表一天 (10/2.63)-> 转换成S-> (10/2.63)*60-> 气泵运行总时间时间 ：(10/2.63)*60*365
		// 气泵1秒钟运行多少天：  tick */60*（10/2.63） 
		temp =  gstFilte.Mix_FlowRate*0.04384;//gstFilte.RO_DayCnt*2.63/60
		
		if(temp >3600ul )
			temp = 3600ul;
		
		temp = 	3600-temp;	

		ftemp = ((float)temp)/36;
			
		gstAM901.Run.Bit.FlowResidualPercent =  (u8)ftemp;
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

		if(FactoryModeFlag)
		{
			return ;
		}
	
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
						sMWaterStepCnt++;
						if(sMWaterStepCnt >_1Min_Per50MS)//一分钟后关闭回流阀
						{
								sMWaterStepCnt = 0;
								sMWaterStep++;		
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
extern void TDS_Calulate(void);
extern  void KeyGaoYaSwitch(void);
extern void KeyRest(void);
extern void KeySelect(void);
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
						TDS_Calulate();
						
            ////EepromProcess();   
        }
        break;
				
        case ev1S:
        {
						FilterSysRunTime();//滤芯寿命计算
						FirstPowerOnProcess();//首次上电冲洗15分钟
						FactoryProcess();
						UART0_SendData();
        }
        break;
   
        default:
        break;
    }
}
