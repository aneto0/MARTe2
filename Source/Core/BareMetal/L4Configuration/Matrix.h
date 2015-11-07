/**
 * @file Matrix.h
 * @brief Header file for class Matrix
 * @date 05/11/2015
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

 * @details This header file contains the declaration of the class Matrix
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATRIX_H_
#define MATRIX_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * TODO
 */
class MatrixCore: public AnyType {
public:
    /**
     * TODO
     */
    MatrixCore() {
        dataPointer = NULL;
    }

    /**
     * TODO
     */
    virtual ~MatrixCore() {

    }

    /**
     * TODO
     */
    MatrixCore(const AnyType &sourceType) {
        dataPointer = NULL;
        dataDescriptor = sourceType.GetTypeDescriptor();
        numberOfDimensions = 2u;
        numberOfElements[0] = sourceType.GetNumberOfElements(0u);
        numberOfElements[1] = sourceType.GetNumberOfElements(1u);
        staticDeclared = sourceType.IsStaticDeclared();
    }

    /**
     * TODO
     */
    MatrixCore(void **existingArray,
               uint32 nOfRows,
               uint32 nOfColumns) {
        dataPointer = existingArray;
        numberOfDimensions = 2u;
        numberOfElements[0] = nOfColumns;
        numberOfElements[1] = nOfRows;
    }

    /**
     * TODO
     */
    void *GetElementAt(uint32 idx) {
        return (static_cast<char8 *>(dataPointer) + (idx * GetTypeDescriptor().numberOfBits / 8u));
    }

    /**
     * TODO
     */
    bool Serialize(void *&destination) {
        uint32 copySize = 0u;
        uint32 totalCopySize = 0u;
        bool ok = true;

        destination = HeapManager::Malloc(numberOfElements[0] * numberOfElements[1] * sizeof(void *));
        void **copyArray = static_cast<void **>(destination);

        uint32 r = 0u;
        uint32 c = 0u;
        uint32 idx = 0u;
        for (r = 0; ok && (r < numberOfElements[1]); r++) {
            for (c = 0; ok && (c < numberOfElements[0]); c++) {
                idx = c + r * numberOfElements[0];
                if (GetTypeDescriptor().type == StreamStringType) {
                    StreamString *stream = NULL;
                    if (IsStaticDeclared()) {
                        stream = static_cast<StreamString *>(dataPointer);
                        stream += idx;
                    }
                    else {
                        stream = &static_cast<StreamString **>(dataPointer)[r][c];
                    }
                    copySize = stream->Size() + 1u;
                }
                else if (GetTypeDescriptor().type == CCString) {
                    const char8 *srcArray = NULL_PTR(const char8 *);
                    if (IsStaticDeclared()) {
                        srcArray = static_cast<const char **>(dataPointer)[idx];
                    }
                    else {
                        srcArray = static_cast<char ***>(dataPointer)[r][c];
                    }
                    copySize = (StringHelper::Length(srcArray) + 1u);
                }
                else {
                    copySize = GetTypeDescriptor().numberOfBits / 8u;
                }

                ok = (copySize > 0u);

                if (ok) {
                    copyArray[idx] = HeapManager::Malloc(copySize);
                    const char *srcArray = NULL_PTR(const char *);
                    if (GetTypeDescriptor().type == StreamStringType) {
                        StreamString *stream = NULL;
                        if (IsStaticDeclared()) {
                            stream = static_cast<StreamString *>(dataPointer);
                            stream += idx;
                        }
                        else {
                            stream = &static_cast<StreamString **>(dataPointer)[r][c];
                        }
                        srcArray = stream->BufferReference();
                    }
                    else if (GetTypeDescriptor().type == CCString) {
                        if (IsStaticDeclared()) {
                            srcArray = static_cast<const char **>(dataPointer)[idx];
                        }
                        else {
                            srcArray = static_cast<char ***>(dataPointer)[r][c];
                        }
                    }
                    else {
                        srcArray = static_cast<char *>(dataPointer) + totalCopySize;
                    }
                    ok = MemoryOperationsHelper::Copy(copyArray[idx], srcArray, copySize);
                    totalCopySize += copySize;
                }
            }
        }

        return ok;
    }

