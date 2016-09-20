/**
 * @file MultiThreadService.cpp
 * @brief Source file for class MultiThreadService
 * @date Sep 2, 2016
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

 * @details This source file contains the definition of all the methods for
 * the class MultiThreadService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MultiThreadService.h"
#include "ReferenceT.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

bool  MultiThreadService::Initialise(StructuredDataI &data){
    ErrorManagement::ErrorType  err;
    threadPool.SetName("ThreadPool");
    err = data.Read("MinNumberOfThreads",minNumberOfThreads);
    return err;
}

ErrorManagement::ErrorType MultiThreadService::AddThread(){
    ErrorManagement::ErrorType  err;
    if ((threadPool.Size()< minNumberOfThreads) && (err.ErrorsCleared())){
        ReferenceT<EmbeddedThread> thread(new (NULL) EmbeddedThread(method));
        err.fatalError = ! thread.IsValid();
        if (err.ErrorsCleared()) {
            err = thread->Start();
        }
        if (err.ErrorsCleared()) {
            threadPool.Insert(thread);
        }
    }
    return err;
}


ErrorManagement::ErrorType MultiThreadService::Start(){
    ErrorManagement::ErrorType  err;
    while((threadPool.Size()< minNumberOfThreads) && (err.ErrorsCleared())){
        err = AddThread();
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::Stop(){
    ErrorManagement::ErrorType  err;
    uint32 i;
    for (i=0;i<threadPool.Size();i++){
        ReferenceT<EmbeddedThread> thread = threadPool.Get(i);
        if (thread.IsValid()){
            thread->Stop();
        }
    }
    // perform kill if necessary
    for (i=0;i<threadPool.Size();i++){
        ReferenceT<EmbeddedThread> thread = threadPool.Get(i);
        if (thread.IsValid()){
            thread->Stop();
        }
    }
    // remove dead threads
    i = 0;
    while((i<threadPool.Size()) && (err.ErrorsCleared())){
        ReferenceT<EmbeddedThread> thread = threadPool.Get(i);
        if (thread.IsValid()){
            if (thread->GetStatus() == EmbeddedThread::OffState){
                threadPool.Delete(thread);
            } else {
                i++;
            }
        } else {
            // some unexpected content or something seriously wrong!!
            err.internalSetupError = true;
        }
    }
    if (err.ErrorsCleared() && (threadPool.Size() > 0)){
        // some thread die hard
        err.timeout = true;
    }

    return err;

}

MultiThreadService::~MultiThreadService(){
    Stop();
}

}
