/**
 * @file QueuedMessageITest.cpp
 * @brief Source file for class QueuedMessageITest
 * @date 16/09/2016
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
 * the class QueuedMessageITest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassWithCallableMethods.h"
#include "ConfigurationDatabase.h"
#include "Message.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectWithMessages.h"
#include "QueuedMessageITest.h"
#include "ReplyMessageCatcherMessageFilter.h"
#include "RegisteredMethodsMessageFilter.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class QueuedMessageITester: public QueuedMessageI, public Object {
public:
    CLASS_REGISTER_DECLARATION()QueuedMessageITester() {

    }

private:
};

CLASS_REGISTER(QueuedMessageITester, "1.0")
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

QueuedMessageITest::QueuedMessageITest() {
}

QueuedMessageITest::~QueuedMessageITest() {
}

bool QueuedMessageITest::TestDefaultConstructor() {
    using namespace MARTe;
    QueuedMessageI mess;
    return true;
}

bool QueuedMessageITest::TestStart() {
    using namespace MARTe;
    ErrorManagement::ErrorType status;
    ReferenceT<ClassWithCallableMethods> destination = ReferenceT<ClassWithCallableMethods>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<QueuedMessageITester> receiver = ReferenceT<QueuedMessageITester>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    filter->SetDestination(destination.operator->());

    receiver->InstallMessageFilter(filter);
    receiver->SetName("receiver");
    receiver->Start();

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "MethodWithVoidParameters");

    if (!msg->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = QueuedMessageI::SendMessage(msg, NULL);
    if (status != ErrorManagement::NoError) {
        return false;
    }

    //Wait 1 second for the answer to eventually arrive
    uint32 counter = 0;
    bool ret = false;
    while ((!ret) && (counter < 10)) {
        ret = (StringHelper::Compare(destination->GetLastMethodExecuted().Buffer(), "MethodWithVoidParameters(void)") == 0);
        counter++;
        Sleep::Sec(0.1);
    }
    receiver->Stop();
    return ret;
}

bool QueuedMessageITest::TestStart2() {
    using namespace MARTe;
    ErrorManagement::ErrorType status;
    ReferenceT<ClassWithCallableMethods> destination = ReferenceT<ClassWithCallableMethods>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<QueuedMessageITester> receiver = ReferenceT<QueuedMessageITester>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    filter->SetDestination(destination.operator->());

    receiver->InstallMessageFilter(filter);
    receiver->SetName("receiver");

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "MethodWithVoidParameters");

    if (!msg->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = QueuedMessageI::SendMessage(msg, NULL);
    if (status != ErrorManagement::NoError) {
        return false;
    }

    //Wait 1 second for the answer to eventually arrive
    uint32 counter = 0;
    bool ret = false;
    while ((!ret) && (counter < 10)) {
        ret = (StringHelper::Compare(destination->GetLastMethodExecuted().Buffer(), "MethodWithVoidParameters(void)") == 0);
        counter++;
        Sleep::Sec(0.1);
    }

    return !ret;
}

bool QueuedMessageITest::TestStop() {
    using namespace MARTe;
    ErrorManagement::ErrorType status;
    ReferenceT<ClassWithCallableMethods> destination = ReferenceT<ClassWithCallableMethods>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<QueuedMessageITester> receiver = ReferenceT<QueuedMessageITester>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    filter->SetDestination(destination.operator->());

    receiver->InstallMessageFilter(filter);
    receiver->SetName("receiver");
    receiver->Start();

    Sleep::Sec(1.0);

    receiver->Stop();

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "MethodWithVoidParameters");

    if (!msg->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = QueuedMessageI::SendMessage(msg, NULL);
    if (status != ErrorManagement::NoError) {
        return false;
    }

    //Wait 1 second for the answer to eventually arrive
    uint32 counter = 0;
    bool ret = false;
    while ((!ret) && (counter < 10)) {
        ret = (StringHelper::Compare(destination->GetLastMethodExecuted().Buffer(), "MethodWithVoidParameters(void)") == 0);
        counter++;
        Sleep::Sec(0.1);
    }
    receiver->Stop();
    return !ret;
}

bool QueuedMessageITest::TestInstallMessageFilter() {
    using namespace MARTe;

    QueuedMessageI queuedMessageI;
    ReferenceT<ReplyMessageCatcherMessageFilter> filter = ReferenceT<ReplyMessageCatcherMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    return queuedMessageI.InstallMessageFilter(filter);
}

bool QueuedMessageITest::TestRemoveMessageFilter() {
    using namespace MARTe;

    QueuedMessageI queuedMessageI;
    ReferenceT<ReplyMessageCatcherMessageFilter> filter = ReferenceT<ReplyMessageCatcherMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    queuedMessageI.InstallMessageFilter(filter);
    return queuedMessageI.RemoveMessageFilter(filter);
}
