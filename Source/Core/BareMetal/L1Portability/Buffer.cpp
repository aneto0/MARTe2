/**
 * @file Buffer.cpp
 * @brief Source file for class Buffer
 * @date 05/04/2017
 * @author Andre' Torres
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
 * the class Buffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Buffer.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
Buffer::Buffer(const uint32 bufferSize, const uint32 typeSize) {

    size = bufferSize;
    sizeOf = typeSize;
    buffer = NULL_PTR(char8 *);

    uint32 memorySize = size * sizeOf;

    /*lint -e{925} [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9] cast pointer to pointer required as malloc returns a void * */
    buffer = reinterpret_cast<char8 *>((GlobalObjectsDatabase::Instance()->GetStandardHeap())->Malloc(memorySize));

    bool ok = (buffer != NULL_PTR(char8 *));

    if (!ok) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Failed to allocate memory buffer");
    }

}

Buffer::~Buffer() {

    if (buffer != NULL_PTR(char8 *)) {
        /*lint -e{925} [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9] cast pointer to pointer required as free requires void * */
        void * ref = static_cast<void *>(buffer);
        /*lint -e{1551} the implementation does not throw exception*/
        (GlobalObjectsDatabase::Instance()->GetStandardHeap())->Free(ref);
        buffer = NULL_PTR(char8 *);
    }

}

bool Buffer::Initialise(const char8 * const data) {

    bool ok = (buffer != NULL_PTR(char8 *));

    uint32 index;

    for (index = 0u; (index < size) && (ok); index ++) {
        ok = PutData(data, index);
    }

    return ok;
}

uint32 Buffer::GetSize() const {
    return size;
}

bool Buffer::GetData(char8 * const &  data, const uint32 index) const {

    bool ok = (buffer != NULL_PTR(char8 *));

    if (ok) {
        ok = (index < size);
    }

    if (ok) {
        uint64 memSize=static_cast<uint64>(index)*sizeOf;
        /*lint -e{925} [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9] cast pointer to pointer required as memcpy requires void * */
        ok = MemoryOperationsHelper::Copy(reinterpret_cast<void *>(data), &buffer[memSize], sizeOf);
    }

    return ok;
}

bool Buffer::PutData(const char8 * const data, const uint32 index) {

    bool ok = (buffer != NULL_PTR(char8 *));

    if (ok) {
        ok = (index < size);
    }

    if (ok) {
        uint64 memSize=static_cast<uint64>(index)*sizeOf;
        /*lint -e{925} [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9] cast pointer to pointer required as memcpy requires void * */
        ok = MemoryOperationsHelper::Copy(&buffer[memSize], reinterpret_cast<const void *>(data), sizeOf);
    }

    return ok;
}


}

	
