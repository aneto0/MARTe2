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
#define DLL_API

#include "AnyObject.h"
#include "AdvancedErrorManagement.h"
#include "ClassRegistryItemT.h"
#include "Matrix.h"
#include "MemoryOperationsHelper.h"
#include "StreamString.h"
#include "StringHelper.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static bool SerializeStaticTensor(const AnyType &typeIn,
                                  AnyType &typeOut) {
    uint32 numberOfColumns = typeIn.GetNumberOfElements(0u);
    uint32 numberOfRows    = typeIn.GetNumberOfElements(1u);
    uint32 numberOfPages   = typeIn.GetNumberOfElements(2u);
    TypeDescriptor sourceDescriptor = typeIn.GetTypeDescriptor();
    void *sourcePointer = typeIn.GetDataPointer();

    bool ret = true;

    // allocate the memory block on destination
    uint32 memoryAllocationSize = 0u;
    bool isString   = (sourceDescriptor.type == SString);
    bool isCCString = (sourceDescriptor.type == BT_CCString);

    // the SString will be changed into a CCString
    if (isString) {
        memoryAllocationSize = static_cast<uint32>(sizeof(char8 *) * numberOfRows * numberOfColumns * numberOfPages);
    }
    else {
        memoryAllocationSize = typeIn.GetByteSize() * numberOfRows * numberOfColumns * numberOfPages;
    }

    void* destPointer = HeapManager::Malloc(memoryAllocationSize);
    typeOut = typeIn;
    typeOut.SetDataPointer(destPointer);

    if ((isString) || (isCCString)) {
        // matrix of pointers; copy element by element
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Serialisation of string AnyType with dimension > 2 not supported!");
        ret = false;
    }
    else {
        // copy all the block
        ret = MemoryOperationsHelper::Copy(destPointer, sourcePointer, memoryAllocationSize);
    }
    
    return ret;
}

static bool SerializeTensor(const AnyType &typeIn,
                            AnyType &typeOut) {
    bool ret = false;
    if (typeIn.IsStaticDeclared()) {
        ret = SerializeStaticTensor(typeIn, typeOut);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Serialisation of non-static AnyType with dimension > 2 not supported!");
        ret = false;
    }

    return ret;
}

static bool SerializeStaticMatrix(const AnyType &typeIn,
                                  AnyType &typeOut) {
    uint32 numberOfColumns = typeIn.GetNumberOfElements(0u);
    uint32 numberOfRows = typeIn.GetNumberOfElements(1u);
    TypeDescriptor sourceDescriptor = typeIn.GetTypeDescriptor();
    void *sourcePointer = typeIn.GetDataPointer();

    // allocate the memory block on destination
    uint32 memoryAllocationSize = 0u;
    bool isString = (sourceDescriptor.type == SString);
    bool isCCString = (sourceDescriptor.type == BT_CCString);

    // the SString will be changed into a CCString
    if (isString) {
        memoryAllocationSize = static_cast<uint32>(sizeof(char8 *) * numberOfRows * numberOfColumns);
    }
    else {
        memoryAllocationSize = typeIn.GetByteSize() * numberOfRows * numberOfColumns;
    }

    void* destPointer = HeapManager::Malloc(memoryAllocationSize);
    typeOut = typeIn;
    typeOut.SetDataPointer(destPointer);

    bool ret = true;

    if ((isString) || (isCCString)) {
        // matrix of pointers; copy element by element
        for (uint32 r = 0u; (r < numberOfRows) && (ret); r++) {
            for (uint32 c = 0u; (c < numberOfColumns) && (ret); c++) {
                uint32 index = (r * numberOfColumns) + c;
                const char8 *token = static_cast<const char8*>(NULL);
                if (isCCString) {
                    token = reinterpret_cast<const char8**>(sourcePointer)[index];
                }
                if (isString) {
                    token = (reinterpret_cast<StreamString*>(sourcePointer)[index]).Buffer();
                }
                char8 **destBegin = static_cast<char8 **>(destPointer);
                uint32 tokenSize = StringHelper::Length(token) + 1u;
                destBegin[index] = static_cast<char8*>(HeapManager::Malloc(tokenSize));
                ret = StringHelper::Copy(destBegin[index], token);
            }
        }
    }
    else {
        // copy all the block
        ret = MemoryOperationsHelper::Copy(destPointer, sourcePointer, memoryAllocationSize);
    }
    return ret;
}

