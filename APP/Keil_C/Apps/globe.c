
#include "includeall.h"
#include "globe.h"








//////////////��С���////////////////////
ST_PushPop gstPushPop;
volatile byte	gbFlagData[10];
volatile sysTick_t gstRDsysTick;
Uchar gEventBuffer[EVENT_BUFFER_LENGTH];



//////////////��С���////////////////////

ST_Filter gstFilte;
volatile ST_AM901  gstAM901;
volatile STAD_Collect   gstADCollect;
u8 gKeyValue;
byte gSendData[8]={0};
#if 0
signed char gTempHeat,gTempAD;
STFactory   gstFactory; 

uint8_t gErrorCode;
byte gbNoInitFlagData[10];

//////////////AD////////////////////
volatile uint16_t ADtable[ADMax];


//////////////����LED����////////////////////
u8  gLightAdjust = 0xc0;
#endif





