/**
 * @file TypeCharacteristics.h
 * @brief Header file for class TypeCharacteristics
 * @date 28/08/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class TypeCharacteristics
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECHARACTERISTICS_H_
#define TYPECHARACTERISTICS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

/**
 * @brief Provides information about the template type in input.
 */
template<typename T>
class TypeCharacteristics {
public:

    /**
     *  @brief Returns true if the type is signed, false otherwise.
     *  @return true if the type is signed, false otherwise.
     */
    static inline bool IsSigned();

    /**
     * @brief Returns the maximum possible value of the template type.
     * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
     */
    static inline const T MaxValue();

    /**
     * @brief Returns the minimum possible value of the template type.
     * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
     */
    static inline const T MinValue();


    static inline const T UsableBitSize();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool TypeCharacteristics<T>::IsSigned() {
    return ((static_cast<T>(-1)) < 0);
}

template<typename T>
const T TypeCharacteristics<T>::MaxValue() {
    if (IsSigned()) {
        // 0x7fff..f if signed
        T temp = 1 << (sizeof(T) * 8 - 1);
        return ~temp;
    }
    else {
        // 0xffff...f if unsigned
        T temp = 0;
        return ~temp;
    }
}

template<typename T>
const T TypeCharacteristics<T>::MinValue() {
    if (IsSigned()) {
        // 0x80...0 if signed
        T temp = 1 << (sizeof(T) * 8 - 1);
        return temp;
    }
    else {
        // 0 if unsigned
        T temp = 0;
        return temp;
    }
}

template<typename T>
const T TypeCharacteristics<T>::UsableBitSize() {
    if (IsSigned()) {
        // 0x80...0 if signed
        T temp = sizeof(T) * 8 - 1;
        return temp;
    }
    else {
        // 0 if unsigned
        T temp = sizeof(T) * 8 ;
        return temp;
    }
}


}

#endif /* TYPECHARACTERISTICS_H_ */

