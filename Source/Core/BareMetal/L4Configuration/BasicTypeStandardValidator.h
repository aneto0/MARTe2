/**
 * @file BasicTypeStandardValidator.h
 * @brief Header file for class BasicTypeStandardValidator
 * @date 11/08/2016
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

 * @details This header file contains the declaration of the class BasicTypeStandardValidator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTYPESTANDARDVALIDATOR_H_
#define BASICTYPESTANDARDVALIDATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ValidatorI.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Validates all the basic types on the base of the configuration data in input
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

 */
class BasicTypeStandardValidator: public ValidatorI {
public:
    CLASS_REGISTER_DECLARATION();
    /**
     * @brief Constructor
     */
    BasicTypeStandardValidator();

    /**
     * @brief Destructor
     */
    virtual ~BasicTypeStandardValidator();

    /**
     * @see Object::Initialise(*)
     * @details \a data can contain the following fields that will be used for the validation of a basic type value
     *   min        = ...    ---> the minimum value;\n
     *   max        = ...    ---> the maximum value;\n
     *   values     = {...}  ---> the list of possible values;\n
     *   min_size   = ...    ---> the minimum size;\n
     *   max_size   = ...    ---> the maximum size;
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Checks if the \a value in input respects all the constraints imposed by configuration.
     * @details If the \a value is multi-dimensional (i.e a matrix or a vector), the function returns true only if
     * the constraints are respected by each element.
     * @param[in] value is the element that has to be validate
     * @return true if \a value respects all the specified constraints, false otherwise.
     */
    virtual bool Validate(const AnyType &value);

private:

    /**
     * @brief Stores the parameters that will be used for the validation.
     */
    ConfigurationDatabase parameters;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTYPESTANDARDVALIDATOR_H_ */

