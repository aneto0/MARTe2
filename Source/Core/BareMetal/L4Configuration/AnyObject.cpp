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
#include "StreamString.h"
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
    uint32 nOfDimensions = typeIn.GetNumberOfDimensions();
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;
    bool ok = true;

    if (nOfDimensions == 0u) {
        if (typeIn.GetTypeDescriptor().type == StreamStringType) {
            StreamString *stream = static_cast<StreamString *>(typeIn.GetDataPointer());
            copySize = stream->Size() + 1u;
        }
        else if (typeIn.GetTypeDescriptor().type == CCString) {
            copySize = (StringHelper::Length(static_cast<char *>(typeIn.GetDataPointer())) + 1u);
        }
        else {
            copySize = typeIn.GetTypeDescriptor().numberOfBits / 8u;
        }
        value = HeapManager::Malloc(copySize);
        if (typeIn.GetTypeDescriptor().type == StreamStringType) {
            StreamString *stream = static_cast<StreamString *>(typeIn.GetDataPointer());
            ok = MemoryOperationsHelper::Copy(value, stream->BufferReference(), copySize);
        }
        else {
            ok = MemoryOperationsHelper::Copy(value, typeIn.GetDataPointer(), copySize);
        }

    }
    else if (nOfDimensions == 1u) {
        VectorCore vecToSer(typeIn);
        vecToSer.SetDataPointer(typeIn.GetDataPointer());
        vecToSer.Serialize(value);
    }
    else if (nOfDimensions == 2u) {
        MatrixCore matToSer(typeIn);
        matToSer.SetDataPointer(typeIn.GetDataPointer());
        matToSer.Serialize(value);
    }

#if 0
    if (nOfDimensions == 0u) {
        if (typeIn.GetTypeDescriptor().type == StreamStringType) {
            copySize = sizeof(void *);
        }
        else {
            copySize = typeIn.GetTypeDescriptor().numberOfBits / 8u;
        }
    }
    else if (nOfDimensions < 4u) {
        uint32 totalElements = 1u;
        uint32 i;
        for (i = 0u; i < nOfDimensions; i++) {
            if (typeIn.GetNumberOfElements(i) != 0u) {
                totalElements *= typeIn.GetNumberOfElements(i);
            }
        }
        copySize = sizeof(void *) * totalElements;
    }
    else {
        ok = false;
    }

    if (ok) {
        ok = (copySize > 0u);
        if (ok) {
            value = HeapManager::Malloc(copySize);
        }
    }

    if (ok) {
        void **copyArray = static_cast<void **>(value);

        uint32 c = 0u;
        uint32 r = 0u;
        uint32 z = 0u;
        uint32 numberOfColumns = typeIn.GetNumberOfElements(0u);
        uint32 numberOfRows = typeIn.GetNumberOfElements(1u);
        uint32 numberOfZ = typeIn.GetNumberOfElements(2u);
        if (numberOfColumns == 0u) {
            numberOfColumns = 1u;
        }
        if (numberOfRows == 0u) {
            numberOfRows = 1u;
        }
        if (numberOfZ == 0u) {
            numberOfZ = 1u;
        }

        for (z = 0u; ok && (z < numberOfZ); z++) {
            for (r = 0u; ok && (r < numberOfRows); r++) {
                for (c = 0u; ok && (c < numberOfColumns); c++) {
                    uint32 idx = c + (r * numberOfColumns) + (z * numberOfRows * numberOfColumns);

                    if (typeIn.GetTypeDescriptor().type == StreamStringType) {
                        StreamString *stream = NULL_PTR(StreamString *);
                        //Assume it is zero terminated...
                        if (typeIn.IsStaticDeclared() || (typeIn.GetNumberOfDimensions() < 2u)) {
                            stream = static_cast<StreamString *>(typeIn.GetDataPointer());
                            stream += idx;
                        }
                        else {
                            stream = &static_cast<StreamString **>(typeIn.GetDataPointer())[r][c];
                        }
                        copySize = stream->Size() + 1u;
                    }
                    else if (typeIn.GetTypeDescriptor().type == CCString) {
                        const char8 *srcArray = NULL_PTR(const char8 *);
                        if (typeIn.GetNumberOfDimensions() < 2u) {
                            srcArray = static_cast<const char *>(typeIn.GetDataPointer());
                            srcArray += (idx * sizeof(char *));
                        }
                        else if (typeIn.IsStaticDeclared()) {
                            srcArray = static_cast<const char **>(typeIn.GetDataPointer())[idx];
                        }
                        else {
                            srcArray = &static_cast<char **>(typeIn.GetDataPointer())[r][c];
                        }
                        copySize = (StringHelper::Length(srcArray) + 1u);
                    }
                    else {
                        copySize = typeIn.GetTypeDescriptor().numberOfBits / 8u;
                    }

                    ok = (copySize > 0u);

                    if (ok) {
                        copyArray[idx] = HeapManager::Malloc(copySize);
                        const char *srcArray = NULL_PTR(const char *);
                        if (typeIn.GetTypeDescriptor().type == StreamStringType) {
                            StreamString *stream = NULL_PTR(StreamString *);
                            if (typeIn.IsStaticDeclared() || (typeIn.GetNumberOfDimensions() < 2u)) {
                                stream = static_cast<StreamString *>(typeIn.GetDataPointer());
                                stream += idx;
                            }
                            else {
                                stream = &static_cast<StreamString **>(typeIn.GetDataPointer())[r][c];
                            }
                            srcArray = stream->BufferReference();
                        }
                        else if (typeIn.GetTypeDescriptor().type == CCString) {
                            if (typeIn.GetNumberOfDimensions() < 2u) {
                                srcArray = static_cast<const char *>(typeIn.GetDataPointer());
                                srcArray += (idx * sizeof(char *));
                            }
                            else if (typeIn.IsStaticDeclared()) {
                                srcArray = static_cast<const char **>(typeIn.GetDataPointer())[idx];
                            }
                            else {
                                srcArray = &static_cast<char **>(typeIn.GetDataPointer())[r][c];
                            }
                        }
                        else {
                            srcArray = static_cast<char *>(typeIn.GetDataPointer()) + totalCopySize;
                        }
                        ok = MemoryOperationsHelper::Copy(copyArray[idx], srcArray, copySize);
                        totalCopySize += copySize;
                    }
                }
            }
        }
    }
#endif

    type = typeIn;
    type.SetDataPointer(value);

    return ok;
}

#if 0
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
#endif

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

