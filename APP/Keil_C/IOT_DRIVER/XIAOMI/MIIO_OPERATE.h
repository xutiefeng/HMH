/***********************************************************************************************************
 
 ************************************************************************************************************/

#ifndef __MIIO_OPERATE_H
#define __MIIO_OPERATE_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "SC_Init.h"


#define MIIO_RESEND_COUNT (10)
#define MIIO_RXCOUNT_LIMIT (512)

#define XMODERN_BUFFER_SIZE (133)

    typedef enum
    {
        MI_Tx,
        MI_Wait,
        MI_TO,
        MI_Rx,
    } MIIO_State_t;

    typedef enum
    {
        MIR_NA,
        MIR_TO,
        MIR_R1,
        MIR_R2,
    } MIIO_Result_t;

    typedef struct
    {
        u16 Count;
        u16 Loop;
    } MIIO_Time_t;

    typedef struct
    {
        u16 Count;
        char Memory[512];
    } MIIO_Buffer_t;

    typedef struct
    {
        MIIO_State_t State;
        MIIO_Time_t Time;
        MIIO_Buffer_t Buffer;
        u8 ReSendCount;
    } MIIO_IAP_t;


    typedef enum
    {
        XS_Comm,
        XS_Finish,
        XS_Warn,
        XS_Fault,
		XS_Can,
    } XmodemStatus_t;

    typedef enum
    {
        XR_NA   = 0x00,
		XR_SOH  = 0x01,
		XR_EOT  = 0x04,
        XR_ACK  = 0x06,
        XR_NACK = 0x15,
        XR_CAN  = 0x18,
        XR_C    = 0x43,
    } XmodemReply_t;

    typedef enum
    {
        XM_CS,
        XM_CRC,
    } XmodemModeVER_t;

    typedef enum
    {
        XS_Send,
        XS_Receive,
        XS_ReceiveEnd,
        XS_Update,
        XS_UpFinish,
        XS_Error,
        //
        XS_ErrFUNC,   //函数配置
        XS_ErrSOH,    //帧头
        XS_ErrPN,     //包头
        XS_ErrVERIFY, //校验
        XS_ErrPNS,    //包序号
        XS_ErrTO,     //超时
        XS_ErrOV,     //数据溢出
        XS_ErrPF,     //处理函数错误
    } XmodemState_t;

    typedef enum
    {
        XP_Start,
        XP_Packing,
        XP_PackEnd,
    } XmodemPhase_t;

    typedef struct
    {
        u16 Limit;
        u16 Count;
    } XmodemTime_t;

    typedef struct
    {
        u8 EndCount;
        u8 PNSeq;
        u16 PNTotal;
    } XmodemOp_t;

    typedef struct
    {
        u16 Count;
        u16 Size;
        u8 *Memory;
    } XmodemPackage_t;

    typedef struct
    {
        void (*SendByte)(u8 Udata);
        u16 (*Verify)(u16 init, u8 *Udata, u32 length);
        bool (*PackProess)(u32 seq, u8 *Udata, u32 length);
        bool (*PackFinish)(void);
        bool (*PackError)(void);
    } XmodemFunc_t;

    typedef struct
    {
		XmodemModeVER_t ModeVER;
        XmodemState_t State;
        XmodemPhase_t Phase;
        XmodemReply_t Reply;

        XmodemTime_t Time;
		XmodemPackage_t Package;
    } XmodemClient_t;

    extern XmodemClient_t XMClient;

	extern u16 IAP_crc;
	extern u32 IAP_Size;
	extern u8  FileCrcerror;
	
    void XmodemClient_ModeInit(     //
                               XmodemModeVER_t modever, //
                               u8 *PackMemory);         // Size 133Byte

    void XmodemClient_FuncInit(XmodemClient_t *pXC,
                               void (*SendByte)(u8 Udata));

    XmodemStatus_t XmodemClient_Proess(IAPDATA_INFO *OTA_Data);
	void XmodemClient_ACK(void);
    void XmodemClient_ReceiveHandle(u8 Udata);
    bool IAP_Update_WriteErase_Process(u32 seq, u8 *Updata, u32 length);


extern volatile MIIO_IAP_t MIIO_IAP;
bool CmpStr(char *Uart_Rdata,unsigned char* string);
void MIIO_IAP_InData(s8 Data);

MIIO_Result_t MIIO_IAP_Cmd(char *cmd,                 //
                               char *reply1,              //
                               char *reply2,              //
                               u32 TimeOut,               //
                               void UR_Send(char *Udata)); //						   
   

#ifdef __cplusplus
}
#endif

#endif
