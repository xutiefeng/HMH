/* 
 * File:   bsp_Led.h
 * Author: lix
 *
 * Created on 2022?11?15?, ??8:23
 */

#ifndef BSP_LED_H
#define	BSP_LED_H

#include "..\Apps\globe.h"
#include "..\Apps\config.h"

#ifdef	__cplusplus
extern "C" {
#endif


void LED_sCan(void);	
void LED_Process(void);
void LED_ForceOutput(u8 v);
void FactoryErrowDisplay(u8 v);
void CloseAllLED(void);
	
///void setLED(u8 num,u8 color, U_LED state);
#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

