

#include "bsp_uart.h"



#define		HEAD_CODE_NORMAL					0x03	
#define		HEAD_CODE_EVENT						0x06	
#define		ZHUKONGBAN_ADDR						0X01
#define		RX_Len                              0X05

//uint8_t     SCICheckout;                             
//uint8_t     RxLen; 

#define ReceiveRestart()	 RxLen = 0, SCICheckout  = 0
#define RECEIVE_CONTROLLER_DATA_LENGTH 2

const	unsigned	char  code	display_tab[]=  		  					   
{
	0x3f,//0
	0x06,//1
	0x5b,//2
	0x4f,//3	
	0x66,//4 
	0x6d,//5
	0x7d,//6
	0x07,//7
	0x7f, //8
	0x6f,//9
	0x77,//a
	0x7c,	//b	
	0x39,//c 
	0x5e,//d
	0x79, //e
	0x71,	//F
};
/*
´Ëº¯Êý
*/

void    UART0_SendData( void)
{
		u8 i;
		
	  gSendData[0].all = 0xA5;
		gSendData[1].all = 8;
		gSendData[2].all = 0;
		gSendData[3].all = 0;
		gSendData[4].all = 0;
		gSendData[5].all = 0;
		gSendData[6].all = 0;
	
	  gSendData[7].all = 0;
		for(i = 0;i<7;i++)
		{
				gSendData[7].all +=gSendData[i].all;
		}
		SBUF = 0xA5;
}

void    RDUART0_RcvByte(void)
{
    u8   temp;
    static u8 RxLen;
    static u8 SCICheckout;
	  static u8 rx[8];
    //LPUART_Getchar(LPUART0, &temp);
   
				temp = SBUF;
				if(RxLen == 0)
				{
					if((temp == 0Xa5))
					{
						rx[0] = temp;
						SCICheckout += temp;
						RxLen++;
					}
					else
					{
						 ReceiveRestart();
					}
				}
	
				else if(RxLen == 7)
				{
 					if(temp ==  SCICheckout)
					{
//							gKeyValue = rx[2];  

//							if(gKeyValue > 0)
//							{
//									EvRecvFlag = 1;
//							}       
							PanelLostFlag = 0;
						
							ReceiveRestart();
					}						
					else
					{						
						 ReceiveRestart();
					}
				}
				
				else
				{
					rx[RxLen] = temp;
					RxLen++;
					SCICheckout += temp;
				}
}

