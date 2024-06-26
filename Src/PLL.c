/**
 ******************************************************************************
 * @file    PLL.c
 * @author  MCU Application Team
 * @Version V1.0.0
 * @Date    2023-03-01
 * @brief   BLDC FOC Sensorless under 1R Sensing Current
 ******************************************************************************
 **/

/************************************************************************
 Included File:
*************************************************************************/
#include "main.h"

PUYA_SEMI_MC_PLL_Base_TYPE mc_PLL;

void PUYA_SEMI_MC_PLL_Init(PUYA_SEMI_MC_PLL_Base_TYPE *PLL)
{
	//======================================================================================================================================//
	//  Var Initial
	//======================================================================================================================================//
	PLL->We = 0;
	PLL->SinCosPLL.uwSitaPu = 0;

	//======================================================================================================================================//
	// Motor-Parameter Setting for PLL Observer
	// Note: 1. If motor para is not reasonable which leading to overflow, the system will repor Fault of "MOTOR_ELSE_ERROR"
	//       2. These Parameters just using during the begining of start-up, after runing system will auto-online-tuning the parameters
	//======================================================================================================================================//
	PLL->swLsPu = 591;
	PLL->swRsPu = 3881;
}

/*************************************************************************
 End of this File (EOF):
 Do not put anything after this part!
*************************************************************************/
