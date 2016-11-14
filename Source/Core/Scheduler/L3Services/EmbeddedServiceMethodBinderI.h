/**
 * @file EmbeddedServiceMethodBinderI.h
 * @brief Header file for class EmbeddedServiceMethodBinderI
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

 * @details This header file contains the declaration of the class EmbeddedServiceMethodBinderI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDSERVICEMETHODBINDERI_H_
#define EMBEDDEDSERVICEMETHODBINDERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ErrorType.h"
#include "ExecutionInfo.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Only to be used by the EmbeddedServiceMethodBinderT. Allows to register
 * a user specific callback function to be executed in the context of an EmbeddedServiceI.
 */
class DLL_API EmbeddedServiceMethodBinderI {

public:
    /**
     * @brief Default constructor. NOOP.
     */
    EmbeddedServiceMethodBinderI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedServiceMethodBinderI();

    /**
     * @brief Callback function that is executed in the context of a thread spawned by the EmbeddedServiceI.
     * @details This function is a one-to-one mapping to the user-registered callback function (see EmbeddedServiceMethodBinderT).
     * This allows to call functions with any name and to call, on the same object instance, different functions from different threads.
     * @param[in] info information about the current state of the execution thread.
     * @return the ErrorType returned by the user function.
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info) = 0;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EMBEDDEDSERVICEMETHODBINDERI_H_ */

