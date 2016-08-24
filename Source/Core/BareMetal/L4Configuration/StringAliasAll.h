/**
 * @file StringAliasAll.h
 * @brief Header file for class StringAliasAll
 * @date 12/ago/2016
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

 * @details This header file contains the declaration of the class StringAliasAll
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGALIASALL_H_
#define STRINGALIASALL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AliasConverterI.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Converts a string to a type depending on the configuration provided in input.
 * @details In the configuration data an enumeration of string(source) = type(destination)
 * must be provided and this object is capable to convert a source to a destination and vice-versa
 * if it will be matched in the enumeration.
 * @details For instance, imagine to provide the following StructuredDataI to the Initialise(*)
 * function:
 *    A = 1
 *    B = 2
 *    C = 3
 *    E = 8
 * then passing "A" to SourceToDestination(*) function, it will return "1" in the destination and so on.
 * The function DestinationToSource(*) performs the opposite conversion (passing "1" it retrieves "A").
 */
namespace MARTe {
class StringAliasAll: public AliasConverterI {
public:
    CLASS_REGISTER_DECLARATION();

    /**
     * @brief Constructor
     */
    StringAliasAll();

    /**
     * @brief Destructor
     */
    virtual ~StringAliasAll();

    /**
     * @see Object::Initialise(*)
     * @details data has to be in the following format
     *    source (string) = destination (any uni or multidimensional type)
     *    ...
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @see AliasConverterI::SourceToDestination(*)
     * @details Performs the conversion from the source (always a string in this case) in input to the destination type
     * if it matches one of the element in the enum.
     */
    virtual bool SourceToDestination(const AnyType &source, AnyObject &destination);

    /**
     * @see AliasConverterI::DestinationToSource(*)
     * @details Performs the conversion from the destination value in input to the source (always a string in this case)
     * if it matches one of the element in the enum.
     */
    virtual bool DestinationToSource(const AnyType &destination, AnyObject &source);

private:

    /**
     * Stores the enumeration alias list.
     */
    ConfigurationDatabase parameters;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGALIASALL_H_ */

