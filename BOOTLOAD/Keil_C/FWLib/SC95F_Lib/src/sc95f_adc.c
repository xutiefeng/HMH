//************************************************************
//  Copyright (c) ��������Ԫ΢���ӹɷ����޹�˾
//	�ļ�����: 			sc95f_adc.c
//	����:					��ԪӦ���Ŷ�
//	ģ�鹦��: 			ADC�̼��⺯��C�ļ�
// 	����������:	2024��1��18��
// 	�汾:				V1.0002
//  ˵��:				���ļ���������SC95Fϵ��оƬ
//*************************************************************
#include "sc95f_adc.h"

/**************************************************
*��������:void ADC_DeInit(void)
*��������:ADC��ؼĴ�����λ��ȱʡֵ
*��ڲ���:void
*���ڲ���:void
**************************************************/
void ADC_DeInit(void)
{
  ADCCON = 0x00;
  ADCCFG0 = 0X00;
  ADCCFG1 = 0X00;
  ADCCFG2 = 0X00;
  ADCVL = 0X00;
  ADCVH = 0X00;
  EADC = 0;
  IPADC = 0;
}

/**************************************************
*��������:void ADC_Init(ADC_PresSel_TypeDef ADC_PrescalerSelection,ADC_Cycle_TypeDef ADC_Cycle)
*��������:ADC��ʼ�����ú���
*��ڲ���:
ADC_PresSel_TypeDef:ADC_PrescalerSelection:ADCԤ��Ƶѡ��
ADC_Cycle_TypeDef:ADC_Cycle:����ʱ��ѡ��
*���ڲ���:void
**************************************************/
void ADC_Init(ADC_PresSel_TypeDef ADC_PrescalerSelection, ADC_Cycle_TypeDef ADC_Cycle)
{
  /* ����ADC����ʱ�ӷ�Ƶ */
  ADCCFG2 = ADC_PrescalerSelection;
  /* �������95Fϵ����Ч */
  ADC_Cycle = 0x00;
}

/**************************************************
*��������:void ADC_ChannelConfig(ADC_Channel_TypeDef ADC_Channel, FunctionalState NewState)
*��������:ADC��������ú���
*��ڲ���:
ADC_Channel_TypeDef:ADC_Channel:ADC�����ѡ��
FunctionalState:NewState:ADCxʹ��/�ر�ѡ��
*���ڲ���:void
**************************************************/
void ADC_ChannelConfig(ADC_Channel_TypeDef ADC_Channel, FunctionalState NewState)
{
  uint16_t tmpreg;

  /* ADC����ͨ��ѡ���ⲿͨ��������Ӧ��ADC���������Ϊģ������ģʽ */
  if(ADC_Channel < ADC_CHANNEL_VDD_D4)
  {
    tmpreg = (0x0001 << ADC_Channel);			//ͨ����λ�õ�ADCCFG�Ĵ���������ֵ
    if(NewState == DISABLE)
    {
      /* ADC������˳�ģ������ģʽ */
      ADCCFG0 &= (~(uint8_t)tmpreg);
      ADCCFG1 &= (~(uint8_t)(tmpreg >> 8));
    }
    else
    {
      /* ADC���������Ϊģ������ģʽ */
      ADCCFG0 |= ((uint8_t)tmpreg);
      ADCCFG1 |= ((uint8_t)(tmpreg >> 8));
    }
  }

  /* ����ADC����ʱ�ӷ�Ƶ */
  ADCCON = (ADCCON & 0XE0) | ADC_Channel;
}

/**************************************************
*��������:void ADC_EAINConfig(ADC_EAIN_TypeDef ADC_Channel, FunctionalState NewState)
*��������:����Ӧ��ADC���������Ϊģ������ģʽ
*��ڲ���:
ADC_EAIN_TypeDef:ADC_EAIN_Select:ѡ����Ҫ���õ�ADC�ڣ�uint16_t��Ϊ��Σ��������λ�������
FunctionalState:NewState:ADCͨ��ʹ��/�ر�ѡ��
*���ڲ���:void
**************************************************/
void ADC_EAINConfig(uint16_t ADC_EAIN_Select, FunctionalState NewState)
{
  if(NewState == DISABLE)
  {
    /* ADC������˳�ģ������ģʽ */
    ADCCFG0 &= (~(uint8_t)ADC_EAIN_Select);
    ADCCFG1 &= (~(uint8_t)(ADC_EAIN_Select >> 8));
  }
  else
  {
    /* ADC���������Ϊģ������ģʽ */
    ADCCFG0 |= ((uint8_t)ADC_EAIN_Select);
    ADCCFG1 |= ((uint8_t)(ADC_EAIN_Select >> 8));
  }
}

