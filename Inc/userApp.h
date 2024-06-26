/**
 ******************************************************************************
 * @file    userApp.h
 * @author  MCU Application Team
 * @brief   Header for userApp.c file.
 *          This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USERAPP_H
#define __USERAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define KEY_TASK_TIME_BASE_MS				(39)
#define FUN_TASK_BASE_MS					(3)
#define HEATER_TASK_TIME_BASE_MS			(10)
#define HMI_TASK_TIME_BASE_MS				(0)

#define KEY_DUBUG_TIME 						(40)		   							// 按键消抖 MS
#define KEY_LONG_PRESS_TIME 				(3000) 									// 长按时间

#define BUTTON_ONE_KEY_OFF 					0x01									// 一键冷风
#define BUTTON_SPEED_LEVE					0x02									// 速度档位
#define BUTTON_HEAT_LEVE 					0x04									// 加热档位
#define BUTTON_ONE_KEY_ON 					0x08									// 一键冷风

#define ZERO_VAL		HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_5)							//读过零点引脚
	
#define SW1_VAL			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)						//按键
#define SW2_VAL			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)						//按键
#define SW3_VAL			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)						//按键

#define PTC_CTR_ONN 	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)		//可控硅控制脚
#define PTC_CTR_OFF 	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)
	
#define D6_LED1_ONN		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, GPIO_PIN_RESET);		//加热LED最高档
#define D6_LED1_OFF		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, GPIO_PIN_SET);
	
#define D7_LED2_ONN		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);		//加热LED中档
#define D7_LED2_OFF		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

#define D8_LED3_ONN		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);		//加热LED最低档
#define D8_LED3_OFF		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);	
	
#define D9_LED4_ONN		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);		//风速LED最低档
#define D9_LED4_OFF		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

#define D10_LED5_ONN	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);		//风速LED中档
#define D10_LED5_OFF	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

#define D11_LED6_ONN	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);		//风速LED最高档
#define D11_LED6_OFF	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);	


typedef enum
{
	KEY_NULL = 0,																	//没按键事件
	KEY_ACTIC,																		//有按键事件
}keyEven_e;

typedef enum
{
	SPEED_CMD_STOP = 0,
	SPEED_CMD_LOWW = 1,
	SPEED_CMD_MIDL = 2,
	SPEED_CMD_HIGH = 3,
}sw_speed_cmd_e;																	//风速档位

typedef enum
{
	HEAT_CMD_STOP = 0,
	HEAT_CMD_LOWW = 1,
	HEAT_CMD_MIDL = 2,
	HEAT_CMD_HIGH = 3,
}sw_heat_cmd_e;																		//加热丝档位

typedef enum
{
	HEAT_DIS = 0,
	HEAT_EN = 1,
}sw_heat_en_e;																		//一键冷风	

typedef enum
{
	SAVE_DIS = 0,
	SAVE_EN = 1,
	SAVE_OVER = 2,
}save_e;																			//记录使能标

typedef enum
{
	UP = 0,
	DOWN = 1,
}upDown_e;

typedef enum
{
	NTC_OK = 0,
	NTC_SHORT = 1,
	NTC_OPEN = 2,
}ntcSta_e;

typedef enum
{
	TEMP_OK = 0,
	TEMP_HIGH = 1,
}tempAlam_e;

typedef struct 
{
	uint8_t keyVal;																	//按键值
	keyEven_e keyEven;																//按键事件
	upDown_e speedUpDown;															//档位升降标记
	upDown_e heatUpDown;															//档位升降标记
	sw_speed_cmd_e sw_speed_cmd;													//风速档位 		0-1-2-3
	sw_heat_cmd_e sw_heat_cmd;														//发热丝档位	0-1-2-3
	sw_heat_en_e sw_heat_en;														//一键冷风 		0:冷风 1:加热
	ntcSta_e ntcSta;																//NTC状态
	save_e save;																	//记录标记		0:不记录1：记录
	int16_t targetTemperature;														//目标温度
	tempAlam_e tempAlam;															//温度警报
}userPara_t;

extern userPara_t userPara;															//用户应用参数（全局变量）

extern void userAppInit(void);
extern void keyTask(void);
extern void funTask(void);
extern void heaterTask(void);
extern void hmiUpdateTask(void);
extern void updateLed(void);
extern void allLedOff(void);
extern void updatHeatSet(void);

#ifdef __cplusplus
}
#endif

#endif /* __USERAPP_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
