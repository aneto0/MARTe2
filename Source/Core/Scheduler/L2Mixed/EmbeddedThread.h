/**
 * @file EmbeddedThread.h
 * @brief Header file for class EmbeddedThread
 * @date Aug 22, 2016
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

#include "ErrorType.h"
#include "Threads.h"

namespace MARTe{


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/**
 * @brief States of an EmbeddedThread

 */
enum EmbeddedThreadStates {
    /**
     * No Status
     */
    None,
    /**
     * No Thread running =  (threadId = 0)
     */
    Off,

    /**
     *TODO
     */
    Starting,

    /**
     * TODO
     */
    TimeoutStarting,

    /**
     * (threadId != 0) && TODO
     */
    Running,

    /**
     * TODO
     */
    Stopping,

    /**
     * TODO
     */
    TimeoutStopping,

    /**
     * TODO
     */
    Killing,
    /**
     * TODO
     */
    TimeoutKilling

};

/**
 * @brief States of an EmbeddedThread

 */
enum EmbeddedThreadCommands {
    /**
     * set by Start() at the start of thread life
     */
    Start,
    /**
     * set by thread before entering loop
     */
    KeepRunning,

    /**
     * nice request to stop
     */
    Stop,

    /**
     * Stop called twice - performing async killing
     */
    Kill

};

/**
 * TODO
 * a container for a thread
 */
class EmbeddedThread {

public:
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
    */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * TODO
     */
    ErrorManagement::ErrorType Start();

    /**
     * TODO
     */
    ErrorManagement::ErrorType Stop();

    /**
     * TODO
     */
    EmbeddedThreadStates GetStatus();

    /**
     * TODO
     */
    inline ThreadIdentifier Id();

    /**
     * TODO
     */
    void SetTimeout(TimeoutType msecTimeout);


protected:

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Loop()=0;


private:
    /**
     * TODO
     */
    void ThreadStartUp();

    /**
     * TODO
     */
    ThreadIdentifier threadId;

    /**
     * TODO
     */
    EmbeddedThreadCommands commands;

    /**
     * TODO
     * maxCommandCompletionHRT = HighResolutionTimer::Counter32 + timeout
     *
     */
    uint32 maxCommandCompletionHRT;

    /**
     * TODO
     * 2000000000 ticks max
     */

    int32 timeoutHRT;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ThreadIdentifier EmbeddedThread::Id() {
    return threadId;
}




}
#endif /* L2MIXED_EMBEDDEDTHREADOBJECT_H_ */
	
