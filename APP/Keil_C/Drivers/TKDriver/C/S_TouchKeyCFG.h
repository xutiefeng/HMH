//*************************************************************************************************
//Copyright (c) ��������Ԫ΢�������޹�˾
//�ļ�����:  S_TouchKeyCFG.h
//����: 
//ģ�鹦��:  ���ؼ������ļ�
//�汾:  V0.2
//���ļ�¼:
//************************************************************************************************
#ifndef __S_TOUCHKEYCFG_H__
#define __S_TOUCHKEYCFG_H__
#define		    SOCAPI_SET_TOUCHKEY_TOTAL					  2
#define			SOCAPI_SET_TOUCHKEY_CHANNEL					  0x00004002
#define			SOCAPI_SET_TOUCHKEY_CHANNEL2			      0x00000000
unsigned int  code TKCFG[17] = {0,1,0,3,10,3000,200,100,2,0,0,5,0,1,65535,65535,24}; 
unsigned char code TKChannelCfg[2][8]={
0x03,0x48,0x05,0x08,0x1f,0x05,0x01,0x44,
0x03,0x48,0x05,0x08,0x1a,0x05,0x01,0x91,
};
#endif
