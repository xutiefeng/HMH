/***********************************************************************************************************
  ************************************************************************************************************/
#include "IOTCONFIG_XIAOMI.H"
/******************************************************************************
 * Name:    CRC-16/XMODEM       x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000
 * Alias:   CRC-16/ZMODEM,CRC-16/ACORN
 *****************************************************************************/
 #define TABLE_SIZE 256

static uint32_t crc_table[TABLE_SIZE];
static const uint32_t rcrc32 = 0xEDB88320;
 
 
u16 CRC16_Xmodem(u16 crc, u8 *Data, u32 length)
{
    u8 i;
    while (length--)
    {
        crc ^= (u16)(*Data++) << 8; // crc ^= (u16)(*data)<<8; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

//
u16 CheckSum16(u16 cs, u8 *Pdata, u32 length)
{
    u8 i;
    for (i = 0; i < length; i++)
    {
        cs += Pdata[i];
    }

    return cs;
}
//


void soft_crc32_init(void)
{
	uint32_t crc = 0;
	unsigned char i;
	uint32_t j;

	for (j = 0; j < TABLE_SIZE; j++) {
		crc = 0;
		for (i = 0x01; i != 0x00; i <<= 1) {
			if ((crc & 0x00000001) != 0) {
				crc >>= 1;
				crc ^= rcrc32;
			} else {
				crc >>= 1;
			}
			if ((j & i) != 0)
				crc ^= rcrc32;
		}
		crc_table[j] = crc;
	}
}

uint32_t CRC32_Xmodem(uint32_t crc,u8 *Data, u32 data_size )
{
	u32  result = crc;
	unsigned char crc_H8;
	soft_crc32_init();
	
	while (data_size--) {
		crc_H8 = (unsigned char)(result & 0x000000FF);
		result >>= 8;
		result ^= crc_table[crc_H8 ^ (*Data)];
		Data++;
	}

	return result;
}