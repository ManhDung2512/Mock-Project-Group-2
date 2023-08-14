/**
 * @file HAL.h
 * @author Group2
 * @date Aug 2023
 * @brief Header file for HAL.h
  */
#ifndef __HAL_H
#define __HAL_H
 
 /*========================================================================================================
 *                                            INCLUDES
 *======================================================================================================*/

/*========================================================================================================
 *                                           LOCAL MACROS
 *======================================================================================================*/

/*========================================================================================================
 *                                           GLOBAL MACROS
 *======================================================================================================*/
#define DEFAULT_BYTS_PER_SEC (0x200U)
#define DEFAULT_SECTOR_BYTE_NUM  (512u)
/*========================================================================================================
 *                                         GLOBAL VARIABLES
 *======================================================================================================*/

  typedef enum
 {
 	HAL_READ_OK        = 0u,
 	HAL_READ_FAIL      = 1u,
 	HAL_ERR_MEMORY     = 2u,
 	HAL_DISK_NOT_FOUND = 3u,
 	HAL_DISK_OK        = 4u,
 	
 } HAL_Return_Type;
 
/*========================================================================================================
 *                                         GLOBAL FUNCTIONS
 *======================================================================================================*/

 /**
 * @name HAL_Read_One_Sector
 * @brief Read one sector from 'index' of disk.
 * @param uint32_t sectorIndex (in): The index of sector to read.
 * @param uint8_t * pData: The struct that contain content of sector 'index'.
 * return the status of read if fail or success
 */
 HAL_Return_Type HAL_Read_One_Sector(uint32_t sectorIndex, uint8_t *pData);
 
/**
 * @name HAL_Read_MultiSector
 * @brief Read multi sector from 'index' of disk.
 * @param u32Index (in): The index of sector to read.
 * @param u32Num (in)  : The number of sector to read.
 * @param au8Buff (out): The buffer that contain content of sector 'index'.
 * @return Number of bytes that was read.
 */
 HAL_Return_Type HAL_Read_Multi_Sector(uint32_t sectorIndex, uint32_t sectorNum ,uint8_t *pData);
/**
 * @name HAL_OpenDisk
 * @brief Open a disk image to read and write.
 * @param - au8DiskPath (in): Path to disk image.
 * @return E_OK if success, otherwise return E_NOT_OK.
 */
 HAL_Return_Type HAL_Open_Disk(uint8_t* userPath);
 
  /* CLOSE A DISK ( A FLOPPY IMG FILE)
 */
 void HAL_Close_Disk();
 #endif
