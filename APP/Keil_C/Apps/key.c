#include <SC95F8x1xB_C.H>
#include	"key.h"

#include "includeall.h"



extern unsigned	char	xdata		SOCAPI_TouchKeyStatus;					//API�ӿ�״̬��bit7-һ��ɨ����ɱ�־  1:��� 	0��δ���

static u8 sKeyValue = 0;
static u8 sKeySelectCnt=0;
static u8 sKeyReskCnt=0;


/****************************************************************************************************************************************** 
* ��������:	KeyRest
* ����˵��:	������λ��3�룬����������һ����0.2��
						������λ��5�룬����������һ����2��
* ��    ��: ��	
* ��    ��: ��
* ע������:
******************************************************************************************************************************************/
void KeyRest(void)
{	
		static u8 sUseFactoryCnt =0;
	  static u8 sKeyReskCnt =0;//���¼�ʱ
	  static u8 sKeyPressCnt =0;//���´���

		if(sUseFactoryCnt < 0xff)
			sUseFactoryCnt++;
		
		if(NoKeyPressFalg)
		{
///////////////////////////////�˳�����ģʽ////////////////////////////////////////		
			if(sKeyReskCnt >0 && sKeyReskCnt < 10 )
			{

			
				if(sUseFactoryCnt < _10S_Per50MS && !FactoryModeFlag)	//�ϵ�10����		
				{
					
					sKeyPressCnt++;
					
					Buzzer3Flag = 1;
					
					if(sKeyPressCnt >4 )//��λ������ν��빤��ģʽ
					{
						FactoryModeFlag = 1;					
						sKeyReskCnt = 0;
						KeyRestFlag = 0;
					}		
				}

				sKeyReskCnt = 0;
				KeyRestFlag = 0;	
				LongPress_5sFlag = 0;
				LongPress_3sFlag = 0;

			}
		}	
		
		if(KeyRestFlag)
		{
			sKeyReskCnt++;
		}
			
			
		else if(sKeyReskCnt > _10S_Per50MS || !KeyRestFlag)
		{
				return;
		}
				
		
		if(sKeyReskCnt == _3S_Per50MS)
		{
					Buzzer2Flag = 1;
					LongPress_3sFlag = 1;
					#if 0
					LED1_R = 0;
					LED2_R = 0;
					LED3_R = 0;
					LED4_R = 0;
					LED1_L = 1;
					LED2_L = 1;
					LED3_L = 1;
					LED4_L = 1;
					#endif
		}
		else if(sKeyReskCnt == _5S_Per50MS)
		{
					Buzzer3Flag = 0;
					LongPress_3sFlag = 1;
					#if 0
					LED1_R = 1;
					LED2_R = 1;
					LED3_R = 1;
					LED4_R = 1;
					LED1_L = 0;
					LED2_L = 0;
					LED3_L = 0;
					LED4_L = 0;
					#endif
		}
			
}



/****************************************************************************************************************************************** 
* ��������:	KeySelect
* ����˵��:	ѡ�񰴼�
* ��    ��: ��	
* ��    ��: ��
* ע������: �̰���Ч
******************************************************************************************************************************************/
	static u8 sKeyNoPress_Cnt =0;
void KeySelect(void)
{
	
		sKeyNoPress_Cnt = 0;
		if(NoKeyPressFalg)
		{
			if(sKeySelectCnt >0 && sKeySelectCnt < 10 )
			{

				///////////////////////////�����߼�����//////////////////////////////
					if(LongPress_5sFlag)
					{
						sKeyNoPress_Cnt++;
					}
						
					else if(LongPress_3sFlag)
					{
						sKeyNoPress_Cnt++; 	
						if(gstFilte.type == ROFilter)
						{
							gstFilte.type = MixFilter;
						}
						else
						{
							gstFilte.type = ROFilter;
						}
						Buzzer2Flag = 1;
					}			
			}

			
			
			sKeySelectCnt = 0;
			KeySelecetFlag = 0;
		}
			
		if(KeySelecetFlag)
		{
			sKeySelectCnt++;
		}
			
		else if(sKeySelectCnt > _10S_Per50MS || !KeySelecetFlag)
		{
				return;
		}
}

void KeyGaoYaSwitch(void)
{
		if(InputKey_io == 0 && !KeySwitchFlag1)
		{
				KeySwitchFlag1 = 1;
		}
		else if(InputKey_io == 0 && KeySwitchFlag1)
		{
		
			DLy_8616B();
				
			if(InputKey_io == 0)
			{
				KeySwitchFlag  = 1;
			}
				
			else
			{
				KeySwitchFlag1 = 0;
			}
				 
		}
		else
		{
				KeySwitchFlag = 0;
				KeySwitchFlag1= 0;
		}
		
}

/****************************************************************************************************************************************** 
* ��������:	KeyProcess
* ����˵��:	�����������̰�����
* ��    ��: ��	
* ��    ��: ��
* ע������: ����ʱ��50ms
******************************************************************************************************************************************/
void NoKeyProcess(void)
{ 
	static u8 sNoKeytCnt=0;	
	if(NoKeyPressFalg)
	{
		sNoKeytCnt++;
	}
	
		if(sNoKeytCnt > _10S_Per50MS)
		{
				LongPress_3sFlag = 0;
				LongPress_5sFlag = 0;		
		}	
	
}

