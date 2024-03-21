/**********************************************************************************************************
 ************************************************************************************************************/

#ifndef __CRC_UTIL_H
#define __CRC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

//#include "ht32.h"

    u16 CRC16_Xmodem(u16 crc, u8 *Udata, u32 length);
    u16 CheckSum16(u16 cs, u8 *Updata, u32 length);
    uint32_t CRC32_Xmodem(uint32_t crc,u8 *Data, u32 data_size );
#ifdef __cplusplus
}
#endif

#endif
