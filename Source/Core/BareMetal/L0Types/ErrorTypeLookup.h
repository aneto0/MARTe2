/**
 * @file ErrorTypeLookup.h
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

 * @details This header file contains the declaration of the class ErrorTypeLookup
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ERRORTYPELOOKUP_H_
#define ERRORTYPELOOKUP_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * To generate a field in the table of error lookup
 * depends from the ERROR_CONSTANT_MACRO(GENERATE_ERROR_CONSTANTS)
 */
#define GENERATE_ERROR_LOOKUP(ErrorName,errorName,bit)    \
        { #ErrorName, ErrorName ## _bit},


namespace ErrorManagement {

/**
 * Allow converting errors to strings
 */
struct ErrorTypeLookup{
	/**
	 * The name of the error field
	 */
    CCString 			name;

    /**
     * The corresponding bit
     */
    ErrorIntegerFormat 	errorBitSet;
};

extern DLL_API ErrorTypeLookup errorTypeLookup[];


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ERRORTYPELOOKUP_H_ */
