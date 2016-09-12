/**
 * @file MessageFilterPoolTest.cpp
 * @brief Source file for class MessageFilterPoolTest
 * @date 12/09/2016
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
 * the class MessageFilterPoolTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MessageFilter.h"
#include "MessageFilterPool.h"
#include "MessageFilterPoolTest.h"
#include "ReplyMessageCatcherMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MessageFilterPoolTestPermanentFilter: public MARTe::MessageFilter {
public:
    MessageFilterPoolTestPermanentFilter() :
            MessageFilter(true) {

    }

    MARTe::ErrorManagement::ErrorType ConsumeMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest) {
        return true;
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MessageFilterPoolTest::TestDefaultConstructor() {
    using namespace MARTe;
    MessageFilterPool pool;
    return (pool.Size() == 0u);
}

bool MessageFilterPoolTest::TestReceiveMessage() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg->SetAsReply(true);
    ReferenceT<ReplyMessageCatcherMessageFilter> catcherFilter = ReferenceT<ReplyMessageCatcherMessageFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!catcherFilter.IsValid()) {
        return false;
    }
    catcherFilter->SetMessageToCatch(msg);
    MessageFilterPool pool;
    pool.Insert(catcherFilter);
    ErrorManagement::ErrorType err = pool.ReceiveMessage(msg);
    return err.ErrorsCleared();
}

bool MessageFilterPoolTest::TestReceiveMessage_ErrorInFilter() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    //Not setting the message as a reply will trigger an error.
    ReferenceT<ReplyMessageCatcherMessageFilter> catcherFilter = ReferenceT<ReplyMessageCatcherMessageFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!catcherFilter.IsValid()) {
        return false;
    }
    catcherFilter->SetMessageToCatch(msg);
    MessageFilterPool pool;
    pool.Insert(catcherFilter);
    ErrorManagement::ErrorType err = pool.ReceiveMessage(msg);
    return !err.ErrorsCleared();
}

bool MessageFilterPoolTest::TestReceiveMessage_MessageNotFound() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ReplyMessageCatcherMessageFilter> catcherFilter = ReferenceT<ReplyMessageCatcherMessageFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!catcherFilter.IsValid()) {
        return false;
    }
    ReferenceT<Message> msg2 = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg2->SetAsReply(true);
    catcherFilter->SetMessageToCatch(msg2);
    MessageFilterPool pool;
    pool.Insert(catcherFilter);
    ErrorManagement::ErrorType err = pool.ReceiveMessage(msg);
    return (err == ErrorManagement::UnsupportedFeature);
}

bool MessageFilterPoolTest::TestReceiveMessage_NotPermanentRemoved() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg->SetAsReply(true);
    ReferenceT<ReplyMessageCatcherMessageFilter> catcherFilter = ReferenceT<ReplyMessageCatcherMessageFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!catcherFilter.IsValid()) {
        return false;
    }
    catcherFilter->SetMessageToCatch(msg);
    MessageFilterPool pool;
    pool.Insert(catcherFilter);
    pool.ReceiveMessage(msg);
    //Not permanent filter to be removed.
    return (pool.Size() == 0);
}

bool MessageFilterPoolTest::TestReceiveMessage_PermanentNotRemoved() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg->SetAsReply(true);
    ReferenceT<MessageFilterPoolTestPermanentFilter> permanentFilter = ReferenceT<MessageFilterPoolTestPermanentFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!permanentFilter.IsValid()) {
        return false;
    }
    MessageFilterPool pool;
    pool.Insert(permanentFilter);
    ErrorManagement::ErrorType err = pool.ReceiveMessage(msg);
    //Permanent filter should not be removed.
    return (err.ErrorsCleared()) && (pool.Size() == 1);
}
