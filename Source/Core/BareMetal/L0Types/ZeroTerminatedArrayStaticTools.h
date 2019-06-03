/**
 * @file ZeroTerminatedArrayTools.h
 * @brief Header file for class AnyType
 * @date 12 Dec 2018
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class ZeroTerminatedArrayTools
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ZEROTERMINATEDARRAYTOOLS_H_
#define ZEROTERMINATEDARRAYTOOLS_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief common functions to handle ZTAs
 */
class ZeroTerminatedArrayStaticTools{
public:
    /**
     * @brief calculate the number of elements of any ZeroTermArray whose elements have size elSize
     * @param[in] pointer is the pointer to the data array
     * @param[in] elSize is the size of each element in the array
     * @return the number of elements until one with all zeroes is found
     */
    static uint32 ZTAGetSize(const uint8 *pointer, uint32 elSize);

    /**
     * @brief checks if an element at a given position is all zeroes
     * @param[in] pointer is the pointer to the data array
     * @param[in] elSize is the size of each element in the array
     * @return true if all elements are zero
     */
    static bool ZTAIsZero(const uint8 *pointer, uint32 elSize);


    /**
     * @brief Sets an element at a given location to zero
     * @param[in] pointer is the pointer to the data array
     * @param[in] elSize is the size of each element in the array
     */
    static void ZTAZero(uint8 *pointer, uint32 elSize);

    /**
     * @brief searches for an element
     * @param[in] pointer is the pointer to the data array
     * @param[in] data is the pointer to the searched element
     * @param[in] elSize is the size of each element in the array
     * @return the location of the element or 0xFFFFFFFF if not found
     */
    static uint32 ZTAFind(const uint8 *pointer,const uint8 *data, uint32 elSize);

    /**
     * @brief compares elements
     * @param[in] pointer is the pointer to the data array
     * @param[in] data is the pointer to the searched element
     * @param[in] elSize is the size of each element in the array
     * @return true if have same memory
     */
    static bool ZTAIsSame(const uint8 *pointer,const uint8 *data, uint32 elSize);

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ZEROTERMINATEDARRAYTOOLS_H_ */
