/**
 * @file MemoryOperationsHelper_CLIB_Generic.cpp
 * @brief Source file for module MemoryOperationsHelper
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
 * the module MemoryOperationsHelper (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifndef LINT

#include <stdlib.h>
#include <string.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "../../MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace MemoryOperationsHelper {

bool Copy(void* const destination,
          const void * const source,
          const uint32 size) {

    bool ret = false;
    if ((source != NULL) && (destination != NULL)) {
        ret = memcpy(destination, source, static_cast<osulong>(size)) != NULL;
        if (!ret) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "MemoryOperationsHelper: Failed memcpy()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "MemoryOperationsHelper: Invalid input arguments");
    }

    return ret;
/*lint -e{593} custodial destination pointer is always to be freed by the caller*/
}

int32 Compare(const void * const mem1,
              const void * const mem2,
              const uint32 size) {

    int32 ret = -1;

    if ((mem1 != NULL) && (mem2 != NULL)) {
        int32 temp = memcmp(mem1, mem2, static_cast<size_t>(size));
        if (temp < 0) {
            ret = 1; // 1 if mem1<mem2
        }
        if (temp > 0) {
            ret = 2; // 2 if mem1>mem2
        }
        if (temp == 0) {
            ret = 0; // 0 if mem1==mem2
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "MemoryOperationsHelper: Invalid input arguments");
    }

    return ret;

}

const void* Search(const void * const mem,
                   const char8 c,
                   const uint32 size) {
    const void* ret = static_cast<const void*>(NULL);
    if (mem != NULL) {

        ret = memchr(mem, c, static_cast<size_t>(size));

    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "MemoryOperationsHelper: Invalid input arguments");
    }

    return ret;
}

bool Move(void * const destination,
          const void * const source,
          const uint32 size) {

    bool ret = false;

    if ((source != NULL) && (destination != NULL)) {

        ret = memmove(destination, source, static_cast<size_t>(size)) != NULL;
        if (!ret) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "MemoryOperationsHelper: Failed memmove()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "MemoryOperationsHelper: Invalid input arguments");
    }
    return ret;

}

bool Set(void * const mem,
         const char8 c,
         const uint32 size) {
    bool ret = false;
    if (mem != NULL) {

        ret = memset(mem, c, static_cast<size_t>(size)) != NULL;
        if (!ret) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "MemoryOperationsHelper: Failed memset()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "MemoryOperationsHelper: Invalid input arguments");
    }

    return ret;

}

}

}
