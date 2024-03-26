#ifndef _OTADriver_H_
#define _OTADriver_H_

//<<<Use SPTML>>>
#define UartSelect 0//UartSelect 
#define UartBaud 115200
#define Fsoc X
#define CheckMode 0//CheckMode 
#define Encryption 0//Encryption 
#define EncryptionKeyH 0x181D050C//EncryptionKeyH 
#define EncryptionKeyL 0x000D1517//EncryptionKeyL 
#define FLASH_SIZE 0x10000//FLASH_SIZE 
#define BOOTLOAD_SPACE_SIZE 0x1000//BOOTLOAD_SPACE_SIZE 
#define MEMORY_AREA_SPACE_SIZE 0x200//MEMORY_AREA_SPACE_SIZE 
#define PROJECT_SWITCH 0x01//PROJECT_SWITCH 
#define LDROM_MODE 0//LDROM_MODE 
#define BACKUPS_MODE 1//BACKUPS_MODE 
#define FORCE_UPDATA 0//FORCE_UPDATA 
#define VERSION_DATE 0x20230414//VERSION_DATE 
#define ISP_TIME_WINDOW 4//ISP_TIME_WINDOW 
#define DEBUG_WAITCH 1//DEBUG_WAITCH 
#define DEBUG_PLATFORM 1//DEBUG_PLATFORM 
#define FIRMWARE_DOWMLOADER_MODE 1//FIRMWARE_DOWMLOADER_MODE 
//<<<end of SPTML>>>

#if LDROM_MODE == 1 
#define CUSTOMERADRESS	BeginAddr>=0x0000		//允许进行IAP更新操作的程序范围

#else

#define CUSTOMERADRESS	BeginAddr>=BOOTLOAD_SPACE_SIZE		//允许进行IAP更新操作的程序范围
#define INTERRUPT_ADDRESS_APP		   BOOTLOAD_SPACE_SIZE		//APP   中断向量重映射地址
#define INTERRUPT_ADDRESS_BOOT		 0x0100		//BOOT中断向量重映射地址
#endif

#if(CheckMode==1)
#define CommunicationLength1          64+6  	    //通讯一帧长度最大值 用于数据帧缓冲数组
#endif
#if(CheckMode==0)
#define CommunicationLength1          64+3  	    //通讯一帧长度最大值 用于数据帧缓冲数组
#endif

#if (LDROM_MODE == 0)
#define BOOTLOAD_END_ADDRESS 	  			BOOTLOAD_SPACE_SIZE
#else
#define BOOTLOAD_END_ADDRESS 	  			0
#endif

//APP程序占用存储区域大小 (flash空间大小 - BOOTLOAD 区域大小 - 备份区大小 - 记忆存储区域大小) ，空间大小设置应该为512B的倍数
#if  (BACKUPS_MODE == 1)	//双区配置的宏定义
    #if((((FLASH_SIZE - BOOTLOAD_SPACE_SIZE - MEMORY_AREA_SPACE_SIZE)/2)&0xFFFE00)+BOOTLOAD_END_ADDRESS)<0x10000
        #define APP_SPACE_SIZE	  (((FLASH_SIZE - BOOTLOAD_SPACE_SIZE - MEMORY_AREA_SPACE_SIZE)/2)&0xFFFE00)	//空间大小设置应该为512B的倍数	
    #else
        #define APP_SPACE_SIZE    0x10000-BOOTLOAD_END_ADDRESS
    #endif
#else
    #define  APP_SPACE_SIZE   (((FLASH_SIZE - BOOTLOAD_SPACE_SIZE - MEMORY_AREA_SPACE_SIZE)/1)&0xFFFE00)	//单区BOOTLOADER
#endif


#define     INITFLAG	                                             0XA55A0FF0		 //用于判断RUN APP是否为最初的APP
#define	APP_UPDATA_FLAG_NUMBER	            0XAA,0X55,0X55,0XAA 	//表示程序正常或通过APP的串口进入BL的数字码，最大16Byte

