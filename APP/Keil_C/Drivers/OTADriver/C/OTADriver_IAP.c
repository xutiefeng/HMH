#include "..\H\OTADriver_IAP.h"
#include "..\H\OTADriver_Statement.H"
/**************************************************
*函数名称：IAP_ReadOneByte
*函数功能：单字节读
*入口参数：IAP_IapAddr 读取地址
*出口参数：unsigned char 读取的值  
**************************************************/
unsigned char IAP_ReadOneByte(unsigned long IAP_IapAddr)
{
	unsigned char IAP_IapData;
	IAP_IapData=IAP_ReadByte(IAP_IapAddr,IAP_MEMTYPE_APROM);
	return IAP_IapData;
}

/*****************************************************
*函数名称: void IAP_ReadNByte(unsigned int IAP_IapAddr,int Length, void *BuffAddr)		
*函数功能: 读N字节IAP操作
*入口参数：IAP_IapAddr ：地址	Length :读取长度  BuffAddr 存储buff
*出口参数：void
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
*函数名称：IAP_WriteOneByte
*函数功能：IAP 单字节写ROM
*入口参数：IAP_IapAddr 写入地址 Write_IAP_IapData 写入数据
*出口参数：unsigned char 1 ：写入真确 0：写入错误  
**************************************************/
unsigned char IAP_WriteOneByte(unsigned long IAP_IapAddr,unsigned char Write_IAP_IapData)//
{
	return IAP_ProgramByte(IAP_IapAddr,Write_IAP_IapData,IAP_MEMTYPE_ROM,0xF0);

}


/*****************************************************
*函数名称: unsigned char IAP_WriteNByte(unsigned int IAP_IapAddr, int Length, void* Write_IAP_IapData)
*函数功能: 写N字节
*入口参数：IAP_IapAddr 地址， Length 写长度，Write_IAP_IapData 数据BUFF
*出口参数：1 写入准确 0 失败  
*****************************************************/
unsigned char IAP_WriteNByte(unsigned long IAP_IapAddr, int Length, void* Write_IAP_IapData)//写N字节IAP操作
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
*函数名称：void IAP_Erase_512B(unsigned long IAP_IapAddr)
*函数功能：擦除一个块（512B）
*入口参数：unsigned long IAP_IapAddr 地址
*出口参数：void
*备注	 ：
**************************************************/
void IAP_Erase_512B(unsigned long IAP_IapAddr)//
{
	IAP_SectorErase(IAP_MEMTYPE_ROM,IAP_IapAddr,0xF0);
}
//#endif
