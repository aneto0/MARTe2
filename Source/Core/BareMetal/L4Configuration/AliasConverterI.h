/**
 * @file AliasConverterI.h
 * @brief Header file for class AliasConverterI
 * @date 12/08/2016
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

 * @details This header file contains the declaration of the class AliasConverterI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ALIASCONVERTERI_H_
#define ALIASCONVERTERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "AnyObject.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class AliasConverterI: public ReferenceContainer {
public:

    /**
     * @brief Constructor
     */
    AliasConverterI();

    /**
     * @brief Destructor
     */
    virtual ~AliasConverterI();

    /**
     * @brief Performs the conversion from the source (always a string in this case) in input to the destination type
     * if it matches one of the element in the enum.
     * @param[in] source is the value which has to be converted
     * @param[in] destination is the converted value in output.
     * @return true if source has been matched in the enumeration, false otherwise.
     */
    virtual bool SourceToDestination(const AnyType &source, AnyObject &destination)=0;

    /**
     * @brief Performs the conversion from the destination value in input to the source (always a string in this case)
     * if it matches one of the element in the enum.
     * @param[in] destination is the value which has to match with one of the enum fields
     * @param[in] source is the name of the enum field which has matched with \a destination.
     * @return true if destination has been found in the enumeration, false otherwise.
     */
    virtual bool DestinationToSource(const AnyType &destination, AnyObject &source)=0;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ALIASCONVERTERI_H_ */

