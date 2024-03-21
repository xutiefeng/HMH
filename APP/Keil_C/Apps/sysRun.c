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
*�������ƣ�void CMP_Init(uchar CMPIS, uchar CMPRF)
*�������ܣ�ģ��Ƚ�����ʼ��
*��ڲ�����CMPIS=����ͨ����CMPRF=���˵�ѹ
*���ڲ�����void
*****************************************************/
void CMP_Init(void)
{
	CMPCON = 0x86;   //����ģ��Ƚ�����Դ ѡ�񸺶˵�ѹ vdd/3
	CMPCFG = 0x18;//����ѡ��1.5v   �������˵�ѹС�ڸ��˵�ѹ�����˵�ѹ���ڸ��˵�ѹ �����ж�  
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
* ��������:	FilterlifeTime
* ����˵��:	���ڼ�����о����ʣ��ʱ��
* ��    ��: ��	
* ��    ��: ��
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
* ��������:	FilterSysRunTime
* ����˵��:	������о����Ӌ��.��λ��
* ��    ��: ��	
* ��    ��: ��
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
* ��������:	FilterSysRunTime
* ����˵��:	����о����Ӌ��
* ��    ��: ��	
* ��    ��: ��
*************************************************************************/
void PumpSysRunTime(void)
{
	
}

/************************************************************************* 
* ��������:	FirstPowerOnProcess
* ����˵��:	�״��ϵ紦��
* ��    ��: ��	
* ��    ��: ��
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
* ��������:	makeWaterProcess
* ����˵��:	������ˮ
* ��    ��: ��	
* ��    ��: ��
*************************************************************************/



void MakeWaterProcess(void)
{		
		static u8 sMWaterStep = 5;
		static u8 sMWaterStepCnt = 0;
		
		if(KeySwitchFlag && !MakeWaterFlag)//��⵽��ѹ���ش�,���豸û����ˮ����ʼ��ˮ
		{
			sMWaterStep =   0;
			MakeWaterFlag = 1;				
		}
		else if(!KeySwitchFlag && sMWaterStep ==2)//����ѹ���عرգ� �����豸������ˮ����رս�ˮ����0.5���ر�����
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
				
				case 1://0.5������ѹ��
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
					JinShuiFa_io = 0;//�رս�ˮ��
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
* ��������:	sysRuning
* ����˵��:	������������
* ��    ��: ��	
* ��    ��: ��
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
						FilterSysRunTime();//��о��������
						FirstPowerOnProcess();//�״��ϵ��ϴ15����
        }
        break;
   
        default:
        break;
    }
}
