/**
 * @file FAT_Print.h
 * @author Group2
 * @date Aug 2023
 * @brief Header file for FAT_Print.h
 */
#ifndef __FAT_PRINT_H
#define __FAT_PRINT_H

/*========================================================================================================
 *                                            INCLUDES
 *======================================================================================================*/
#include "FAT.h"
#include  <stdio.h>
/*========================================================================================================
 *                                           LOCAL MACROS
 *======================================================================================================*/

/*========================================================================================================
 *                                           GLOBAL MACROS
 *======================================================================================================*/
#define DEFAULT_BYTS_PER_SEC (0x200U)
/*========================================================================================================
 *                                         GLOBAL VARIABLES
 *======================================================================================================*/
 
/*========================================================================================================
 *                                         GLOBAL FUNCTIONS
 *======================================================================================================*/
/**
 * @name FAT_Print_Boot
 * @brief Print Boot sector detail
 */
void FAT_Print_Boot();
/**
 * @name FAT_Print_One_Entry
 * @brief Print all composition of one Entry
 * @param FAT_Entry_t* entryAddr : address of Entry
 * @param uint32_t entryNum : numberical order of entries
 */
void FAT_Print_One_Entry(FAT_Entry_t* entryAddr, uint32_t entryNum);
/**
 * @name FAT_Print_Folder_Opening
 * @brief Print directory and subdirectory
 */
void FAT_Print_Folder_Opening();
void FAT_Print_Msg(FAT_Return_t status);


#endif
