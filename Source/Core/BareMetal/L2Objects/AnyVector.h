/**
 * @file AnyVector.h
 * @brief Header file for class AnyVector
 * @date 28/10/2015
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

 * @details This header file contains the declaration of the class AnyVector
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ANYVECTOR_H_
#define ANYVECTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class AnyVector: public AnyType {
public:
    template<typename T, uint32 size>
    AnyVector(T(&source)[size]);

    template<typename T>
    void Load(T *source, uint32 nOfElements);

    template<typename T>
    void Load(const T *source, uint32 nOfElements);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template<typename T, uint32 size>
AnyVector::AnyVector(T(&source)[size]) {
    dataPointer = static_cast<void *>(&source[0]);
    printf("dataPointer = %p\n", dataPointer);
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    numberOfDimensions = 1u;
    if(size > 0u){
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = size;
        printf("numberOfElements = %u\n", numberOfElements[0]);
    }
}

template<typename T>
void AnyVector::Load(T *source, uint32 nOfElements) {
    dataPointer = static_cast<void *>(source);
    numberOfElements[0] = nOfElements;
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    numberOfDimensions = 1u;
    if(nOfElements > 0u){
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
    }
}

template<typename T>
void AnyVector::Load(const T *source, uint32 nOfElements) {
    dataPointer = static_cast<const void *>(source);
    numberOfElements = nOfElements;
    bitAddress = 0u;
    dataDescriptor.isConstant = true;
    numberOfDimensions = 1u;
    if(nOfElements > 0u){
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
    }
}


}
#endif /* ANYVECTOR_H_ */

