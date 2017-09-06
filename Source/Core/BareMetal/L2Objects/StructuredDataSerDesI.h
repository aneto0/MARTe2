/**
 * @file StructuredDataSerDesI.h
 * @brief Header file for class StructuredDataSerDesI
 * @date 05/09/2017
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

 * @details This header file contains the declaration of the class StructuredDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRUCTURED_DATA_SERDES_I_H_
#define STRUCTURED_DATA_SERDES_I_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


#include "StructuredDataI.h"
#include "StreamI.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**

 */
/*lint -e{9109} forward declaration of this class is required in other modules*/
class DLL_API StructuredDataSerDesI {
public:

    /**
     * @brief Destructor.
     */
    virtual ~StructuredDataSerDesI();

    virtual ErrorManagement::ErrorType ConvertToStream(const StructuredDataI &input, StreamI &stream)=0;

    virtual ErrorManagement::ErrorType ParseStream(StructuredDataI &output, StreamI &stream)=0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/




#endif /* CONFIGURATION_DATABASE_H_ */
