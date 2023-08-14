/**
 * @file FAT_Print.c
 * @author Group2
 * @date Aug 2023
 * @brief Header file for FAT_Print.c
 */
#include"FAT_Print.h"

void FAT_Print_Boot() {
	printf("       Welcome to FAT file reader\n");
	printf("       Content of Boot sector\n");
	printf("Bytes per sector:                     %d\n", FAT_BOOT.Byte_p_Sec);
	printf("Sectors per cluster:                  %d\n", FAT_BOOT.Sec_p_Clus);
	printf("Number of reserved sectors:           %d\n", FAT_BOOT.Num_rSec);
	printf("Number of FATs:                       %d\n", FAT_BOOT.Num_FATs);
	printf("Max number of root directory entries: %d\n", FAT_BOOT.Max_RootEntries);
	printf("Total sector count:                   %d\n", FAT_BOOT.Total_SecCount);
	printf("Sectors per FAT:                      %d\n", FAT_BOOT.Sec_p_FAT);
	printf("Boot signature:                       %x\n", FAT_BOOT.BootSignature);
	printf("File system type:                     %s\n", FAT_BOOT.FAT_type);
	printf("System Signature:                     %x\n", FAT_BOOT.SystemSignature);	
	
	printf("\n \n");
}

void FAT_Print_One_Entry(FAT_Entry_t* entryAddr, uint32_t entryNum)
{
	Time_t fileTime;
	fileTime.Time = entryAddr->LastWriteTime;
	Date_t fileDate;
	fileDate.Date = entryAddr->LastWriteDate;
	printf("%d-", entryNum);
	printf("%s", entryAddr->File_Name);
	printf("    ");
	printf("%s", entryAddr->File_Ext);
	printf("    ");
	if(entryAddr->isFolder)
	{
		printf("Folder");
	}
	else
	{
		printf("File");
	}
	printf(" \t ");
	printf("%d:%d:%d", fileTime.FileTime.hour, fileTime.FileTime.minute, fileTime.FileTime.second * 2);
	printf(" \t ");
	printf("%d/%d/%d", fileDate.FileDate.day, fileDate.FileDate.month, fileDate.FileDate.year + 1980u);
	printf("\n");
}


void FAT_Print_Folder_Opening()
{
	printf("  Name");
	printf("    ");
	printf("Extension");
	printf("  ");
	printf("Type");
	printf(" \t ");
	printf("Time");
	printf(" \t \t ");
	printf("Date");
	printf("\n");
}
void FAT_Print_Msg(FAT_Return_t status)
{

	if(FAT_DISK_NOT_EXIST == status)
	{
		printf("DISK NOT FOUND\n");
	}
	else if(FAT_DISK_SIGNATURE_ERR == status)
	{
		printf("DISK IS CORRUPTED\n");
	}
}


