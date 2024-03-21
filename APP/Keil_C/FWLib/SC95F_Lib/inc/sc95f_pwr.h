//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 		sc95f_pwr.h
//	����		:
//	ģ�鹦��: 		PWR�̼��⺯��ͷ�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��:���ļ���������SC95Fϵ��оƬ
//*************************************************************

#ifndef _sc95f_PWR_H_
#define _sc95f_PWR_H_

#include "sc95f.h"
#include <intrins.h>

void PWR_DeInit(void);
void PWR_EnterSTOPMode(void);
void PWR_EnterIDLEMode(void);
void PWR_SoftwareReset(void);

#endif

/******************* (C) COPYRIGHT 2018 SinOne Microelectronics *****END OF FILE****/