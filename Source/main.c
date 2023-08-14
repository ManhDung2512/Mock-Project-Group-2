/**
 * @file main.c
 * @author Group2
 * @date Aug 2023
 * @brief Header file for main.c
 */
#include<stdio.h>
#include<stdlib.h>
#include "FAT.h"
#include "FAT_Print.h"

int main(int argc, char** argv)
{
	if(2 != argc)
	{
		printf("Argument Error: FAT12_Reader [disk file path]\n");
		return 1;
	}
	FAT_Return_t initStatus =  FAT_Initialize(argv[1]);
	FAT_Print_Msg(initStatus);
	if(FAT_OK == initStatus)
	{
		printf("Type in -1 to exit\n");
		while(1)
	{
		int32_t userChoice = -2;
		do{
			printf("Type in a number to open a file or folder: ");
			scanf("%d", &userChoice);
			fflush(stdin);
		} while(userChoice <= -2);
		if(userChoice == -1u)
		{
			FAT_Close();
			return 0;
		}
		else if (userChoice > WorkingFilesCount)
		{
			printf("There is no %d file\n", userChoice);
		}
		else{
			userChoice--;
			if(WorkingFiles[userChoice].isFolder)
			{
				system("cls");
				if(WorkingFiles[userChoice].FirstCluster == 0u)
				{
					FAT_Read_All_Entry(RootDirAddr);
				}
				else
				{
					FAT_Read_All_Entry(WorkingFiles[userChoice].FirstCluster + 31u);
				}
			}
		    else
		    {
		    	FAT_Read_File(WorkingFiles[userChoice].FirstCluster);
			}
		}
	}
	}
	
}

