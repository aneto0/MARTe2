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
class EmbeddedThreadI {
public:
    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this thread.
     * @post
     *   GetThreadId() == InvalidThreadIdentifier &&
     *   GetThreadNumber == 0u &&
     *   GetCommands() == StopCommand
     */
    EmbeddedThreadI(EmbeddedServiceMethodBinderI &binder);

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
     * @brief Thread callback function which executes the internal state-machine and calls
     * the Execute method with the correct status information.
     * @details public access as it is called by the thread callback function.
     */
    virtual void ThreadLoop() = 0;

    /**
     * @brief Gets the thread unique number (with-in the context of a pool).
     * @return the thread unique number (with-in the context of a pool).
     */
    uint16 GetThreadNumber() const;

    /**
     * @brief Sets the thread unique number (with-in the context of a pool).
     * @param[in] numberIn the thread unique number (with-in the context of a pool).
     */
    void SetThreadNumber(uint16 threadNumberIn);

    /**
     * @brief Gets the embedded thread identifier.
     * @return the embedded thread identifier.
     */
    ThreadIdentifier GetThreadId() const;

    /**
     * @brief Spawns a new thread which will call the ThreadLoop method.
     * @pre
     *   !Threads::IsAlive(GetThreadId())
     */
    void LaunchThread();

    /**
     * @brief Gets the current command being executed in the ThreadLoop.
     * @return the current command.
     */
    Commands GetCommands() const;

    /**
     * @brief Sets the command to be executed in the ThreadLoop.
     * @param[in] commandsIn the command to be executed in the ThreadLoop.
     */
    void SetCommands(Commands commandsIn);

    /**
     * @brief Sets the thread identifier to InvalidThreadIdentifier.
     * @pre
     *   !Threads::IsAlive(GetThreadId())
     */
    void ResetThreadId();

    /**
     * @brief Callback function that is executed in the context of a thread spawned by this EmbeddedServiceI.
     * @details This function is a one-to-one mapping to the user-registered callback function (see EmbeddedServiceMethodBinderT).
     * This allows to call functions with any name and to call, on the same object instance, different functions from different threads.
     * @param[in] info information about the current state of the execution thread.
     * @return the ErrorType returned by the user function.
     */
    inline ErrorManagement::ErrorType Execute(ExecutionInfo information);

protected:

    /**
     * The registered call-back method to be called by this EmbeddedServiceI instance.
     */
    EmbeddedServiceMethodBinderI &method;

    /**
     * Embedded thread identifier.
     */
    ThreadIdentifier threadId;

    /**
     * The thread unique number (in the context of a pool)
     */
    uint16 threadNumber;

    /**
     * The command being executed in the ThreadLoop.
     */
    Commands commands;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ErrorManagement::ErrorType EmbeddedThreadI::Execute(ExecutionInfo information) {
    return method.Execute(information);
}

}
#endif /* EMBEDDEDTHREADI_H_ */

