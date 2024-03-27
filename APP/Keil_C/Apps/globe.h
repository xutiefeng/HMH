#ifndef GLOBLE_H
#define GLOBLE_H

#include "includeall.h"

/*
////LI GONG SDK
#include "device_registers.h"          
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
*/

#include "config.h"


//#define  TAB_ISUM(tab)              (sizeof(tab)/sizeof(tab[0]))

#define Byte8	byte
#define u8	unsigned char
#define u16	unsigned short
#define Uchar	unsigned char
#define u32	unsigned int
#define EVENT_BUFFER_LENGTH  20


typedef struct
{
	union
	{
			struct
			{
				u32 Data		 		:12;
				u32 cnt         :4;
			}Bit;
			u32         All;
	}buf;  
	u32   type;
 	unsigned long     Mix_DayCnt;//按天计算
	unsigned long     RO_DayCnt;//按天计算
 	unsigned long	  Mix_FlowRate;//按流量计算
 	unsigned long	  RO_FlowRateCnt;//按流量计算
 	
 	
}ST_Filter;

typedef struct
{
		
		union
		{
			struct
			{
				u8 RunFlag 		       :1;
				u8 EepromBlok          :1;
				u8 DayResidualPercent  :7; 
				u8 FlowResidualPercent :7;
			}Bit;
			u16         All;
		}Run;		
		
		ST_Filter *stFilter;
		
}ST_AM901;

typedef enum
{
    AD_YuanShui,
    AD_ChunShui,
    AD_JieShui,
		AD_LouShui,
    ADMax = AD_LouShui ,
}BSP_ADChangeEnum;


//typedef enum
//{
//		EEPROM_ADDR  = 0ul,
//		EEPROM1_ADDR = 0x00001000
//}EM_Addr;


typedef struct
{

	 u8 dis1  ;
	 u8 dis2  ;
	 u8 dis3  ;
	 u8 CurMode;
	
}STDisplayCode;

typedef struct 
{
    u16   YuanShui[10] ;
    u16 	ChunShui[10];
    u16 	LouShui[10];
    u16		JieShui[10];
   	u16 	fYuanShui;
		u16  	fChunShui;
		u16  	fLouShui;
		u16  	fJieShui;
    u8 		ADAvergeCnt;
	
}STAD_Collect;

typedef struct 
{
		u8  FaultCnt;
    u8  ErrorCodeDisplayData;  
    u8  ErrorToNomalCnt;
    u8  ErrorCodeData;
    u8  YSJErrowCnt;    
    u8  LowLevelTimeCnt;
    u8  HighLevelTimeCnt;
    u16 ErrowCnt;
}ST_MakeWater;
	
typedef struct 
{
    u8 WriteEventCount;
    u8 ReadEventCount;
}ST_PushPop;

typedef union 
{
    u32	all;
    struct
    {
        u8 HisEeprom        :8;
        u8 EEpromRun        :3;
				u8 HisPwrOFF        :1;
        u8 SysRun           :1;
        u8 cool             :1;
        u8 heat             :1;
				u8 Lock            	:1;
        signed char 	HeatTempSet      :8;
        signed char   CoolTempSet      :8;
    }Bit;
}STHeatCoolTemp;


typedef struct    
{
    u8 step			;
    u8 sFactoryTime	;
    u8 FactoryCnt	;
		u8 step1			;
}STFactory;










typedef struct 
{
  u8 DisplayMode;
	signed char CompensationValue;
}STSetVaue;

typedef struct 
{
		u8 PowerOnTimer;
		u8 RecoverTimer;//u8 Run;
		union
		{
			struct
			{
				u8 RunFlag 			:1;
				u8 DisplayModeFlag 	:1;
				u8 DisplayValueFlag :1;
			}Bit;
			u8         All;
		}Run;
		
		u8 mode;
		u8 value;
		
}STSetTempCompensation;

typedef struct
{
    u8  KeyNum;
    u8  Value;
}STLingMin;




typedef union
{
	u8	all;
	struct
	{
		u8 REV         :6;
		u8 LightByte	:2;
	}Bit;
}LightByte;

typedef union
{
	u8	all;
	struct
	{
		u8 His         :1;
		u8 cnt			:7;
        
	}Bit;
}ST_Beep;

/////////////////////////
 typedef union
{
	u8	all;
	struct
	{
		u8 b0	:1;
		u8 b1	:1;
		u8 b2	:1;
		u8 b3	:1;
		u8 b4	:1;
		u8 b5	:1;
		u8 b6	:1;
		u8 b7	:1;
	}Bit;
}byte;


typedef enum
{
    BSP_USART0,               
    BSP_USART1,               
    BSP_USART2,               
    BSP_USART3,               
    BSP_UART_MAX = BSP_USART3
}BSP_UART_Port_t;

typedef enum
{
    BSP_BrshMotor,
    BSP_WaterPump,
    BSP_WaterAbsorption,
}MainCircuit_t;

typedef struct
{
  u32 tickCnt;
	u8 tickCnt1;
	u8 tickCnt2;
	u8 tickCnt3;
  u8 tickCnt4;
	u8 tickCnt5;
	Evnt_t parameter;
  void  (*pTickCBFunc)(Uchar evt);                    
 	void  (*pTickEvent)(void); 
}sysTick_t;