/*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: Three pointer indirection levels required for matrices of char *. */
static bool SerializeHeapMatrix(const AnyType &typeIn,
                                AnyType &typeOut) {

    uint32 numberOfColumns = typeIn.GetNumberOfElements(0u);
    uint32 numberOfRows = typeIn.GetNumberOfElements(1u);
    TypeDescriptor sourceDescriptor = typeIn.GetTypeDescriptor();
    void **sourcePointer = static_cast<void **>(typeIn.GetDataPointer());

    // allocate space for pointers (one for each row)
    uint32 sizeRows = static_cast<uint32>(sizeof(void *) * numberOfRows);
    void **destPointer = static_cast<void **>(HeapManager::Malloc(sizeRows));

    typeOut = typeIn;
    typeOut.SetDataPointer(destPointer);

    bool ret = true;
    bool isString = (sourceDescriptor.type == SString);
    bool isCCString = (sourceDescriptor.type == BT_CCString);

    uint32 rowSize = 0u;

    // the string will be changed into a CCString
    if (isString) {
        rowSize = static_cast<uint32>(sizeof(char8*) * numberOfColumns);
    }
    else {
        rowSize = typeIn.GetByteSize() * numberOfColumns;
    }
    for (uint32 r = 0u; (r < numberOfRows) && (ret); r++) {
        destPointer[r] = HeapManager::Malloc(rowSize);
        if ((isString) || (isCCString)) {
            // matrix of pointers; copy element by element
            for (uint32 c = 0u; (c < numberOfColumns) && (ret); c++) {
                const char8 *token = static_cast<const char8 *>(NULL);
                if (isCCString) {
                    token = reinterpret_cast<const char8 ***>(sourcePointer)[r][c];
                }
                if (isString) {
                    token = (reinterpret_cast<StreamString**>(sourcePointer)[r][c]).Buffer();
                }
                char8 ***destBegin = reinterpret_cast<char8 ***>(destPointer);
                uint32 tokenSize = StringHelper::Length(token) + 1u;
                destBegin[r][c] = static_cast<char8*>(HeapManager::Malloc(tokenSize));
                ret = StringHelper::Copy(destBegin[r][c], token);
            }
        }
        else {
            // copy all the row block
            ret = MemoryOperationsHelper::Copy(destPointer[r], sourcePointer[r], rowSize);
        }
    }
    return ret;
}

/**
 * @brief Serialises a matrix AnyType (i.e. one with GetNumberOfElements(0) > 1 && GetNumberOfElements(1) > 1).
 * @return true if all the memory allocation and copy operations are successful.
 * @pre
 *   type.GetNumberOfElements(0) > 1 && type.GetNumberOfElements(1) > 1
 * @post
 *   type.GetDataPointer() != NULL
 */
static bool SerializeMatrix(const AnyType &typeIn,
                            AnyType &typeOut) {
    bool ret = false;
    if (typeIn.IsStaticDeclared()) {
        ret = SerializeStaticMatrix(typeIn, typeOut);
    }
    else {
        ret = SerializeHeapMatrix(typeIn, typeOut);
    }

    return ret;
}

/**
 * @brief Serialises a vector AnyType (i.e. one with GetNumberOfElements(0) > 1).
 * @return true if all the memory allocation and copy operations are successful.
 * @pre
 *   type.GetNumberOfElements(0) > 1
 * @post
 *   type.GetDataPointer() != NULL
 */
