/**
 * @file EmbeddedServiceMethodBinder.h
 * @brief Header file for class EmbeddedServiceMethodBinder
 * @date Sep 1, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class EmbeddedServiceMethodBinder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_EMBEDDEDSERVICEMETHODBINDER_H_
#define L4MESSAGES_EMBEDDEDSERVICEMETHODBINDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


#include "EmbeddedServiceI.h"


namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 */
class EmbeddedServiceMethodBinder{

public:
    /**
     * TODO
     */
    EmbeddedServiceMethodBinder(){}

    /**
     * TODO
     */
    virtual ~EmbeddedServiceMethodBinder(){}

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Call(EmbeddedServiceI::ExecutionInfo info)=0;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4MESSAGES_EMBEDDEDSERVICEMETHODBINDER_H_ */
	
