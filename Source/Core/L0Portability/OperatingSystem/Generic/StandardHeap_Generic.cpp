/**
 * @file StandardHeap_Generic.cpp
 * @brief Source file for class StandardHeap_Generic
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
 * the class StandardHeap_Generic (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardHeap.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace HeapManager {
/**
 * @brief constructor
 */
StandardHeap::StandardHeap() {

    /** initialise memory addresses to NULL as we have no way to obtain this information until malloc is called */
    firstAddress = 0U;
    lastAddress = 0U;
}
/**
 * @brief destructor
 */
StandardHeap::~StandardHeap() {
    lastAddress = 0U;
    firstAddress = 0U;
}
;

/**
 * @brief allocates size bytes of data in the heap. Maximum allocated size is 4Gbytes
 * @return a pointer to the allocated memory or NULL if the allocation fails.
 */
/*lint -e{586} use of malloc function (deprecated) */
void *StandardHeap::Malloc(const uint32 size) {
    //void *pointer = malloc(size);
    //void *pointer = new char8[size];

    void* pointer = NULL_PTR(void*);

    if (size != 0u) {
        pointer = malloc(static_cast<osulong>(size));
    }

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

    return pointer;

}

/**
 * @brief free the pointer data and its associated memory.
 * @param data the data to be freed.
 */
/*lint -e{586} use of free function (deprecated) */
void StandardHeap::Free(void *&data) {
    if (data != NULL) {
        free(data);
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
            data = realloc(data, static_cast<osulong>(newSize));
        }
    }
    return data;

}

/*lint -e{925} cast pointer to pointer required */
void *StandardHeap::Duplicate(const void * const data,
                              const uint32 size) {

    void *duplicate = NULL_PTR(void *);

    // check if 0 zerminated copy to be done
    if (size == 0U) {
        if (data != NULL) {
            duplicate = strdup(static_cast<const char8 *>(data));
        }
    }
    else { // strdup style
        duplicate = StandardHeap::Malloc(size);
        if (duplicate != NULL) {
            const char8 *source = static_cast<const char8 *>(data);
            char8 *destination = static_cast<char8 *>(duplicate);
            uint32 i;
            for (i = 0u; i < size; i++) {
                *destination = *source;
                destination++;
                source++;
            } //copy loop
        } //check Malloc success
    } // copy bound by size

    return duplicate;
}

/**
 * @brief start of range of memory addresses served by this heap.
 * @return first memory address
 */
uintp StandardHeap::FirstAddress() const {
    return firstAddress;
}

/**
 * @brief end (inclusive) of range of memory addresses served by this heap.
 * @return last memory address
 */
uintp StandardHeap::LastAddress() const {
    return lastAddress;
}

/**
 * @brief Returns the name of the heap
 * @return name of the heap
 */
const char8 *StandardHeap::Name() const {
    return "StandardHeap";
}

}

