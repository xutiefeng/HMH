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
#include "string.h"

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



/************************************************************************* 
* 函数名称:	FilterSysRunTime
* 功能说明:	用于滤芯寿命計數.单位秒
* 输    入: 无	
* 输    出: 无
*************************************************************************/
void FilterSysRunTime(void)
{
	if(gstFilte.type == ROFilter)	
	{
		gstFilte.RO_DayCnt++;
	}
	else
	{
		gstFilte.Mix_DayCnt++;
	}
	
	if(gstFilte.type == ROFilter && PunpFlag)	
	{
		gstFilte.RO_FlowRateCnt++;
	}
	else
	{
		gstFilte.Mix_FlowRate++;
	}
}

/************************************************************************* 
* 函数名称:	FilterSysRunTime
* 功能说明:	用于芯寿命計數
* 输    入: 无	
* 输    出: 无
*************************************************************************/
void PumpSysRunTime(void)
{
	
}

/************************************************************************* 
* 函数名称:	FirstPowerOnProcess
* 功能说明:	首次上电处理
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
		static u8 sMWaterStepCnt = 0;
		
		if(KeySwitchFlag && !MakeWaterFlag)//检测到高压开关打开,若设备没有制水，则开始制水
		{
			sMWaterStep =   0;
			MakeWaterFlag = 1;				
		}
		else if(!KeySwitchFlag && sMWaterStep ==2)//检测高压开关关闭， 并且设备正在制水，则关闭进水阀，0.5秒后关闭气泵
		{
			sMWaterStep = 3;//
		}
		
	
		
		switch(sMWaterStep)
		{
				case 0:
					JinShuiFa_io =1;
					sMWaterStep =1;
				
					LED1_L =0;
				  LED2_L =0;
				 	LED3_L =0;
					LED4_L =1;
					PunpFlag = 1;
				break;
				
				case 1://0.5秒后打开增压泵
					sMWaterStepCnt++;
					if(sMWaterStepCnt >9)
					{
						sMWaterStepCnt = 0;
						Pump_io = 1;
						sMWaterStep++;
					}			
				break;
				
				case 2:
					_nop_();
				break;
				
				case 3:
					JinShuiFa_io = 0;//关闭进水阀
					sMWaterStepCnt = 0;
					sMWaterStep++;
					MakeWaterFlag = 0;
					LED1_L =0;
				  	LED2_L =0;
				 	LED3_L =0;
					LED4_L =0;
				break;
				
				case 4:
					sMWaterStepCnt++;
					if(sMWaterStepCnt >9)
					{
						sMWaterStepCnt = 0;
						Pump_io = 0;
						sMWaterStep++;
							
					}
				break;
				
				case 5:
					 PunpFlag = 0;
					_nop_();
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
