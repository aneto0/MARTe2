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

    char *value = NULL_PTR(char *);
    if (type.IsStaticDeclared()) {
        value = static_cast<char *>(HeapManager::Malloc(numberOfRows * numberOfColumns * dataDescriptor.numberOfBits / 8u));
    }
    else {
        value = static_cast<char *>(HeapManager::Malloc(numberOfRows * sizeof(void *)));
    }
    type.SetDataPointer(value);

    uint32 r = 0u;
    uint32 c = 0u;
    uint32 idx = 0u;
    for (r = 0; ok && (r < numberOfRows); r++) {
        if (!type.IsStaticDeclared()) {
            void **destStr = reinterpret_cast<void **>(value);
            destStr[r] = static_cast<char **>(HeapManager::Malloc(sizeof(void *) * numberOfColumns));
            totalCopySize = 0u;
        }

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
                char8 *destArray = NULL_PTR(char8 *);
                const char8 *srcArray = NULL_PTR(const char8 *);
                if (dataDescriptor.type == SString) {
                    String *stream = NULL;
                    if (type.IsStaticDeclared()) {
                        stream = static_cast<String *>(srcDataPointer);
                        stream += idx;
                        char **destStr = reinterpret_cast<char **>(value);
                        destStr[idx] = static_cast<char *>(HeapManager::Malloc(copySize));
                        destArray = destStr[idx];
                    }
                    else {
                        stream = &static_cast<String **>(srcDataPointer)[r][c];
                        char ***destStr = reinterpret_cast<char ***>(value);
                        destStr[r][c] = static_cast<char *>(HeapManager::Malloc(copySize));
                        destArray = destStr[r][c];
                    }
                    srcArray = stream->Buffer();
                }
                else if (dataDescriptor.type == CCString) {
                    if (type.IsStaticDeclared()) {
                        char **destStr = reinterpret_cast<char **>(value);
                        srcArray = static_cast<const char8 **>(srcDataPointer)[idx];
                        destStr[idx] = static_cast<char *>(HeapManager::Malloc(copySize));
                        destArray = destStr[idx];
                    }
                    else {
                        char8 ***destStr = reinterpret_cast<char8 ***>(value);
                        srcArray = static_cast<char8 ***>(srcDataPointer)[r][c];
                        destStr[r][c] = static_cast<char *>(HeapManager::Malloc(copySize));
                        destArray = destStr[r][c];
                    }
                }
                else {
                    if (type.IsStaticDeclared()) {
                        srcArray = static_cast<char8 *>(srcDataPointer) + totalCopySize;
                        destArray = value + totalCopySize;
                    }
                    else {
                        char **src = reinterpret_cast<char **>(srcDataPointer);
                        srcArray = src[r] + totalCopySize;
                        char **dest = reinterpret_cast<char **>(value);
                        destArray = dest[r] + totalCopySize;
                        /*srcArray = static_cast<char8 *>(srcDataPointer) + totalCopySize;
                         destArray = value + totalCopySize;*/
                    }
                }
                ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
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

    char *value = static_cast<char *>(HeapManager::Malloc(numberOfElements * dataDescriptor.numberOfBits / 8u));
    type.SetDataPointer(value);
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
            const char8 *srcArray = NULL_PTR(const char8 *);
            char8 *destArray = NULL_PTR(char8 *);
            if (dataDescriptor.type == SString) {
                char **destStr = reinterpret_cast<char **>(value);
                destStr[idx] = static_cast<char *>(HeapManager::Malloc(copySize));
                String *stream = NULL_PTR(String *);
                stream = static_cast<String *>(srcDataPointer);
                stream += idx;
                srcArray = stream->Buffer();
                destArray = destStr[idx];
            }
            else if (dataDescriptor.type == CCString) {
                char **destStr = reinterpret_cast<char **>(value);
                destStr[idx] = static_cast<char *>(HeapManager::Malloc(copySize));
                srcArray = static_cast<const char8 *>(srcDataPointer);
                srcArray += (idx * sizeof(char8 *));
                destArray = destStr[idx];
            }
            else {
                srcArray = static_cast<char8 *>(srcDataPointer) + totalCopySize;
                destArray = value + totalCopySize;
            }
            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
            totalCopySize += copySize;
        }
    }

    return ok;
}

bool AnyObject::SerializeScalar() {
    uint32 copySize = 0u;
    bool ok = true;

    if (type.GetTypeDescriptor().type == SString) {
        String *stream = static_cast<String *>(type.GetDataPointer());
        copySize = stream->Size() + 1u;
    }
    else if (type.GetTypeDescriptor().type == CCString) {
        copySize = (StringHelper::Length(static_cast<char8 *>(type.GetDataPointer())) + 1u);
    }
    else {
        copySize = type.GetTypeDescriptor().numberOfBits / 8u;
    }
    void *value = HeapManager::Malloc(copySize);
    if (type.GetTypeDescriptor().type == SString) {
        String *stream = static_cast<String *>(type.GetDataPointer());
        ok = MemoryOperationsHelper::Copy(value, stream->Buffer(), copySize);
    }
    else {
        ok = MemoryOperationsHelper::Copy(value, type.GetDataPointer(), copySize);
    }

    type.SetDataPointer(value);
    return ok;
}

bool AnyObject::Serialise(const AnyType &typeIn) {
    uint32 nOfDimensions = typeIn.GetNumberOfDimensions();
    bool ok = true;

    ok = (type.GetDataPointer() == NULL_PTR(void *));
    if (ok) {
        type = typeIn;
        if (nOfDimensions == 0u) {
            ok = SerializeScalar();
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
    }

    return ok;
}

AnyObject::~AnyObject() {
    void *mem = type.GetDataPointer();

    if (type.GetNumberOfDimensions() == 1u) {
        uint32 numberOfColumns = type.GetNumberOfElements(0u);
        bool cString = (type.GetTypeDescriptor().type == CCString);
        bool sString = (type.GetTypeDescriptor().type == SString);
        if (cString || sString) {
            char **charArray = static_cast<char **>(mem);
            uint32 idx = 0u;
            for (idx = 0u; idx < numberOfColumns; idx++) {
                if (charArray[idx] != NULL_PTR(void *)) {
                    void *charMem = reinterpret_cast<void *>(charArray[idx]);
                    HeapManager::Free(charMem);
                }
            }
        }
    }
    else if (type.GetNumberOfDimensions() == 2u) {
        uint32 numberOfColumns = type.GetNumberOfElements(0u);
        uint32 numberOfRows = type.GetNumberOfElements(1u);

        uint32 r = 0u;
        uint32 c = 0u;
        uint32 idx = 0u;
        for (r = 0; r < numberOfRows; r++) {
            for (c = 0; c < numberOfColumns; c++) {
                void *charMem = NULL_PTR(void *);
                if (type.IsStaticDeclared()) {
                    idx = c + r * numberOfColumns;
                    char **destStr = reinterpret_cast<char **>(mem);
                    charMem = reinterpret_cast<void *>(destStr[idx]);
                }
                else {
                    char8 ***destStr = reinterpret_cast<char8 ***>(mem);
                    charMem = reinterpret_cast<void *>(destStr[r][c]);
                }
                HeapManager::Free(charMem);
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

