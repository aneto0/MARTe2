/**
 * @file Validate.h
 * @brief Header file for class Validate
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

 * @details This header file contains the declaration of the class Validate
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
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Parses \a attributes and calls the function ValidatorI::Validate(*) for each ValidatorI
 * found after calling its Initialise(*)
 * @param[in] value is the element to be validated.
 * @param[in] attributes is the configuration stream to be parsed which should contain declarations of ValidatorI to
 * be instantiated.
 */
DLL_API bool Validate(const AnyType &value,
                      const char8* const attributes);

/**
 * @brief Parses \a attributes and calls the function AliasConverter::SourceToDestination(*) for each AliasConverter
 * found after calling its Initialise(*). The member \a membername in \a data will be replaced by the destination
 * returned by AliasConverter::SourceToDestination(*).
 * @param[in] data is the structured data containing the element which has to be replaced.
 * @param[in] memberName is the name of the element which has to be replaced (in case of matching with an alias).
 * @param[in] attributes is the configuration stream to be parsed which should contain declarations of AliasConverter to
 * be instantiated.
 */
DLL_API bool Alias(StructuredDataI& data,
                   const char8* const memberName,
                   const char8* const attributes);

/**
 * @brief Parses \a attributes and calls the function AliasConverter::DestinationToSource(*) for each AliasConverter
 * found after calling its Initialise(*). The member \a membername in \a data will be replaced by the element
 * returned by AliasConverter::DestinationToSource(*).
 * @param[in] data is the structured data containing the element which has to be replaced.
 * @param[in] memberName is the name of the element which has to be replaced (in case of matching with an alias).
 * @param[in] attributes is the configuration stream to be parsed which should contain declarations of AliasConverter to
 * be instantiated.
 */
DLL_API bool InvertAlias(StructuredDataI& data,
                         const char8* const memberName,
                         const char8* const attributes);

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* VALIDATEBASICTYPE_H_ */

