/**
  ******************************************************************************
  * @file    main.h
  * @author  MCU Application Team
  * @brief   Header for main.c file.
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
#ifndef __MAIN_H
#define __MAIN_H



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f072_hal.h"
#include "py32f072_hal_opa.h"
#include "py32f072_hal_opa_ex.h"
	
typedef int8_t      SBYTE; //1 bytes signed
typedef uint8_t     UBYTE; //1 bytes unsigned
typedef int16_t     SWORD; //2 bytes signed
typedef uint16_t    UWORD; //2 bytes unsigned
typedef int32_t     SLONG; //4 bytes signed
typedef uint32_t    ULONG; //4 bytes unsigned
typedef int64_t    	SQWORD;//8 bytes unsigned
typedef uint64_t  	UQWORD;//8 bytes unsigned
typedef void        VOID;

/*----------------------------------------------------------------*/
typedef union{
          struct{           
            UWORD low;
            SWORD hi;             
          }sw;
        SLONG sl;
}SLONG_UNION;
/*----------------------------------------------------------------*/
typedef union{
          struct{
            UWORD low;
            UWORD hi; 
          }uw;
        ULONG ul;
}ULONG_UNION;

#include "task.h"
#include "userApp.h"
#include "key.h"
#include "heater.h"
#include "uartHMI.h"
#include "record.h"
#include "FOC2R.h"
#include "PLL.h"
#include "PuyaMath.h"
	
extern UART_HandleTypeDef UartHandle;

#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOA
#define TIMx_GPIO_PORT_CHANNEL2        GPIOA
#define TIMx_GPIO_PORT_CHANNEL3        GPIOA
#define TIMx_GPIO_PORT_CHANNEL4        GPIOA
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_8
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_9
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_10
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_11


#define DEBUG_USART_CLK_ENABLE()                do { \
                                                     __IO uint32_t tmpreg = 0x00U; \
                                                     SET_BIT(RCC->APBENR1, RCC_APBENR1_USART2EN);\
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APBENR1, RCC_APBENR1_USART2EN);\
                                                     UNUSED(tmpreg); \
                                                   } while(0U)
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported variables prototypes ---------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void APP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
