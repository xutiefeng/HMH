//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//     文件名称    :
//     模块功能    :
//     更正日期    : 2021/2/20
//     版本           : V0.1
//************************************************************
#include"..\H\OTADriver.h"


#if (LDROM_MODE == 0)
EXTRN data(InterruputMode)

//************************************************************

CSEG AT 0x0003

ORG      0003H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      000BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0013H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      001BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0023H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      002BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0033H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      003BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0043H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      004BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0053H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      005BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0063H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      006BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0073H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      007BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0083H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      008BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      0093H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      009BH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      00A3H
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
ORG      00ABH
JNB      InterruputMode.0, $ +6
LJMP     $ -03H + INTERRUPT_ADDRESS_APP
AJMP     $ -06H + INTERRUPT_ADDRESS_BOOT
#endif
	
END
