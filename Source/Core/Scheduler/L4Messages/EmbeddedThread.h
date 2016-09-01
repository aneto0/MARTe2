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

#ifndef L2MIXED_EMBEDDEDTHREADOBJECT_H_
#define L2MIXED_EMBEDDEDTHREADOBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceI.h"
#include "ErrorType.h"
#include "Threads.h"
#include "StructuredDataI.h"
namespace MARTe{


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



/**
 * TODO
 * a container for a thread
 */
class EmbeddedThread: public EmbeddedServiceI {

public:

    /**
     * @brief States of an EmbeddedThread

     */
    enum States {
        /**
         * No Status
         */
        NoneState,

        /**
         * No Thread running =  (threadId = 0)
         */
        OffState,

        /**
         *TODO
         */
        StartingState,

        /**
         * TODO
         */
        TimeoutStartingState,

        /**
         * (threadId != 0) && TODO
         */
        RunningState,

        /**
         * TODO
         */
        StoppingState,

        /**
         * TODO
         */
        TimeoutStoppingState,

        /**
         * TODO
         */
        KillingState,

        /**
         * TODO
         */
        TimeoutKillingState

    };

    /**
     * @brief States of an EmbeddedThread

     */
    enum Commands {
        /**
         * set by Start() at the start of thread life
         */
        StartCommand,
        /**
         * set by thread before entering loop
         */
        KeepRunningCommand,

        /**
         * nice request to stop
         */
        StopCommand,

        /**
         * Stop called twice - performing async killing
         */
        KillCommand

    };

    /**
     * TODO
     */
    EmbeddedThread();

    /**
     * TODO
     */
    virtual ~EmbeddedThread();

    /**
    * TODO
    * same as object interface
    * implementation of EmbeddedServiceI
    */
    virtual ErrorManagement::ErrorType  Initialise(StructuredDataI &data);

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Stop();

    /**
     * TODO
     */
    EmbeddedThread::States GetStatus();

    /**
     * TODO
     */
    inline ThreadIdentifier Id();

    /**
     * TODO
     */
    void SetTimeout(TimeoutType msecTimeout);

    /**
     * TODO
     * Public to be accessed by the thread launcher subroutine
     */
    void ThreadStartUp();


private:

    /**
     * TODO
     */
    ThreadIdentifier      threadId;

    /**
     * TODO
     */
    Commands              commands;

    /**
     * TODO
     * maxCommandCompletionHRT = HighResolutionTimer::Counter32 + timeout
     *
     */
    uint32                maxCommandCompletionHRT;

    /**
     * TODO
     * 2000000000 ticks max
     */

    int32                 timeoutHRT;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ThreadIdentifier EmbeddedThread::Id() {
    return threadId;
}




}
#endif /* L2MIXED_EMBEDDEDTHREADOBJECT_H_ */
	
