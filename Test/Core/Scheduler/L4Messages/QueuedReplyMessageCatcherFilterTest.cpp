/**
 * @file ReplyMessageCatcherMessageFilterTest.cpp
 * @brief Source file for class ReplyMessageCatcherMessageFilterTest
 * @date 10/10/2016
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
 * the class ReplyMessageCatcherMessageFilterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "QueuedReplyMessageCatcherFilterTest.h"

#include "Sleep.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool QueuedReplyMessageCatcherFilterTest::TestDefaultConstructor() {
    using namespace MARTe;
    QueuedReplyMessageCatcherFilter filter;
    return !filter.IsPermanentFilter();
}

bool QueuedReplyMessageCatcherFilterTest::TestConsumeMessage() {
    using namespace MARTe;
    ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceContainer msgs;
    msgs.Insert(msg1);
    msgs.Insert(msg2);
    msg1->SetAsReply(true);
    msg2->SetAsReply(true);

    EventSem sem;
    sem.Create();
    sem.Reset();

    QueuedReplyMessageCatcherFilter filter;
    filter.SetEventSemaphore(sem);
    filter.SetMessagesToCatch(msgs);

    MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg1);
    bool ret = (err == ErrorManagement::NotCompleted);
    if (ret) {
        //This should post the semaphore.
        err = unprotectedFilter.ConsumeMessage(msg2);
        ret = err.ErrorsCleared();
    }
    if (ret) {
        err = sem.Wait(1);
        ret = err.ErrorsCleared();
    }
    return ret;
}

bool QueuedReplyMessageCatcherFilterTest::TestConsumeMessage_MissingMessage() {
    using namespace MARTe;
    ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceContainer msgs;
    msgs.Insert(msg1);
    msgs.Insert(msg2);
    msg1->SetAsReply(true);
    msg2->SetAsReply(true);

    EventSem sem;
    sem.Create();
    sem.Reset();

    QueuedReplyMessageCatcherFilter filter;
    filter.SetEventSemaphore(sem);
    filter.SetMessagesToCatch(msgs);

    MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg1);
    bool ret = (err == ErrorManagement::NotCompleted);
    if (ret) {
        err = sem.Wait(10);
        //Should timeout
        ret = !err.ErrorsCleared();
    }
    return ret;
}

bool QueuedReplyMessageCatcherFilterTest::TestConsumeMessage_MessageNotReply() {
    using namespace MARTe;
    ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceContainer msgs;
    msgs.Insert(msg1);
    msgs.Insert(msg2);
    msg1->SetAsReply(true);
    msg2->SetAsReply(false);

    EventSem sem;
    sem.Create();
    sem.Reset();

    QueuedReplyMessageCatcherFilter filter;
    filter.SetEventSemaphore(sem);
    filter.SetMessagesToCatch(msgs);

    MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg1);
    bool ret = err.ErrorsCleared();
    if (ret) {
        //This should not post the semaphore (msg2 is not a reply).
        err = unprotectedFilter.ConsumeMessage(msg2);
        ret = err.ErrorsCleared();
    }
    if (ret) {
        //This should timeout
        err = sem.Wait(10);
        ret = !err.ErrorsCleared();
    }
    return ret;
}

bool QueuedReplyMessageCatcherFilterTest::TestConsumeMessage_InvalidMessage() {
    using namespace MARTe;
    ReferenceT<ReferenceContainer> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceContainer msgs;
    msgs.Insert(msg1);
    msgs.Insert(msg2);
    msg2->SetAsReply(true);

    EventSem sem;
    sem.Create();
    sem.Reset();

    QueuedReplyMessageCatcherFilter filter;
    filter.SetEventSemaphore(sem);
    filter.SetMessagesToCatch(msgs);

    MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg2);
    return !err.ErrorsCleared();
}

bool QueuedReplyMessageCatcherFilterTest::TestConsumeMessage_MessageNoEventSem() {
    using namespace MARTe;
    ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceContainer msgs;
    msgs.Insert(msg1);
    msg1->SetAsReply(true);

    EventSem sem;
    sem.Create();
    sem.Reset();

    QueuedReplyMessageCatcherFilter filter;
    filter.SetMessagesToCatch(msgs);

    MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg1);
    return !err.ErrorsCleared();
}


bool QueuedReplyMessageCatcherFilterTest::TestSetMessagesToCatch() {
    return TestConsumeMessage();
}

bool QueuedReplyMessageCatcherFilterTest::TestSetEventSemaphore() {
    return TestConsumeMessage();
}
