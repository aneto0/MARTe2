/**
 * @file SingleThreadService.h
 * @brief Header file for class SingleThreadService
 * @date 22/08/2016
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

 * @details This header file contains the declaration of the class SingleThreadService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SINGLETHREADSERVICE_H_
#define SINGLETHREADSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceI.h"
#include "EmbeddedThread.h"
#include "ErrorType.h"
#include "Object.h"
#include "StructuredDataI.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Associates a thread to a class method.
 * @details This class allows associating a class method in the form (MARTe::ErrorManagement::ErrorType (*)(MARTe::EmbeddedServiceI::ExecutionInfo &)) to a thread context.
 * This method will be continuously called (see Start) with the stage encoded in the Information parameter. In particular this class allows to request for a "kind" Stop of the
 * embedded thread and, if this fails, for a direct killing of the thread.
 */
class SingleThreadService: public EmbeddedServiceI {

public:

    /**
     * @brief Constructor.
     * @param[in] binder contains the function to be executed by this SingleThreadService.
     * @post
     *   GetTimeout() == TTInfiniteWait &&
     *   GetStatus() == OffState
     */
    SingleThreadService(EmbeddedServiceMethodBinderI &binder);

    /**
     * @brief Constructor.
     * @param[in] binder contains the function to be executed by this SingleThreadService.
     * @post
     *   GetTimeout() == TTInfiniteWait &&
     *   GetStatus() == OffState
     */
    template<typename className>
    SingleThreadService(EmbeddedServiceMethodBinderT<className> &binder);

    /**
     * @brief Destructor.
     * @details Calls the Stop() method.
     */
    virtual ~SingleThreadService();


    /**
     * @brief Starts the SingleThreadService.
     * @return ErrorManagement::NoError if the thread can be successfully started.
     * @pre
     *   GetStatus() == OffState && GetThread().GetThreadId() == 0
     * @post
     *   GetStatus() != OffState && GetThread().GetThreadId() > 0
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Stops the SingleThreadService.
     * @details If the thread was not running this function does nothing.
     * If the thread is running, a stop command with timeout is issued (notice that if GetTimeout() == TTInfiniteWait) the Stop() command will block forever) .
     * If the thread was being stopped (Stop() had already been called), the thread is killed.
     * @return ErrorManagement::NoError if the thread can be successfully stopped.
     * @pre
     *   GetStatus() != OffState && GetThread().GetThreadId() != 0
     * @post
     *   GetStatus() == OffState && GetThread().GetThreadId() = 0
     */
    virtual ErrorManagement::ErrorType Stop();

    /**
     * @brief Sets the maximum time to execute a state change.
     * @param[in] msecTimeout the maximum time in milliseconds to execute a state change.
     */
    virtual void SetTimeout(const TimeoutType &msecTimeoutIn);

    /**
     * @brief Gets the Status of the EmbeddedThreadI.
     * @return the Status of the EmbeddedThreadI.
     */
    EmbeddedThreadI::States GetStatus();

    /**
     * @brief Sets the thread priority class.
     * @param[in] priorityClassIn the thread priority class.
     * @pre
     *   GetStatus() == OffState
     */
    virtual void SetPriorityClass(Threads::PriorityClassType priorityClassIn);

    /**
     * @brief Sets the thread priority level.
     * @param[in] priorityLevelIn the thread priority level.
     * @pre
     *   GetStatus() == OffState
     */
    virtual void SetPriorityLevel(uint8 priorityLevelIn);

    /**
     * @brief Sets the thread CPU mask (i.e. thread affinity).
     * @param[in] cpuMaskIn the thread CPU mask (i.e. thread affinity).
     * @pre
     *   GetStatus() == OffState
     */
    virtual void SetCPUMask(const ProcessorType& cpuMaskIn);

private:

    /**
     * The embedded thread.
     */
    EmbeddedThread embeddedThread;

    /*lint -e{1712} This class does not have a default constructor because
     * the callback method must be defined at construction time and will remain constant
     * during the object's lifetime*/
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename className>
SingleThreadService::SingleThreadService(EmbeddedServiceMethodBinderT<className> &binder) :
        EmbeddedServiceI(),
        embeddedThread(binder) {
    SetTimeout(TTInfiniteWait);
}

}
#endif /* SINGLETHREADSERVICE_H_ */

