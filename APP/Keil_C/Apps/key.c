#include <SC95F8x1xB_C.H>
#include	"key.h"

#include "includeall.h"



extern unsigned	char	xdata		SOCAPI_TouchKeyStatus;					//API�ӿ�״̬��bit7-һ��ɨ����ɱ�־  1:��� 	0��δ���

static u8 sKeyValue = 0;

static u8 sKeyReskCnt=0;


/****************************************************************************************************************************************** 
* ��������:	RestKeyProcess
* ����˵��:	��λ�����£�ȡ��������о
* ��    ��: ��	
* ��    ��: ��
* ע������:
******************************************************************************************************************************************/
void RestKeyProcess(void)
{
		//LongPress_3sFlag = 0;
}

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
			if(sKeyReskCnt >0 && sKeyReskCnt < 20 )
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
				RestKeyProcess();//�̰�����
			}
			else
			{
					sKeyReskCnt = 0;
			}
		}	
		
		if(KeyRestFlag)
		{
			KeyRestFlag = 0;
			
			sKeyReskCnt++;
		}
			
			
		else if(sKeyReskCnt > _10S_Per50MS || !KeyRestFlag)
		{
				return;
		}
				
		
		if(sKeyReskCnt == _3S_Per50MS)
		{
					Buzzer2Flag = 1;
					LongPress_5sFlag = 0;
					LongPress_3sFlag = 1;
		}
		else if(sKeyReskCnt == _5S_Per50MS)
		{
					Buzzer3Flag = 0;
					LongPress_5sFlag = 1;
					LongPress_3sFlag = 0;
		}
			
}



/****************************************************************************************************************************************** 
* ��������:	KeySelect
* ����˵��:	ѡ�񰴼�
* ��    ��: ��	
* ��    ��: ��
* ע������: �̰���Ч
******************************************************************************************************************************************/
extern void setLED(u8 num,emColor color ,U_LED state,u8 time);




void KeySelect(void)
{
	
		static u8 sKeySelectCnt=0;
		
		if(KeySelecetFlag)//���°���
		{
			KeySelecetFlag = 0;
			sKeySelectCnt++;
		}

		else if(NoKeyPressFalg)//�ɿ�����
		{
			if(sKeySelectCnt >0 && sKeySelectCnt < 10 )
			{

				///////////////////////////�����߼�����//////////////////////////////
					if(LongPress_5sFlag)
					{	
							LongPress_5sFlag = 0;
							RestFilter();
					}
						
					else if(LongPress_3sFlag)
					{
						if(gstFilte.type == ROFilter)
						{
							gstFilte.type = MixFilter;
							setLED(1,BlueColor,blink,_4S_Per100MS);
						}
						else
						{
							gstFilte.type = ROFilter;
							setLED(2,BlueColor,blink,_4S_Per100MS);
						}
						Buzzer2Flag = 1;
						
					}			
			}

			
			
			sKeySelectCnt = 0;
			KeySelecetFlag = 0;
		}
}


/****************************************************************************************************************************************** 
* ��������:	KeyGaoYaSwitch
* ����˵��:	����ѹ���رպϻ��ǶϿ�
* ��    ��: ��	
* ��    ��: ��
* ע������: �̰���Ч
******************************************************************************************************************************************/

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
		else
		{
			sNoKeytCnt = 0;
		}
	
		if(sNoKeytCnt > _10S_Per50MS)
		{
				LongPress_3sFlag = 0;
				LongPress_5sFlag = 0;		
		}	
	
}

