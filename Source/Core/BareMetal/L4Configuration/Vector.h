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
#include "String.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
class Vector: public AnyType {
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
    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
    dataPointer = NULL;
}

template<typename T>
Vector<T>::Vector(uint32 nOfElements) {
    dataPointer = new T[nOfElements];
    numberOfDimensions = 1u;
    numberOfElements[0] = nOfElements;
    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
Vector<T>::Vector(T *existingArray,
                  uint32 size) {
    dataPointer = existingArray;
    numberOfDimensions = 1u;
    numberOfElements[0] = size;
    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

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

