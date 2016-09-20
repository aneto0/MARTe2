/**
 * @file EmbeddedThread.h
 * @brief Header file for class EmbeddedThread
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

#include "EmbeddedServiceI.h"
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
class EmbeddedThread: public EmbeddedServiceI {

public:

    /**
     * @brief List of possible states of an EmbeddedThread.
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

    /**
     * @brief Constructor.
     * @param[in] binder contains the function to be executed by this EmbeddedThread.
     * @post
     *   GetTimeout() == TTInfiniteWait &&
     *   GetThreadId() == 0 &&
     *   GetStatus() == OffState
     */
    EmbeddedThread(MethodBinderI &binder);

    /**
     * @brief Constructor.
     * @param[in] binder contains the function to be executed by this EmbeddedThread.
     * @post
     *   GetTimeout() == TTInfiniteWait &&
     *   GetThreadId() == 0 &&
     *   GetStatus() == OffState
     */
    template<typename className>
    EmbeddedThread(MethodBinderT<className> &binder);

    /**
     * @brief Destructor.
     * @details Calls the Stop() method.
     */
    virtual ~EmbeddedThread();

    /**
     * @brief Reads the Timeout from the data input.
     * @param[in] data shall contain a parameter with name "Timeout" holding the timeout in milliseconds.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Starts the EmbeddedThread.
     * @return ErrorManagement::NoError if the thread can be successfully started.
     * @pre
     *   GetStatus() == OffState && GetThreadId() == 0
     * @post
     *   GetStatus() != OffState && GetThreadId() > 0
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Stops the EmbeddedThread.
     * @details If the thread was not running this function does nothing.
     * If the thread is running, a stop command with timeout is issued (notice that if GetTimeout() == TTInfiniteWait) the Stop() command will block forever) .
     * If the thread was being stopped (Stop() had already been called), the thread is killed.
     * @return ErrorManagement::NoError if the thread can be successfully stopped.
     * @pre
     *   GetStatus() != OffState && GetThreadId() != 0
     * @post
     *   GetStatus() == OffState && GetThreadId() = 0
     */
    virtual ErrorManagement::ErrorType Stop();

    /**
     * @brief Gets the current thread status.
     * @return
     *  - OffState if the thread is not running
     *  - RunningState if the thread is being executed (i.e. calling the callback function in a loop)
     *  - StartingState if the thread is starting
     *  - TimeoutStartingState if the thread has timed-out while starting
     *  - StoppingState is the thread is stopping
     *  - TimeoutStoppingState if the thread has timed-out while stopping
     *  - KillingState if the thread is being killed
     *  - TimeoutKillingState if the thread has timed-out while being killed
     */
    EmbeddedThread::States GetStatus();

    /**
     * @brief Gets the embedded thread identifier.
     * @return the embedded thread identifier.
     */
    ThreadIdentifier GetThreadId();

    /**
     * @brief Sets the maximum time to execute a state change.
     * @param[in] msecTimeout the maximum time in milliseconds to execute a state change.
     */
    void SetTimeout(TimeoutType msecTimeoutIn);

    /**
     * @brief Gets the maximum time to execute a state change.
     * @return the maximum time to execute a state change.
     */
    TimeoutType GetTimeout() const;

    /**
     * @brief Thread callback function which executes the internal state-machine and calls
     * the Execute method with the correct status information.
     * @details public access as it is called by the thread callback function.
     */
    void ThreadLoop();

protected:
    /**
     * @brief List of possible commands to an EmbeddedThread
     */
    enum Commands {
        /**
         * Set by Start() at the start of thread life
         */
        StartCommand,
        /**
         * Set by the thread before entering loop
         */
        KeepRunningCommand,
        /**
         * Nice request to stop
         */
        StopCommand,

        /**
         * Stop called twice - performing async killing
         */
        KillCommand

    };
    /**
     * Embedded thread identifier.
     */
    ThreadIdentifier threadId;

    /**
     * Command to change the internal status of the thread state-machine.
     */
    Commands commands;

    /**
     * Information about the status of the thread being executed.
     */
    ExecutionInfo information;

    /**
     * Maximum absolute time to execute a state change.
     * maxCommandCompletionHRT = HighResolutionTimer::Counter32 + timeoutHRT
     */
    uint32 maxCommandCompletionHRT;

    /**
     * The timeout in high resolution counts.
     */
    int32 timeoutHRT;

    /**
     * The maximum time to execute a state change.
     */
    TimeoutType msecTimeout;



};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 */
template<typename className>
EmbeddedThread::EmbeddedThread(MethodBinderT<className> &binder) :
        EmbeddedServiceI(binder) {
    threadId = InvalidThreadIdentifier;
    commands = StopCommand;
    maxCommandCompletionHRT = 0;
    timeoutHRT = -1;
    information.Reset();
}

}
#endif /* L2MIXED_EMBEDDEDTHREAD_H_ */

