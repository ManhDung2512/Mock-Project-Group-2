/**
 * @file FAT.c
 * @author Group2
 * @date Aug 2023
 * @brief Header file for FAT.h
 */
#ifndef __FAT_H
#define __FAT_H

/*========================================================================================================
 *                                            INCLUDES
 *======================================================================================================*/
#include<stdint.h>
/*========================================================================================================
 *                                           LOCAL MACROS
 *======================================================================================================*/

/*========================================================================================================
 *                                           GLOBAL MACROS
 *======================================================================================================*/
#define  DEFAULT_BYTS_PER_SEC (0x200U)
#define  ENTRY_TIME_OFFSET(X) 					X * 32u + 0u
#define  ENTRY_EXTENSION_OFFSET(X) 				X * 32u + 8u
#define  ENTRY_ATTRIBUTE_OFFSET(X) 				X * 32u + 11u
#define  ENTRY_LAST_WRITE_TIME_OFFSET(X) 		X * 32u + 22u
#define  ENTRY_LAST_WRITE_DATE_OFFSET(X)	 	X * 32u + 24u
#define  ENTRY_FIRST_LOGICAL_CLUSTER_OFFSET(X) 	X * 32u + 26u

#define BOOT_BYTES_P_SEC 						11
#define BOOT_RESERVED_SEC_NUM 					14
#define BOOT_SECTER_P_CLUSTER 					13
#define BOOT_NUM_FATs		  					16
#define BOOT_MAX_ROOT_ENTRY	  					17
#define BOOT_TOTAL_SECTOR_COUNT	  				19
#define BOOT_SECTOR_P_FAT	  					22
#define BOOT_SIGNATURE		  					38
#define BOOT_SYSTEM_TYPE 						54
#define BOOT_SYSTEM_SIGNATURE 					510

/*========================================================================================================
 *                                         GLOBAL VARIABLES
 *======================================================================================================*/

typedef enum
{
	FAT_OK    =   1u,
	FAT_DISK_NOT_EXIST = 2u,
	FAT_DISK_SIGNATURE_ERR = 3u,
} FAT_Return_t;
 
typedef struct BootSector {
	uint16_t 	Byte_p_Sec;
	uint8_t  	Sec_p_Clus;
	uint16_t 	Num_rSec;
	uint8_t  	Num_FATs;
	uint16_t 	Max_RootEntries;
	uint16_t 	Total_SecCount;
	uint16_t 	Sec_p_FAT;
	uint8_t  	BootSignature;
	char 		FAT_type[9];
	uint16_t 	SystemSignature;
} BootSector_t;

typedef struct {
	uint8_t 	File_Name[9];
	uint8_t 	File_Ext[4];
	uint8_t 	isFolder;
	uint16_t 	LastWriteTime;
	uint16_t 	LastWriteDate;
	uint16_t 	FirstCluster;
} FAT_Entry_t;

typedef struct {
	uint16_t second : 5u;
	uint16_t minute : 6u;
	uint16_t hour	: 5u;
} FileTime_t;
typedef union{
	uint16_t Time;
	FileTime_t FileTime;
} Time_t;

typedef struct {
	uint16_t day   : 5u;
	uint16_t month : 4u;
	uint16_t year  : 7u;
} FileDate_t;

typedef union{
	uint16_t 	Date;
	FileDate_t 	FileDate;
} Date_t;


typedef struct
{
	uint8_t 	isFolder;
	uint16_t 	FirstCluster;
} FAT_File_Clus_t;

BootSector_t FAT_BOOT;

uint64_t RootDirAddr;

uint32_t WorkingFilesCount;

FAT_File_Clus_t WorkingFiles[100u];
/*========================================================================================================
 *                                         GLOBAL FUNCTIONS
 *======================================================================================================*/


/**
 * @name FAT_Initialize
 * @brief Open a disk image.
 * @param - au8DiskPath (in): Path to disk image.
 */
FAT_Return_t FAT_Initialize(uint8_t* diskPath);
/**
 * @name FAT_Read_File
 * @brief Read file from First Logical Cluster
 * @param - FirstCluster from Entries 
 */
void FAT_Read_File(uint32_t FirstCLuster);
/**
 * @name FAT_Read_All_Entry
 * @brief Read entries and its attributes
 * @param - uint64_t dirAddr from boot sector
 */
void FAT_Read_All_Entry (uint64_t dirAddr);
void FAT_Close();

#endif
