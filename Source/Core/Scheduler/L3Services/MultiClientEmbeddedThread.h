/**
 * @file MultiClientEmbeddedThread.h
 * @brief Header file for class MultiClientEmbeddedThread
 * @date 05/09/2016
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

 * @details This header file contains the declaration of the class MultiClientEmbeddedThread
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTICLIENTSERVICETHREAD_H_
#define MULTICLIENTSERVICETHREAD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MultiClientService.h"
#include "EmbeddedThreadI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Multiple client connection oriented EmbeddedThreadI implementation.
 * @details The ThreadLoop only terminates when the GetCommands !== KeepRunningCommand or when
 *  the MultiClientService already has enough threads handling client connections (see
 *   MultiClientService::MoreThanEnoughThreads).

 * The user callback function should not block and should return ErrorManagement::Timeout
 * while waiting to serve. In this stage, it will be continuously called by the ThreadLoop with
 * ExecutionInfo::MainStage and ExecutionInfo::WaitRequestStageSpecific.
 *
 * When the user callback returns with no error it will be recalled with ExecutionInfo::ServiceRequestStageSpecific
 *  so to handle the connection request.
 * In parallel, a new thread will be launched by the manager to handle any new subsequent connection requests (see MultiClientService::AddThread)
 */
class MultiClientEmbeddedThread: public EmbeddedThreadI {

public:

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     * @param[in] managerIn the MultiClientService responsible for managing the number of threads.
     */
    MultiClientEmbeddedThread(EmbeddedServiceMethodBinderI &binder,
                              MultiClientService &managerIn);

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     * @param[in] managerIn the MultiClientService responsible for managing the number of threads.
     */
    template<typename className>
    MultiClientEmbeddedThread(EmbeddedServiceMethodBinderT<className> &binder,
                              MultiClientService &managerIn);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MultiClientEmbeddedThread();

    /**
     * @brief Continuously calls the user callback function.
     * @details The user callback function should not block and should return ErrorManagement::Timeout
     * while waiting to serve. In this stage, it will be continuously called by the ThreadLoop with
     * ExecutionInfo::MainStage and ExecutionInfo::WaitRequestStageSpecific.
     *
     * When the user callback returns with no error it will be recalled with ExecutionInfo::ServiceRequestStageSpecific
     *  so to handle the connection request.
     * In parallel, a new thread will be launched by the manager to handle any new subsequent connection requests (see MultiClientService::AddThread)
     */
    virtual void ThreadLoop();

private:
    /**
     * Responsible for managing the number of threads.
     */
    /*lint -e{1725} manager is a reference that is initialised during construction and is the basic mechanism to register
     * the callback MultiClientService that manages the creation and destruction of threads. */
    MultiClientService &manager;

    /*lint -e{1712} This class does not have a default constructor because
     * the MultiClientService manager must be defined at construction and will remain constant
     * during the object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
template<typename className>
MultiClientEmbeddedThread::MultiClientEmbeddedThread(EmbeddedServiceMethodBinderT<className> &binder,
                                                     MultiClientService &managerIn) :
        EmbeddedThreadI(binder),
        manager(managerIn) {
}

}
#endif /* MULTICLIENTSERVICETHREAD_H_ */