static bool SerializeVector(const AnyType &typeIn,
                            AnyType &typeOut) {

    uint32 numberOfElements = typeIn.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = typeIn.GetTypeDescriptor();
    void *sourcePointer = typeIn.GetDataPointer();

    // allocate the memory block on destination
    uint32 memoryAllocationSize = 0u;
    bool isString = (sourceDescriptor.type == SString);
    bool isCCString = (sourceDescriptor.type == BT_CCString);
    bool isCArray = (sourceDescriptor.type == CArray);
    bool isStaticDeclared = (typeIn.IsStaticDeclared());

    bool isCArrayOnHeap = ((isCArray) && (!isStaticDeclared));

    // the SString will be changed into a CCString
    if ((isString) || (isCArrayOnHeap)) {
        memoryAllocationSize = static_cast<uint32>(sizeof(char8 *) * numberOfElements);
    }
    else {
        memoryAllocationSize = typeIn.GetByteSize() * numberOfElements;
    }

    void* destPointer = HeapManager::Malloc(memoryAllocationSize);

    typeOut = typeIn;
    typeOut.SetDataPointer(destPointer);

    bool ret = true;
    if ((isString) || (isCCString) || (isCArrayOnHeap)) {
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
            const char8 *token = static_cast<const char8 *>(NULL);
            uint32 tokenLength = 0u;
            if (isCCString) {
                token = reinterpret_cast<const char8 **>(sourcePointer)[i];
                tokenLength = StringHelper::Length(token) + 1u;
            }
            if (isCArrayOnHeap) {
                token = reinterpret_cast<const char8 **>(sourcePointer)[i];
                tokenLength = typeIn.GetByteSize() * numberOfElements;
            }
            if (isString) {
                token = (reinterpret_cast<StreamString *>(sourcePointer)[i]).Buffer();
                tokenLength = StringHelper::Length(token) + 1u;
            }
            char8 **destBegin = reinterpret_cast<char8 **>(destPointer);
            destBegin[i] = static_cast<char8 *>(HeapManager::Malloc(tokenLength));
            ret = MemoryOperationsHelper::Copy(destBegin[i], token, tokenLength);
        }
    }
    else {
        // it works also for static matrix of characters!!
        ret = MemoryOperationsHelper::Copy(destPointer, sourcePointer, memoryAllocationSize);
    }

    return ret;

}

/**
 * @brief Serialises a scalar AnyType (i.e. one with GetNumberOfElements(0) == 1).
 * @return true if all the memory allocation and copy operations are successful.
 * @pre
 *   type.GetNumberOfElements(0) == 0
 * @post
 *   type.GetDataPointer() != NULL
 */
