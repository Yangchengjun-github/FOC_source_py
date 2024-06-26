/**
 ******************************************************************************
 * @file    user.c
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
#include "userApp.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
DMA_HandleTypeDef HdmaCh1;
uint32_t gADCxConvertedData[3];
ADC_ChannelConfTypeDef sConfig;
TIM_HandleTypeDef TimHandle;
TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef PWMConfig;
TIM_BreakDeadTimeConfigTypeDef sBreakConfig;
OPA_HandleTypeDef OpaHandle;


userPara_t userPara;															//用户应用参数

/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
#if WDT_EN
IWDG_HandleTypeDef   IwdgHandle;
void wdgInit(void)
{
	IwdgHandle.Instance = IWDG;                     /* Select IWDG */
	IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;  /* Configure prescaler as 32 */
	IwdgHandle.Init.Reload = (1000);                /* IWDG counter reload value is 1000, 1s */
	/* Initialize IWDG */
	if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)       
	{
	APP_ErrorHandler();
	}
}
#endif
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
#if WDT_EN
void clearWdg(void)
{
	HAL_IWDG_Refresh(&IwdgHandle);
}
#endif
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
static void slowErrorDetect(void)
{
	if (mc_stFoc2R.SysStateId > 1)
	{
		if (mc_stFoc2R.TmpX10Degree > MAX_TEMP_TH)								//温度
		{
			mc_stFoc2R.MotorErrCode = MOTOR_OH_ERROR;
			mc_stFoc2R.SysStateId = STATE_STOP_ERROR;
		}

		if (mc_stFoc2R.slVdcLpf.sw.hi > MAX_VDC_THRESHOLD)
		{
			mc_stFoc2R.MotorErrCode = MOTOR_OV_ERROR;
			mc_stFoc2R.SysStateId = STATE_STOP_ERROR;
		}
	}
}

/********************************************************************************
Description:	配置复位脚为IO
Input:
Output:
Return:
*********************************************************************************/
void setResetPinIO(void)
{
	FLASH_OBProgramInitTypeDef OBInitCfg={0};

	if( READ_BIT(FLASH->OPTR, FLASH_OPTR_NRST_MODE) == OB_RESET_MODE_RESET)		//检查是否不是IO
	{
		HAL_FLASH_Unlock();        /* Unlock Flash */
		HAL_FLASH_OB_Unlock();     /* Unlock Option */

		OBInitCfg.OptionType = OPTIONBYTE_USER;

		OBInitCfg.USERType =    OB_USER_IWDG_SW | OB_USER_WWDG_SW | OB_USER_NRST_MODE | OB_USER_nBOOT1 | OB_USER_IWDG_STOP;

		/* Software mode watchdog/GPIO function/System memory as startup area/IWDG STOP mode continues counting */
		OBInitCfg.USERConfig = OB_IWDG_SW | OB_WWDG_SW | OB_RESET_MODE_GPIO | OB_BOOT1_SYSTEM | OB_IWDG_STOP_ACTIVE;

		/* Start option byte programming */
		HAL_FLASH_OBProgram(&OBInitCfg);

		HAL_FLASH_Lock();      /* Lock Flash */
		HAL_FLASH_OB_Lock();   /* Lock Option */

		/* Option Launch */
		HAL_FLASH_OB_Launch();
	}
}

#define USE_ONE_KEY_TEST			(0)											//复用一键冷风按键去标定档位

