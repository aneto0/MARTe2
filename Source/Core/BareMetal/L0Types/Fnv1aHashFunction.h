/**
 * @file Fnv1aHashFunction.h
 * @brief Header file for class Fnv1aHashFunction
 * @date 19/08/2019
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class Fnv1aHashFunction
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L0TYPES_FNV1AHASHFUNCTION_H_
#define SOURCE_CORE_BAREMETAL_L0TYPES_FNV1AHASHFUNCTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HashFunction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief Fowler/Noll/Vo (FNV) hash function
 */
class Fnv1aHashFunction: public HashFunction {
public:

    /**
     * @brief Constructor
     */
    Fnv1aHashFunction();

    /**
     * @brief Destructor
     */
    virtual ~Fnv1aHashFunction();

    /**
     * @see HashFunction::Compute
     */
    virtual uint32 Compute(const char8 * const in, const uint32 size=0u);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_FNV1AHASHFUNCTION_H_ */

