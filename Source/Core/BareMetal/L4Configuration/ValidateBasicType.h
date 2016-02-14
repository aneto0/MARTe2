/**
 * @file ValidateBasicType.h
 * @brief Header file for class ValidateBasicType
 * @date 25/01/2016
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

 * @details This header file contains the declaration of the class ValidateBasicType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef VALIDATEBASICTYPE_H_
#define VALIDATEBASICTYPE_H_

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

/**
 * @brief Checks the \a value in input looking to the specified \a attributes.
 * @details
 * The string attributes must be written in the standard MARTe configuration language and provides a series
 * of attributes to be associated to a basic type member. Most common attributes are:
 *
 *   min        = ...    ---> the minimum value;\n
 *   max        = ...    ---> the maximum value;\n
 *   values     = {...}  ---> the list of possible values;\n
 *   min_size   = ...    ---> the minimum size;\n
 *   max_size   = ...    ---> the maximum size;
 *
 * @details
 * If the AnyType in input is multi-dimensional (i.e a matrix or a vector), the function returns true only if
 * the constraints are respected by each element.
 *
 * @return true if \a value respects the constraints specified by the \a attributes, false otherwise.
 */
bool ValidateBasicType(const AnyType &value,
                       const char8* const attributes);

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* VALIDATEBASICTYPE_H_ */

