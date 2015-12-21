/**
 * @file MemoryOperationsHelper.h
 * @brief Header file for module MemoryOperationsHelper
 * @date 14/08/2015
 * @author Filippo Sartori
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the module MemoryOperationsHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYOPERATIONSHELPER_H_
#define MEMORYOPERATIONSHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace MemoryOperationsHelper {

/**
 * @brief Copies a block of memory into another.
 * @details A copy of size bytes from source location to destination position.
 * @param[in,out] destination is the pointer to the destination memory location.
 * @param[in] source is the pointer to the source memory location.
 * @param[in] size is the size of the memory to be copied.
 * @return true if source, destination and destination after copy are not NULL.
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
DLL_API bool Copy(void * const destination,
                  const void * const source,
                  uint32 size);

/**
 * @brief Compares the first specified bytes of two blocks of memories.
 * @param[in] mem1 is the pointer to the first memory location.
 * @param[in] mem2 is the pointer to the second memory location.
 * @param[in] size is the number of byte to compare.
 * @return (0 if mem1 == mem2), (1 if mem1 < mem2), (2 if mem1 > mem2).
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
DLL_API int32 Compare(const void * const mem1,
                      const void * const mem2,
                      uint32 size);

/**
 * @brief Searches a character in the specified memory block.
 * @param[in] mem is the memory location.
 * @param[in] c is the character to find.
 * @param[in] size is the size of the memory area.
 * @return the pointer to the first occurrence of c in the memory. NULL if c is absent.
 * @pre the size parameter must be minor than the memory block size.
 */
DLL_API const void *Search(const void * const mem,
                           const char8 c,
                           const uint32 size);

/**
 * @brief Copies a block of memory into another allowing overlapping.
 * @param[in,out] destination is the pointer to the destination memory location.
 * @param[in] source is the pointer to the source memory location.
 * @param[in] size is the number of bytes to be copied.
 * @return true if source, destination, and destination after the copy are not NULL.
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
DLL_API bool Move(void * const destination,
                  const void * const source,
                  const uint32 size);

/**
 * @brief Sets a defined number bytes of the specified memory area equal to a specified character.
 * @param[in,out] mem is the pointer to the memory location.
 * @param[in] c is the character to store.
 * @param[in] size is the number of bytes where c will be written.
 * @return true if the memory could be set to desired value.
 * @pre the size parameter must be minor than the memory block size.
 */
DLL_API bool Set(void * const mem,
                 const char8 c,
                 const uint32 size);

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYOPERATIONSHELPER_H_ */

