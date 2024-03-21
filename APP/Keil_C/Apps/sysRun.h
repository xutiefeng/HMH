#ifndef sysRun_H
#define sysRun_H

//#include "includeall.h"

//BOOLEAN LPUART0_SenBuf(INT8U *pucDat, INT32U ulLen)
void sysRuning(void);
void sysRest(void);

#define  BSP_UART_TxData     ;//SERCOM0_USART_Write

#endif