/********************************************************************************
Description:	根据风速档设置发热丝档位
Input:
Output:
Return:
*********************************************************************************/
void updatHeatSet(void)
{
	#if 1
	if(userPara.sw_heat_cmd != HEAT_CMD_STOP)
	{
		#if USE_ONE_KEY_TEST 
		//stPtc.PTC_Rank = XXXX; 
		#else 
		switch (userPara.sw_speed_cmd)
		{
		case SPEED_CMD_LOWW://在低速风档条件下
			switch (userPara.sw_heat_cmd)
			{
			case HEAT_CMD_LOWW:
				stPtc.PTC_Rank = 6;
				break;
			case HEAT_CMD_MIDL:
				stPtc.PTC_Rank = 10;
				break;
			case HEAT_CMD_HIGH:
				stPtc.PTC_Rank = 13;
				break;
			}
			break;
		case SPEED_CMD_MIDL://在中速风档条件下
			switch (userPara.sw_heat_cmd)
			{
			case HEAT_CMD_LOWW:
				stPtc.PTC_Rank = 9;
				break;
			case HEAT_CMD_MIDL:
				stPtc.PTC_Rank = 12;
				break;
			case HEAT_CMD_HIGH:
				stPtc.PTC_Rank = 16;
				break;
			}
			break;
		case SPEED_CMD_HIGH://在高速风档条件下
			switch (userPara.sw_heat_cmd)
			{
			case HEAT_CMD_LOWW:
				stPtc.PTC_Rank = 11;
				break;
			case HEAT_CMD_MIDL:
				stPtc.PTC_Rank = 15;
				break;
			case HEAT_CMD_HIGH:
				stPtc.PTC_Rank = 19;
				break;
			}
			break;	
		default:
			break;
		}	
		#endif 		
	}
	else //HEAT_CMD_STOP
	{
		userPara.sw_heat_en = HEAT_DIS;		//禁止加温	
	}	
	#else
	//用于PI调节
	if(userPara.sw_heat_cmd != HEAT_CMD_STOP)
	{
		switch (userPara.sw_heat_cmd)
		{
		case HEAT_CMD_LOWW:
			userPara.targetTemperature = 5000;
			break;
		case HEAT_CMD_MIDL:
			userPara.targetTemperature = 6000;
			break;
		case HEAT_CMD_HIGH:
			userPara.targetTemperature = 7000;
			break;
		case HEAT_CMD_STOP:
			userPara.targetTemperature = 0;
			userPara.sw_heat_en = HEAT_DIS;		//禁止加温	
			break;
		}	
	}
	#endif
}

