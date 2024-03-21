//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//  文件名称		: Encryption_AES.c
//  模块功能		: 赛元95系列 解密 TEA方式 C文件
//  更正日期		: 2019/5/30
//  版本				: V1.0
//************************************************************
#include "..\H\Decrypt_TEA.H"
#include"..\H\OTADriver.h"

#if(Encryption == 1)//如果数据加密了在此进行解密
union TeaChunckType idata TeaChunck;

void decrypt()
{
  unsigned char i;
  unsigned int delta = 0x9e37;
  unsigned int sum = delta << 5;

  for(i = 0; i < 32; i++)
  {
    TeaChunck.Chunk16bit[1] -= ((TeaChunck.Chunk16bit[0] << 4) + Key_TEA[2]) ^ (TeaChunck.Chunk16bit[0] + -sum) ^ ((TeaChunck.Chunk16bit[0] >> 5) + Key_TEA[3]);
    TeaChunck.Chunk16bit[0] -= ((TeaChunck.Chunk16bit[1] << 4) + Key_TEA[0]) ^ (TeaChunck.Chunk16bit[1] + -sum) ^ ((TeaChunck.Chunk16bit[1] >> 5) + Key_TEA[1]);
    sum -= delta;
  }

}

#endif
