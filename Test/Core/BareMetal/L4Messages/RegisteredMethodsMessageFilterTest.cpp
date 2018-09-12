/**
 * @file RegisteredMethodsMessageFilterTest.cpp
 * @brief Source file for class RegisteredMethodsMessageFilterTest
 * @date 9/09/2016
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
 * the class RegisteredMethodsMessageFilterTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "ClassWithCallableMethods.h"
#include "ConfigurationDatabase.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectWithMessages.h"
#include "RegisteredMethodsMessageFilterTest.h"
#include "ReplyMessageCatcherMessageFilter.h"
#include "Sleep.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
class ClassWithCallableMethodsAndMessageI: public ClassWithCallableMethods, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()ClassWithCallableMethodsAndMessageI() {
    }

    void Setup(ReferenceT<Message> toCatch) {
        catcherFilter = ReferenceT<ReplyMessageCatcherMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        catcherFilter->SetMessageToCatch(toCatch);
        InstallMessageFilter(catcherFilter);
    }

    bool ReplyReceived() {
        return (catcherFilter->Wait(1));
    }

    ReferenceT<ReplyMessageCatcherMessageFilter> catcherFilter;
};
CLASS_REGISTER(ClassWithCallableMethodsAndMessageI, "1.0")
CLASS_METHOD_REGISTER(ClassWithCallableMethodsAndMessageI, MethodWithVoidParameters)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool RegisteredMethodsMessageFilterTest::TestDefaultConstructor() {
    using namespace MARTe;
    RegisteredMethodsMessageFilter filter;
    return filter.IsPermanentFilter();
}

bool RegisteredMethodsMessageFilterTest::TestSetDestinationObject() {
    using namespace MARTe;
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> destination(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    destination->SetName("RegisteredMethodsMessageFilterTestDestination");

    ConfigurationDatabase data;
    data.Write("Destination", "RegisteredMethodsMessageFilterTestDestination");
    data.Write("Function", "ReceiverMethod");
    msg->Initialise(data);

    RegisteredMethodsMessageFilter filter;
    filter.SetDestination(destination.operator->());

    MARTe::MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg);
    return err.ErrorsCleared();
}

bool RegisteredMethodsMessageFilterTest::TestConsumeMessage() {
    using namespace MARTe;
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ClassWithCallableMethods> destination(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    destination->SetName("TestConsumeMessageDestination");

    ConfigurationDatabase data;
    data.Write("Destination", "TestConsumeMessageDestination");
    data.Write("Function", "MethodWithVoidParameters");
    msg->Initialise(data);

    RegisteredMethodsMessageFilter filter;
    filter.SetDestination(destination.operator->());

    MARTe::MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg);
    bool ret = err.ErrorsCleared();
    ret &= (StringHelper::Compare(destination->GetLastMethodExecuted().Buffer(), "MethodWithVoidParameters(void)") == 0);
    return ret;
}

bool RegisteredMethodsMessageFilterTest::TestConsumeMessage_Reply() {
    using namespace MARTe;
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ClassWithCallableMethods> destination(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    destination->SetName("TestConsumeMessage_ReplyDestination");

    ConfigurationDatabase data;
    data.Write("Destination", "TestConsumeMessage_ReplyDestination");
    data.Write("Function", "MethodWithVoidParameters");
    msg->Initialise(data);
    msg->SetAsReply(true);

    RegisteredMethodsMessageFilter filter;
    filter.SetDestination(destination.operator->());

    MARTe::MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg);
    bool ret = (err == ErrorManagement::UnsupportedFeature);
    ret &= (destination->GetLastMethodExecuted().Size() == 0);
    return ret;
}

bool RegisteredMethodsMessageFilterTest::TestConsumeMessage_IndirectReply() {
    using namespace MARTe;
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ClassWithCallableMethodsAndMessageI> destination(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    destination->SetName("TestConsumeMessage_IndirectReplyDestination");
    destination->Setup(msg);
    ObjectRegistryDatabase::Instance()->Insert(destination);

    ConfigurationDatabase data;
    data.Write("Destination", "TestConsumeMessage_IndirectReplyDestination");
    data.Write("Function", "MethodWithVoidParameters");
    data.Write("Mode", "ExpectsIndirectReply");
    msg->Initialise(data);
    msg->SetSender(destination.operator ->());

    RegisteredMethodsMessageFilter filter;
    filter.SetDestination(destination.operator->());

    MARTe::MessageFilter &unprotectedFilter = filter;

    ErrorManagement::ErrorType err = unprotectedFilter.ConsumeMessage(msg);
    bool ret = err.ErrorsCleared();
    ret &= (StringHelper::Compare(destination->GetLastMethodExecuted().Buffer(), "MethodWithVoidParameters(void)") == 0);
    ret &= destination->ReplyReceived();
    return ret;
}

bool RegisteredMethodsMessageFilterTest::TestIsPermanentFilter() {
    using namespace MARTe;
    RegisteredMethodsMessageFilter filter;
    return filter.IsPermanentFilter();
}
