/**
  ******************************************************************************
  * @file    PuyaMath.c
  * @author  MCU Application Team
  * @Version V1.0.0
  * @Date    2023-03-01
  * @brief   BLDC FOC Sensorless under 1R Sensing Current
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PuyaMath_H
#define __PuyaMath_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
  * @brief PuyaMath parameters  
  */
extern const signed int Tabls2[];
extern const signed int SinTab_1024[];
void voTabSinCos(SIN_COS *in);
extern void PUYA_SEMI_MC_FOC1R_PI_Ctrl(PUYA_SEMI_MC_FOC1R_PI_TYPE *stPI);
extern SWORD man_swVd,man_swVq;
#endif
/*************************************************************************
 End of this File (EOF):
 !!!!!!Do not put anything after this part!!!!!!!!!!!
*************************************************************************/
