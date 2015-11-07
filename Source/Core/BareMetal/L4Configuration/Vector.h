/**
 * @file Vector.h
 * @brief Header file for class Vector
 * @date 04/11/2015
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

 * @details This header file contains the declaration of the class Vector
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef VECTOR_H_
#define VECTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "HeapManager.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * TODO
 */
class VectorCore: public AnyType {
public:
    /**
     * TODO
     */
    VectorCore() {
        dataPointer = NULL;
    }

    /**
     * TODO
     */
    virtual ~VectorCore(){

    }

    /**
     * TODO
     */
    VectorCore(const AnyType &sourceType) {
        dataPointer = NULL;
        dataDescriptor = sourceType.GetTypeDescriptor();
        numberOfDimensions = 1u;
        numberOfElements[0] = sourceType.GetNumberOfElements(0u);
    }

    /**
     * TODO
     */
    VectorCore(void *existingArray,
               uint32 size) {
        dataPointer = existingArray;
        numberOfDimensions = 1u;
        numberOfElements[0] = size;
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
        uint32 idx = 0u;
        bool ok = true;

        destination = HeapManager::Malloc(numberOfElements[0] * sizeof(void *));
        void **copyArray = static_cast<void **>(destination);

        for (idx = 0; ok && (idx < numberOfElements[0]); idx++) {
            if (GetTypeDescriptor().type == StreamStringType) {
                StreamString *stream = NULL_PTR(StreamString *);
                //Assume it is zero terminated...
                stream = static_cast<StreamString *>(dataPointer);
                stream += idx;

                copySize = stream->Size() + 1u;
            }
            else if (GetTypeDescriptor().type == CCString) {
                const char8 *srcArray = NULL_PTR(const char8 *);
                srcArray = static_cast<const char *>(dataPointer);
                srcArray += (idx * sizeof(char *));
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
                    StreamString *stream = NULL_PTR(StreamString *);
                    stream = static_cast<StreamString *>(dataPointer);
                    stream += idx;
                    srcArray = stream->BufferReference();
                }
                else if (GetTypeDescriptor().type == CCString) {
                    srcArray = static_cast<const char *>(dataPointer);
                    srcArray += (idx * sizeof(char *));
                }
                else {
                    srcArray = static_cast<char *>(dataPointer) + totalCopySize;
                }
                ok = MemoryOperationsHelper::Copy(copyArray[idx], srcArray, copySize);
                totalCopySize += copySize;
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
        uint32 idx = 0u;
        bool ok = true;
        void **sourceArray = static_cast<void **>(sourceType.GetDataPointer());

        for (idx = 0; ok && (idx < numberOfElements[0]); idx++) {
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
                        StreamString *streamDest = static_cast<StreamString *>(GetDataPointer());
                        streamDest += idx;
                        ok = streamDest->Write(srcArray, copySize);
                    }
                    else if (GetTypeDescriptor().type == CCString) {
                        //If the destination size is not sufficient this is going to have an ugly end...
                        char *destArray = static_cast<char *>(GetDataPointer());
                        destArray += (idx * sizeof(char *));
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
                        //If the destination size is not sufficient this is going to have an ugly end...
                        char *destArray = static_cast<char *>(GetDataPointer());
                        destArray += (idx * sizeof(char *));
                        ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                    }
                    else if (GetTypeDescriptor().type == StreamStringType) {
                        StreamString *streamDest = static_cast<StreamString *>(GetDataPointer());
                        streamDest += idx;
                        ok = streamDest->Write(srcArray, copySize);
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

        return ok;
    }
};

template<typename T>
class Vector: public VectorCore {
public:
    /**
     * TODO
     */
    Vector();

    /**
     * TODO
     */
    Vector(uint32 nOfElements);

    /**
     * TODO
     */
    Vector(T *existingArray,
           uint32 size);

    /**
     * TODO
     */
    //Vector(const Vector &toCopy);
    /**
     * TODO
     */
    template<uint32 nOfElementsStatic>
    Vector(T (&source)[nOfElementsStatic]);

    /**
     * TODO
     */
    T &operator [](uint32 element);

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
Vector<T>::Vector() {
    T typeDiscovery;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
    dataPointer = NULL;
}

template<typename T>
Vector<T>::Vector(uint32 nOfElements) {
    dataPointer = new T[nOfElements];
    numberOfDimensions = 1u;
    numberOfElements[0] = nOfElements;
    T typeDiscovery;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
Vector<T>::Vector(T *existingArray,
                  uint32 size) :
        VectorCore(existingArray, size) {
    T typeDiscovery;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

/*template<typename T>
 Vector<T>::Vector(const Vector &toCopy) {
 dataPointer = toCopy.value;
 numberOfDimensions = toCopy.numberOfDimensions;
 numberOfElements[0] = toCopy.numberOfElements[0];
 dataDescriptor = toCopy.dataDescriptor;

 SetDataPointer (value);
 }*/

template<typename T>
template<uint32 nOfElementsStatic>
Vector<T>::Vector(T (&source)[nOfElementsStatic]) {
    dataPointer = reinterpret_cast<T *>(&source[0]);
    numberOfDimensions = 1u;
    numberOfElements[0] = nOfElementsStatic;
    staticDeclared = true;

    T typeDiscovery = 0;
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
T &Vector<T>::operator [](uint32 element) {
    T* t = reinterpret_cast<T *>(dataPointer);
    return t[element];
}

}

#endif /* VECTOR_H_ */

