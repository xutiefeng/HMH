//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: Date.c
//	模块功能	: 赛元BootLoader APP示例 C文件
//  更正日期	: 2021/4/21
// 	版本		: V2.0
//************************************************************
#include "..\H\OTADriver.h"
//可设版本号或日期，HEX格式，不能含有A~F
//APP版本/日期设置，升级APP版本/日期需要大于旧APP版本/日期
unsigned long code VersionDate = VERSION_DATE;