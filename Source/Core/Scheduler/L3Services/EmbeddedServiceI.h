/**
 * @file EmbeddedService.h
 * @brief Header file for class EmbeddedService
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
 * @brief Interface to a family of objects that allows interfacing a class method to a thread or to a pool of threads.
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

    /**
     * @brief List of possible states of an SingleServiceThread.
     */
    enum States {
        /**
         * No Status
         */
        NoneState,

        /**
         * No Thread running = (threadId = 0)
         */
        OffState,

        /**
         * Thread is starting
         */
        StartingState,

        /**
         * Thread timed-out while starting
         */
        TimeoutStartingState,

        /**
         * (threadId != 0)
         */
        RunningState,

        /**
         * Thread is stopping
         */
        StoppingState,

        /**
         * Thread timed-out while stopping
         */
        TimeoutStoppingState,

        /**
         * Thread is being killed
         */
        KillingState,

        /**
         * Thread timed-out while being killed
         */
        TimeoutKillingState
    };


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* EMBEDDEDSERVICE_H_ */

