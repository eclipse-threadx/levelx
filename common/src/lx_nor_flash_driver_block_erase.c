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
/**   NOR Flash                                                           */
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
/*    _lx_nor_flash_driver_block_erase                    PORTABLE C      */ 
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function performs a NOR flash block erase.                     */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    nor_flash                             NOR flash instance            */ 
/*    block                                 Block number to erase         */ 
/*    erase_count                           Erase count for this block    */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    return status                                                       */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    (lx_nor_flash_driver_block_erase)     Actual driver block erase     */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Application Code                                                    */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     William E. Lamie         Initial Version 6.0           */
/*  09-30-2020     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Bhupendra Naphade        Modified comment(s),          */
/*                                            resulting in version 6.1.7  */
/*  03-08-2023     Xiuwen Cai               Modified comment(s),          */
/*                                            added new driver interface, */
/*                                            resulting in version 6.2.1 */
/*                                                                        */
/**************************************************************************/
UINT  _lx_nor_flash_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count)
{

UINT    status;

#ifndef LX_NOR_DISABLE_EXTENDED_CACHE

UINT    i;
ULONG   *block_start_address;
ULONG   *block_end_address;
ULONG   *cache_entry_start;
ULONG   *cache_entry_end;


    /* Calculate the block starting address.  */
    block_start_address =  nor_flash -> lx_nor_flash_base_address + (block * nor_flash -> lx_nor_flash_words_per_block);
    block_end_address =    block_start_address + nor_flash -> lx_nor_flash_words_per_block;
    
    /* Loop through the cache entries to see if there is a sector in cache.  */
    for (i = 0; i < nor_flash -> lx_nor_flash_extended_cache_entries; i++)
    {
        
        /* Search through the cache to see if this cache entry needs to be invalidated.  */
                
        /* Determine the cache entry addresses.  */
        cache_entry_start =  nor_flash -> lx_nor_flash_extended_cache[i].lx_nor_flash_extended_cache_entry_sector_address;
        cache_entry_end =    cache_entry_start + LX_NOR_SECTOR_SIZE;
                
        /* Determine if the flash address in in the cache entry.  */
        if ((cache_entry_start) && (block_start_address <= cache_entry_start) && (block_end_address > cache_entry_end))
        {
    
            /* Yes, this cache entry is in the block to be erased so invalidate it.  */
            nor_flash -> lx_nor_flash_extended_cache[i].lx_nor_flash_extended_cache_entry_sector_address =  LX_NULL;
            nor_flash -> lx_nor_flash_extended_cache[i].lx_nor_flash_extended_cache_entry_access_count =    0;
        }
    }
#endif

    /* Call the actual driver block erase function.  */
#ifdef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
    status =  (nor_flash -> lx_nor_flash_driver_block_erase)(nor_flash, block, erase_count);
#else
    status =  (nor_flash -> lx_nor_flash_driver_block_erase)(block, erase_count);
#endif

    /* Return completion status.  */
    return(status);   
}


