/**
 * @file ConfigurationDatabase.cpp
 * @brief Source file for class ConfigurationDatabase
 * @date 08/01/2020
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
 * the class ConfigurationDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabaseNodeTest.h"
#include "ConfigurationDatabaseNode.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ConfigurationDatabaseNodeTest::TestDefaultConstructor() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    return true;
}

bool ConfigurationDatabaseNodeTest::TestPurge() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    ReferenceT<Object> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ref->SetName("T");
    bool ok = cdbn.Insert(ref);
    ok &= (cdbn.Size() == 1u);
    cdbn.Purge();
    ok &= (cdbn.Size() == 0u);
    return ok;
}

bool ConfigurationDatabaseNodeTest::TestPurge_ReferenceContainer() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    ReferenceT<Object> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ref->SetName("T");
    bool ok = cdbn.Insert(ref);
    ok &= (cdbn.Size() == 1u);
    ReferenceContainer rc;
    cdbn.Purge();
    ok &= (cdbn.Size() == 0u);
    return ok;
}

bool ConfigurationDatabaseNodeTest::TestInsert() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    uint32 n;
    uint32 nnodes = 64u;
    bool ok = true;
    for (n = 0u; n < nnodes; n++) {
        ReferenceT<Object> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        StreamString nname;
        nname.Printf("%d", n);
        ref->SetName(nname.Buffer());
        ok &= cdbn.Insert(ref);
    }
    ok &= (cdbn.Size() == nnodes);
    return ok;
}

bool ConfigurationDatabaseNodeTest::TestSize() {
    return TestInsert();
}

bool ConfigurationDatabaseNodeTest::TestGet() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    uint32 n;
    uint32 nnodes = 64u;
    bool ok = true;
    for (n = 0u; n < nnodes; n++) {
        ReferenceT<Object> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        StreamString nname;
        nname.Printf("%d", n);
        ref->SetName(nname.Buffer());
        ok &= cdbn.Insert(ref);
    }
    for (n = 0u; n < nnodes; n++) {
        ReferenceT<Object> ref = cdbn.Get(n);
        StreamString nname;
        nname.Printf("%d", n);
        ok &= (nname == ref->GetName());
    }

    return ok;
}

bool ConfigurationDatabaseNodeTest::TestFind() {
    using namespace MARTe;
    ReferenceT<ConfigurationDatabaseNode> parent(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    parent->SetName("A");
    ReferenceT<ConfigurationDatabaseNode> child(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    child->SetName("B");
    ReferenceT<ConfigurationDatabaseNode> grandChild(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    grandChild->SetName("C");
    grandChild->SetParent(child);
    child->Insert(grandChild);
    child->SetParent(parent);
    parent->Insert(child);
    Reference ref = parent->Find("B.C");
    bool ok = ref.IsValid();
    StreamString expectedName = "C";
    ok &= (expectedName == ref->GetName());
    ref = parent->Find("B.D");
    ok &= !ref.IsValid();
    return ok;
}

bool ConfigurationDatabaseNodeTest::TestFindLeaf() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    uint32 n;
    uint32 nnodes = 64u;
    bool ok = true;
    for (n = 0u; n < nnodes; n++) {
        ReferenceT<Object> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        StreamString nname;
        nname.Printf("%d", n);
        ref->SetName(nname.Buffer());
        ok &= cdbn.Insert(ref);
    }
    for (n = 0u; n < nnodes; n++) {
        StreamString nname;
        nname.Printf("%d", n);
        ReferenceT<Object> ref = cdbn.Find(nname.Buffer());
        ok &= (nname == ref->GetName());
    }

    return ok;
}

bool ConfigurationDatabaseNodeTest::TestDelete() {
    using namespace MARTe;
    ConfigurationDatabaseNode cdbn;
    uint32 n;
    uint32 nnodes = 64u;
    bool ok = true;
    for (n = 0u; n < nnodes; n++) {
        ReferenceT<Object> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        StreamString nname;
        nname.Printf("%d", n);
        ref->SetName(nname.Buffer());
        ok &= cdbn.Insert(ref);
    }

    cdbn.Delete(cdbn.Get(1));
    cdbn.Delete(cdbn.Get(0));
    cdbn.Delete(cdbn.Get(61));

    ok &= (cdbn.Size() == (nnodes - 3));
    for (n = 2u; n < 63; n++) {
        StreamString nname;
        nname.Printf("%d", n);
        ReferenceT<Object> ref = cdbn.Find(nname.Buffer());
        ok &= (nname == ref->GetName());
    }

    return ok;
}

bool ConfigurationDatabaseNodeTest::TestGetParent() {
    using namespace MARTe;
    ReferenceT<ConfigurationDatabaseNode> parent(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    parent->SetName("A");
    ReferenceT<ConfigurationDatabaseNode> child(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    child->SetName("B");
    ReferenceT<ConfigurationDatabaseNode> grandChild(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    grandChild->SetName("C");
    grandChild->SetParent(child);
    return (grandChild->GetParent() == child);
}

bool ConfigurationDatabaseNodeTest::TestSetParent() {
    return TestGetParent();
}
