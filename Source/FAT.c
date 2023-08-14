/**
 * @file FAT.c
 * @author Group2
 * @date Aug 2023
 * @brief Header file for FAT.c
 */
 
 /*========================================================================================================
 *                                            INCLUDES
 *======================================================================================================*/
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"FAT.h"
#include"HAL.h"
#include"FAT_Print.h"
/*========================================================================================================
 *                                           LOCAL MACROS
 *======================================================================================================*/

/*========================================================================================================
 *                                           GLOBAL MACROS
 *======================================================================================================*/

/*========================================================================================================
 *                                         GLOBAL VARIABLES
 *======================================================================================================*/

/*========================================================================================================
 *                                          LOCAL VARIABLES
 *======================================================================================================*/

static uint8_t* sectorBuffer = NULL;
static uint8_t* MainFAT;


static inline uint64_t FAT_Convert_L2B_Edian (uint8_t* firstByte, uint8_t dataLen);
static uint16_t FAT_Find_Next_Cluster(uint32_t n);

/*========================================================================================================
 *                                        LOCAL PROTOTYPES
 *======================================================================================================*/

/*========================================================================================================
 *                                         LOCAL FUNCTIONS
 *======================================================================================================*/

/*========================================================================================================
 *                                        GLOBAL PROTOTYPES
 *======================================================================================================*/
 
 /*========================================================================================================
 *                                         GLOBAL FUNCTIONS
 *=======================================================================================================*/

/**
 * @name FAT_Read_Boot
 * @brief Read composition of boot sector
 */
void FAT_Read_Boot() {
	sectorBuffer = malloc(DEFAULT_SECTOR_BYTE_NUM);
	HAL_Read_One_Sector(0, sectorBuffer);
	
	FAT_BOOT.Byte_p_Sec = FAT_Convert_L2B_Edian (&sectorBuffer[BOOT_BYTES_P_SEC], 2);
	FAT_BOOT.Sec_p_Clus = sectorBuffer[BOOT_SECTER_P_CLUSTER];
	FAT_BOOT.Num_rSec = FAT_Convert_L2B_Edian(&sectorBuffer[BOOT_RESERVED_SEC_NUM], 2);
	FAT_BOOT.Num_FATs = sectorBuffer[BOOT_NUM_FATs];
	FAT_BOOT.Max_RootEntries = FAT_Convert_L2B_Edian(&sectorBuffer[BOOT_MAX_ROOT_ENTRY], 2);
	FAT_BOOT.Total_SecCount = FAT_Convert_L2B_Edian(&sectorBuffer[BOOT_TOTAL_SECTOR_COUNT], 2);
	FAT_BOOT.Sec_p_FAT = FAT_Convert_L2B_Edian(&sectorBuffer[BOOT_SECTOR_P_FAT], 2);
	FAT_BOOT.BootSignature = sectorBuffer[BOOT_SIGNATURE];
	
	//BootSector_t.FAT_type
	memcpy(&FAT_BOOT.FAT_type, &sectorBuffer[BOOT_SYSTEM_TYPE] ,8);
	FAT_BOOT.FAT_type[8] = '\0'; 
	//BootSector_t.SystemSignature
	FAT_BOOT.SystemSignature = FAT_Convert_L2B_Edian(&sectorBuffer[BOOT_SYSTEM_SIGNATURE], 2);
	
	free(sectorBuffer); 
};

void FAT_Close()
{
	free(sectorBuffer);
	free(MainFAT);
	HAL_Close_Disk();
}

/**
 * @name FAT_Initialize
 * @brief Open a disk image.
 * @param - au8DiskPath (in): Path to disk image.
 */
FAT_Return_t FAT_Initialize(uint8_t* diskPath)
{
	HAL_Return_Type status =  HAL_Open_Disk(diskPath);
	if(HAL_DISK_NOT_FOUND == status)
	{
		return FAT_DISK_NOT_EXIST;
	}
	// read boot and print out
	FAT_Read_Boot();
	FAT_Print_Boot();
	if(FAT_BOOT.SystemSignature != 0xAA55u)
	{
		return FAT_DISK_SIGNATURE_ERR;
	}
	
	//read FAT and store
	MainFAT = malloc(FAT_BOOT.Sec_p_FAT * DEFAULT_SECTOR_BYTE_NUM);
	HAL_Read_Multi_Sector(1, FAT_BOOT.Sec_p_FAT, MainFAT);
	
	// calculate and read root dir
	RootDirAddr = FAT_BOOT.Num_FATs * FAT_BOOT.Sec_p_FAT + FAT_BOOT.Num_rSec;
	FAT_Print_Folder_Opening();
	FAT_Read_All_Entry(RootDirAddr);
	return FAT_OK;
	
	
}
/**
 * @name FAT_Read_All_Entry
 * @brief Read entries and its attributes
 * @param - uint64_t dirAddr from boot sector
 */
