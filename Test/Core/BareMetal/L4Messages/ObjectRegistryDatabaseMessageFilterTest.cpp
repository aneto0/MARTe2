/**
 * @file ObjectRegistryDatabaseMessageFilterTest.cpp
 * @brief Source file for class ObjectRegistryDatabaseMessageFilterTest
 * @date 19/06/2018
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
 * the class ObjectRegistryDatabaseMessageFilterTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "Message.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectRegistryDatabaseMessageI.h"
#include "ObjectRegistryDatabaseMessageFilter.h"
#include "ObjectRegistryDatabaseMessageFilterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ObjectRegistryDatabaseMessageFilterTest::ObjectRegistryDatabaseMessageFilterTest() {
}

ObjectRegistryDatabaseMessageFilterTest::~ObjectRegistryDatabaseMessageFilterTest() {
}

bool ObjectRegistryDatabaseMessageFilterTest::TestDefaultConstructor() {
    using namespace MARTe;
    ObjectRegistryDatabaseMessageFilter mess;
    return true;
}

bool ObjectRegistryDatabaseMessageFilterTest::TestConsumeMessage_Purge() {
    using namespace MARTe;
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ord->Purge();
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("+ObjectRegistryDatabaseMessageHandler");
    cdb.Write("Class", "ObjectRegistryDatabaseMessageI");
    cdb.CreateAbsolute("+A");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("+A.+B");
    cdb.Write("Class", "ReferenceContainer");
    cdb.MoveToRoot();
    bool ok = ord->Initialise(cdb);
    if (ok) {
        ok = (ord->Size() == 2);
    }
    if (ok) {
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Function", "purge");
        cdbMsg.Write("Destination", "ObjectRegistryDatabaseMessageHandler");
        ReferenceT<ConfigurationDatabase> cdbMsgPayload(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdbMsgPayload->Write("Root", "A.B");
        msg->Initialise(cdbMsg);
        msg->Insert(cdbMsgPayload);
        ErrorManagement::ErrorType err = MessageI::SendMessage(msg);
        ok = err.ErrorsCleared();
    }
    if (ok) {
        ok = (ord->Size() == 1);
    }
    ord->Purge();
    return ok;
}