/********************************************************************************
Description:	主应用逻辑功能
Input:
Output:
Return:
*********************************************************************************/
void funTask(void)
{	
	static uint16_t ledFlashTimeCnt=0;
	static uint8_t flag = 0;

	slowErrorDetect();															//慢速错误检测
	//---------------------------------------------------------------------------
	if(mc_stFoc2R.SysStateId == STATE_STOP_NTC_ERROR)							//进入NTC错误时 LED闪烁
	{
		D9_LED4_OFF;															//风速灯灭
		D10_LED5_OFF;
		D11_LED6_OFF;
		if(++ledFlashTimeCnt > (200/FUN_TASK_BASE_MS))							//300ms闪烁频率
		{
			ledFlashTimeCnt=0;
			if(flag)
			{
				flag = 0;
				D6_LED1_ONN;
				D7_LED2_ONN;
				D8_LED3_ONN;
			}
			else 
			{
				flag = 1;
				D6_LED1_OFF;
				D7_LED2_OFF;
				D8_LED3_OFF;
			}
		}
	}
	//---------------------------------------------------------------------------

	#if 1
	if(mc_stFoc2R.SysStateId != STATE_StableRun)								//只有稳定状下按键才生效
		userPara.keyEven = KEY_NULL;
	#endif

	if(userPara.keyEven == KEY_ACTIC )
	{
		userPara.keyEven = KEY_NULL;

		switch (userPara.keyVal)
		{
		case BUTTON_ONE_KEY_OFF:
			//------------------------------------------------------------------			
			#if USE_ONE_KEY_TEST 												
			stPtc.PTC_Rank++;
			if(stPtc.PTC_Rank>20)stPtc.PTC_Rank = 1; 							//用于标定发热丝档位 必须删掉
			#else 
			userPara.sw_heat_en = HEAT_DIS;										//禁止发热丝
			#endif
			//------------------------------------------------------------------
			break;
		case BUTTON_ONE_KEY_ON:
			//------------------------------------------------------------------
			#if 1
			if(userPara.sw_heat_cmd != HEAT_CMD_STOP)
			{
				userPara.sw_heat_en = HEAT_EN;	
				updatHeatSet();//根据风速档 调节发热丝档位
			}
			#endif
			//------------------------------------------------------------------
			break;
		case BUTTON_SPEED_LEVE:
			//------------------------------------------------------------------
			switch (userPara.sw_speed_cmd)										//调风速档				
			{
			case SPEED_CMD_LOWW:
				userPara.speedUpDown = UP;
				userPara.sw_speed_cmd = SPEED_CMD_MIDL;	
				mc_stFoc2R.SpdRefCmd = MIDL_SPEED_CMD;							//执行中速档调速
				break;
			case SPEED_CMD_MIDL:
				if(userPara.speedUpDown == UP) //UP
				{
					userPara.sw_speed_cmd = SPEED_CMD_HIGH;	
					mc_stFoc2R.SpdRefCmd = HIGH_SPEED_CMD;						//执行高速档调速
				}
				else //DOWN
				{
					userPara.sw_speed_cmd = SPEED_CMD_LOWW;
					mc_stFoc2R.SpdRefCmd = LOWW_SPEED_CMD;						//执行低速档调速
				}
				break;
			case SPEED_CMD_HIGH:
				userPara.speedUpDown = DOWN;
				userPara.sw_speed_cmd = SPEED_CMD_MIDL;	
				mc_stFoc2R.SpdRefCmd = MIDL_SPEED_CMD;							//执行中速档调速
				break;
			default:
				break;
			}
			updatHeatSet();//根据风速档 调节发热丝档位
			//------------------------------------------------------------------
			break;
		case BUTTON_HEAT_LEVE:
			//------------------------------------------------------------------
			#if 1
			switch (userPara.sw_heat_cmd)
			{
			case HEAT_CMD_STOP:
				userPara.heatUpDown = UP;
				userPara.sw_heat_cmd = HEAT_CMD_LOWW;							//低温档
				break;
			case HEAT_CMD_LOWW:
				if(userPara.heatUpDown==UP)
					userPara.sw_heat_cmd = HEAT_CMD_MIDL;						//中温档
				else //DOWN
					userPara.sw_heat_cmd = HEAT_CMD_STOP;						//没温档
				break;			
			case HEAT_CMD_MIDL:
				if(userPara.heatUpDown==UP)
					userPara.sw_heat_cmd = HEAT_CMD_HIGH;						//高温档
				else //DOWN
					userPara.sw_heat_cmd = HEAT_CMD_LOWW;						//低温档
				break;
			case HEAT_CMD_HIGH:
				userPara.heatUpDown = DOWN;
				userPara.sw_heat_cmd = HEAT_CMD_MIDL;							//中温档
				break;						
			}
			updatHeatSet();//根据风速档 调节发热丝档位
			#endif
			//------------------------------------------------------------------
			break;
		default:
			break;
		}//end of switch (userPara.keyVal)
		updateLed();	//刷新LED	
	}
	//---------------------------------------------------------------------------
	#if 1
	if(userPara.save == SAVE_EN)  //断电停止时才写入Flash
	{
		D6_LED1_OFF;
		D7_LED2_OFF;
		D8_LED3_OFF;
		D9_LED4_OFF;
		D10_LED5_OFF;
		D11_LED6_OFF;
	
		//LOG_I("Writing");//只在断电前才写入
		//LOG_D("seed: %d heat: %d En: %d",userPara.sw_speed_cmd,userPara.sw_heat_cmd,userPara.sw_heat_en);
		inSideFlash_Write_Bytes((uint8_t *)&userPara,USER_PARA_ADDR,sizeof(userPara_t));//保存设置到Flash
		//LOG_I("End");
		userPara.save = SAVE_OVER;
	}
	#endif
	//---------------------------------------------------------------------------
}

