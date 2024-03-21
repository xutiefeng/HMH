
#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#define AM901_8616B 1



#if AM901_8616B

#include "..\FWLib\SC95F_Lib\inc\sc95f_conf.h"
#include "sc95f_iap.h"
#include "IAP_Option_EW.h"

#define Chanel_IO1 P33
#define Chanel_IO2 P35//

#define led1_io       P37
#define led2_io       P34
#define led3_io       P32
#define led4_io       P31//

#define Redled_io     P10
#define Blueled_io    P12

#define JinShuiFa_io  P42
#define Pump_io       P40

#define FeiShuiFa_IO  P41
#define KongShuiFa_IO P43
#define HuiLiuFa_io   P53

#define InputKey_io  P05   
#define DLy_8616B() _nop_();\
					_nop_();\
					_nop_();\
					_nop_();\
					_nop_();\
					_nop_();\
					_nop_();\
					_nop_();\
					_nop_();\
					_nop_()
#endif

#endif
