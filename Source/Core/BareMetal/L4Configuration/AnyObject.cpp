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
#include "Vector.h"
#include "Matrix.h"
#include "StreamI.h"
#include "String.h"
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
}

bool AnyObject::SerializeMatrix() {
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;
    uint32 numberOfColumns = type.GetNumberOfElements(0u);
    uint32 numberOfRows = type.GetNumberOfElements(1u);
    TypeDescriptor dataDescriptor = type.GetTypeDescriptor();
    void *srcDataPointer = type.GetDataPointer();

    bool ok = true;

    void *value = HeapManager::Malloc(numberOfRows * numberOfColumns * sizeof(void *));
    type.SetDataPointer(value);
    void **copyArray = static_cast<void **>(value);

    uint32 r = 0u;
    uint32 c = 0u;
    uint32 idx = 0u;
    for (r = 0; ok && (r < numberOfRows); r++) {
        for (c = 0; ok && (c < numberOfColumns); c++) {
            idx = c + r * numberOfColumns;
            if (dataDescriptor.type == SString) {
                String *stream = NULL;
                if (type.IsStaticDeclared()) {
                    stream = static_cast<String *>(srcDataPointer);
                    stream += idx;
                }
                else {
                    stream = &static_cast<String **>(srcDataPointer)[r][c];
                }
                copySize = stream->Size() + 1u;
            }
            else if (dataDescriptor.type == CCString) {
                const char8 *srcArray = NULL_PTR(const char8 *);
                if (type.IsStaticDeclared()) {
                    srcArray = static_cast<const char8 **>(srcDataPointer)[idx];
                }
                else {
                    srcArray = static_cast<char8 ***>(srcDataPointer)[r][c];
                }
                copySize = (StringHelper::Length(srcArray) + 1u);
            }
            else {
                copySize = dataDescriptor.numberOfBits / 8u;
            }

            ok = (copySize > 0u);

            if (ok) {
                copyArray[idx] = HeapManager::Malloc(copySize);
                const char8 *srcArray = NULL_PTR(const char8 *);
                if (dataDescriptor.type == SString) {
                    String *stream = NULL;
                    if (type.IsStaticDeclared()) {
                        stream = static_cast<String *>(srcDataPointer);
                        stream += idx;
                    }
                    else {
                        stream = &static_cast<String **>(srcDataPointer)[r][c];
                    }
                    srcArray = stream->Buffer();
                }
                else if (dataDescriptor.type == CCString) {
                    if (type.IsStaticDeclared()) {
                        srcArray = static_cast<const char8 **>(srcDataPointer)[idx];
                    }
                    else {
                        srcArray = static_cast<char8 ***>(srcDataPointer)[r][c];
                    }
                }
                else {
                    srcArray = static_cast<char8 *>(srcDataPointer) + totalCopySize;
                }
                ok = MemoryOperationsHelper::Copy(copyArray[idx], srcArray, copySize);
                totalCopySize += copySize;
            }
        }
    }

    return ok;
}

bool AnyObject::SerializeVector() {
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;
    uint32 idx = 0u;
    uint32 numberOfElements = type.GetNumberOfElements(0u);
    TypeDescriptor dataDescriptor = type.GetTypeDescriptor();
    void *srcDataPointer = type.GetDataPointer();
    bool ok = true;

    void *value = HeapManager::Malloc(numberOfElements * sizeof(void *));
    type.SetDataPointer(value);
    void **copyArray = static_cast<void **>(value);
    for (idx = 0; ok && (idx < numberOfElements); idx++) {
        if (dataDescriptor.type == SString) {
            String *stream = NULL_PTR(String *);
            //Assume it is zero terminated...
            stream = static_cast<String *>(srcDataPointer);
            stream += idx;

            copySize = stream->Size() + 1u;
        }
        else if (dataDescriptor.type == CCString) {
            const char8 *srcArray = NULL_PTR(const char8 *);
            srcArray = static_cast<const char8 *>(srcDataPointer);
            srcArray += (idx * sizeof(char8 *));
            copySize = (StringHelper::Length(srcArray) + 1u);
        }
        else {
            copySize = dataDescriptor.numberOfBits / 8u;
        }

        ok = (copySize > 0u);

        if (ok) {
            copyArray[idx] = HeapManager::Malloc(copySize);
            const char8 *srcArray = NULL_PTR(const char8 *);
            if (dataDescriptor.type == SString) {
                String *stream = NULL_PTR(String *);
                stream = static_cast<String *>(srcDataPointer);
                stream += idx;
                srcArray = stream->Buffer();
            }
            else if (dataDescriptor.type == CCString) {
                srcArray = static_cast<const char8 *>(srcDataPointer);
                srcArray += (idx * sizeof(char8 *));
            }
            else {
                srcArray = static_cast<char8 *>(srcDataPointer) + totalCopySize;
            }
            ok = MemoryOperationsHelper::Copy(copyArray[idx], srcArray, copySize);
            totalCopySize += copySize;
        }
    }

    return ok;
}

