/**
 * @file MemoryCheck.h
 * @brief Header file for module MemoryCheck
 * @date 18/06/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the module MemoryCheck
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORY_H_
#define MEMORY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace MemoryCheck {

/**
 * Set of flags indicating desired access modes to the
 * memory by processes.
 */
enum MemoryTestAccessMode {

    /** read and execute */
    Execute = 0x00000001,

    /** read  */
    Read = 0x00000002,

    /** read  */
    Write = 0x00000004

};

/**
 * @brief Checks if the process has the access to the specified memory area.
 * @param[in] address The pointer to the memory block to be checked.
 * @param[in] accessMode The type of memory access to perform.
 * @param[in] size is the number of bytes to check.
 * @return true if the process has the access to the memory with the specified permissions, false otherwise.
 */
DLL_API bool Check(const void * const address,
                   const MemoryTestAccessMode accessMode,
                   const uint32 size = 4u);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* MEMORY_H_ */
