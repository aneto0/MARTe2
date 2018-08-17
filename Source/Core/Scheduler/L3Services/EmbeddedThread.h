/**
 * @file EmbeddedThread.h
 * @brief Header file for class EmbeddedThread
 * @date 21/09/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class EmbeddedThread
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDTHREAD_H_
#define EMBEDDEDTHREAD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedThreadI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief EmbeddedThreadI implementation that calls Execute while GetCommands == KeepRunningCommand.
 * @details The ThreadLoop only terminates when the GetCommands != KeepRunningCommand.
 * The user call-back function should not block. It will be continuously called by the
 * ThreadLoop with ExecutionInfo::MainStage set.
 */
class EmbeddedThread: public EmbeddedThreadI {
public:
    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     */
    EmbeddedThread(EmbeddedServiceMethodBinderI &binder);

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     * @param[in] threadNumberIn a thread unique identifier which will be set in the ExecutionInfo (see ExecutionInfo::SetThreadNumber).
     */
    EmbeddedThread(EmbeddedServiceMethodBinderI &binder, uint16 threadNumberIn);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedThread();

    /**
     * @brief Continuously calls the user callback function.
     * @details Only terminates when the GetCommands != KeepRunningCommand.
     * The user call-back function should not block. It will be continuously called by the
     * ThreadLoop with ExecutionInfo::MainStage set.
     * The first call to the user-function will have ExecutionInfo::StartupStage set.
     */
    virtual void ThreadLoop();

private:
    /**
     * Information about the status of the thread being executed.
     */
    ExecutionInfo information;


    /*lint -e{1712} This class does not have a default constructor because
     * the callback method must be defined at construction time and will remain constant
     * during the object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EMBEDDEDTHREAD_H_ */

