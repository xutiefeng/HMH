
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
#include "..\Apps\includeall.h"

#define CLOSE_ALL_LED()  gbFlagData[3].all=0;BlueLedFlag =0;RedLedtFlag =0
#define OPEN_ALL_LED()   gbFlagData[3].all=0xff;BlueLedFlag =1;RedLedtFlag =1


#define STEP_1()  Pump_io =1;LED1_L=1;sFactoryStep++
#define STEP_2()  JinShuiFa_io =1;LED2_L=1;sFactoryStep++
#define STEP_3()  FeiShuiFa_IO =1;LED4_L=1;sFactoryStep++
#define STEP_4()  HuiLiuFa_io =1;LED3_L=1;sFactoryStep++
#define STEP_5()  KongShuiFa_IO =1;LED4_R=1;sFactoryStep++

#define STEP_6()  BlueLedFlag =1;RedLedtFlag =1;gbFlagData[3].all =0XFF;Buzzer5Flag =1;sFactoryStep++
#define STEP_7()  sFactoryStep++;
extern void setLED(u8 num,emColor color ,U_LED state,u8 time);
extern void test_TDS(u16 D, float *pp);

void CloseFuZai(void)
{
	JinShuiFa_io = 0;
	FeiShuiFa_IO = 0;
	HuiLiuFa_io = 0;
	KongShuiFa_IO = 0;
	Pump_io = 0;
}

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
				CloseFuZai();
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
	 FstModeFlag = 0;
	 KeyCancelRestFlag = 0;
}





/************************************************************************* 
* ��������:	FilterlifeTime
* ����˵��:	���ڼ�����о����ʣ��ʱ��
* ��    ��: ��	
* ��    ��: ��
*************************************************************************/
void FilterlifeTime(void)
{
	if(FstModeFlag)
	{
		if(gstFilte.type   == ROFilter)
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

		if( gstFilte.type   == ROFilter)
		{
			
			gstFilte.RO_FlowRateCnt++;
		}
		else
		{
			gstFilte.Mix_FlowRate++;
		}
	}
	

}

