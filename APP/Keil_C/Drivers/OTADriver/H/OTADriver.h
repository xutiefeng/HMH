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
#define CUSTOMERADRESS	BeginAddr>=0x0000		//�������IAP���²����ĳ���Χ

#else

#define CUSTOMERADRESS	BeginAddr>=BOOTLOAD_SPACE_SIZE		//�������IAP���²����ĳ���Χ
#define INTERRUPT_ADDRESS_APP		   BOOTLOAD_SPACE_SIZE		//APP   �ж�������ӳ���ַ
#define INTERRUPT_ADDRESS_BOOT		 0x0100		//BOOT�ж�������ӳ���ַ
#endif

#if(CheckMode==1)
#define CommunicationLength1          64+6  	    //ͨѶһ֡�������ֵ ��������֡��������
#endif
#if(CheckMode==0)
#define CommunicationLength1          64+3  	    //ͨѶһ֡�������ֵ ��������֡��������
#endif

#if (LDROM_MODE == 0)
#define BOOTLOAD_END_ADDRESS 	  			BOOTLOAD_SPACE_SIZE
#else
#define BOOTLOAD_END_ADDRESS 	  			0
#endif

//APP����ռ�ô洢�����С (flash�ռ��С - BOOTLOAD �����С - ��������С - ����洢�����С) ���ռ��С����Ӧ��Ϊ512B�ı���
#if  (BACKUPS_MODE == 1)	//˫�����õĺ궨��
    #if((((FLASH_SIZE - BOOTLOAD_SPACE_SIZE - MEMORY_AREA_SPACE_SIZE)/2)&0xFFFE00)+BOOTLOAD_END_ADDRESS)<0x10000
        #define APP_SPACE_SIZE	  (((FLASH_SIZE - BOOTLOAD_SPACE_SIZE - MEMORY_AREA_SPACE_SIZE)/2)&0xFFFE00)	//�ռ��С����Ӧ��Ϊ512B�ı���	
    #else
        #define APP_SPACE_SIZE    0x10000-BOOTLOAD_END_ADDRESS
    #endif
#else
    #define  APP_SPACE_SIZE   (((FLASH_SIZE - BOOTLOAD_SPACE_SIZE - MEMORY_AREA_SPACE_SIZE)/1)&0xFFFE00)	//����BOOTLOADER
#endif


#define     INITFLAG	                                             0XA55A0FF0		 //�����ж�RUN APP�Ƿ�Ϊ�����APP
#define	APP_UPDATA_FLAG_NUMBER	            0XAA,0X55,0X55,0XAA 	//��ʾ����������ͨ��APP�Ĵ��ڽ���BL�������룬���16Byte

#define APP_STARTUP_DEVIATION_BEGIN_ADDRESS	BOOTLOAD_END_ADDRESS+0x100	  				//�û�������APP��STARTUP����*ƫ��*��ַ,��APP����Keil Option �����õ�STARTUP��ַ
#define APP_INFO_DEVIATION_BEGIN_ADDRESS	                APP_STARTUP_DEVIATION_BEGIN_ADDRESS+0x100			//APP��Ϣλ�õ�*ƫ��*��ַ,��APP����Keil Option �����õ�APP_INFO��ַ

#define RUN_APP_BEGIN_ADDRESS 	  			BOOTLOAD_END_ADDRESS				                //�û�������APP����ʼ��ַ
#define RUN_APP_END_ADDRESS				(RUN_APP_BEGIN_ADDRESS + APP_SPACE_SIZE)	  		//������APP����ַ
#define RUN_APP_STARTUP_ADDRESS				(RUN_APP_BEGIN_ADDRESS+0X100L)   				//�û�APP��STARTUP�����ַ
#define RUN_APP_INFO_ADDRESS				(RUN_APP_BEGIN_ADDRESS+0X100L+0X100L)			//������APP��Ϣ��ŵ�ַ
#define RUN_APP_MEMORY_INFO_ADRESS 			(RUN_APP_END_ADDRESS - 0x10)     				//��������Ϣ�洢��ַ


#if  BACKUPS_MODE
    #define BACKUPS_APP_BEGIN_ADDRESS			RUN_APP_END_ADDRESS                         //˫��BOOTLOADER,������APP��ʼ��ַ��������APP������
#else
    #define BACKUPS_APP_BEGIN_ADDRESS			RUN_APP_BEGIN_ADDRESS                      //����BOOTLOADER,���������������ص�
#endif
#define BACKUPS_APP_END_ADDRESS				(BACKUPS_APP_BEGIN_ADDRESS+ APP_SPACE_SIZE)		//������APP����ַ
#define BACKUPS_APP_INFO_ADDRESS			                (BACKUPS_APP_BEGIN_ADDRESS +0x100L + 0x100L)                           //������APP��Ϣ��ŵ�ַ
#define BACKUPS_APP_MEMORY_INFO_ADRESS 		                (BACKUPS_APP_END_ADDRESS - 0x10)     		               //��������Ϣ�洢��ַ

#define	MEMORY_AREA_BEGIN_ADDRESS			BACKUPS_APP_END_ADDRESS								//����洢����ʼ��ַ
#define	MEMORY_AREA_END_ADDRESS			(MEMORY_AREA_BEGIN_ADDRESS + MEMORY_AREA_SPACE_SIZE)//����洢������ַ


/*************************ͨѶЭ����غ궨��*******************************/
//����֡��ʽ��֡ͷ+����λ+(��ʼ��ַ+)֡β
//����֡��ʽ��֡ͷ+����+����+У��
/**************************************************************************/
#define CommunicationLength0   		 3			//ͨѶһ֡���� ��������֡
#define CommunicationDataHeader                        0X55		//����֡ͷ
#define CommunicationCommandHeader               0Xaa		//����֡ͷ
#define CommunicationCommandEnd		 0xEF		//����֡β
#define IAPMODEENTER 			 0X01		//����IAP����ģʽ�����
#define IAPMODEEXIT			0xff		//�˳�IAP����ģʽ�����
#define IAPSUCCESS 	                                0X02		//IAP�����ɹ�
#define IAPFAIL						 0x03		//IAP����ʧ��
#define IAPIDLE						 0x00       //����
#define IAPWRITE					 0X10		//���յ�IAPд����
#define IAPREALLY					 0x11		//׼����������
#define IAPADRESSERROR				 0X44		//������ַ�����û���
/***********************************************************************/

/****************************״̬�궨��**********************************/
#define CHECKOK 	0xaa		//���³ɹ�
#define CHECKFAIL   0x55		//����ʧ��
/***********************************************************************/

//**********************UARTͨѶ�ӿ�**********************************

#endif