#define APP_STARTUP_DEVIATION_BEGIN_ADDRESS	BOOTLOAD_END_ADDRESS+0x100	  				//用户运行区APP的STARTUP程序*偏移*地址,既APP程序Keil Option 中设置的STARTUP地址
#define APP_INFO_DEVIATION_BEGIN_ADDRESS	                APP_STARTUP_DEVIATION_BEGIN_ADDRESS+0x100			//APP信息位置的*偏移*地址,既APP程序Keil Option 中设置的APP_INFO地址

#define RUN_APP_BEGIN_ADDRESS 	  			BOOTLOAD_END_ADDRESS				                //用户运行区APP的起始地址
#define RUN_APP_END_ADDRESS				(RUN_APP_BEGIN_ADDRESS + APP_SPACE_SIZE)	  		//运行区APP最后地址
#define RUN_APP_STARTUP_ADDRESS				(RUN_APP_BEGIN_ADDRESS+0X100L)   				//用户APP的STARTUP程序地址
#define RUN_APP_INFO_ADDRESS				(RUN_APP_BEGIN_ADDRESS+0X100L+0X100L)			//运行区APP信息存放地址
#define RUN_APP_MEMORY_INFO_ADRESS 			(RUN_APP_END_ADDRESS - 0x10)     				//运行区信息存储地址


#if  BACKUPS_MODE
    #define BACKUPS_APP_BEGIN_ADDRESS			RUN_APP_END_ADDRESS                         //双区BOOTLOADER,备份区APP起始地址在运行区APP区后面
#else
    #define BACKUPS_APP_BEGIN_ADDRESS			RUN_APP_BEGIN_ADDRESS                      //单区BOOTLOADER,备份区与运行区重叠
#endif
#define BACKUPS_APP_END_ADDRESS				(BACKUPS_APP_BEGIN_ADDRESS+ APP_SPACE_SIZE)		//备份区APP最后地址
#define BACKUPS_APP_INFO_ADDRESS			                (BACKUPS_APP_BEGIN_ADDRESS +0x100L + 0x100L)                           //备份区APP信息存放地址
#define BACKUPS_APP_MEMORY_INFO_ADRESS 		                (BACKUPS_APP_END_ADDRESS - 0x10)     		               //备份区信息存储地址

#define	MEMORY_AREA_BEGIN_ADDRESS			BACKUPS_APP_END_ADDRESS								//记忆存储区起始地址
#define	MEMORY_AREA_END_ADDRESS			(MEMORY_AREA_BEGIN_ADDRESS + MEMORY_AREA_SPACE_SIZE)//记忆存储区最后地址


/*************************通讯协议相关宏定义*******************************/
//命令帧格式：帧头+命令位+(起始地址+)帧尾
//数据帧格式：帧头+长度+数据+校验
/**************************************************************************/
#define CommunicationLength0   		 3			//通讯一帧长度 用于命令帧
#define CommunicationDataHeader                        0X55		//数据帧头
#define CommunicationCommandHeader               0Xaa		//命令帧头
#define CommunicationCommandEnd		 0xEF		//命令帧尾
#define IAPMODEENTER 			 0X01		//进入IAP操作模式命令宏
#define IAPMODEEXIT			0xff		//退出IAP操作模式命令宏
#define IAPSUCCESS 	                                0X02		//IAP操作成功
#define IAPFAIL						 0x03		//IAP操作失败
#define IAPIDLE						 0x00       //待机
#define IAPWRITE					 0X10		//接收到IAP写命令
#define IAPREALLY					 0x11		//准备接收数据
#define IAPADRESSERROR				 0X44		//操作地址不在用户区
/***********************************************************************/

/****************************状态宏定义**********************************/
#define CHECKOK 	0xaa		//更新成功
#define CHECKFAIL   0x55		//更新失败
/***********************************************************************/

//**********************UART通讯接口**********************************

#endif