static bool SerializeScalar(const AnyType &typeIn,
                            AnyType &typeOut) {

    bool ret = true;

    TypeDescriptor sourceDescriptor = typeIn.GetTypeDescriptor();
    void* sourcePointer = typeIn.GetDataPointer();

    bool isString = (sourceDescriptor.type == SString);
    bool isCCString = (sourceDescriptor.type == BT_CCString);
    bool isCArray = (sourceDescriptor.type == CArray);
    bool isStaticDeclared = (typeIn.IsStaticDeclared());
    bool isCArrayOnHeap = ((isCArray) && (!isStaticDeclared));
    bool isPointer = (sourceDescriptor.type == Pointer);

    void* destPointer = static_cast<void *>(NULL);

    if ((isString) || (isCCString) || (isCArrayOnHeap)) {
        const char8 *token = static_cast<const char8 *>(NULL);
        if (isString) {
            token = reinterpret_cast<StreamString*>(sourcePointer)->Buffer();
        }
        if ((isCCString) || (isCArrayOnHeap)) {
            token = reinterpret_cast<const char8*>(sourcePointer);
        }
        uint32 tokenLength = StringHelper::Length(token) + 1u;
        destPointer = HeapManager::Malloc(tokenLength);
        ret = MemoryOperationsHelper::Copy(destPointer, token, tokenLength);
    }
    else if (isPointer) {
        destPointer = sourcePointer;
    }
    else {
        uint32 memoryAllocationSize = typeIn.GetByteSize();
        destPointer = HeapManager::Malloc(memoryAllocationSize);
        ret = MemoryOperationsHelper::Copy(destPointer, sourcePointer, memoryAllocationSize);
    }

    typeOut = typeIn;
    typeOut.SetDataPointer(destPointer);

    return ret;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

AnyObject::AnyObject() :
        Object() {
}

/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively managed by this class.". */
AnyObject::~AnyObject() {
    CleanUp();
}

bool AnyObject::Serialise(const AnyType &typeIn) {
    CleanUp();
    uint32 nOfDimensions = typeIn.GetNumberOfDimensions();
    bool ok = (type.GetDataPointer() == NULL_PTR(void *));
    if (ok) {
        if (nOfDimensions == 0u) {
            ok = SerializeScalar(typeIn, type);
        }
        else if (nOfDimensions == 1u) {
            ok = SerializeVector(typeIn, type);
        }
        else if (nOfDimensions == 2u) {
            ok = SerializeMatrix(typeIn, type);
        }
        else if (nOfDimensions == 3u) {
            ok = SerializeTensor(typeIn, type);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Serialisation of AnyType with dimension > 3 not supported!");
            ok = false;
        }

        //Strings are serialised as arrays of CCString. The type as to be updated accordingly.
        if (typeIn.GetTypeDescriptor().type == SString) {
            void *tmp = type.GetDataPointer();
            type = AnyType(static_cast<char8 *>(typeIn.GetDataPointer()));
            type.SetDataPointer(tmp);
            type.SetNumberOfDimensions(typeIn.GetNumberOfDimensions());
            type.SetNumberOfElements(0u, typeIn.GetNumberOfElements(0u));
            type.SetNumberOfElements(1u, typeIn.GetNumberOfElements(1u));
            type.SetNumberOfElements(2u, typeIn.GetNumberOfElements(2u));
        }
        type.SetStaticDeclared(typeIn.IsStaticDeclared());
    }

    return ok;
}

void AnyObject::CleanUp() {
    void *typePointer = type.GetDataPointer();
    bool cString = (type.GetTypeDescriptor().type == BT_CCString);
    bool sString = (type.GetTypeDescriptor().type == SString);
    bool cArray = (type.GetTypeDescriptor().type == CArray);
    bool staticDeclared = type.IsStaticDeclared();
    bool cArrayOnHeap = ((cArray) && (!staticDeclared));
    bool isPointer = (type.GetTypeDescriptor().type == Pointer);

    if (typePointer != NULL) {
        if (type.GetNumberOfDimensions() == 1u) {
            uint32 numberOfColumns = type.GetNumberOfElements(0u);
            if ((cString) || (sString) || (cArrayOnHeap)) {
                char8 **charArray = static_cast<char8 **>(typePointer);
                for (uint32 idx = 0u; idx < numberOfColumns; idx++) {
                    void *charMem = reinterpret_cast<void *>(charArray[idx]);
                    if (!HeapManager::Free(charMem)) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "HeapManager::Free failed. Vector memory not deallocated.");
                    }
                }
            }
        }
        if (type.GetNumberOfDimensions() == 2u) {

            uint32 numberOfColumns = type.GetNumberOfElements(0u);
            uint32 numberOfRows = type.GetNumberOfElements(1u);

            for (uint32 r = 0u; r < numberOfRows; r++) {
                for (uint32 c = 0u; c < numberOfColumns; c++) {
                    if (type.IsStaticDeclared()) {
                        if (cString || sString) {
                            uint32 idx = c + (r * numberOfColumns);
                            char8 **destStr = reinterpret_cast<char8 **>(typePointer);
                            void *charMem = reinterpret_cast<void *>(destStr[idx]);
                            if (!HeapManager::Free(charMem)) {
                                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "HeapManager::Free failed. AnyObject memory not deallocated.");
                            }
                        }
                    }
                    else {
                        if (cString || sString) {
                            /*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: Three pointer indirection levels required for matrices of char *. */
                            char8 ***destStr = reinterpret_cast<char8 ***>(typePointer);
                            char8 **charRow = destStr[r];
                            if (charRow != NULL) {
                                void *charMem = reinterpret_cast<void*>(charRow[c]);
                                if (!HeapManager::Free(charMem)) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "HeapManager::Free failed. AnyObject memory not deallocated.");
                                }
                            }
                        }
                    }
                }
                if (!staticDeclared) {
                    void **destStr = reinterpret_cast<void **>(typePointer);
                    void* charMem = reinterpret_cast<void *>(destStr[r]);
                    if (!HeapManager::Free(charMem)) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "HeapManager::Free failed. AnyObject memory not deallocated.");
                    }
                }
            }
        }
        if(!isPointer) {
            if (!HeapManager::Free(typePointer)) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "HeapManager::Free failed. AnyObject memory not deallocated.");
            }
        }
        type = voidAnyType;
    }
}

AnyType AnyObject::GetType() const {
    return type;
}

CLASS_REGISTER(AnyObject, "1.0")

}

