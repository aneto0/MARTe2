/**
 * @file HashFunction.h
 * @brief Header file for class HashFunction
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

 * @details This header file contains the declaration of the class HashFunction
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L0TYPES_HASHFUNCTION_H_
#define SOURCE_CORE_BAREMETAL_L0TYPES_HASHFUNCTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "TemplateParametersVerificator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Interface for hash function objects.
 * @details Implementors of this interface know how to compute an hash from a string ID.
 */
class DLL_API HashFunction {
public:

    /**
     * @brief Constructor
     */
    HashFunction();

    /**
     * @brief Destructor
     */
    virtual ~HashFunction();

    /**
     * @brief Computes the hash function generating a key from a string id.
     * @param[in] in the input string
     * @param[in] size the size of the input (if 0 is the string length)
     * @return the generated key.
     */
    virtual uint32 Compute(const char8 * const in, const uint32 size = 0u) = 0;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_HASHFUNCTION_H_ */

