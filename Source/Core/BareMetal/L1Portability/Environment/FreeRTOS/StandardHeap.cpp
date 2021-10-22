/**
 * @file StandardHeap.cpp
 * @brief Source file for class StandardHeap
 * @date 13/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class StandardHeap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardHeap.h"
#include <string.h>
#include <stdlib.h>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StandardHeap::StandardHeap() {
    /* initialise memory addresses to NULL as we have no way to obtain this information until malloc is called */
    firstAddress = 0U;
    lastAddress = 0U;
}

StandardHeap::~StandardHeap() {
    lastAddress = 0U;
    firstAddress = 0U;
}

/*lint -e{586} use of malloc function (deprecated) */
void *StandardHeap::Malloc(const uint32 size) {
    //void *pointer = malloc(size);
    //void *pointer = new char8[size];

    void* pointer = NULL_PTR(void*);

    if (size != 0u) {
        pointer = pvPortMalloc(static_cast<size_t>(size));
    }

    if (pointer != NULL) {

        /*lint -e{9091} -e{923} the casting from pointer type to integer type is required
         * in order to be able to update the range of addresses provided by this heap
         * uintp is an integer type that has by design the same span as a pointer in all systems*/
        uintp address = reinterpret_cast<uintp>(pointer);
        if ((firstAddress > address) || (firstAddress == 0U)) {
            firstAddress = address;
        }
        address += size;
        if ((lastAddress < address) || (lastAddress == 0U)) {
            lastAddress = address;
        }

    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "StandardHeap: Failed Failed malloc()");
    }
    return pointer;

}

/*lint -e{586} use of free function (deprecated) */
void StandardHeap::Free(void *&data) {
    if (data != NULL) {
        vPortFree(data);
    }
//    delete[] (reinterpret_cast<char8 *>(data));
    data = NULL_PTR(void *);
//    free(data);
}

/*lint -e{586} use of realloc function (deprecated) */
void *StandardHeap::Realloc(void *&data,
                            const uint32 newSize) {

    if (data == NULL) {
        data = StandardHeap::Malloc(newSize);
    }
    else {
        if (newSize == 0u) {
            StandardHeap::Free(data);
        }
        else {
            void *oldData = data;
            data = Malloc(newSize);
            //Will copy garbage as well...
            memcpy(data, oldData, static_cast<size_t>(newSize));
            StandardHeap::Free(oldData);
        }
    }
    return data;
}

/*lint -e{925} cast pointer to pointer required */
void *StandardHeap::Duplicate(const void * const data,
                              uint32 size) {

    void *duplicate = NULL_PTR(void *);

    // check if 0 terminated copy to be done
    if (size == 0U) {
        const char8* inputData = static_cast<const char8 *>(data);
        /*lint -e{586} the use of strlen is necessary because
         * the size of the array is unknown */
        size = static_cast<uint32>(strlen(inputData)) + 1u;
    }
    // strdup style
    duplicate = StandardHeap::Malloc(size);
    if (duplicate != NULL) {
        const char8 *source = static_cast<const char8 *>(data);
        char8 *destination = static_cast<char8 *>(duplicate);
        uint32 i;
        for (i = 0u; i < size; i++) {
            destination[i] = source[i];
        } //copy loop
    } //check Malloc success
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "StandardHeap: Failed malloc()");
    }
    // copy bound by size

    if (duplicate != NULL) {
        /*lint -e{9091} -e{923} the casting from pointer type to integer type is required
         * in order to be able to update the range of addresses provided by this heap
         * uintp is an integer type that has by design the same span as a pointer in all systems*/
        uintp address = reinterpret_cast<uintp>(duplicate);
        if ((firstAddress > address) || (firstAddress == 0U)) {
            firstAddress = address;
        }
        address += size;
        if ((lastAddress < address) || (lastAddress == 0U)) {
            lastAddress = address;
        }
    }

    return duplicate;
}

uintp StandardHeap::FirstAddress() const {
    return firstAddress;
}

uintp StandardHeap::LastAddress() const {
    return lastAddress;
}

const char8 *StandardHeap::Name() const {
    return "StandardHeap";
}

}

