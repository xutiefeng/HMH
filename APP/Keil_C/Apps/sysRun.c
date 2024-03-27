
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
* ��������:	FilterlifeTime
* ����˵��:	���ڼ�����о����ʣ��ʱ��
* ��    ��: ��	
* ��    ��: ��
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
* ��������:	TimeReminder
* ����˵��:	�������ڴ���
* ��    ��: ��	
* ��    ��: ��
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
		if(temp <= 1)//��������
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
	
		else if(temp <=3)//�����쵽��
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
		if(temp <= 3)//��������
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
	
		else if(temp <= 10)//�����쵽��
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
* ��������:	FilterSysRunTime
* ����˵��:	������о����Ӌ��.��λ��
* ��    ��: ��	
* ��    ��: ��
*************************************************************************/

#define RO_DAY 1080ul
#define MIX_3INI_DAY 360ul
void FilterSysRunTime(void)
{
	 unsigned long  temp;
	 float ftemp;
	if(gstFilte.type == ROFilter)	
	{
	/* ����ʹ��
		�ٷ�֮2   90514640
		�ٷ�֮3   0x5651D00
		�ٷ�֮10  0x5017200
		
		*/
		if(gstFilte.RO_DayCnt <0x058FD400 )	//3600*24*1080 0x01E13380
			FilterlifeDayTime();
			
		// �������ж��ٷ��Ӵ���һ�� (10/2.63)-> ת����S-> (10/2.63)*60-> ����������ʱ��ʱ�� ��(10/2.63)*60*365
		// ����1�������ж����죺  tick */60*��10/2.63�� 
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
		/* ����ʹ��
		�ٷ�֮3   0x01C9CE63
		�ٷ�֮10   0x1AB2600
		�ٷ�֮90  3110405
		*/
		if(gstFilte.Mix_DayCnt <0x01DA9C00 )//3600*24*360 
			FilterlifeDayTime();
			
		// �������ж��ٷ��Ӵ���һ�� (10/2.63)-> ת����S-> (10/2.63)*60-> ����������ʱ��ʱ�� ��(10/2.63)*60*365
		// ����1�������ж����죺  tick */60*��10/2.63�� 
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

		/* ����ʹ��
		�ٷ�֮1   90514640
		�ٷ�֮3   0x5651D00
		�ٷ�֮100  246388ul
		
		*/	
			
		// �������ж��ٷ��Ӵ���һ�� (10/2.63)-> ת����S-> (10/2.63)*60-> ����������ʱ��ʱ�� 2.63*60*10800 
		// ����1�������ж����죺  tick */60*��10/2.63�� 
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
			
		

			
		// �������ж��ٷ��Ӵ���һ�� (10/2.63)-> ת����S-> (10/2.63)*60-> ����������ʱ��ʱ�� ��(10/2.63)*60*365
		// ����1�������ж����죺  tick */60*��10/2.63�� 
		temp =  gstFilte.Mix_FlowRate*0.04384;//gstFilte.RO_DayCnt*2.63/60
		
		if(temp >3600ul )
			temp = 3600ul;
		
		temp = 	3600-temp;	

		ftemp = ((float)temp)/36;
			
		gstAM901.Run.Bit.FlowResidualPercent =  (u8)ftemp;
	}
}

 

/************************************************************************* 
* ��������:	DevicePowerOn
* ����˵��:	�豸�ϵ��ʼ��
* ��    ��: ��	
* ��    ��: ��
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
* ��������:	FirstPowerOnProcess
* ����˵��:	�״��ϵ���豸��ϴ����
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
		static u16 sMWaterStepCnt = 0;

		if(FactoryModeFlag)
		{
			return ;
		}
	
		if(FirstPownOnFlag)//���״��ϵ��ʼ���ǻ����ϵ
		{
				return;
		}	
		
		if(KeySwitchFlag && !MakeWaterFlag)//��⵽��ѹ���ش�,���豸û����ˮ����ʼ��ˮ
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
				
				case 1://0.5������ѹ��
					sMWaterStepCnt++;
					if(sMWaterStepCnt > _500MS_Per50MS)
					{
						sMWaterStepCnt = 0;
						Pump_io = 1;
						sMWaterStep++;
					}			
				break;
				
				case 2:
					if(!KeySwitchFlag)//ˮ��
					{
							sMWaterStep++;
							LED4_L =0;
					}
				break;
				
				case 3:
					JinShuiFa_io = 0;//�رս�ˮ��
					sMWaterStepCnt = 0;
					sMWaterStep++;
					MakeWaterFlag = 0;
				break;
				
				case 4://0.5���ر՚��
					sMWaterStepCnt++;
					if(sMWaterStepCnt > _500MS_Per50MS)
					{
						sMWaterStepCnt = 0;
						Pump_io = 0;
						PunpFlag = 0;
						sMWaterStep++;							
					}
				break;
				
				case 5://��������Ӽ�⵽ˮ�����򿪻�����
					  
					  sMWaterStepCnt++;
						if(sMWaterStepCnt >_5Min_Per50MS)
						{
								sMWaterStepCnt = 0;
								sMWaterStep++;		
								
						}
				break;
						
				case 6:
						sMWaterStepCnt++;
						if(sMWaterStepCnt >_1Min_Per50MS)//һ���Ӻ�رջ�����
						{
								sMWaterStepCnt = 0;
								sMWaterStep++;		
								HuiLiuFa_io = 0;
								Pump_io = 0;
						}
						else
						{
								HuiLiuFa_io = 1;
								Pump_io = 1;//�򿪻�����
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
* ��������:	sysRuning
* ����˵��:	������������
* ��    ��: ��	
* ��    ��: ��
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
						FilterSysRunTime();//��о��������
						FirstPowerOnProcess();//�״��ϵ��ϴ15����
						FactoryProcess();
						UART0_SendData();
        }
        break;
   
        default:
        break;
    }
}
