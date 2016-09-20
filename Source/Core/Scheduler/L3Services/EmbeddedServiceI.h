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

#include "BitRange.h"
#include "BitBoolean.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "ErrorType.h"
#include "ExecutionInfo.h"
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Interface to a family of objects that allows interfacing a class method to a thread or to a pool of threads.
 * @details The method interface is specified in EmbeddedServiceI::MethodBinderT<class>::MethodPointer.
 * It returns an ErrorType and has one parameter of type EmbeddedServiceI::ExecutionInfo&
 */
class EmbeddedServiceI: public Object {
public:

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this service.
     */
    EmbeddedServiceI(EmbeddedServiceMethodBinderI &binder);

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this service.
     */
    template<typename className>
    EmbeddedServiceI(EmbeddedServiceMethodBinderT<className> &binder);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedServiceI();

    /**
     * @brief Starts the embedded service (which will call the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the service can be successfully started.
     */
    virtual ErrorManagement::ErrorType Start()=0;

    /**
     * @brief Stops the embedded service (which is calling the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the service can be successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop()=0;

protected:

    /**
     * @brief Callback function that is executed in the context of a thread spawned by this EmbeddedServiceI.
     * @details This function is a one-to-one mapping to the user-registered callback function (see EmbeddedServiceMethodBinderT).
     * This allows to call functions with any name and to call, on the same object instance, different functions from different threads.
     * @param[in] info information about the current state of the execution thread.
     * @return the ErrorType returned by the user function.
     */
    inline ErrorManagement::ErrorType Execute(ExecutionInfo information);

    /**
     * The registered call-back method to be called by this EmbeddedServiceI instance.
     */
    EmbeddedServiceMethodBinderI &method;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename className>
EmbeddedServiceI::EmbeddedServiceI(EmbeddedServiceMethodBinderT<className> &binder) :
        Object(),
        method(binder) {
}

ErrorManagement::ErrorType EmbeddedServiceI::Execute(ExecutionInfo information) {
    return method.Execute(information);
}
}

#endif /* EMBEDDEDSERVICE_H_ */

