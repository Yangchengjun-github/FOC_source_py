/**
  ******************************************************************************
  * @file    key.c
  * @author  MCU Application Team
  * @brief   
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
#include "key.h"

/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void keyInit(void)
{

	
}

/********************************************************************************
Description:	按键扫描
Input:
Output:
Return:
*********************************************************************************/
void keyTask(void)
{
//	static uint8_t pressTimeCount = 0;
	static uint8_t debugTime = 0;
	static uint8_t lastKey = (BUTTON_ONE_KEY_OFF|BUTTON_SPEED_LEVE|BUTTON_HEAT_LEVE);

	uint8_t curentKey = 0x00;

	if (SW1_VAL==GPIO_PIN_RESET)
		curentKey |= BUTTON_SPEED_LEVE;
	if (SW2_VAL==GPIO_PIN_RESET)
		curentKey |= BUTTON_HEAT_LEVE;
	if (SW3_VAL==GPIO_PIN_RESET)
		curentKey |= BUTTON_ONE_KEY_OFF;

	if (curentKey != lastKey)
	{
		if (++debugTime <= (KEY_DUBUG_TIME / KEY_TASK_TIME_BASE_MS)) //消抖 80ms
			return;
		lastKey = curentKey;

		if (curentKey & BUTTON_ONE_KEY_OFF) //一键冷风 按下
		{
			//LOG_I("ONE_KEY_OFF Press");
			userPara.keyVal = BUTTON_ONE_KEY_OFF;
			userPara.keyEven = KEY_ACTIC;
		}
		else 
		{
			userPara.keyVal = BUTTON_ONE_KEY_ON;
			userPara.keyEven = KEY_ACTIC;			
		}
		
		if (curentKey & BUTTON_SPEED_LEVE) //风速档 按下
		{
			//LOG_I("SPEED_LEVE Press");
			userPara.keyVal = BUTTON_SPEED_LEVE;
			userPara.keyEven = KEY_ACTIC;
		}
		if (curentKey & BUTTON_HEAT_LEVE) //温度档 按下
		{
			//LOG_I("HEAT_LEVE Press");
			userPara.keyVal = BUTTON_HEAT_LEVE;
			userPara.keyEven = KEY_ACTIC;
		}
	}
	else
	{
		debugTime = 0; //清消抖
		#if 0
		if (curentKey & (BUTTON_ONE_KEY_OFF)) //SW键长按
		{
			if (pressTimeCount <= (KEY_LONG_PRESS_TIME / KEY_TASK_TIME_BASE_MS))
			{
				if (pressTimeCount++ == (KEY_LONG_PRESS_TIME / KEY_TASK_TIME_BASE_MS)) //长按
				{
					LOG_W("LONG Press");
					// userPara.keyVal = BUTTON_ONE_KEY_OFF;
					// userPara.keyEven = KEY_ACTIC;
				}
			}
		}
		else
			pressTimeCount = 0; //清空计数
		#endif
	}	
}


/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
