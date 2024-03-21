#include "..\H\OTADriver_IAP.h"
#include "..\H\OTADriver_Statement.H"
/**************************************************
*�������ƣ�IAP_ReadOneByte
*�������ܣ����ֽڶ�
*��ڲ�����IAP_IapAddr ��ȡ��ַ
*���ڲ�����unsigned char ��ȡ��ֵ  
**************************************************/
unsigned char IAP_ReadOneByte(unsigned long IAP_IapAddr)
{
	unsigned char IAP_IapData;
	IAP_IapData=IAP_ReadByte(IAP_IapAddr,IAP_MEMTYPE_APROM);
	return IAP_IapData;
}

/*****************************************************
*��������: void IAP_ReadNByte(unsigned int IAP_IapAddr,int Length, void *BuffAddr)		
*��������: ��N�ֽ�IAP����
*��ڲ�����IAP_IapAddr ����ַ	Length :��ȡ����  BuffAddr �洢buff
*���ڲ�����void
*****************************************************/
void IAP_ReadNByte(unsigned long IAP_IapAddr,int Length, void *BuffAddr)
{
	int i;
	
	for(i = 0; i < Length; i++)
	{
		 *((char *)BuffAddr + i)= IAP_ReadOneByte(IAP_IapAddr + i);
	}
}


/**************************************************
*�������ƣ�IAP_WriteOneByte
*�������ܣ�IAP ���ֽ�дROM
*��ڲ�����IAP_IapAddr д���ַ Write_IAP_IapData д������
*���ڲ�����unsigned char 1 ��д����ȷ 0��д�����  
**************************************************/
unsigned char IAP_WriteOneByte(unsigned long IAP_IapAddr,unsigned char Write_IAP_IapData)//
{
	return IAP_ProgramByte(IAP_IapAddr,Write_IAP_IapData,IAP_MEMTYPE_ROM,0xF0);

}


/*****************************************************
*��������: unsigned char IAP_WriteNByte(unsigned int IAP_IapAddr, int Length, void* Write_IAP_IapData)
*��������: дN�ֽ�
*��ڲ�����IAP_IapAddr ��ַ�� Length д���ȣ�Write_IAP_IapData ����BUFF
*���ڲ�����1 д��׼ȷ 0 ʧ��  
*****************************************************/
unsigned char IAP_WriteNByte(unsigned long IAP_IapAddr, int Length, void* Write_IAP_IapData)//дN�ֽ�IAP����
{
	int i;
	
	for(i = 0; i < Length; i ++)
	{
		if(!IAP_WriteOneByte(IAP_IapAddr + i, *((unsigned char *)Write_IAP_IapData + i)))
			return 0;
	}
	
	return 1;
}

/**************************************************
*�������ƣ�void IAP_Erase_512B(unsigned long IAP_IapAddr)
*�������ܣ�����һ���飨512B��
*��ڲ�����unsigned long IAP_IapAddr ��ַ
*���ڲ�����void
*��ע	 ��
**************************************************/
void IAP_Erase_512B(unsigned long IAP_IapAddr)//
{
	IAP_SectorErase(IAP_MEMTYPE_ROM,IAP_IapAddr,0xF0);
}
//#endif
