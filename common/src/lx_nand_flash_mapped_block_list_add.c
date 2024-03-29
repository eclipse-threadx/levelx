/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** LevelX Component                                                      */ 
/**                                                                       */
/**   NAND Flash                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define LX_SOURCE_CODE


/* Disable ThreadX error checking.  */

#ifndef LX_DISABLE_ERROR_CHECKING
#define LX_DISABLE_ERROR_CHECKING
#endif


/* Include necessary system files.  */

#include "lx_api.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _lx_nand_flash_mapped_block_list_add                PORTABLE C      */ 
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Xiuwen Cai, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function adds a block to mapped block list.                    */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    nand_flash                            NAND flash instance           */ 
/*    block_mapping_index                   Block mapping index           */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    return status                                                       */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Internal LevelX                                                     */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  03-08-2023     Xiuwen Cai               Initial Version 6.2.1        */
/*                                                                        */
/**************************************************************************/
UINT  _lx_nand_flash_mapped_block_list_add(LX_NAND_FLASH* nand_flash, ULONG block_mapping_index)
{

ULONG insert_position;
ULONG search_position;
UCHAR new_block_erase_count;


    /* Get insert position for the mapped block list.  */
    insert_position = nand_flash -> lx_nand_flash_mapped_block_list_head;

    /* Check if the list if full.  */
    if (insert_position < nand_flash -> lx_nand_flash_free_block_list_tail)
    {

        /* Return an error.  */
        return(LX_ERROR);
    }

    /* Get the erase count.  */
    new_block_erase_count = nand_flash -> lx_nand_flash_erase_count_table[nand_flash -> lx_nand_flash_block_mapping_table[block_mapping_index]];

    /* Add one block to the free list.  */
    nand_flash -> lx_nand_flash_mapped_block_list_head--;

    /* Initialize the search pointer.  */
    search_position = insert_position + 1;

    /* Loop to search the insert position.  */
    while ((search_position < nand_flash -> lx_nand_flash_block_list_size) &&
           (nand_flash -> lx_nand_flash_erase_count_table[nand_flash -> lx_nand_flash_block_mapping_table[nand_flash -> lx_nand_flash_block_list[search_position]]] < new_block_erase_count))
    {

        /* Move the item in the list.  */
        nand_flash -> lx_nand_flash_block_list[insert_position] = nand_flash -> lx_nand_flash_block_list[search_position];
        search_position++;
        insert_position++;
    }

    /* Insert the new block to the list.  */
    nand_flash -> lx_nand_flash_block_list[insert_position] = (USHORT)block_mapping_index;

    /* Return successful completion.  */
    return(LX_SUCCESS);
}

