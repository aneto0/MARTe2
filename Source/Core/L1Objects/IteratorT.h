/**
 * @file IteratorT.h
 * @brief Header file for class IteratorT
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

 * @details This header file contains the declaration of the class IteratorT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ITERATORT_H_
#define ITERATORT_H_

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
 * @brief Template iterator interface.
 */
template<typename T>
class IteratorT {
public:
    /**
     * @brief Destructor.
     */
    virtual ~IteratorT() {

    }
    /**
     * @brief The function performing the desired specific action on the input parameter.
     * @param[in] data is a generic template type type.
     */
    virtual void Do(T data)=0;

    /**
     * @brief A more specialized form of the IteratorT::Do function to be used on certain applications.
     * @param[in] data is a generic template type.
     * @param[in] mode is a flag used to specify special operations.
     * @return a SFTestType code.
     */
    virtual void Do2(T data,
                     SFTestType mode = SFTTNull) {
        Do(data);
    }
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ITERATORT_H_ */

