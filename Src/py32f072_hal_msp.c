/**
 ******************************************************************************
 * @file    py32f072_hal_msp.c
 * @author  MCU Application Team
 * @brief   This file provides code for the MSP Initialization
 *          and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
 * All rights reserved.</center></h2>
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/
extern DMA_HandleTypeDef HdmaCh1;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;
/********************************************************************************
Description:	初始化全局MSP
Input:
Output:
Return:
*********************************************************************************/
void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}
/********************************************************************************
Description:	初始化TIM相关MSP
Input:
Output:
Return:
*********************************************************************************/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* TIM1时钟使能 */
	__HAL_RCC_TIM1_CLK_ENABLE();
	/* GPIOA时钟使能 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* GPIOB时钟使能 */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* GPIOF时钟使能 */
	__HAL_RCC_GPIOF_CLK_ENABLE();

	/*初始化PB7/PB8/PB3/PB4/PB5/PB6为TIM*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_TIM1;

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//	GPIO_InitStruct.Alternate = GPIO_AF14_TIM1;
	//  GPIO_InitStruct.Pin = GPIO_PIN_8; HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


}
/********************************************************************************
Description:	初始化ADC相关MSP
Input:
Output:
Return:
*********************************************************************************/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE(); /* SYSCFG时钟使能 */
	__HAL_RCC_DMA_CLK_ENABLE();	   /* DMA时钟使能 */
	__HAL_RCC_GPIOA_CLK_ENABLE();  /* 使能GPIOA时钟 */

	/* ------------ */
	/* DMA配置      */
	/* ------------ */
	HdmaCh1.Instance = DMA1_Channel1;						/* 选择DMA通道1 */
	HdmaCh1.Init.Direction = DMA_PERIPH_TO_MEMORY;			/* 方向为从外设到存储器 */
	HdmaCh1.Init.PeriphInc = DMA_PINC_DISABLE;				/* 禁止外设地址增量 */
	HdmaCh1.Init.MemInc = DMA_MINC_ENABLE;					/* 使能存储器地址增量 */
	HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* 外设数据宽度为32位 */
	HdmaCh1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;	/* 存储器数据宽度位32位 */
	HdmaCh1.Init.Mode = DMA_CIRCULAR;						/* 循环模式 */
	HdmaCh1.Init.Priority = DMA_PRIORITY_VERY_HIGH;			/* 通道优先级为很高 */

	HAL_DMA_DeInit(&HdmaCh1); /* DMA反初始化 */
	HAL_DMA_Init(&HdmaCh1);	  /* 初始化DMA通道1 */

	HAL_DMA_ChannelMap(&HdmaCh1, DMA_CHANNEL_MAP_ADC1);
	__HAL_LINKDMA(hadc, DMA_Handle, HdmaCh1); /* 连接DMA句柄 */



	/************************************************************************************************/
	GPIO_InitTypeDef GPIO_InitStruct;
	// OPA IO Initial
	/* 使能GPIOA时钟 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitStruct.Alternate = 0;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	/* OPA2输出 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* OPA2负端输入 */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* OPA2正端输入 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* PA3 AN输入--NTC */ 							//暂时可能没用到？
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* PA4 AN输入--COMP2P4---AN4--IDC_OC */			//比较器输入
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* PB0 AN输入--Vbus */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/* PB1 AN输入--IdcMean */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	

	/*DI FOR 按键SW1*/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*DI FOR 按键SW2*/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*DI FOR 按键SW3*/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*DO FOR LED 显示*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*DO FOR 可控硅控制*/
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); // #define PTC_CTR_ONN 	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
	PTC_CTR_OFF;
	/*DI FOR 过零点输入*/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}
/********************************************************************************
Description:	初始化UART相关MSP
Input:
Output:
Return:
*********************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	/* 使能时钟 */
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_DMA_CLK_ENABLE();

	//  /**USART2 引脚配置
	//    PA2     ------> USART2_TX
	//    PA3     ------> USART2_RX
	//    */
	//  GPIO_InitTypeDef  GPIO_InitStruct;
	//  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//  GPIO_InitStruct.Pull = GPIO_PULLUP;
	//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
	//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART2 DMA相关配置 */
	/* USART2_TX 初始化 */
	hdma_usart2_tx.Instance = DMA1_Channel2;
	hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx.Init.Mode = DMA_NORMAL;
	hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_usart2_tx);
	/* USART2_RX 初始化 */
	hdma_usart2_rx.Instance = DMA1_Channel4;
	hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_rx.Init.Mode = DMA_NORMAL;
	hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_usart2_rx);

	/* 配置DMA请求映像 */
	HAL_DMA_ChannelMap(&hdma_usart2_tx, DMA_CHANNEL_MAP_USART2_WR);
	HAL_DMA_ChannelMap(&hdma_usart2_rx, DMA_CHANNEL_MAP_USART2_RD);

	/* 使能NVIC */
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	//  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 1);
	//  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
