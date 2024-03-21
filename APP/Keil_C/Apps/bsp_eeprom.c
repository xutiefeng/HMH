

#include <string.h>
#include "bsp_eeprom.h"
#include "globe.h"

#define WriteToEEprom(X)   				WriteEEpromData(EEPROM##X##_ADDR,gstAM901.stFilter,EEPROM_DATA_LEN) 
#define ReadFromEEprom(X)  				IAP_ReadByteArray(EEPROM##X##_ADDR,(u8 *)gstAM901.stFilter,EEPROM_DATA_LEN,IAP_MEMTYPE_EEPROM)
#define EraseEEprom(X)            IAP_EEPROMSectorEraseOption(EEPROM##X##_ADDR)

#define EEPROM_DATA_LEN 20
#define Block_Size 0XFFff
/**************************************************
*函数名称:void IAP_SectorEraseOption(Address,Data,IAP_MemType)
*函数功能:IAP扇区擦除
*入口参数:uint32_t:Address:IAP操作地址
*         IAP_MemType_TypeDef:IAP_MemType:IAP操作对象
*出口参数:void
**************************************************/

u8 testAyy[]={0x60,0x33,0x99,0x90,0x90,0x90,0xD5,0xFA};
#define _INIT_DADA 0x5b
#define  EEPROM0_ADDR 0ul
#define  EEPROM1_ADDR 0x200UL
#define  EEPROM2_ADDR 0x400UL
#define  EEPROM3_ADDR 0x600UL
#define EEPROM_SIZE 0XFFFF

static u8 FromEeppAry[8];


u8 WriteEEpromData(u32 x,u8 *p,u8 len)
{
	u8 i ,FromEepromData;
	u8 *pData = p;
		
	
	
	for(i= 0;i < 20; i++)
	{
		loop:		
		IAP_EEPROMProgramByteOption(x+i,pData[i]);
		FromEepromData =IAP_ReadByte(x+i,IAP_MEMTYPE_EEPROM);
		if(FromEepromData != pData[i] )
			goto loop;
	}
	return 1;
}




void WriteTempEeprom(void)
{
	//IAP_ReadByteArray(EEPROM_ADDR,FromEeppAry,8,IAP_MEMTYPE_EEPROM);
	if(!gstAM901.Run.Bit.EepromBlok )
	{
			gstAM901.stFilter->type++;
			if(WriteToEEprom(0) == 0)
			{
					if(WriteToEEprom(0) == 0)
					{
							if(WriteToEEprom(0) == 0)
							{
									WriteToEEprom(1);
							}

					}
			}
	}
 	 
 	else
	{
			gstAM901.stFilter->type  = 0;
			WriteToEEprom(1);
	}
 	  


//while(1);
}


u8 readEEprom(u32 x)
{
	 IAP_ReadByteArray(x,(u8 *)gstAM901.stFilter,EEPROM_DATA_LEN,IAP_MEMTYPE_EEPROM);
	 return gstAM901.stFilter->type;
}

/****************************************************************************************************************************************** 
* 函数名称:	 ReadTempEeprom
* 功能说明:  读取EEPROM接口
* 输    入: 无	
* 输    出: 无
******************************************************************************************************************************************/



void EEpromInit(void)
{
		u8 a;
		if(ReadFromEEprom(0) == 0)
		{
				ReadFromEEprom(0);
		}
		if(gstAM901.stFilter->type< EEPROM_SIZE)
		{
				gstAM901.Run.Bit.EepromBlok = 0;
				EraseEEprom(0);
				return;
		}
		else
		{
				if(ReadFromEEprom(1) == 0)
				{
						ReadFromEEprom(1);
				}
		}
		
	  if(gstAM901.stFilter->type < EEPROM_SIZE)//5a 表示上一次用的是EEPROM_ADDR 块，标记成F5;这一次用EEPROM1_ADDR
		{
				gstAM901.Run.Bit.EepromBlok = 1;//gstAM901.Run.Bit.EepromBlok = 1;
				EraseEEprom(1);//EraseEEprom(1);
		}
		else//首次使用
		{
					gstAM901.Run.Bit.EepromBlok = 0;
					//EraseEEprom(0);
					EraseEEprom(1);
					memset((u8 *)gstAM901.stFilter,0,EEPROM_DATA_LEN);
					FirstPownOnFlag = 1;
		}
}

void ReadTempEeprom(void)
{
		
	 
}

/****************************************************************************************************************************************** 
* 函数名称:	 ReadBingXangState
* 功能说明:  读取冰箱状态	 
* 输    入: 无	
* 输    出: 无
******************************************************************************************************************************************/
void ReadBingXangState(void )
{
   
}

/****************************************************************************************************************************************** 
* 函数名称:	 BSP_EEPROM_Initialize
* 功能说明:  初始化EEPROM
* 输    入: 无	
* 输    出: 无
******************************************************************************************************************************************/
void BSP_EEPROM_Initialize ( void )
{
    
	
}




/****************************************************************************************************************************************** 
* 函数名称:	EepromProcess
* 功能说明:	每个100ms检测一次，是否有开机状态变化，温度设置的变化
* 输    入: 无	
* 输    出: 无
******************************************************************************************************************************************/
void EepromProcess(void)
{
		
}
