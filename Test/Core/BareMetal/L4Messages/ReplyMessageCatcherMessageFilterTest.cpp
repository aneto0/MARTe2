/**
 * @file ReplyMessageCatcherMessageFilterTest.cpp
 * @brief Source file for class ReplyMessageCatcherMessageFilterTest
 * @date 9/09/2016
 * @author Andre Nneto
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
 * the class ReplyMessageCatcherMessageFilterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReplyMessageCatcherMessageFilterTest.h"
#include "Sleep.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ReplyMessageCatcherMessageFilterTest::TestDefaultConstructor() {
    using namespace MARTe;
    ReplyMessageCatcherMessageFilter filter;
    return !filter.IsPermanentFilter();
}

bool ReplyMessageCatcherMessageFilterTest::TestSetMessageToCatch() {
    using namespace MARTe;
    ReplyMessageCatcherMessageFilter filter;
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter.SetMessageToCatch(msg);
    MessageFilter &unprotectedFilter = filter;
    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg);
    bool ret = !err.ErrorsCleared();
    if (ret) {
        msg->SetAsReply(true);
        err = (unprotectedFilter.ConsumeMessage(msg) == ErrorManagement::NoError);
        ret = err.ErrorsCleared();
    }
    return ret;
}

static void ReplyMessageCatcherMessageFilterTestWaitCallCallback(ReplyMessageCatcherMessageFilterTest &test) {
    test.waitState = 1;
    while (test.waitState == 1) {
        MARTe::Sleep::MSec(1);
    }
    MARTe::Sleep::MSec(10);
    MARTe::MessageFilter &unprotectedFilter = test.waitFilter;
    unprotectedFilter.ConsumeMessage(test.waitMessage);
}

bool ReplyMessageCatcherMessageFilterTest::TestWait() {
    using namespace MARTe;
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    waitMessage = msg;
    waitFilter.SetMessageToCatch(msg);
    waitState = 0;
    Threads::BeginThread((ThreadFunctionType) ReplyMessageCatcherMessageFilterTestWaitCallCallback, this);
    while (waitState == 0) {
        Sleep::MSec(1);
    }
    waitState = 2;
    ErrorManagement::ErrorType err = waitFilter.Wait();
    return err;
}

bool ReplyMessageCatcherMessageFilterTest::TestWait_Timeout() {
    using namespace MARTe;
    ErrorManagement::ErrorType err = waitFilter.Wait(1);
    return (err == ErrorManagement::Timeout);
}