void FAT_Read_All_Entry (uint64_t dirAddr)
{
	
	while(1)
	{
		sectorBuffer = malloc(FAT_BOOT.Byte_p_Sec);
		HAL_Read_One_Sector(dirAddr, sectorBuffer);
		WorkingFilesCount = 0u;
		
		for(uint32_t i = 0; i < 16u; i++)
		{
			if(0x00u == sectorBuffer[ENTRY_TIME_OFFSET(i)])
			{
				free(sectorBuffer);
				return;
			}
			
			if(0x0Fu == sectorBuffer[ENTRY_ATTRIBUTE_OFFSET(i)])
			{
				continue;
			}
			
			FAT_Entry_t entryBuffer;
			memcpy(entryBuffer.File_Name, &sectorBuffer[ENTRY_TIME_OFFSET(i)], 8);			
			entryBuffer.File_Name[8] = '\0';
			
			memcpy(entryBuffer.File_Ext, &sectorBuffer[ENTRY_EXTENSION_OFFSET(i)], 3);
			entryBuffer.File_Ext[3] = '\0';
			
			entryBuffer.isFolder = (0x10u == sectorBuffer[ENTRY_ATTRIBUTE_OFFSET(i)]);
			entryBuffer.LastWriteTime = FAT_Convert_L2B_Edian(&sectorBuffer[ENTRY_LAST_WRITE_TIME_OFFSET(i)], 2);
			entryBuffer.LastWriteDate = FAT_Convert_L2B_Edian(&sectorBuffer[ENTRY_LAST_WRITE_DATE_OFFSET(i)], 2);
			entryBuffer.FirstCluster  = FAT_Convert_L2B_Edian(&sectorBuffer[ENTRY_FIRST_LOGICAL_CLUSTER_OFFSET(i)], 2);
			
			FAT_Print_One_Entry(&entryBuffer, WorkingFilesCount + 1u);
			
			WorkingFiles[WorkingFilesCount].isFolder = entryBuffer.isFolder;
			WorkingFiles[WorkingFilesCount].FirstCluster = entryBuffer.FirstCluster;
			
			WorkingFilesCount++;
		}
		
		free(sectorBuffer);
		dirAddr++;
	}
}

static inline uint64_t FAT_Convert_L2B_Edian (uint8_t* firstByte, uint8_t dataLen)
{
	uint64_t retVal = 0u;
	for(uint8_t i = 0u; i < dataLen; i++)
	{
		retVal |= (firstByte[i] << (i * 8u)); 
	}
	return retVal;
}



uint16_t FAT_Find_Next_Cluster(uint32_t n)// pass first cluster index
{

	uint16_t nextAddress;
	if ((n % 2) == 0)
	{
		nextAddress = (((MainFAT[((3 * n)/2) + 1] << 8) | (MainFAT[(3 * n)/2]))& 0x0FFFu);
	}
	else
	{
		nextAddress = (((MainFAT[(3 * n)/2] >> 4)  | (MainFAT[((3 * n)/2) + 1] << 4 ))& 0x0FFFu); 	
	}
	return nextAddress;
	 
}

void FAT_Read_File(uint32_t Firtclusterindex)
{
	uint32_t index = Firtclusterindex;
	uint32_t sectorindex = 33 + index - 2;

	do
	{
		sectorBuffer = malloc(DEFAULT_SECTOR_BYTE_NUM);		
		HAL_Read_One_Sector(sectorindex, sectorBuffer);		
		
		printf("%s", sectorBuffer);		
		free(sectorBuffer);		
		index = FAT_Find_Next_Cluster(index);
		
		if(0xFF8u <= index)
		{
			break;
		}
	
		sectorindex = 33 + index - 2;
	}while(1);
	
		printf("\n");

}














