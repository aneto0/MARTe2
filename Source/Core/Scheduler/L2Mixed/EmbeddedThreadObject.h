/**
 * @file EmbeddedThreadObject.h
 * @brief Header file for class EmbeddedThreadObject
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

 * @details This header file contains the declaration of the class EmbeddedThreadObject
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

namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/**
 * @brief States of an EmbeddedThreadObject

 */
enum EmbeddedThreadObjectStates {
    /**
     * No Status
     */
    None,
    /**
     * No Thread running =  (threadId = 0)
     */
    Off,

    /**
     * (threadId != 0) && TODO
     */
    Running,

    /**
     *TODO
     */
    Starting,

    /**
     * TODO
     */
    Stopping,

    /**
     * TODO
     */
    TimeoutStarting,

    /**
     * TODO
     */
    TimeoutStopping
};

/**
 * @brief States of an EmbeddedThreadObject

 */
enum EmbeddedThreadObjectCommands {
    /**
     * set by Start() at the start of thread life
     */
    Start,
    /**
     * set by thread before entering loop
     */
    KeepRunning,

    /**
     * (threadId != 0) && TODO
     */
    Stop

};

/**
 * TODO
 */
class EmbeddedThreadObject {

    /**
     * TODO
     */
    EmbeddedThreadObject();

    /**
     * TODO
     */
    virtual ~EmbeddedThreadObject();

public:

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
    EmbeddedThreadObjectStates GetStatus();

    /**
     * TODO
     */
    void SetTimeout(TimeoutType msecTimeout){
        if (msecTimeout == TTInfiniteWait) {
            timeoutHRT = -1;
        } else {
            uint64 tt64 = msecTimeout.HighResolutionTimerTicks();
            if (tt64 < 0x7FFFFFFF){
                timeoutHRT = tt64;
            } else {
                timeoutHRT = 0x7FFFFFFF;
            }
        }
    }


protected:

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Loop()=0;

private:

    /**
     * TODO
     */
    ThreadIdentifier threadId;

    /**
     * TODO
     */
    EmbeddedThreadObjectCommands commands;

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

/**
 * TODO
 */
EmbeddedThreadObject::EmbeddedThreadObject(){
    threadId = (ThreadIdentifier)0;
    commands = Stop;
}

/**
 * TODO
 */
virtual EmbeddedThreadObject::~EmbeddedThreadObject(){
    Stop();

}

EmbeddedThreadObjectStates EmbeddedThreadObject::GetStatus(){
    EmbeddedThreadObjectStates  etos = None;

    if (ThreadIdentifier == 0) {
                etos = Off;
    }

    if (etos == None){
        if (commands == KeepRunning)  {
                etos = Running;
        }
        if (commands == Start) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                etos = TimeoutStarting;
            }
            else            {
                etos = Starting;
            }
        }
        if (commands == Stop) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                etos = TimeoutStopping;
            }
            else            {
                etos = Stopping;
            }
        }
    }
    return etos;
}

static void  EmbeddedThreadObjectThreadLauncher(const void * const parameters){}


ErrorManagement::ErrorType EmbeddedThreadObject::Start(){
    ErrorManagement::ErrorType err;

    //check if thread already running
    if (GetStatus() != Off ){
        err.illegalOperation = true;
    }

    if (err.ErrorsCleared()){
        commands = Start;
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        threadId = Threads::BeginThread(EmbeddedThreadObjectThreadLauncher);

        err.fatalError =  (threadId == 0);
    }

    return err;
}

/**
 * TODO
 */
ErrorManagement::ErrorType EmbeddedThreadObject::Stop(){
    ErrorManagement::ErrorType err;

    //check if thread already running
    if (GetStatus() != Running ){
        err.illegalOperation = true;
    }


    TODO


    return err;
}



}
#endif /* L2MIXED_EMBEDDEDTHREADOBJECT_H_ */
	