    /**
     * TODO
     */
    bool Deserialize(const AnyType &sourceType) {
        uint32 copySize = 0u;
        uint32 totalCopySize = 0u;
        bool ok = true;
        void **sourceArray = static_cast<void **>(sourceType.GetDataPointer());

        uint32 r = 0u;
        uint32 c = 0u;
        uint32 idx = 0u;
        for (r = 0; ok && (r < numberOfElements[1]); r++) {
            for (c = 0; ok && (c < numberOfElements[0]); c++) {
                idx = c + r * numberOfElements[0];

                if ((sourceType.GetTypeDescriptor().type == StreamStringType) || (sourceType.GetTypeDescriptor().type == CCString)) {
                    const char *srcArray = static_cast<const char *>(sourceArray[idx]);
                    copySize = (StringHelper::Length(srcArray) + 1u);
                }
                else {
                    copySize = sourceType.GetTypeDescriptor().numberOfBits / 8u;
                }

                ok = (copySize > 0u);

                if (ok) {
                    if (sourceType.GetTypeDescriptor().type == StreamStringType) {
                        const char *srcArray = static_cast<const char *>(sourceArray[idx]);
                        if (GetTypeDescriptor().type == StreamStringType) {
                            StreamString *stream = NULL;
                            if (IsStaticDeclared()) {
                                stream = static_cast<StreamString *>(dataPointer);
                                stream += idx;
                            }
                            else {
                                stream = &static_cast<StreamString **>(dataPointer)[r][c];
                            }
                            stream->Write(srcArray, copySize);
                        }
                        else if (GetTypeDescriptor().type == CCString) {
                            char8 *destArray = NULL_PTR(char8 *);
                            if (IsStaticDeclared()) {
                                destArray = static_cast<char **>(dataPointer)[idx];
                            }
                            else {
                                destArray = static_cast<char ***>(dataPointer)[r][c];
                            }
                            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                        }
                        else {
                            StreamString sourceStreamString = srcArray;
                            //TODO
                            //ok = BasicTypeConvert(destination, sourceStreamString);
                        }
                    }
                    else if (sourceType.GetTypeDescriptor().type == CCString) {
                        const char *srcArray = static_cast<const char *>(sourceArray[idx]);
                        if (GetTypeDescriptor().type == CCString) {
                            char8 *destArray = NULL_PTR(char8 *);
                            if (IsStaticDeclared()) {
                                destArray = static_cast<char **>(dataPointer)[idx];
                            }
                            else {
                                destArray = static_cast<char ***>(dataPointer)[r][c];
                            }
                            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                        }
                        else if (GetTypeDescriptor().type == StreamStringType) {
                            StreamString *stream = NULL;
                            if (IsStaticDeclared()) {
                                stream = static_cast<StreamString *>(dataPointer);
                                stream += idx;
                            }
                            else {
                                stream = &static_cast<StreamString **>(dataPointer)[r][c];
                            }
                            stream->Write(srcArray, copySize);
                        }
                        else {
                            AnyType sourceCCString(srcArray);
                            //TODO
                            //                        ok = BasicTypeConvert(destination, sourceCCString);
                        }
                    }
                    else {
                        void *srcArray = sourceArray[idx];
                        void *destArray = static_cast<char *>(dataPointer) + totalCopySize;
                        if (sourceType.GetTypeDescriptor() == GetTypeDescriptor()) {
                            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                        }
                        else {
                            //AnyType sourceBasicType(source);
                            //sourceBasicType.SetDataPointer(srcArray);
                            //TODO
                            //ok = BasicTypeConvert(this?destinationBasicType, sourceBasicType);
                        }
                    }
                    totalCopySize += copySize;
                }
            }
        }
        return ok;
    }
};

/**
 * TODO
 */
template<typename T>
class Matrix: public MatrixCore {
public:
    /**
     * TODO
     */
    Matrix(uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * TODO
     */
    Matrix(T **existingArray,
           uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * TODO
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * TODO
     */
    Vector<T> operator [](uint32 element);

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
Matrix<T>::Matrix(uint32 nOfRows,
                  uint32 nOfColumns) {
    dataPointer = reinterpret_cast<T*>(new T*[nOfRows]);
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumns;
    numberOfElements[1] = nOfRows;

    T* dataPointerT = static_cast<T *>(dataPointer);
    T** rows = reinterpret_cast<T **>(&dataPointerT[0]);
    uint32 i;
    for (i = 0; i < nOfRows; i++) {
        rows[i] = new T[nOfColumns];
    }
    T typeDiscovery;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
Matrix<T>::Matrix(T **existingArray,
                  uint32 nOfRows,
                  uint32 nOfColumns) :
        MatrixCore(reinterpret_cast<void **>(existingArray), nOfRows, nOfColumns) {
    T typeDiscovery;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
Matrix<T>::Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]) {
    dataPointer = reinterpret_cast<T *>(&source);
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumnsStatic;
    numberOfElements[1] = nOfRowsStatic;
    staticDeclared = true;

    T typeDiscovery;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
Vector<T> Matrix<T>::operator [](uint32 element) {
    Vector<T> vec;
    T* dataPointerT = static_cast<T *>(dataPointer);

    if (!staticDeclared) {
        T** rows = reinterpret_cast<T **>(&dataPointerT[0]);
        vec = Vector<T>(reinterpret_cast<T *>(rows[element]), numberOfElements[0]);
    }
    else {
        vec = Vector<T>(reinterpret_cast<T *>(&dataPointerT[element * numberOfElements[0]]), numberOfElements[0]);
    }
    return vec;
}

}
#endif /*MATRIX_H_ */

