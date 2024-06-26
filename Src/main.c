/**
 ******************************************************************************
 * @file    main.c
 * @author  MCU Application Team
 * @brief   Main program body
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
#include "main.h"

/********************************************************************************
Description:	系统时钟配置函数
Input:
Output:
Return:
*********************************************************************************/
static void systemClockConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/* 配置时钟源HSE/HSI/LSE/LSI */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;						  			/* 开启HSI */
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;						  			/* 不分频 */
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; 			/* 配置HSI输出时钟为24MHz */
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;						 			/* 关闭HSE */
	RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;					  			/* HSE工作频率范围16M~32M */
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;						  			/* 关闭LSI */
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;						  			/* 关闭LSE */
	RCC_OscInitStruct.LSEDriver = RCC_ECSCR_LSE_DRIVER_1;			  			/* LSE默认驱动能力 */
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;					  			/* 开启PLL */
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;			  			/* PLL的时钟源 */
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;					  			/* PLL 3倍频输出 */

	/* 初始化RCC振荡器 */
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	/*初始化CPU,AHB,APB总线时钟*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCC系统时钟类型 */
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;					/* 配置PLL为系统时钟 */
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;							/* APH时钟不分频 */
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;							/* APB时钟不分频 */
	/* 初始化RCC系统时钟 */
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		APP_ErrorHandler();
	}
}
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
int main(void)
{
	HAL_Init();																	// 初始化所有外设，Flash接口，SysTick 
	systemClockConfig();														// 系统时钟配置
																				// 电机相关初始化		
	userAppInit();																// 用户应用初始化

	taskRegister(keyTask,KEY_TASK_TIME_BASE_MS);								// 按键（输入）		
	taskRegister(funTask,FUN_TASK_BASE_MS);										// 用户功能逻辑（处理）
	taskRegister(heaterTask,HEATER_TASK_TIME_BASE_MS);							// 发热丝温度控制（输入输出）
	taskRegister(hmiUpdateTask,HMI_TASK_TIME_BASE_MS);							// 人机交互（输入输出）
	while (1)
	{
		taskRun();																// 分时调用
	}
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  输出产生断言错误的源文件名及行号
 * @param  file：源文件名指针
 * @param  line：发生断言错误的行号
 * @retval 无
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* 用户可以根据需要添加自己的打印信息,
	   例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* 无限循环 */
	while (1)
	{
	}
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
