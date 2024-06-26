/**
  ******************************************************************************
  * @file    heater.h
  * @author  MCU Application Team
  * @brief   Header for heater.c file.
  *          
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
#ifndef __HEATER_H
#define __HEATER_H



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define	Success			0x01
#define	Fail				0x00

#define POWER_VOL					4.6			//5.0V是串联电路中的电源电压，可根据实际电路修改
#define RES_20K						20			//单位kΩ；温度传感器串联分压电阻，可根据实际电路修改


extern void heaterInit(void);
extern void heaterTask(void);
extern int16_t PUYA_SEMI_MC_Adc2TmpDegreeX100(int16_t NtcAdc);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