void FilterlifeDayTime(void)
{
	if(FstModeFlag)
	{
		if( gstFilte.type   == ROFilter)
		{
			gstFilte.RO_DayCnt+=86439ul;//86400ul-1
		}
		else
		{
			gstFilte.Mix_DayCnt+=86439ul;//86400ul-1
		}
	}
	else
	{
		if( gstFilte.type  == ROFilter)
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


extern void CloseAllLED(void);
void TimeReminder(void)
{
	static ST_TimeReminder sTimeReminderCnt ={0};
	u8  temp =gstAM901.Run.Bit.DayResidualPercent;

	if(FactoryModeFlag ||!SysRunFlag)
	{
		return ;
	}
 
	
	if(gstAM901.Run.Bit.DayResidualPercent < temp)
		temp = gstAM901.Run.Bit.DayResidualPercent;
	
		
	if( gstFilte.type  == ROFilter)
	{
		if(temp <= 1)//��������
		{
				if(!sTimeReminderCnt.Bit.buzeerOn1 && KeySwitchFlag1 /*&& ShuiLongTouOpen*/)
				{
					sTimeReminderCnt.Bit.buzeerOn = 0;
					sTimeReminderCnt.Bit.buzeerOn1 = 1;
					Buzzer1Flag = 1;
				}
				setLED(2,BlueColor,lightOff,0);
				setLED(2,RedColor,lightOn,0);
		}
	
		else if(temp <=3)//�����쵽��
		{
		
				
				if(!sTimeReminderCnt.Bit.buzeerOn && KeySwitchFlag1/*&& ShuiLongTouOpen*/)
				{
					sTimeReminderCnt.Bit.buzeerOn = 1;
					sTimeReminderCnt.Bit.buzeerOn1 = 0;
					Buzzer4Flag = 1;
					setLED(2,BlueColor,lightOff,0);
				  setLED(2,RedColor,blink,_4S_Per100MS);
				}
				
				
				sTimeReminderCnt.Bit.cnt++;
				
				if(sTimeReminderCnt.Bit.cnt >= 10)
					sTimeReminderCnt.Bit.cnt = 0;
				
		}
		else
		{
				setLED(2,RedColor,lightOff,0);
				setLED(1,BlueColor,lightOff,0);
				setLED(2,BlueColor,lightOn,0);	
		}
	}
	else
	{
		
		
		if(temp <= 3)//��������
		{
				
			  setLED(1,RedColor,lightOn,0);  
				if(!sTimeReminderCnt.Bit.buzeerOn1 && KeySwitchFlag1/*&& ShuiLongTouOpen*/)
				{
					sTimeReminderCnt.Bit.buzeerOn = 0;
					sTimeReminderCnt.Bit.buzeerOn1 = 1;
					Buzzer1Flag = 1;
				}

		}
	
		else if(temp <= 10)//�����쵽��
		{

				if(!sTimeReminderCnt.Bit.buzeerOn && KeySwitchFlag1/*&& ShuiLongTouOpen*/)
				{
					sTimeReminderCnt.Bit.buzeerOn = 1;
					sTimeReminderCnt.Bit.buzeerOn1 = 0;
					Buzzer4Flag = 1;
					setLED(1,BlueColor,lightOff,0);
					setLED(1,RedColor,blink,_4S_Per100MS);
				}
				
				
				sTimeReminderCnt.Bit.cnt++;
				
				if(sTimeReminderCnt.Bit.cnt >= 10)
					sTimeReminderCnt.Bit.cnt = 0;
		}
		else
		{
				setLED(2,BlueColor,lightOff,0);
				setLED(1,RedColor,lightOff,0);
				setLED(1,BlueColor,lightOn,0);
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
* ��������:	RestFilter
* ����˵��:	��λ��о
* ��    ��: ��	
* ��    ��: ��
*************************************************************************/
void RestFilter(void)
{
		WriteTempEeprom(0);
		if(gstFilte.type == ROFilter)
		{			
					gstFilte.RO_DayCnt =0;
					gstFilte.RO_FlowRateCnt = 0;
					setLED(2,BlueColor,lightOff,0);
					setLED(2,BlueColor,blink,_4S_Per100MS);
		}
		else
		{
					gstFilte.Mix_DayCnt =0;
					gstFilte.Mix_FlowRate = 0;	
					setLED(1,BlueColor,lightOff,0);
					setLED(1,BlueColor,blink,_4S_Per100MS);
		}
}


/************************************************************************* 
* ��������:	PaiShuiProcess
* ����˵��:	������ˮ
* ��    ��: ��	
* ��    ��: ��
*************************************************************************/


void PaiShuiProcess(void)//1S������һ��
{
	static u8 sTDS_Time =0;
	
	test_TDS(gstADCollect.fChunShui,&gstADCollect.tds_ChunShui);
	test_TDS(gstADCollect.fYuanShui,&gstADCollect.tds_YuanShui);//���10����ƽ��ֵ
	test_TDS(gstADCollect.fJieShui,&gstADCollect.tds_JieShui);//���10����ƽ��ֵ
	test_TDS(gstADCollect.fLouShui,&gstADCollect.tds_LouShui);//���10����ƽ��ֵ
	TDS_ChunShuiFalg = 1;
	if(sTDS_Time < 60)
	{
			TDS_YuanShuiFalg =1;
			
			if(gstADCollect.tds_YuanShui <= TDS_100)
			{	
				gstAM901.PaiShuiDaoJiShi = _9Min_Per1S;
			}

			else if(gstADCollect.tds_YuanShui <= TDS_300)
			{
				gstAM901.PaiShuiDaoJiShi = _8Min_Per1S;//KongShuiFa_IO = 1;	
			}

			else if(gstADCollect.tds_YuanShui <= TDS_500)
			{
				gstAM901.PaiShuiDaoJiShi = _4Min_Per1S;//KongShuiFa_IO = 1;	
			}
			else//�ϵ�10���ڣTTDS <100
			{
				gstAM901.PaiShuiDaoJiShi  =0;//KongShuiFa_IO = 1;	
			}
	}
	if(gstADCollect.tds_JieShui > TDS_1000)
	{
			gstAM901.PaiShuiDaoJiShi = 1;
	}

	if(sTDS_Time < 60)
	{
		sTDS_Time++;
	}		
	else
	{
		if(gstAM901.PaiShuiDaoJiShi > 0 && (gstAM901.PaiShuiDaoJiShi !=0xffff))
		{
			gstAM901.PaiShuiDaoJiShi--;
		}
		
		else
		{
			KongShuiFa_IO = 1;
			gstAM901.PaiShuiDaoJiShi =0xffff;
		}
		
		
		if(!KeySwitchFlag)//ˮ��
		{
				KongShuiFa_IO = 0;
		}
	}
	

}



void MakeWaterProcess(void)
{		
		static u8 sMWaterStep = 5;
		static u16 sMWaterStepCnt = 0;

		if(ErrowFlag||FactoryModeFlag || !SysRunFlag)
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
						ChuShuiFlag = 1;
						
					}			
				break;
				
				case 2:
				
					if(!KeySwitchFlag)//ˮ��
					{
							if(!ErrowFlag)
							{
									gstAM901.LianXuZhiShuiCnt =0;
							}							
							sMWaterStep++;
							LED4_L =0;
					}

					if(_20Min_Per50MS < gstAM901.LianXuZhiShuiCnt++)
					{
						ChuShuiFlag = 0;
						ErrowFlag =  1;
						CloseFuZai();
						setLED(3,RedColor ,blink,_4S_Per100MS);
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
						ChuShuiFlag = 0;
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

// TDSֵУ׼

void TDS_JiaoZhun(u16 v0,u16 AdValue_0,u16 v1,u16 AdValue_1)
{
	float a,b;

	
		if(v0 > v1)
		{
				a= (v0- v1);
			  a= a*4095/(float)(AdValue_0-AdValue_1);
				b = AdValue_1/(float)4095;
				b = b*a;
				b =  v1- b;
		}
		else
		{
				a= (v1- v0);
			  a= a*4095/(float)(AdValue_1-AdValue_0);
			  b = AdValue_1/(float)4095;
				b = b*a;
				b =  v1- b;
				b= 0;
		}
	
}

void PowerDownProcess(void)
{
				EA =0;
        GPIO_Init(GPIO0, GPIO_PIN_2,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO0, GPIO_PIN_4,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO4, GPIO_PIN_3,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO4, GPIO_PIN_2,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO4, GPIO_PIN_1,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO3, GPIO_PIN_7,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO3, GPIO_PIN_5,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO3, GPIO_PIN_4,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO3, GPIO_PIN_3,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO3, GPIO_PIN_2,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO3, GPIO_PIN_1,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO1, GPIO_PIN_0,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO1, GPIO_PIN_2,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO1, GPIO_PIN_4,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO2, GPIO_PIN_5,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO2, GPIO_PIN_6,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO2, GPIO_PIN_7,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO0, GPIO_PIN_0,GPIO_MODE_IN_HI);
        GPIO_Init(GPIO0, GPIO_PIN_1,GPIO_MODE_IN_HI);        
        WriteTempEeprom(1);
        EA =1;     	
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
extern void NoKeyProcess(void);
u16 gPPM1 = 110 ,gPPM2 = 50 ,gAD1 = 1420 ,gAD2 =1120;

void sysRuning(void)
{	

	EventCollect();
    switch(PopEvent())
    {    
			case evPowerDown:
			{
					PowerDownProcess();
			}				
			break;
			
        case ev5MS:       
					BuzzerProcess();	
					
        break;  

				
        case ev50MS:
        {
						KeySelect();
						KeyRest();
						KeyCancelRestKey();
						NoKeyProcess();
						KeyGaoYaSwitch();
						MakeWaterProcess();				
        }
        break;
				
        case ev100MS:
        {
						
						TimeReminder();
						TDS_Calulate();
						UART0_SendData(); 
						LED_Process();	
        }
        break;
				
        case ev1S:
        {
						TDS_JiaoZhun(gPPM1,gAD1,gPPM2,gAD2);
						FilterSysRunTime();//��о��������
						FirstPowerOnProcess();//�״��ϵ��ϴ15����
						FactoryProcess();
						SysRunFlag =1;
						PaiShuiProcess();
        }
        break;
   
        default:
        break;
    }
}
