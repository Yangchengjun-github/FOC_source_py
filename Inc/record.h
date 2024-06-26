/**
  ******************************************************************************
  * @file    record.h
  * @author  MCU Application Team
  * @brief   Header for record.c file.
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
#ifndef __RECORD_H
#define __RECORD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define USER_PARA_ADDR					(0x0800F000U)							//参数保存在60K处

#define PAGE_SIZE 						FLASH_PAGE_SIZE							//注意：和030不同 这里是256 Byte
#define SECTOR_SIZE						FLASH_SECTOR_SIZE						//注意：和030不同 这里是8192 Byte

extern ErrorStatus inSideFlash_Read_Bytes(uint8_t *pbData, uint32_t wNum, uint32_t dwAddr);
extern ErrorStatus inSideFlash_Write_Bytes(uint8_t *pBuffer, uint32_t WriteAddr,uint32_t NumByteToWrite);


#ifdef __cplusplus
}
#endif

#endif /* __RECORD_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
