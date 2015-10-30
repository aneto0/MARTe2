/**
 * @file AnyObject.cpp
 * @brief Source file for class AnyObject
 * @date 27/10/2015
 * @author Andre' Neto
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
 * the class AnyObject (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "StreamI.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

AnyObject::AnyObject() {
    value = NULL_PTR(void *);
}

bool AnyObject::Load(const AnyType &typeIn) {
    uint32 typeSize = typeIn.GetTypeDescriptor().numberOfBits / 8u;
    uint32 nOfDimensions = typeIn.GetNumberOfDimensions();
    uint32 copySize = 0u;
    uint32 totalElements = 1u;
    bool ok = true;

    if (typeIn.GetTypeDescriptor().type == Stream) {
        StreamI *stream = static_cast<StreamI *>(typeIn.GetDataPointer());
        if (stream != NULL) {
            copySize = stream->Size();
        }
        else {
            ok = false;
        }
    }
    else if ((nOfDimensions == 0u) && (typeIn.GetTypeDescriptor().type == CCString)) {
        copySize = (StringHelper::Length(static_cast<const char *>(typeIn.GetDataPointer())) + 1u);
    }
    else {
        uint32 i;
        for (i = 0u; i < nOfDimensions; i++) {
            if (typeIn.GetNumberOfElements(i) != 0u) {
                totalElements *= typeIn.GetNumberOfElements(i);
            }
        }
        copySize = typeSize * totalElements;
    }

    value = HeapManager::Malloc(copySize);

    if (typeIn.GetTypeDescriptor().type == Stream) {
        StreamI *stream = static_cast<StreamI*>(typeIn.GetDataPointer());
        ok = stream->Seek(0u);
        if (ok) {
            stream->Read(static_cast<char8 *>(value), copySize);
        }
    }
    else if ((nOfDimensions == 1u) && (typeIn.GetTypeDescriptor().type == CCString)) {
        uint32 j;
        char **destArray = static_cast<char **>(value);
        const char **stringArray = static_cast<const char **>(typeIn.GetDataPointer());
        //TODO .GetNumberOfElements(0u) should walk all the dimension
        for (j = 0; ok && (j < typeIn.GetNumberOfElements(0u)); j++) {
            copySize = (StringHelper::Length(stringArray[j]) + 1u);
            destArray[j] = static_cast<char *>(HeapManager::Malloc(copySize));
            if (!MemoryOperationsHelper::Copy(static_cast<void *>(destArray[j]), static_cast<const void *>(stringArray[j]), copySize)) {
                ok = false;
            }
        }
    }
    else {
        if (!MemoryOperationsHelper::Copy(value, typeIn.GetDataPointer(), copySize)) {
            ok = false;
        }
    }

    type = typeIn;
    type.SetDataPointer(value);
    if (typeIn.GetTypeDescriptor().type == Stream) {
        type.SetNumberOfElements(2u, copySize);
    }
    return ok;
}

AnyObject::~AnyObject() {
    if (value != NULL_PTR(void *)) {
        HeapManager::Free(value);
    }
    //TODO must destroy multidim types
}

AnyType AnyObject::GetType() {
    return type;
}

CLASS_REGISTER(AnyObject, "1.0")

}