bool AnyObject::Load(const AnyType &typeIn) {
    uint32 nOfDimensions = typeIn.GetNumberOfDimensions();
    uint32 copySize = 0u;
    bool ok = true;
    type = typeIn;

    if (nOfDimensions == 0u) {
        if (typeIn.GetTypeDescriptor().type == SString) {
            String *stream = static_cast<String *>(typeIn.GetDataPointer());
            copySize = stream->Size() + 1u;
        }
        else if (typeIn.GetTypeDescriptor().type == CCString) {
            copySize = (StringHelper::Length(static_cast<char8 *>(typeIn.GetDataPointer())) + 1u);
        }
        else {
            copySize = typeIn.GetTypeDescriptor().numberOfBits / 8u;
        }
        void *value = HeapManager::Malloc(copySize);
        type.SetDataPointer(value);
        if (typeIn.GetTypeDescriptor().type == SString) {
            String *stream = static_cast<String *>(typeIn.GetDataPointer());
            ok = MemoryOperationsHelper::Copy(value, stream->Buffer(), copySize);
        }
        else {
            ok = MemoryOperationsHelper::Copy(value, typeIn.GetDataPointer(), copySize);
        }

    }
    else if (nOfDimensions == 1u) {
        ok = SerializeVector();
    }
    else if (nOfDimensions == 2u) {
        ok = SerializeMatrix();
    }

    //Strings are serialised as arrays of CCString. The type as to be updated accordingly.
    if (typeIn.GetTypeDescriptor().type == SString) {
        void *tmp = type.GetDataPointer();
        type = AnyType(static_cast<char8 *>(typeIn.GetDataPointer()));
        type.SetDataPointer(tmp);
        type.SetStaticDeclared(typeIn.IsStaticDeclared());
        type.SetNumberOfDimensions(typeIn.GetNumberOfDimensions());
        type.SetNumberOfElements(0u, typeIn.GetNumberOfElements(0u));
        type.SetNumberOfElements(1u, typeIn.GetNumberOfElements(1u));
        type.SetNumberOfElements(2u, typeIn.GetNumberOfElements(2u));
    }

    return ok;
}

AnyObject::~AnyObject() {
    void *mem = type.GetDataPointer();

    if (type.GetNumberOfDimensions() > 0u) {
        uint32 rows = type.GetNumberOfElements(1u);
        uint32 columns = type.GetNumberOfElements(0u);
        uint32 zs = type.GetNumberOfElements(2u);
        if (rows == 0u) {
            rows = 1u;
        }
        if (zs == 0u) {
            zs = 1u;
        }
        uint32 nOfElements = rows * columns * zs;
        if (nOfElements > 0u) {
            void **multiDimMem = static_cast<void **>(mem);
            uint32 idx = 0u;
            for (idx = 0u; idx < nOfElements; idx++) {
                if (multiDimMem[idx] != NULL_PTR(void *)) {
                    HeapManager::Free(multiDimMem[idx]);
                }
            }
        }
    }
    if (mem != NULL_PTR(void *)) {
        HeapManager::Free(mem);
    }
}

AnyType AnyObject::GetType() {
    return type;
}

CLASS_REGISTER(AnyObject, "1.0")

}

