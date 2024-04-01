#include <SC95F8x1xB_C.H>
#include	"key.h"

#include "includeall.h"



extern unsigned	char	xdata		SOCAPI_TouchKeyStatus;					//API接口状态：bit7-一轮扫描完成标志  1:完成 	0：未完成

static u8 sKeyValue = 0;
static u8 sKeySelectCnt=0;
static u8 sKeyReskCnt=0;


/****************************************************************************************************************************************** 
* 函数名称:	KeyRest
* 功能说明:	长按复位键3秒，蜂鸣器短鸣一声：0.2秒
						长按复位键5秒，蜂鸣器长鸣一声：2秒
* 输    入: 无	
* 输    出: 无
* 注意事项:
******************************************************************************************************************************************/
void KeyRest(void)
{	
		static u8 sUseFactoryCnt =0;
	  static u8 sKeyReskCnt =0;//按下计时
	  static u8 sKeyPressCnt =0;//按下次数

		if(sUseFactoryCnt < 0xff)
			sUseFactoryCnt++;
		
		if(NoKeyPressFalg)
		{
///////////////////////////////退出工程模式////////////////////////////////////////		
			if(sKeyReskCnt >0 && sKeyReskCnt < 10 )
			{

			
				if(sUseFactoryCnt < _10S_Per50MS && !FactoryModeFlag)	//上电10秒内		
				{
					
					sKeyPressCnt++;
					
					Buzzer3Flag = 1;
					
					if(sKeyPressCnt >4 )//复位键按五次进入工程模式
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
* 函数名称:	KeySelect
* 功能说明:	选择按键
* 输    入: 无	
* 输    出: 无
* 注意事项: 短按有效
******************************************************************************************************************************************/
	static u8 sKeyNoPress_Cnt =0;
void KeySelect(void)
{
	
		sKeyNoPress_Cnt = 0;
		if(NoKeyPressFalg)
		{
			if(sKeySelectCnt >0 && sKeySelectCnt < 10 )
			{

				///////////////////////////按键逻辑处理//////////////////////////////
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
* 函数名称:	KeyProcess
* 功能说明:	处理按键长按短按功能
* 输    入: 无	
* 输    出: 无
* 注意事项: 触发时间50ms
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