typedef union
{
	u8	all;
	struct
	{
		u8 buzeerOn	:1;
		u8 buzeerOn1	:1;
		u8 cnt		:6;
	}Bit;
}ST_TimeReminder;



//////////////最小框架////////////////////
extern ST_PushPop gstPushPop;
extern volatile byte	gbFlagData[10];
extern volatile sysTick_t gstRDsysTick;
extern Uchar gEventBuffer[EVENT_BUFFER_LENGTH];

//////////////最小框架////////////////////
extern ST_Filter gstFilte;
extern volatile STAD_Collect   gstADCollect;
extern u8 gKeyValue;
extern ST_AM901  gstAM901;
extern byte gSendData[8];
#if 0
signed char gTempHeat,gTempAD;
STFactory   gstFactory; 
byte gSendData[3];
uint8_t gErrorCode;
byte gbNoInitFlagData[10];

//////////////AD////////////////////
volatile uint16_t ADtable[ADMax];


//////////////调节LED亮度////////////////////
u8  gLightAdjust = 0xc0;
#endif



#define  RDSystickProcess() gstRDsysTick.tickCnt++; \
	gstRDsysTick.tickCnt1++;\
	gstRDsysTick.tickCnt2++;\
	gstRDsysTick.tickCnt3++;\
  gstRDsysTick.tickCnt4++;\
	gstRDsysTick.tickCnt5++;\
	if(0 == gstRDsysTick.tickCnt5%3){Ev3MSFlag = 1; gstRDsysTick.tickCnt5  = 0;}\
  if(0 == gstRDsysTick.tickCnt4%5){Ev5MSFlag = 1; gstRDsysTick.tickCnt4  = 0;}\
	if(0 == gstRDsysTick.tickCnt3%20){Ev20MSFlag = 1;gstRDsysTick.tickCnt3  = 0;}\
	if(0 == gstRDsysTick.tickCnt2%50){Ev50MSFlag = 1;gstRDsysTick.tickCnt2  = 0;}\
	if(0 == (gstRDsysTick.tickCnt)%1000){Ev1SFlag = 1;gstRDsysTick.tickCnt = 0;}\
	if(0 == (gstRDsysTick.tickCnt1)%100){Ev100MSFlag = 1;gstRDsysTick.tickCnt1 = 0;}\
	ADCollectProcess();
	
	
#define  SCUCESS        1
#define  FAIL           0

#if 1//第一版显示板
#define KEY1_MASK 0x01
#define KEY2_MASK 0x02
#define KEY3_MASK 0x04
#define KEY4_MASK 0x08
#define KEY5_MASK 0x10
#define KEY6_MASK 0x20
#define KEY7_MASK 0x40
#define KEY8_MASK 0x80
#else
#define KEY1_MASK 0x20
#define KEY2_MASK 0x10
#define KEY3_MASK 0x08
#define KEY4_MASK 0x04
#define KEY5_MASK 0x02
#define KEY6_MASK 0x01
#define KEY7_MASK 0x40
#define KEY8_MASK 0x80
#endif

#define  _150MS_Per5MS     30
#define  _100MS_Per5MS     20

#define  _100MS_Per20MS    5
#define  _120MS_Per20MS    6
#define  _140MS_Per20MS    7
#define  _200MS_Per20MS    10
#define  _220MS_Per20MS    11
#define  _240MS_Per20MS    12
#define  _260MS_Per20MS    13
#define  _280MS_Per20MS    14
#define  _3S_Per20MS      50*3
#define  _4S_Per20MS      50*4

#define  _500MS_Per50MS  9
#define  _10S_Per50MS  20*10
#define  _5S_Per50MS   20*5
#define  _3S_Per50MS   20*3
#define  _2S_Per50MS   20*2
#define  _1S_Per50MS   20*1
#define  _30S_Per50MS  20*30
#define  _1Min_Per50MS   20*60
#define  _5Min_Per50MS   20*300

#define  _90Min_Per100MS      10*60*90
#define  _15Min_Per100MS      10*60*15
#define  _3S400MS_Per100MS         34
#define  _15S_Per100MS        10*15
#define  _30S_Per100MS        10*30
#define  _3Min_Per100MS       10*60*3
#define  _13Min_Per100MS      10*60*13
#define  _20Min_Per100MS      10*60*20
#define  _30Min_Per100MS      10*60*30
#define  _40Min_Per100MS      10*60*40

#define  _200MS_Per100MS   2
#define  _500MS_Per100MS   5
#define  _2S_Per100MS   10*2
#define  _1S_Per100MS   10*1
#define  _10S_Per100MS   10*10
#define  _25S_Per100MS  10*20

#define  _1h_Per1S      60*60
#define  _30Min_Per1S   60*30
#define  _60Min_Per1S   120*30
#define  _15Min_Per1S   60*15
#define  _20Min_Per1S   60*20
#define  _5Min_Per1S	60*5
#define  _2Min_Per1S	60*2
#define  _4Min_Per1S	60*4
#define  _1Min_Per1S	60*1
#define  _10S_Per1S	    10

#define  _35S_Per1S		35
#define  _50S_Per1S		50
#define  _3Min_Per1S		60*3
#endif
