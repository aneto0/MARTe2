/**
 * @file EmbeddedThreadTest.cpp
 * @brief Source file for class EmbeddedThreadTest
 * @date 19/09/2016
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

 * @details This source file contains the definition of all the methods for
 * the class EmbeddedThreadTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "EmbeddedThread.h"
#include "EmbeddedThreadTest.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/\

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EmbeddedThreadTest::EmbeddedThreadTest() {
    completed = false;
    main = false;
    badTermination = false;
    startup = false;
}

EmbeddedThreadTest::~EmbeddedThreadTest() {
}

MARTe::ErrorManagement::ErrorType EmbeddedThreadTest::CallbackFunction(MARTe::ExecutionInfo &information) {
    if (information.GetStage() == MARTe::ExecutionInfo::StartupStage) {
        startup = true;
    }
    if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
        main = true;
        if (!completed) {
            return MARTe::ErrorManagement::Completed;
        }
        else {
            return MARTe::ErrorManagement::FatalError;
        }
    }
    if (information.GetStage() == MARTe::ExecutionInfo::TerminationStage) {
        completed = true;
    }
    if (information.GetStage() == MARTe::ExecutionInfo::BadTerminationStage) {
        badTermination = true;
    }
    return MARTe::ErrorManagement::NoError;
}

bool EmbeddedThreadTest::TestDefaultConstructor() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadTest> binder(*this, &EmbeddedThreadTest::CallbackFunction);
    EmbeddedThread embeddedThread(binder);
    bool ok = (embeddedThread.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThread.GetThreadNumber() == 0);
    ok &= (embeddedThread.GetCommands() == EmbeddedThread::StopCommand);
    return ok;
}

bool EmbeddedThreadTest::TestThreadLoop() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderT<EmbeddedThreadTest> binder(*this, &EmbeddedThreadTest::CallbackFunction);
    //Easiest way to test the EmbeddedThread (handles all the threading).
    SingleThreadService service(binder);

    ErrorManagement::ErrorType err = service.Start();
    bool ok = (err == ErrorManagement::NoError);
    uint32 maxCounter = 10;
    while ((maxCounter > 0) && (badTermination != true)) {
        Sleep::Sec(1.0);
        maxCounter--;
    }
    ok &= (main && completed && startup && badTermination);

    service.Stop();
    return ok;
}
