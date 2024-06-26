/************************************************************************
 Project:PMSM_SLCTRL
 Filename: sciScope.h
 Partner Filename: sciScope.c
 Description: Partner file of sciScope.c
 Complier: Keil uVision 5.0
*************************************************************************
 Copyright (c) 2022 Puya Semiconductor Co.
 Creating by Michael.Wang(Wang Weizi)
 All rights reserved.
*************************************************************************
 Revising History (ECL of this file):
 PMSM_SLCTRL_20130930A, by Michael.Wang(Wang Weizi), create sciScope.c file
 .....
*************************************************************************/

/************************************************************************
 Beginning of File, do not put anything above here except notes
 Compiler Directives:
*************************************************************************/
#ifndef _SCISCOPE_C_
    #define _SCISCOPE_C_
#endif

/************************************************************************
 Included File:
*************************************************************************/
#include "main.h"

uartHMI_Struct Scope;

UART_HandleTypeDef UartHandle;

/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
//#if DEBUG_LOG_EN
#if (defined(__CC_ARM)) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);
	return (ch);
}
int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);
	return (ch);
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);
	return (ch);
}
#endif
//#endif
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void UART_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	DEBUG_USART_CLK_ENABLE();

	/* USART2初始化 */
	UartHandle.Instance = USART2;
	UartHandle.Init.BaudRate = 2000000;//115200; //2000000
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UartHandle);

	__HAL_RCC_GPIOF_CLK_ENABLE();

	//  /**USART GPIO Configuration
	//    A4--AF9     ------> USART2_TX
	//    A3--AF1     ------> USART2_RX
	//    */
	//  GPIO_InitStruct.Pin = GPIO_PIN_4;
	//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//  GPIO_InitStruct.Pull = GPIO_PULLUP;
	//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//  GPIO_InitStruct.Alternate = GPIO_AF9_USART2;
	//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//
	//  GPIO_InitStruct.Pin = GPIO_PIN_3;
	//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//  GPIO_InitStruct.Pull = GPIO_PULLUP;
	//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
	//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/**USART GPIO Configuration
	  F0--AF9     ------> USART2_TX
	  A3--AF1     ------> USART2_RX
	  */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_USART2;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/********************************************************************************
Description:	周期调用
Input:
Output:
Return:
*********************************************************************************/
void hmiUpdateTask(void)
{
	#if 0
	sci_voTxUpate(); //DMA发送
	#else 
	//printf("%d,%d,%d,%d\n",mc_stFoc2R.slVdcLpf.sw.hi,mc_stFoc2R.WeCmd,mc_stFoc2R.SysStateId,mc_stFoc2R.MotorErrCode);	
	//printf("%d,%d,%d,%d\n",mc_stFoc2R.IbusAdc1,mc_stFoc2R.slVdcLpf.sw.hi,mc_stFoc2R.SysStateId,mc_stFoc2R.MotorErrCode);
	//printf("%d,%d,%d,%d\n",userPara.ntcSta,mc_stFoc2R.NtcTemp,mc_stFoc2R.SysStateId,mc_stFoc2R.MotorErrCode);
	#endif
}

/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void sci_voTxUpate(void)
{
	APP_UsartTransmit_DMA_Setting(USART2,(uint8_t *)Scope.Buf, TX_SIZE*4);
	Scope.Buf[0] = mc_stFoc2R.IbusAdc1;
	Scope.Buf[1] = mc_stFoc2R.slVdcLpf.sw.hi;
	Scope.Buf[2] = mc_stFoc2R.SysStateId;
	Scope.Buf[3] = mc_stFoc2R.MotorErrCode;
	Scope.Buf[TX_SIZE-1] = 0x7F800000;//End

	//APP_UsartTransmit_DMA_Setting(USART2,(uint8_t *)Scope.TxBuf, TXBUFSIZE);
	// Scope.TxBuf[2]=2;
	// Scope.TxBuf[3]=3;
	// Scope.TxBuf[4]=4;	
	// Scope.TxBuf[5]=5;
	// Scope.TxBuf[6]=6;
	// Scope.TxBuf[7]=7;
	// Scope.TxBuf[8]=8;
	// Scope.TxBuf[9]=9;
	// Scope.TxBuf[10]=10;
	// Scope.TxBuf[11]=11;
	// Scope.TxBuf[12]=12;
	// Scope.TxBuf[13]=0;
	// for(Scope.iTmp=0;Scope.iTmp<TXBUFSIZE-1;Scope.iTmp++)
	// 	Scope.TxBuf[13]=Scope.TxBuf[13]^Scope.TxBuf[Scope.iTmp];
	
	Scope.EnUpdate=1;
}

/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void APP_UsartTransmit_DMA_Setting(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size)
{
	CLEAR_BIT(DMA1_Channel2->CCR ,DMA_CCR_EN);
	WRITE_REG(DMA1->IFCR, DMA_IFCR_CGIF1);
	uint32_t *temp = (uint32_t *)&pData;
	WRITE_REG(DMA1_Channel2->CMAR ,*(uint32_t *)temp);
	WRITE_REG(DMA1_Channel2->CPAR ,(uint32_t)&(USARTx->DR));
	WRITE_REG(DMA1_Channel2->CNDTR ,Size);
	//SET_BIT(DMA1_Channel2->CCR ,DMA_CCR_TCIE);
	SET_BIT(DMA1_Channel2->CCR ,DMA_CCR_EN);
		
	WRITE_REG(USARTx->SR, ~(USART_SR_TC));

	SET_BIT(USARTx->CR3, USART_CR3_DMAT);

	while(READ_BIT(DMA1->ISR, DMA_ISR_TCIF2) != (DMA_ISR_TCIF2));

	//if(READ_BIT(DMA1->ISR, DMA_ISR_TCIF2)== (DMA_ISR_TCIF2))
	{
		WRITE_REG(DMA1->IFCR, DMA_IFCR_CGIF2);
		CLEAR_BIT(USART2->CR3, USART_CR3_DMAT);
	}
}

/*************************************************************************
 Creating by Michael.Wang(Wang Weizi)
 All rights reserved.
*************************************************************************/
#ifdef _SCISCOPE_C_
    #undef _SCISCOPE_C_
#endif

/*************************************************************************
 End of this File (EOF):
 Do not put anything after this part!
*************************************************************************/
