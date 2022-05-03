/**
 * @file EmbeddedThreadI.h
 * @brief Header file for class EmbeddedThreadI
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

#ifndef EMBEDDEDTHREADI_H_
#define EMBEDDEDTHREADI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "FastPollingMutexSem.h"
#include "Object.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Thread wrapper interface.
 * @details Specialisations of this class implement the execution logic (i.e. what to do
 * on all the possible commands states) in the ThreadLoop function. Inside the ThreadLoop function
 * the Execute method is expected to be called with the correct stage information. Note that the
 * Execute method is not virtual for performance reasons.
 */
class EmbeddedThreadI: public Object {
public:
    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     * @post
     *   GetThreadId() == InvalidThreadIdentifier &&
     *   GetThreadNumber() == 0xFFFF &&
     *   GetCommands() == StopCommand &&
     *   GetPriorityClass() == Threads::NormalPriorityClass &&
     *   GetPriorityLevel() == 0 &&
     *   GetCPUMask() == UndefinedCPUs &&
     *   GetStackSize() == THREADS_DEFAULT_STACKSIZE
     */
    EmbeddedThreadI(EmbeddedServiceMethodBinderI &binder);

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     * @param[in] threadNumberIn a thread unique identifier which will be set in the ExecutionInfo (see ExecutionInfo::SetThreadNumber).
     * @post
     *   GetThreadId() == InvalidThreadIdentifier &&
     *   GetThreadNumber() == threadNumberIn &&
     *   GetCommands() == StopCommand &&
     *   GetPriorityClass() == Threads::NormalPriorityClass &&
     *   GetPriorityLevel() == 0 &&
     *   GetCPUMask() == UndefinedCPUs &&
     *   GetStackSize() == THREADS_DEFAULT_STACKSIZE
     */
    EmbeddedThreadI(EmbeddedServiceMethodBinderI &binder, uint16 threadNumberIn);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedThreadI();

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

    /**
     * @brief Thread callback function which executes the internal state-machine and calls
     * the Execute method with the correct status information.
     * @details public access as it is called by the thread callback function.
     */
    virtual void ThreadLoop() = 0;

    /**
     * @brief Gets the embedded thread identifier.
     * @return the embedded thread identifier.
     */
    ThreadIdentifier GetThreadId() const;

    /**
     * @brief Gets the embedded thread unique number.
     * @return the embedded thread unique number.
     */
    uint16 GetThreadNumber() const;

    /**
     * @brief Gets the current command being executed in the ThreadLoop.
     * @return the current command.
     */
    Commands GetCommands() const;

    /**
     * @brief Sets the command to be executed in the ThreadLoop.
     * @param[in] commandsIn the command to be executed in the ThreadLoop.
     */
    void SetCommands(const Commands commandsIn);

    /**
     * @brief Sets the thread identifier to InvalidThreadIdentifier.
     * @pre
     *   not Threads::IsAlive(GetThreadId())
     */
    void ResetThreadId();

    /**
     * @brief Callback function that is executed in the context of a thread spawned by this EmbeddedServiceI.
     * @details This function is a one-to-one mapping to the user-registered callback function (see EmbeddedServiceMethodBinderT).
     * This allows to call functions with any name and to call, on the same object instance, different functions from different threads.
     * @param[in] information information about the current state of the execution thread.
     * @return the ErrorType returned by the user function.
     */
    inline ErrorManagement::ErrorType Execute(ExecutionInfo & information);

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
    States GetStatus();

    /**
     * @brief Sets the maximum time to execute a state change.
     * @param[in] msecTimeoutIn the maximum time in milliseconds to execute a state change.
     */
    void SetTimeout(const TimeoutType &msecTimeoutIn);

    /**
     * @brief Gets the maximum time to execute a state change.
     * @return the maximum time to execute a state change.
     */
    TimeoutType GetTimeout() const;

    /**
     * @brief Starts the embedded thread (which will call the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the thread can be successfully started.
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Stops the embedded thread (which is calling the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the thread can be successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop();

    /**
     * @brief Gets the thread priority class.
     * @return the thread priority class.
     */
    Threads::PriorityClassType GetPriorityClass() const;

    /**
     * @brief Sets the thread priority class.
     * @param[in] priorityClassIn the thread priority class.
     * @pre
     *   GetStatus() == OffState
     */
    void SetPriorityClass(Threads::PriorityClassType priorityClassIn);

    /**
     * @brief Gets the thread priority level.
     * @return the thread priority level.
     */
    uint8 GetPriorityLevel() const;

    /**
     * @brief Sets the thread priority level.
     * @param[in] priorityLevelIn the thread priority level.
     * @pre
     *   GetStatus() == OffState
     */
    void SetPriorityLevel(uint8 priorityLevelIn);

    /**
     * @brief Gets the thread stack size.
     * @return the thread stack size.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Sets the thread stack size.
     * @param[in] stackSizeIn the stack size.
     * @pre
     *   GetStatus() == OffState
     */
    void SetStackSize(uint32 stackSizeIn);

    /**
     * @brief Gets the thread CPU mask (i.e. thread affinity).
     * @return the thread CPU mask.
     */
    ProcessorType GetCPUMask() const;

    /**
     * @brief Sets the thread CPU mask (i.e. thread affinity).
     * @param[in] cpuMaskIn the thread CPU mask (i.e. thread affinity).
     * @pre
     *   GetStatus() == OffState
     */
    void SetCPUMask(const ProcessorType& cpuMaskIn);

    /**
     * @brief Sets a unique number to define the thread in the context of the Service.
     * @param[in] threadNumberIn the thread number to set.
     */
    void SetThreadNumber(uint16 threadNumberIn);

protected:
    /**
     * Embedded thread identifier.
     */
    ThreadIdentifier threadId;

    /**
     * The thread unique number (in the context of a pool)
     */
    uint16 threadNumber;

    /**
     * Mutex semaphore to avoid racing condition between the start of thread and its premature stopping.
     */
    FastPollingMutexSem mux; 

private:

    /**
     * The registered call-back method to be called by this EmbeddedServiceI instance.
     */
    /*lint -e{1725} method is a reference that is initialised during construction and is the basic mechanism to register
     * the callback function to be called by this EmbeddedThreadI. */
    EmbeddedServiceMethodBinderI &method;

    /**
     * The command being executed in the ThreadLoop.
     */
    Commands commands;

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

    /**
     * The thread priority class.
     */
    Threads::PriorityClassType priorityClass;

    /**
     * The thread priority level.
     */
    uint8 priorityLevel;

    /**
     * The thread stack size
     */
    uint32 stackSize;

    /**
     * The thread CPU mask
     */
    ProcessorType cpuMask;

    /*lint -e{1712} This class does not have a default constructor because
     * the callback method must be defined at construction and will remain constant
     * during the object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ErrorManagement::ErrorType EmbeddedThreadI::Execute(ExecutionInfo & information) {
    return method.Execute(information);
}

}
#endif /* EMBEDDEDTHREADI_H_ */