/*****************************************************
*��������:uint16_t ADC_GetConversionValue(void)
*��������:���һ��ADת������
*��ڲ���:void
*���ڲ���:uint16_t
*****************************************************/
uint16_t ADC_GetConversionValue(void)
{
  return ((ADCVH << 4) + (ADCVL >> 4));
}

/*****************************************************
*��������:void ADC_Cmd(FunctionalState NewState)
*��������:ADC���ܿ��غ���
*��ڲ���:
FunctionalState:NewState:��������/�ر�ѡ��
*���ڲ���:void
*****************************************************/
void ADC_Cmd(FunctionalState NewState)
{
  /* ʹ��/ʧ��ADC���� */
  if(NewState == DISABLE)
  {
    ADCCON &= 0X7F;
  }
  else
  {
    ADCCON |= 0x80;
  }
}

/*****************************************************
*��������:void ADC_VrefConfig(ADC_Vref_TypeDef ADC_Vref)
*��������:����ADC�Ĳο���ѹ
*��ڲ���:
ADC_Vref_TypeDef:ADC_Vref:ѡ��ADC�ο���ѹ
*���ڲ���:void
*****************************************************/
void ADC_VrefConfig(ADC_Vref_TypeDef ADC_Vref)
{
  OPINX = 0xC2;

  if(ADC_Vref == ADC_VREF_2_048V)
  {
    OPREG &= 0x3f;
    OPREG |= 0x40; //ѡ��ο��ڲ���ѹ2.048V��
  }

  if(ADC_Vref == ADC_VREF_1_024V)
  {
    OPREG &= 0x3f;
    OPREG |= 0x80; //ѡ��ο��ڲ���ѹ1.024V��
  }

  if(ADC_Vref == ADC_VREF_VDD)
  {
    OPREG &= 0x3f; //ѡ��ο���ѹVDD��
  }
}

/*****************************************************
*��������:uint16_t ADC_GetConversionAverageValue(ADC_SamplesNum_TypeDef ADC_SamplesNum)
*��������:��ȡADC��ת��ֵƽ��ֵ
*				ע:��ȡת��ֵƽ��ֵ�����л�ر�ADC�ж�
*��ڲ���:
ADC_SamplesNum_TypeDef:ADC_SamplesNum:ADC�����Ĵ���
*���ڲ���:
uint16_t:���ADC������ƽ��ֵ
*****************************************************/
#include "sc95f_mdu.h"
uint16_t ADC_GetConversionAverageValue(ADC_SamplesNum_TypeDef ADC_SamplesNum)
{
  uint8_t tmpCnt;
  uint16_t tmpValueSum = 0, tmpValue;
  uint16_t tmpValue_Max = 0, tmpValue_Min = 0xFFFF;
  bit tmpStaus = EADC;
  EADC = 0;

  for(tmpCnt = 0; tmpCnt < ADC_SamplesNum; tmpCnt ++)
  {
    ADC_StartConversion();			//����һ��ADCת��
    while(!ADC_GetFlagStatus());			//�ȴ�ADCת������
    ADC_ClearFlag();			//���ADCת����־λ
    tmpValue = ADC_GetConversionValue();			//��ȡ����ת��ֵ

    /* ��ȡ���ת��ֵ�е����ֵ */
    if(tmpValue > tmpValue_Max)
      tmpValue_Max = tmpValue;
    /* ��ȡ���ת��ֵ�е���Сֵ */
    if(tmpValue < tmpValue_Min)
      tmpValue_Min = tmpValue;

    tmpValueSum += tmpValue;//�ۼӶ��ת��ֵ�ĺ�
  }

  /* ͨ��Ӳ���˳�����������ƽ��ֵ */
  {
    OPERCON |= 0X40;
    /* ������Ϊ16λ���� */
    tmpValueSum -= (tmpValue_Max + tmpValue_Min);
    EXA0 = tmpValueSum;
    EXA1 = tmpValueSum >> 8;
    EXBL = ADC_SamplesNum - 2;			//����Ϊ8λ����

    OPERCON |= 0x80;				//��ʼ��������
    while(OPERCON & 0x80);			//�ȴ��������

    //��ȡ��
    tmpValue = (uint16_t)EXA0;
    tmpValue |= EXA1 << 8;
  }

  EADC = tmpStaus;			//�ָ�EADC
  return tmpValue;			//����ƽ��ֵ
}

/******************* (C) C.OPYRIGHT 2022 SinOne Microelectronics *****END OF FILE****/