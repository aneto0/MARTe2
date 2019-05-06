/**
 * @file HashFunction.h
 * @brief Header file for class HashFunction
 * @date 14 mar 2019
 * @author pc
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

 * @details This header file contains the declaration of the class HashFunction
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HASHFUNCTION_H_
#define HASHFUNCTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TemplateParametersVerificator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace HashFunction {

/* D. J. Bernstein hash function */
template<typename T>
uint32 DJB(T in) {
    uint8 *input = (uint8*) (&in);
    uint32 hash = 5381;
    if (input != NULL) {
        while (*input) {
            hash = 33 * hash ^ (unsigned char) *input++;
        }
    }
    return hash;
}

/* D. J. Bernstein hash function */
template<typename T>
uint32 DJB(T* in) {
    uint8 *input = (uint8*) (in);
    uint32 hash = 5381;
    if (input != NULL) {
        while (*input) {
            hash = 33 * hash ^ (unsigned char) *input++;
        }
    }
    return hash;
}

/* Fowler/Noll/Vo (FNV) hash function, variant 1a */
template<typename T>
uint32 FNV1A(T in) {

    uint8 *input = NULL;
    if(IsPointerType<T>::VALUE==1) {
        input=(uint8*)in;
    }
    else {
        input=(uint8*)(&in);
    }

    uint32 hash = 0x811c9dc5;
    if(input!=NULL) {
        while (*input) {
            hash ^= (unsigned char) *input++;
            hash *= 0x01000193;
        }
    }
    return hash;
}

/* Fowler/Noll/Vo (FNV) hash function, variant 1a */
template<typename T>
uint32 FNV1A(T* in) {

    uint8 *input = (uint8*) in;
    uint32 hash = 0x811c9dc5;
    if (input != NULL) {
        while (*input) {
            hash ^= (unsigned char) *input++;
            hash *= 0x01000193;
        }
    }
    return hash;
}

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_HASHFUNCTION_H_ */

