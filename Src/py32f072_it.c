/**
 ******************************************************************************
 * @file    py32f072_it.c
 * @author  MCU Application Team
 * @brief   Interrupt Service Routines.
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
#include "py32f072_it.h"
extern TIM_HandleTypeDef TimHandle;
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*          Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
	while (1)
	{
	}
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

/**
 * @brief This function handles 20K->50uS.
 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	if (TIM1->CNT > MOTOR_PWM_TIM1_ISR_TH)
	{
		;
	}
	else
	{
		WRITE_REG(TIM1->CCR4, mc_stFoc2R.DutyMax);
		mc_stFoc2R.CH4_Sample_ID = 0;
		PwmIsrCallback();
	}

	__HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief This function handles .
 */
void TIM1_CC_IRQHandler(void)
{
	if (mc_stFoc2R.CH4_Sample_ID == 0)
	{
		mc_stFoc2R.IbusAdc1 = (int16_t)ADC1->JDR1;
		WRITE_REG(TIM1->CCR4, mc_stFoc2R.DutyMid);
		mc_stFoc2R.CH4_Sample_ID = 1;
	}
	else
	{
		PUYA_SEMI_MC_FOC1R_IdcSample2();
	}
	__HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC4);
	SET_BIT(ADC1->CR2, (ADC_CR2_JSWSTART | ADC_CR2_EXTTRIG));
}

/**
 * @brief This function handles .
 */
void ADC_COMP_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag(COMP_EXTI_LINE_COMP2) != RESET)
	{
		PUYA_SEMI_MC_FocPWM_OFF();
		mc_stFoc2R.MotorErrCode = MOTOR_HOC_ERROR;
		mc_stFoc2R.SysStateId = STATE_STOP_ERROR;
	}
	mc_stFoc2R.HOC_Cnt++;
	LL_EXTI_ClearFlag((COMP_EXTI_LINE_COMP1 | COMP_EXTI_LINE_COMP2)); /* ???D?? */
}

/**
 * @brief This function handles DMA1 channel2 and channel3 Interrupt .
 */
void DMA1_Channel2_3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void DMA1_Channel4_5_6_7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
 * @brief This function handles USART2 Interrupt .
 */
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}
/******************************************************************************/
/* PY32F072 Peripheral Interrupt Handlers                                     */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file.                                          */
/******************************************************************************/

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
