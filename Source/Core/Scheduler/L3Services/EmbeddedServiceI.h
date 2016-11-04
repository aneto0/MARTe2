/**
 * @file EmbeddedService.h
 * @brief Header file for class EmbeddedService
 * @date 01/09/2016
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

 * @details This header file contains the declaration of the class EmbeddedService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDSERVICE_H_
#define EMBEDDEDSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "EmbeddedThreadI.h"
#include "ErrorType.h"
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Family of objects that enable the interfacing of a class method to a thread or to a pool of threads.
 * @details The method callback interface is specified in EmbeddedServiceMethodBinderT<class>::MethodPointer.
 */
class EmbeddedServiceI: public Object {
public:
    /**
     * @brief Constructor. NOOP.
     */
    EmbeddedServiceI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedServiceI();

    /**
     * @brief Starts the embedded service (which will call the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the service can be successfully started.
     */
    virtual ErrorManagement::ErrorType Start() = 0;

    /**
     * @brief Stops the embedded service (which is calling the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the service can be successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop() = 0;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* EMBEDDEDSERVICE_H_ */

