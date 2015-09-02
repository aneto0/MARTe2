/**
 * @file MemoryOS.cpp
 * @brief Source file for module MemoryOS
 * @date 27/07/2015
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

 * @details This source file contains the definition of all the methods for
 * the module MemoryOS (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <Windows.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../MemoryCheck.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MemoryCheck {

bool Check(const void * const address,
           const MemoryTestAccessMode accessMode,
           const uint32 size) {

    if (address == NULL) {
        return false;
    }

    uint8 check = 0;

    //determines if the calling process has read access to the specified address (if the process has read and execute permissions).
    if (accessMode & Execute) {
        check++;
        if (IsBadCodePtr((FARPROC) address)) {
            return false;
        }
    }

    //determines if the calling process has the read access to the specified range of memory (if the process has read permissions).
    if (accessMode & Read) {
        check++;
        if (IsBadReadPtr((void*) address, size)) {
            return false;
        }
    }

    //determines if the calling process has the write access to the specified range of memory (if the process has write permissions).
    if (accessMode & Write) {
        check++;
        if (IsBadWritePtr((void*) address, size)) {
            return false;
        }
    }

    return check != 0;
}

}
