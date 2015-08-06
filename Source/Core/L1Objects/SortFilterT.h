/**
 * @file SortFilterT.h
 * @brief Header file for class SortFilterT
 * @date 06/ago/2015
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

 * @details This header file contains the declaration of the class SortFilterT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SORTFILTERT_H_
#define SORTFILTERT_H_

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

/**
 * @brief Template sort filter object interface.
 */
template<typename T>
class SortFilterT {
public:

    /**
     * @brief Destructor.
     */
    virtual ~SortFilterT() {

    }
    /**
     * @brief A function that can be used to compare two object.
     * @details The correct implementation should be to return a positive value if data1 is not ordered with data2, negative otherwise.
     * @param[in] data1 is the first object.
     * @param[in] data2 is the second object.
     * @return a positive value if data1 is not ordered with data2, negative otherwise.
     */
    virtual int32 Compare(T data1,
                          T data2)=0;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SORTFILTERT_H_ */

