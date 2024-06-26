/************************************************************************
 Project:PMSM_SLCTRL
 Filename: uartHMI.h
 Partner Filename: uartHMI.c
 Description: Partner file of uartHMI.c
*************************************************************************
 Revising History (ECL of this file):
 .....
*************************************************************************/


/************************************************************************
 Compiler Directives
*************************************************************************/
#ifdef _uartHMI_C_
   #define _uartHMI_EXT
#else
   #define _uartHMI_EXT extern
#endif

/************************************************************************
 Definitions & Macros (#define ...)
*************************************************************************/
#define SCI_SCOPE_UARTPORT_NO   2//1//2

#define TXDATSIZE		100
#define TXBUFSIZE		14
#define RXBUFSIZE		12//6
#define SLOWCH_NUM	20//15//10
#define TX_SIZE			(4+1)


/************************************************************************
 TypeDefs & Structure defines (N/A)
*************************************************************************/
#define HiTxBufSize		700
typedef struct 
{
	uint8_t RxBuf[RXBUFSIZE];
	uint8_t TxBuf[TXBUFSIZE];
	uint32_t Buf[TX_SIZE];
	uint8_t RxID;
	uint8_t RxOK;
	uint8_t TxID;
	uint8_t EnUpdate;

	uint8_t iTmp;
	uint8_t SlowID;
	uint16_t SlwData[20];
	uint16_t Ch1Data;
	uint16_t Ch2Data;
	uint16_t Ch3Data;
	uint16_t Ch4Data;
	uint8_t CntUpdate;
	uint16_t hiCh1Data[HiTxBufSize];
	uint16_t hiCh2Data[HiTxBufSize];
	uint16_t hiCh3Data[HiTxBufSize];
	uint16_t hiCh4Data[HiTxBufSize];	
	uint16_t hiChxSaveID;
	uint16_t hiChxSendID;
} uartHMI_Struct;

/************************************************************************
 Exported Variables:
*************************************************************************/
extern uartHMI_Struct Scope;
/************************************************************************
 RAM ALLOCATION:
*************************************************************************/

/************************************************************************
 Exported Function Call Prototypes (N/A)
*************************************************************************/
extern void UART_Config(void);
extern void hmiUpdateTask(void);
_uartHMI_EXT void sci_voTxUpate(void);
_uartHMI_EXT void sci_voRxIsr(void);
_uartHMI_EXT void sci_voTxIsr(void);
_uartHMI_EXT void APP_UsartTransmit_DMA_Setting(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size);
_uartHMI_EXT void APP_UsartTransmit_DMA_Enable(void);
/************************************************************************
 All rights reserved.
*************************************************************************
 End of this File (EOF):
 !!!!!!Do not put anything after this part!!!!!!!!!!!
*************************************************************************/
