//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: Encryption_AES.c
//	模块功能	: 赛元95系列 解密 TEA方式 C文件
//  更正日期	: 2019/5/30
// 	版本		: V1.0
//************************************************************
#ifndef _Encryption_TEA_H_
#define _Encryption_TEA_H_

extern unsigned int code Key_TEA[4];   //密钥存储数组

union TeaChunckType 
{
  unsigned int Chunk16bit[2];
  unsigned char Chunk8bit[4];
};
extern union TeaChunckType idata TeaChunck;	//待解密的内容和解密完成的内容放入此变量中
extern void decrypt(); //对TeaChunck的内容进行解密运算

#endif