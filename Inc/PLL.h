/**
  ******************************************************************************
  * @file    PLL.h
  * @author  MCU Application Team
  * @Version V1.0.0
  * @Date    2023-03-01
  * @brief   BLDC FOC Sensorless under 1R Sensing Current
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLL_H
#define __PLL_H

#ifdef __cplusplus
extern "C" {
#endif

//************************************************************************************************************************   
// Define SMC Control Parameter
//************************************************************************************************************************ 
	#define		MAX_SMC_ERROR					1
	#define		K_SLIDE								32767					//	32767	*	0.5	
	#define		K_CONSTANT						K_SLIDE/MAX_SMC_ERROR				

//************************************************************************************************************************   
// Define Current Observer Parameter
//************************************************************************************************************************ 	

/** 
  * @brief PLL parameters  
  */
typedef struct
{
	int16_t	swRsPu;
	int16_t	swLsPu;
	int16_t Valfa;
	int16_t Vbeta;
	int16_t Ialfa;
	int16_t Ibeta;
	int16_t We;
	SIN_COS SinCosPLL;
}PUYA_SEMI_MC_PLL_Base_TYPE;             /*!< PLL Data Structure */

void PUYA_SEMI_MC_PLL_Init(PUYA_SEMI_MC_PLL_Base_TYPE *PLL);
void PUYA_SEMI_MC_PLL_Calc(PUYA_SEMI_MC_PLL_Base_TYPE *PLL);

extern PUYA_SEMI_MC_PLL_Base_TYPE mc_PLL;


#endif
/*************************************************************************
 End of this File (EOF):
 !!!!!!Do not put anything after this part!!!!!!!!!!!
*************************************************************************/
