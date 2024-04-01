//*************************************************************************************************
//  Copyright (c) 	????????????
//	????	:  S_TouchKeyCFG.h
//	??		: 
//	????	:  ???????
// 	??		:  V0.1
// 	????	:
//************************************************************************************************
#ifndef __S_TOUCHKEYCFG_H__
#define __S_TOUCHKEYCFG_H__
#define		    SOCAPI_SET_TOUCHKEY_TOTAL					  4 	
#define			SOCAPI_SET_TOUCHKEY_CHANNEL					  0x0000000f																																			 
unsigned int  code TKCFG[17] = {0,0,0,7,10,3000,300,200,2,0,0,2,100,10,65535,65535,10}; 
unsigned char code TKChannelCfg[4][8]={
0x03,0x32,0x04,0x08,0x1f,0x05,0x01,0xd1,
0x03,0x32,0x04,0x08,0x1f,0x05,0x01,0xd1,
0x03,0x32,0x04,0x08,0x1f,0x05,0x01,0xd1,
0x03,0x32,0x04,0x08,0x1f,0x05,0x01,0xd1,

};
#endif 


