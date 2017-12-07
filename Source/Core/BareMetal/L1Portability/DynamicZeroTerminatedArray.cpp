/**
 * @file DynamicZeroTerminatedArray.cpp
 * @brief Source file for class DynamicZeroTerminatedArray
 * @date 27/01/2017
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
 * the class MemoryArea (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DynamicZeroTerminatedArray.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe{

bool DZTInitCopy(uint32 sizeOfData,uint32 sizeOfT,uint32 granularity,void *&dest,void const *src){
    bool ret = true;
	uint32 necessarySize = ((1 + sizeOfData + granularity)/ granularity)*granularity;

    dest = HeapManager::Malloc(necessarySize*sizeOfT);
    if (dest != NULL) {
        MemoryOperationsHelper::Copy(dest,src,(1 + sizeOfData)*sizeOfT);
    } else {
        REPORT_ERROR(ErrorManagement::FatalError, "DZTInitCopy: Error: zero term array duplicated creation failed");
        ret = false;
    }
    return ret;
}

bool DZTAppend1(uint32 sizeOfT,uint32 granularity,uint32 sizeOfDest,void *&dest,void const *src){

    bool ret = true;
    // assuming memory is allocated in a granular way
    // we can use this indicator to assess whether we need to allocate or we can simply write
    // freeSpace is normally granularity - actual free space
    // but in case of zero free space it becomes zero.
    uint32 freeSpace = (sizeOfDest + 1) % granularity ;

    // extreme case indicating in the worst case no more memory
    if (freeSpace == 0){
        uint32 necessarySize = (sizeOfDest + 1 + granularity);

        bool ok = HeapManager::Realloc(dest,necessarySize*sizeOfT);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "DZTAppend1: Failed HeapManager::Realloc()");
        }
        ret = (ok && (dest != NULL_PTR(void *)));
    }
    return ret;
}

bool DZTAppendN(uint32 sizeOfT,uint32 granularity,uint32 sizeOfDest,uint32 toCopy,
				void *&dest,void const *src){

    bool ret = true;
    uint32 size = sizeOfDest;
    uint32 size2 = toCopy;

    uint32 necessarySize = ((size + size2 + 1 + granularity) / granularity) * granularity;

    ret = HeapManager::Realloc(dest,necessarySize*sizeOfT);
    ret = ret && (dest != NULL_PTR(void *));
    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "DZTAppendN: Failed HeapManager::Realloc()");
    }

    if (ret)  {
        void *destOff      = static_cast<void *>(static_cast<char *>(dest)+size*sizeOfT);
        MemoryOperationsHelper::Copy(destOff,src,size2*sizeOfT);
    }

    return ret;
}
}


