/**
 * @file EmbeddedServiceMethodBinderT.h
 * @brief Header file for class EmbeddedServiceMethodBinderT
 * @date 20/09/2016
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

 * @details This header file contains the declaration of the class EmbeddedServiceMethodBinderT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDSERVICEMETHODBINDERT_H_
#define EMBEDDEDSERVICEMETHODBINDERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*-------------------------------------------* --------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Allows to register a user specific callback method to be executed in the context of an EmbeddedServiceI.
 */
template<typename className>
class EmbeddedServiceMethodBinderT: public EmbeddedServiceMethodBinderI {

public:

    /**
     * @brief Type definition for the callback method pointer prototype.
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(const ExecutionInfo &info);

    /**
     * @brief Constructor.
     * @param[in] obj the object with the callback method to be called.
     * @param[in] fun the callback method to be called.
     */
    EmbeddedServiceMethodBinderT(className &obj,
                                 MethodPointer fun);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedServiceMethodBinderT();

    /**
     * @brief see EmbeddedServiceMethodBinderI::Execute.
     * @details Calls the user registered callback function.
     * @param[in] info information about the current state of the execution thread.
     * @return the ErrorType returned by the user function.
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo &info);

private:

    /**
     * The object with the callback method to be called.
     */
    /*lint -e{1725} object is a reference that is initialised during construction and is the basic mechanism to register
     * the callback function to be called by the EmbeddedThreadI. */
    className &object;

    /**
     * The callback method to be called.
     */
    MethodPointer function;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename className>
EmbeddedServiceMethodBinderT<className>::EmbeddedServiceMethodBinderT(className &obj,
                                                                       const typename EmbeddedServiceMethodBinderT<className>::MethodPointer fun) :
        EmbeddedServiceMethodBinderI(),
        object(obj),
        function(fun) {
}

template<typename className>
EmbeddedServiceMethodBinderT<className>::~EmbeddedServiceMethodBinderT() {
}

template<typename className>
ErrorManagement::ErrorType EmbeddedServiceMethodBinderT<className>::Execute(const ExecutionInfo &info) {
    return (object.*function)(info);
}
}

#endif /* EMBEDDEDSERVICEMETHODBINDERT_H_ */