/********************************************************************************
Description:	ADC配置
Input:
Output:
Return:
*********************************************************************************/
static void APP_AdcConfig()
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.ADCClockSelection = RCC_ADCCLKSOURCE_PCLK_DIV4;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	/* ADC时钟使能 */
	__HAL_RCC_ADC_CLK_ENABLE();

	ADC_InjectionConfTypeDef sConfigInjected = {0};

	/** Common config
	 */
	AdcHandle.Instance = ADC1;
	AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;		  /* 分辨率12位 */
	AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;		  // ADC_DATAALIGN_LEFT;//ADC_DATAALIGN_RIGHT;            /* 对齐方式右对齐 */
	AdcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;		  /* 扫描方式使能 */
	AdcHandle.Init.ContinuousConvMode = DISABLE;		  /* 单次模式 */
	AdcHandle.Init.NbrOfConversion = 4;					  /* 转换通道数4*/
	AdcHandle.Init.DiscontinuousConvMode = ENABLE;		  // DISABLE;//             /* 间断模式不使能 */
	AdcHandle.Init.NbrOfDiscConversion = 4;				  /* 间断模式短序列长度为4 */
	AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START; // ADC_EXTERNALTRIGCONV_T1_CC1;  	 /* 软件触发 */

	/* ADC初始化 */
	if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	// OPA2
	sConfig.Channel = ADC_CHANNEL_OPA2_VIN; // IDC
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	/* ADC通道配置 */
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	// Vbus
	sConfig.Channel = ADC_CHANNEL_8; // Vbus
	sConfig.Rank = ADC_REGULAR_RANK_2;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	/* ADC通道配置 */
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	// VTmp
	sConfig.Channel = ADC_CHANNEL_0; // VTmp
	sConfig.Rank = ADC_REGULAR_RANK_3;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	/* ADC通道配置 */
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	// NTC
	sConfig.Channel = ADC_CHANNEL_3; // NTC
	sConfig.Rank = ADC_REGULAR_RANK_4;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	/* ADC通道配置 */
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	/** Configure Injected Channel
	 */
	sConfigInjected.InjectedChannel = ADC_CHANNEL_OPA2_VIN;
	sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
	sConfigInjected.InjectedNbrOfConversion = 1;
	sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	sConfigInjected.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;
	sConfigInjected.AutoInjectedConv = DISABLE;
	sConfigInjected.InjectedDiscontinuousConvMode = ENABLE;
	sConfigInjected.InjectedOffset = 0;
	if (HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &sConfigInjected) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	/* Start ADC conversion on injected group */
	if (HAL_ADCEx_InjectedStart(&AdcHandle) != HAL_OK)
	{
		/* Start Conversation Error */
		APP_ErrorHandler();
	}

	/* ADC校准 */
	if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
	{
		APP_ErrorHandler();
	}
}
/********************************************************************************
Description:	TIM配置函数
Input:
Output:
Return:
*********************************************************************************/
static void APP_TimConfig(void)
{
	__HAL_RCC_TIM1_CLK_ENABLE();									   /* TIM1时钟使能 */
	TimHandle.Instance = TIM1;										   /* TIM1 */
	TimHandle.Init.Period = MOTOR_PWM_TIM1_DUTY - 1;				   /* TIM1重装载值位8000-1 */
	TimHandle.Init.Prescaler = 1 - 1;								   /* 预分频为1000-1 */
	TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;			   /* 时钟不分频 */
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;	   /* 向上计数 */
	TimHandle.Init.RepetitionCounter = 0;							   /* 不重复 */
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /* 自动重装载寄存器没有缓冲 */

	/* 基础时钟初始化 */
	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	PWMConfig.OCMode = TIM_OCMODE_PWM1;				 /* 输出配置为PWM1 */
	PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;		 /* OC通道输出高电平有效 */
	PWMConfig.OCFastMode = TIM_OCFAST_DISABLE;		 /* 输出快速使能关闭 */
	PWMConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;	 /* OCN通道输出高电平有效 */
	PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET; /* 空闲状态OC1N输出低电平 */
	PWMConfig.OCIdleState = TIM_OCIDLESTATE_RESET;	 /* 空闲状态OC1输出低电平 */
	PWMConfig.Pulse = 900;							 /* CC1值为10，占空比=10/50=20% */
	/* 配置通道1 */
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &PWMConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	/* 配置通道2 */
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &PWMConfig, TIM_CHANNEL_2) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	/* 配置通道3 */
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &PWMConfig, TIM_CHANNEL_3) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	/* 配置通道4 */
	PWMConfig.Pulse = 10;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &PWMConfig, TIM_CHANNEL_4) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	sBreakConfig.BreakState = TIM_BREAK_ENABLE;					// TIM_BREAK_ENABLE;  /* 刹车功能使能 */
	sBreakConfig.DeadTime = 72;									// 72;                                    /* 设置死区时间 */
	sBreakConfig.OffStateRunMode = TIM_OSSR_ENABLE;				/* 运行模式下关闭状态选择 OSSR=1 */
	sBreakConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;			/* 空闲状态下关闭状态选择 OSSI=1 */
	sBreakConfig.LockLevel = TIM_LOCKLEVEL_OFF;					/* 锁定关闭 */
	sBreakConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;		/* 刹车输入低电平有效 */
	sBreakConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE; // TIM_AUTOMATICOUTPUT_ENABLE;            /* 自动输出使能 */
	/* 刹车和死区状况配置 */
	if (HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakConfig) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC2REF;                /* 选择更新事件作为触发源 */
	//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;        /* 主/从模式无作用 */
	//  HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig);  /* 配置TIM15*/
	//
	//  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)                       /* TIM15启动 */
	//  {
	//    APP_ErrorHandler();
	//  }

	/* 开启通道1输出 */
	if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	/* 开启通道2输出 */
	if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	/* 开启通道3输出 */
	if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	/* 开启通道1输出 */
	if (HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	/* 开启通道2输出 */
	if (HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	/* 开启通道3输出 */
	if (HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	/* 开启通道4输出 */
	if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
	{
		APP_ErrorHandler();
	}

	//	SET_BIT(TIM1->CR2 , 0x07<<4);//111：比较 – OC4REF 信号被用于作为触发输出(TRGO)。

	////	/*使能UPDATA中断*/
	////  LL_TIM_EnableIT_UPDATE(TIM1);
	////
	////  /*主输出使能*/
	////  LL_TIM_EnableAllOutputs(TIM1);

	/* Enable the Main Output */
	//__HAL_TIM_MOE_ENABLE(&TimHandle);
	//__HAL_TIM_MOE_DISABLE(&TimHandle);

	CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);
	mc_stFoc2R.PwmOnOfState = PWM_OUT_IS_OFF;

	__HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE); /*使能UPDATA中断*/
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);		/*开启UPDATA中断请求*/
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2);

	__HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_CC4);
	NVIC_EnableIRQ(TIM1_CC_IRQn);
	NVIC_SetPriority(TIM1_CC_IRQn, 1);
}
/********************************************************************************
Description:	内部比较器 （用做IDC保护）
Input:
Output:
Return:
*********************************************************************************/
COMP_HandleTypeDef hcomp2;
void InnerComp_Init(void)
{
	GPIO_InitTypeDef COMPINPUT;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_COMP2_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();

	SET_BIT(ADC1->CR2, ADC_CR2_TSVREFE);

	/* GPIO PA4配置为模拟输入 */
	COMPINPUT.Pin = GPIO_PIN_4;
	COMPINPUT.Mode = GPIO_MODE_ANALOG; /* 模拟模式 */
	COMPINPUT.Speed = GPIO_SPEED_FREQ_HIGH;
	COMPINPUT.Pull = GPIO_NOPULL;	  // GPIO_PULLDOWN;               /* 下拉 */
	HAL_GPIO_Init(GPIOA, &COMPINPUT); /* GPIO初始化 */

	hcomp2.Instance = COMP2;									  /* 选择COMP2 */
	hcomp2.Init.InputMinus = COMP_INPUT_MINUS_IO13;				  /* 负输入为VREF(1.2V) */
	hcomp2.Init.InputPlus = COMP_INPUT_PLUS_IO4;				  /* 正输入选择为PA4 */
	hcomp2.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;			  /* COMP2极性选择为不反向 */
	hcomp2.Init.Mode = COMP_POWERMODE_HIGHSPEED;				  /* COMP2功耗模式选择为High speed模式 */
	hcomp2.Init.Hysteresis = COMP_HYSTERESIS_DISABLE;			  /* 迟滞功能关闭 */
	hcomp2.Init.WindowMode = COMP_WINDOWMODE_DISABLE;			  /* 窗口模式关闭 */
	hcomp2.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING_FALLING; /* COMP2上升沿/下降沿触发 */

	/* COMP2初始化 */
	if (HAL_COMP_Init(&hcomp2) != HAL_OK)
	{
		APP_ErrorHandler();
	}
	__HAL_SYSCFG_COMP2_BREAK_TIM1();
	/* COMP2启动 */
	HAL_COMP_Start(&hcomp2);

	LL_EXTI_ClearFlag(COMP_EXTI_LINE_COMP2);
	NVIC_EnableIRQ(ADC_COMP_IRQn);
	NVIC_SetPriority(ADC_COMP_IRQn, 0);
}
/********************************************************************************
Description:	错误执行函数
Input:
Output:
Return:
*********************************************************************************/
void APP_ErrorHandler(void)
{
	/* 无限循环 */
	while (1)
	{
		LOG_E("APP_ErrorHandler");
	}
}
/********************************************************************************
Description:	
Input:
Output:
Return:
*********************************************************************************/
void updateLed(void)
{
	switch (userPara.sw_speed_cmd)												//根据风速档位设置LED												
	{
	case SPEED_CMD_LOWW:
		D9_LED4_ONN;
		D10_LED5_OFF;
		D11_LED6_OFF;
		break;
	case SPEED_CMD_MIDL:
		D9_LED4_ONN;
		D10_LED5_ONN;
		D11_LED6_OFF;
		break;
	case SPEED_CMD_HIGH:
		D9_LED4_ONN;
		D10_LED5_ONN;
		D11_LED6_ONN;
		break;
	}
	if(userPara.sw_heat_en == HEAT_EN)
	{
		switch (userPara.sw_heat_cmd)												//根据发热丝档位设置LED												
		{
		case HEAT_CMD_STOP:
			D6_LED1_OFF;															//发热丝灯全灭
			D7_LED2_OFF;
			D8_LED3_OFF;
			break;
		case HEAT_CMD_LOWW:
			D6_LED1_OFF;									
			D7_LED2_OFF;
			D8_LED3_ONN;									
			break;
		case HEAT_CMD_MIDL:
			D6_LED1_OFF;									
			D7_LED2_ONN;
			D8_LED3_ONN;									
			break;
		case HEAT_CMD_HIGH:
			D6_LED1_ONN;									
			D7_LED2_ONN;
			D8_LED3_ONN;
			break;
		}	
	}
	else 
	{
		D6_LED1_OFF;															//发热丝灯全灭
		D7_LED2_OFF;
		D8_LED3_OFF;		
	}
}
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void allLedOff(void)
{
	D9_LED4_OFF;																//档位灯初始化时全灭
	D10_LED5_OFF;
	D11_LED6_OFF;		
	D6_LED1_OFF;																//发热丝灯初始化时全灭
	D7_LED2_OFF;
	D8_LED3_OFF;	
}
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void userAppInit(void)
{
	setResetPinIO();															//配置复位脚位IO
	UART_Config();	
															//串口初始化
	heaterInit();
	
	mc_stFoc2R.SysStateId = STATE_POWERON;
	PUYA_SEMI_MC_FOC1R_INIT();
	mc_stFoc2R.MotDirCmd = MOT_DIR_CWC;//MOT_DIR_CCW;//MOT_DIR_CWC;				//电机转动方向
	//---------------------------------------------------------------------------
	userPara_t temp;
	inSideFlash_Read_Bytes((uint8_t *)&temp, sizeof(userPara_t),USER_PARA_ADDR);//读Flash里风速档位 和 发热丝档位
	//LOG_D("seed: %d heat: %d En: %d",temp.sw_speed_cmd,temp.sw_heat_cmd,temp.sw_heat_en);

	if((temp.sw_speed_cmd >= SPEED_CMD_LOWW) && (temp.sw_speed_cmd <= SPEED_CMD_HIGH) )
		userPara.sw_speed_cmd = temp.sw_speed_cmd;								//Flash记录的风速档位
	else 
		userPara.sw_speed_cmd = SPEED_CMD_LOWW;									//默认低速档
	//mc_stFoc2R.SpdRefCmd = LOWW_SPEED_CMD;									//执行调速	

	if((temp.sw_heat_cmd >= HEAT_CMD_STOP) && (temp.sw_heat_cmd <= HEAT_CMD_HIGH) )
	{
		if(temp.sw_heat_cmd == HEAT_CMD_STOP)
			userPara.sw_heat_en = HEAT_DIS;
		else 
		{
			userPara.sw_heat_en = HEAT_EN;
			userPara.sw_heat_cmd = temp.sw_heat_cmd;							//Flash记录的发热丝档位
		}
	}
	else 
	{
		userPara.sw_heat_en = HEAT_DIS;											//默热禁止加热
		userPara.sw_heat_cmd = HEAT_CMD_STOP;									//默认不开温
	}
	
	allLedOff();																//上电时LED全灭
	updatHeatSet();																//根据风速档 调节发热丝档位
	//---------------------------------------------------------------------------
	// mc_stFoc2R.SW1_ID = 3;
	// mc_stFoc2R.SW1_Cmd = 3;
	// mc_stFoc2R.SW1_ID=2; mc_stFoc2R.SW1_Cmd=2;
	// mc_stFoc2R.SW1_ID=1; mc_stFoc2R.SW1_Cmd=1;
	stGrid.Status = GRID_OFF;

	/* 使能OPA2*/
	__HAL_RCC_OPA_CLK_ENABLE();
	OPA->CR0 = 1 << 6; // 0x0ffff;
	OPA->CR1 = 1 << 6; // 0x0ffff;

	/* ADC配置 */
	APP_AdcConfig();
	HAL_ADC_Start_DMA(&AdcHandle, gADCxConvertedData, 4);

	/* TIM & COMP 配置 */
	mc_stFoc2R.PwmIsr_cnt2 = 0;
	APP_TimConfig();
	InnerComp_Init();

	/* 使能DIV */
	__HAL_RCC_DIV_CLK_ENABLE();
	Divhandle.Instance = DIV;				  /* 除法器基地址        */
	Calculatervalue.Sign = DIV_MODE_UNSIGNED; /* 无符号除法          */
	//	Calculatervalue.Dividend  = 10000;
	//	Calculatervalue.Divisor   = 100;
	//	HAL_DIV_Calculate(&Divhandle, &Calculatervalue);
	//	mc_stFoc2R.DivVal=Calculatervalue.Quotient;
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
