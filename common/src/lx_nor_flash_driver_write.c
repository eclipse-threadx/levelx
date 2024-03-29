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
/*    _lx_nor_flash_driver_write                          PORTABLE C      */ 
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function performs a write of the NOR flash memory.             */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    nor_flash                             NOR flash instance            */ 
/*    flash_address                         Address of NOR flash to write */ 
/*    source                                Source for the write          */ 
/*    words                                 Number of words to write      */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    return status                                                       */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    (lx_nor_flash_driver_write)           Actual driver write           */ 
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
UINT  _lx_nor_flash_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words)
{

#ifndef LX_NOR_DISABLE_EXTENDED_CACHE

UINT    status;
UINT    i;
ULONG   *cache_entry_start;
ULONG   *cache_entry_end;
ULONG   cache_offset;


    /* Is the request a whole sector or a partial sector.  */
    if ((words == 1) && (nor_flash -> lx_nor_flash_extended_cache_entries))
    {

        /* One word request, which implies that it is a NOR flash metadata write.  */

        /* Loop through the cache entries to see if there is a sector in cache.  */
        for (i = 0; i < nor_flash -> lx_nor_flash_extended_cache_entries; i++)
        {
        
            /* Search through the cache to see if there is a cache entry.  */
                
            /* Determine the cache entry addresses.  */
            cache_entry_start =  nor_flash -> lx_nor_flash_extended_cache[i].lx_nor_flash_extended_cache_entry_sector_address;
            cache_entry_end =    cache_entry_start + LX_NOR_SECTOR_SIZE;
                
            /* Determine if the flash address in in the cache entry.  */
            if ((cache_entry_start) && (flash_address >= cache_entry_start) && (flash_address < cache_entry_end))
            {
                
                /* Yes, we found the entry.  */
                    
                /* Calculate the offset into the cache entry.  */
                cache_offset =  (ULONG)(flash_address - cache_entry_start);
                    
                /* Copy the word into the cache.  */
                *(nor_flash -> lx_nor_flash_extended_cache[i].lx_nor_flash_extended_cache_entry_sector_memory + cache_offset) =  *source;
                
                /* Get out of the loop.  */
                break;
            }
        }
    }
    
    /* In any case, call the actual driver write function.  */
#ifdef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
    status =  (nor_flash -> lx_nor_flash_driver_write)(nor_flash, flash_address, source, words);
#else
    status =  (nor_flash -> lx_nor_flash_driver_write)(flash_address, source, words);
#endif

    /* Return completion status.  */
    return(status);   
   
#else
UINT    status;


    /* Call the actual driver write function.  */
#ifdef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
    status =  (nor_flash -> lx_nor_flash_driver_write)(nor_flash, flash_address, source, words);
#else
    status =  (nor_flash -> lx_nor_flash_driver_write)(flash_address, source, words);
#endif
    
    /* Return completion status.  */
    return(status);   
#endif
}

