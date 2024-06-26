/**
  ******************************************************************************
  * @file    record.c
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
#include "record.h"

/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
// int8_t FLASH_ProgramWord(uint32_t address, uint32_t data)
// {
// 	uint32_t flashOffset = 0,pageOffset = 0 ;
// 	uint32_t pageNum = 0 ,pageAddr = 0;
// 	uint32_t pageError = 0;
// 	FLASH_EraseInitTypeDef EraseInitStruct = {0};
// 	uint8_t pageTempBuf[FLASH_PAGE_SIZE];
// 	uint32_t idx = 0,addr = 0;

// 	//word-aligned
// 	if (address % sizeof(uint32_t))
// 	{
// 		return -1;
// 	}
// 	if ((address > FMC_END_ADDR) || ((address < FMC_START_ADDR)))
// 	{
// 		return -1;
// 	}
// 	//所在page
// 	flashOffset = address - FMC_START_ADDR;
// 	pageNum = (flashOffset / FLASH_PAGE_SIZE);
// 	pageAddr = FMC_START_ADDR + pageNum * FLASH_PAGE_SIZE;
// 	pageOffset = address - pageAddr;

// 	//read page
// 	addr = pageAddr;
	
// 	for(idx= 0; idx < FLASH_PAGE_SIZE; idx++)
// 		pageTempBuf[idx] = *(uint8_t *)(addr + idx);
	
// 	//解锁FLASH
// 	HAL_FLASH_Unlock();                                                                 

// 	//erase page
// 	EraseInitStruct.TypeErase  	  = FLASH_TYPEERASE_PAGEERASE;
// 	EraseInitStruct.PageAddress   = pageAddr;
// 	EraseInitStruct.NbPages 	  = 1;

// 	if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK)	//sector erase
// 	{
// 		return -1;
// 	}
// 	//modify page content
// 	*(uint32_t *)((uint8_t *)pageTempBuf + pageOffset) = data;

// 	//page program
// 	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE,pageAddr,(uint32_t *)pageTempBuf) != HAL_OK)  
// 	{
// 		return -1;
// 	}
// 	//锁定FLASH
// 	HAL_FLASH_Lock();                                                                   

// 	return 0;
// }
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
//word-aligned
// int8_t Flash_Program(uint32_t flashStartAddr, uint32_t *data, uint16_t len)
// {
// 	uint16_t loop = 0;
	
// 	if ((flashStartAddr + (len * sizeof(uint32_t))) > FMC_END_ADDR)
// 	{
// 		return -1;
// 	}

// 	if (flashStartAddr % sizeof(uint32_t))
// 	{
// 			return -1;
// 	}

// 	for (loop = 0; loop < len; loop++)
// 	{
// 		if(FLASH_ProgramWord(flashStartAddr, data[loop]) == 0)
// 		{
// 			flashStartAddr += sizeof(uint32_t);
// 		}
// 		else
// 			return -1;
// 	}
	
// 	return 0;
// }


/********************************************************************************
Description:	页擦除(0x80==128 Byte)
Input:
Output:
Return:
*********************************************************************************/
ErrorStatus flashEeErase(uint32_t pageStarAdd,uint32_t pageNum)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	/* 解锁FLASH */
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGEERASE;		   		//擦写类型FLASH_TYPEERASE_PAGEERASE=Page擦, FLASH_TYPEERASE_SECTORERASE=Sector擦
	EraseInitStruct.PageAddress = pageStarAdd;		   					//擦写起始地址
	EraseInitStruct.NbPages = pageNum;								   //需要擦写的页数量
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) //执行page擦除,PAGEError返回擦写错误的page,返回0xFFFFFFFF,表示擦写成功
	{
		//LOG_E("Erro Erase");
		/* 锁定FLASH */
		HAL_FLASH_Lock();
		return ERROR;
	}
	/* 锁定FLASH */
	HAL_FLASH_Lock();
	return SUCCESS;
}
/********************************************************************************
Description:    内Flash任意地址写多字节(这个版本单次写入最大为128字节)
Input:			
Output:			
Return:			
*********************************************************************************/
uint32_t pageBuf[PAGE_SIZE*2];
ErrorStatus inSideFlash_Write_Bytes(uint8_t *pBuffer, uint32_t WriteAddr,uint32_t NumByteToWrite)
{
	//1、计算开始地址在哪个Page
	//2、计算总要擦除的Page数量
	//3、对跨区的必须在擦除前读、改、写
    uint32_t page_pos;
    uint8_t page_off;
	uint8_t *pPageBuf = (uint8_t *)pageBuf;

	if(NumByteToWrite > PAGE_SIZE)	//这个版本单次写入最大为128字节
	{
		//LOG_E("Too Big");
		return ERROR;
	}

	page_pos = (WriteAddr / PAGE_SIZE);                                             //128字节对齐地址数 
	page_off = WriteAddr % PAGE_SIZE;                                             //128字节剩余字节数
	
	#if 0
	LOG_I("page_pos: %d",page_pos);
	LOG_I("page_off: %d",page_off);
	#endif
	
	memcpy(pPageBuf,(uint8_t *)(page_pos*PAGE_SIZE),PAGE_SIZE*2);		//1、读2页
	flashEeErase((page_pos*PAGE_SIZE),2);								//2、擦2页

	memcpy(&pPageBuf[page_off],pBuffer,NumByteToWrite);					//3、修改 小于或等于1页 
	
	#if 1
	/* 解锁FLASH */
	HAL_FLASH_Unlock();
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, (page_pos*PAGE_SIZE),pageBuf) != HAL_OK) //4、写1页
	{
		//LOG_E("1");
		/* 锁定FLASH */
		HAL_FLASH_Lock();
		return ERROR;
	}
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, ((page_pos+1)*PAGE_SIZE),(uint32_t *)&pPageBuf[PAGE_SIZE]) != HAL_OK) //4、写2页
	{
		//LOG_E("2");
		/* 锁定FLASH */
		HAL_FLASH_Lock();
		return ERROR;
	}
	/* 解锁FLASH */
	HAL_FLASH_Lock();
	#endif
    return SUCCESS;
}

/********************************************************************************
Description:    内Flash读多字节
Input:			
Output:			
Return:			
*********************************************************************************/
ErrorStatus inSideFlash_Read_Bytes(uint8_t *pbData, uint32_t wNum, uint32_t dwAddr)
{
	#if 0
    uint32_t i;
    for(i=0;i<wNum;i++)
       pbData[i] = (*(volatile uint8_t *)(i+dwAddr));
	#else 
	memcpy(pbData,(uint8_t *)(dwAddr),wNum);
	#endif
    return SUCCESS;
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
