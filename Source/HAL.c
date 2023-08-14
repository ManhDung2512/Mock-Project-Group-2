/**
 * @file HAL.c
 * @author Group2
 * @date Aug 2023
 * @brief Header file for HAL.c
 */
#include <stdint.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "HAL.h"

static FILE* diskPtr = NULL;

HAL_Return_Type HAL_Read_One_Sector(uint32_t sectorIndex, uint8_t *pData)
{
	HAL_Return_Type readStatus = HAL_READ_OK;
	uint32_t checkBytesRead = 0u;
	if(NULL == pData)
	{
		readStatus = HAL_ERR_MEMORY;
	}
	else
	{
		memset(pData, 0x00u, sizeof(uint8_t) * DEFAULT_SECTOR_BYTE_NUM);
		fseek(diskPtr, sectorIndex * DEFAULT_SECTOR_BYTE_NUM, SEEK_SET);
		checkBytesRead = fread(pData, sizeof(uint8_t), DEFAULT_SECTOR_BYTE_NUM, diskPtr);
		
		if(DEFAULT_SECTOR_BYTE_NUM != checkBytesRead)
		{
			readStatus = HAL_READ_FAIL;
		} 
	}
	return readStatus;	
}

HAL_Return_Type HAL_Read_Multi_Sector(uint32_t sectorIndex, uint32_t sectorNum ,uint8_t *pData)
{
	HAL_Return_Type readStatus = HAL_READ_OK;
	uint32_t BytesRead = 0u;
	uint32_t loopNum = sectorNum + sectorIndex;
	for(; sectorIndex < loopNum; sectorIndex++)
	{
		readStatus = HAL_Read_One_Sector(sectorIndex, &pData[BytesRead]);
		if(HAL_READ_OK != HAL_READ_OK)
		{
			break;
		}
		else
		{
			BytesRead += DEFAULT_SECTOR_BYTE_NUM;
		}
	}	
	return readStatus;	
}

HAL_Return_Type HAL_Open_Disk(uint8_t* userPath)
{
	HAL_Return_Type openStatus = HAL_DISK_OK;
	diskPtr = fopen(userPath, "rb");
	if(NULL == diskPtr)
	{
		openStatus = HAL_DISK_NOT_FOUND;
	}
	return openStatus;
}

void HAL_Close_Disk()
{
	fclose(diskPtr);
	diskPtr = NULL;
}





