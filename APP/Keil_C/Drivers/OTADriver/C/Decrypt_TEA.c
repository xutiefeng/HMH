//************************************************************
//  Copyright (c) ��������Ԫ΢�������޹�˾
//  �ļ�����		: Encryption_AES.c
//  ģ�鹦��		: ��Ԫ95ϵ�� ���� TEA��ʽ C�ļ�
//  ��������		: 2019/5/30
//  �汾				: V1.0
//************************************************************
#include "..\H\Decrypt_TEA.H"
#include"..\H\OTADriver.h"

#if(Encryption == 1)//������ݼ������ڴ˽��н���
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
