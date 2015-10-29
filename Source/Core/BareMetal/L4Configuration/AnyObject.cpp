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
    uint32 copySize;
    if (nOfDimensions > 0) {
        uint32 i;
        for (i = 0u; i < nOfDimensions; i++) {
            copySize += typeIn.GetNumberOfElements(i) * typeSize;
        }
    }
    else {
        copySize = typeSize;
    }
    value = HeapManager::Malloc(copySize);
    if (!MemoryOperationsHelper::Copy(value, typeIn.GetDataPointer(), copySize)) {
        //TODO
    }
    printf("HERE! %d\n", typeIn.GetNumberOfDimensions());
    printf("HERE! %d\n", typeIn.GetNumberOfElements(0));
    type = typeIn;
    printf("!HERE\n");
}

AnyObject::~AnyObject() {
    if (value != NULL_PTR(void *)) {
        HeapManager::Free(value);
    }
}

AnyType AnyObject::GetType() {
    return type;
}

CLASS_REGISTER(AnyObject, "1.0")

}

