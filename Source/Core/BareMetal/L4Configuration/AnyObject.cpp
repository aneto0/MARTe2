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

AnyObject::AnyObject() :
        Object() {
}

/*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: Three pointer indirection levels required for matrices of char *. */
bool AnyObject::SerializeMatrix() {
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;
    uint32 numberOfColumns = type.GetNumberOfElements(0u);
    uint32 numberOfRows = type.GetNumberOfElements(1u);
    TypeDescriptor dataDescriptor = type.GetTypeDescriptor();
    void *srcDataPointer = type.GetDataPointer();

    bool ok = true;

    uint32 memoryAllocationSize;
    if (type.IsStaticDeclared()) {
        //Memory is treated as a linear arrangement of the elements
        memoryAllocationSize = (numberOfRows * numberOfColumns * dataDescriptor.numberOfBits) / 8u;
    }
    else {
        //Equivalent to new T[], i.e. each row is a pointer to the first column with the data
        memoryAllocationSize = static_cast<uint32>(numberOfRows * sizeof(void *));
    }
    char8 *value = static_cast<char8 *>(HeapManager::Malloc(memoryAllocationSize));
    type.SetDataPointer(value);

    uint32 r;
    uint32 c;
    uint32 idx;
    for (r = 0u; ok && (r < numberOfRows); r++) {
        if (!type.IsStaticDeclared()) {
            //If it was not statically declared, for each row, allocate space for all the columns row[r] = new T[]
            void **destStr = reinterpret_cast<void **>(value);
            destStr[r] = static_cast<char8 **>(HeapManager::Malloc(static_cast<uint32>((dataDescriptor.numberOfBits / 8u) * numberOfColumns)));
            totalCopySize = 0u;
        }

        for (c = 0u; ok && (c < numberOfColumns); c++) {
            //For static arrays move linearly
            idx = c + (r * numberOfColumns);

            //String and CString contain pointers to the memory address actually containing the data and thus require malloc
            if (dataDescriptor.type == SString) {
                String *stream;
                if (type.IsStaticDeclared()) {
                    stream = &static_cast<String *>(srcDataPointer)[idx];
                }
                else {
                    stream = &static_cast<String **>(srcDataPointer)[r][c];
                }
                copySize = static_cast<uint32>(stream->Size());
                copySize += 1u;
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
                copySize = dataDescriptor.numberOfBits;
                copySize /= 8u;
            }

            ok = (copySize > 0u);

            if (ok) {
                char8 *destArray = NULL_PTR(char8 *);
                const char8 *srcArray = NULL_PTR(const char8 *);
                if (dataDescriptor.type == SString) {
                    String *stream = NULL_PTR(String *);
                    if (type.IsStaticDeclared()) {
                        stream = &static_cast<String *>(srcDataPointer)[idx];
                        char8 **destStr = reinterpret_cast<char8 **>(value);
                        destStr[idx] = static_cast<char8 *>(HeapManager::Malloc(copySize));
                        destArray = destStr[idx];
                    }
                    else {
                        stream = &static_cast<String **>(srcDataPointer)[r][c];
                        char8 ***destStr = reinterpret_cast<char8 ***>(value);
                        destStr[r][c] = static_cast<char8 *>(HeapManager::Malloc(copySize));
                        destArray = destStr[r][c];
                    }
                    srcArray = stream->Buffer();
                }
                else if (dataDescriptor.type == CCString) {
                    if (type.IsStaticDeclared()) {
                        char8 **destStr = reinterpret_cast<char8 **>(value);
                        srcArray = static_cast<const char8 **>(srcDataPointer)[idx];
                        destStr[idx] = static_cast<char8 *>(HeapManager::Malloc(copySize));
                        destArray = destStr[idx];
                    }
                    else {
                        char8 ***destStr = reinterpret_cast<char8 ***>(value);
                        srcArray = static_cast<char8 ***>(srcDataPointer)[r][c];
                        destStr[r][c] = static_cast<char8 *>(HeapManager::Malloc(copySize));
                        destArray = destStr[r][c];
                    }
                }
                else {
                    if (type.IsStaticDeclared()) {
                        srcArray = &static_cast<char8 *>(srcDataPointer)[totalCopySize];
                        destArray = &value[totalCopySize];
                    }
                    else {
                        char8 **src = reinterpret_cast<char8 **>(srcDataPointer);
                        char8 *srcRow = src[r];
                        srcArray = &srcRow[totalCopySize];
                        char8 **dest = reinterpret_cast<char8 **>(value);
                        char8 *destRow = dest[r];
                        destArray = &destRow[totalCopySize];
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
    uint32 idx;
    uint32 numberOfElements = type.GetNumberOfElements(0u);
    TypeDescriptor dataDescriptor = type.GetTypeDescriptor();
    void *srcDataPointer = type.GetDataPointer();
    bool ok = true;

    char8 *value = static_cast<char8 *>(HeapManager::Malloc((numberOfElements * dataDescriptor.numberOfBits) / 8u));
    type.SetDataPointer(value);
    for (idx = 0u; ok && (idx < numberOfElements); idx++) {
        if (dataDescriptor.type == SString) {
            String *stream = static_cast<String *>(srcDataPointer);
            copySize = static_cast<uint32>(stream[idx].Size());
            //Assume it is zero terminated...
            copySize += 1u;
        }
        else if (dataDescriptor.type == CCString) {
            const char8 *srcArray = (static_cast<const char8 **>(srcDataPointer))[idx];
            copySize = (StringHelper::Length(srcArray) + 1u);
        }
        else {
            copySize = dataDescriptor.numberOfBits;
            copySize /= 8u;
        }

        ok = (copySize > 0u);

        if (ok) {
            const char8 *srcArray = NULL_PTR(const char8 *);
            char8 *destArray = NULL_PTR(char8 *);
            if (dataDescriptor.type == SString) {
                char8 **destStr = reinterpret_cast<char8 **>(value);
                destStr[idx] = static_cast<char8 *>(HeapManager::Malloc(copySize));
                String *stream = static_cast<String *>(srcDataPointer);
                srcArray = stream[idx].Buffer();
                destArray = destStr[idx];
            }
            else if (dataDescriptor.type == CCString) {
                char8 **destStr = reinterpret_cast<char8 **>(value);
                destStr[idx] = static_cast<char8 *>(HeapManager::Malloc(copySize));
                srcArray = (static_cast<const char8 **>(srcDataPointer))[idx];
                destArray = destStr[idx];
            }
            else {
                srcArray = &static_cast<char8 *>(srcDataPointer)[totalCopySize];
                destArray = &value[totalCopySize];
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
        copySize = static_cast<uint32>(stream->Size()) + 1u;
    }
    else if (type.GetTypeDescriptor().type == CCString) {
        copySize = StringHelper::Length(static_cast<char8 *>(type.GetDataPointer()));
        copySize += 1u;
    }
    else {
        copySize = type.GetTypeDescriptor().numberOfBits;
        copySize /= 8u;
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
    bool ok = (type.GetDataPointer() == NULL_PTR(void *));
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
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Serialisation of AnyType with dimension > 2 not supported!");
            ok = false;
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

/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively managed by this class.". */
AnyObject::~AnyObject() {
    void *mem = type.GetDataPointer();
    bool cString = (type.GetTypeDescriptor().type == CCString);
    bool sString = (type.GetTypeDescriptor().type == SString);

    if (type.GetNumberOfDimensions() == 1u) {
        uint32 numberOfColumns = type.GetNumberOfElements(0u);
        if (cString || sString) {
            char8 **charArray = static_cast<char8 **>(mem);
            uint32 idx;
            for (idx = 0u; idx < numberOfColumns; idx++) {
                if (charArray[idx] != NULL_PTR(void *)) {
                    void *charMem = reinterpret_cast<void *>(charArray[idx]);
                    if (!HeapManager::Free(charMem)) {
                        REPORT_ERROR(ErrorManagement::FatalError, "HeapManager::Free failed. Vector memory not deallocated.");
                    }
                }
            }
        }
    }
    else if (type.GetNumberOfDimensions() == 2u) {
        uint32 numberOfColumns = type.GetNumberOfElements(0u);
        uint32 numberOfRows = type.GetNumberOfElements(1u);

        uint32 r;
        uint32 c;
        uint32 idx;
        for (r = 0u; r < numberOfRows; r++) {
            if (cString || sString) {
                for (c = 0u; c < numberOfColumns; c++) {
                    void *charMem = NULL_PTR(void *);
                    if (type.IsStaticDeclared()) {
                        idx = c + (r * numberOfColumns);
                        char8 **destStr = reinterpret_cast<char8 **>(mem);
                        charMem = reinterpret_cast<void *>(destStr[idx]);
                    }
                    else {
                        /*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: Three pointer indirection levels required for matrices of char *. */
                        char8 ***destStr = reinterpret_cast<char8 ***>(mem);
                        charMem = reinterpret_cast<void *>(destStr[r][c]);
                    }
                    if (!HeapManager::Free(charMem)) {
                        REPORT_ERROR(ErrorManagement::FatalError, "HeapManager::Free failed. Matrix memory not deallocated.");
                    }
                }
            }

            //Free each of the rows
            if (!type.IsStaticDeclared()) {
                void **destStr = reinterpret_cast<void **>(mem);
                if (!HeapManager::Free(destStr[r])) {
                    REPORT_ERROR(ErrorManagement::FatalError, "HeapManager::Free failed. Matrix row memory not deallocated.");
                }
            }
        }
    }
    else {
        if (type.GetNumberOfDimensions() != 0u) {
            REPORT_ERROR(ErrorManagement::FatalError, "Tried to free memory of AnyObject with dimensions > 2.");
        }
    }

    if (mem != NULL_PTR(void *)) {
        if (!HeapManager::Free(mem)) {
            REPORT_ERROR(ErrorManagement::FatalError, "HeapManager::Free failed. AnyObject memory not deallocated.");
        }
    }

}

AnyType AnyObject::GetType() const {
    return type;
}

CLASS_REGISTER(AnyObject, "1.0")

}

