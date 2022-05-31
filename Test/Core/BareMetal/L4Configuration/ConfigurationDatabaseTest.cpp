/**
 * @file ConfigurationDatabaseTest.cpp
 * @brief Source file for class ConfigurationDatabaseTest
 * @date 19/11/2015
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
#include "AnyObject.h"
#include "ConfigurationDatabaseTest.h"
#include "IntrospectionTestHelper.h"
#include "Matrix.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ConfigurationDatabaseTest::TestDefaultConstructor() {
    ConfigurationDatabase cdb;
    bool ok = cdb.Lock(TTInfiniteWait);
    cdb.Unlock();
    return ok;
}

bool ConfigurationDatabaseTest::TestCreateAbsolute_ValidPath() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    ok = cdb.MoveToRoot();
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.CreateAbsolute(".A.B");
    ok &= cdb.MoveAbsolute("A.B.");
    ok &= cdb.CreateAbsolute("A.B.C");
    ok &= cdb.CreateAbsolute("D.E.F");
    ok &= cdb.MoveAbsolute("D.E.F");
    ok &= cdb.MoveAbsolute("A.B.C");
    return ok;
}

bool ConfigurationDatabaseTest::TestCreateAbsolute_InvalidPath() {
    ConfigurationDatabase cdb;
    bool ok = !cdb.MoveAbsolute("A.B");
    ok &= cdb.CreateAbsolute("A.B");
    ok &= cdb.MoveToRoot();
    ok &= cdb.MoveAbsolute("A.B");
    ok &= !cdb.CreateAbsolute("A.B");
    ok &= !cdb.CreateAbsolute("A");
    ok &= cdb.CreateAbsolute("A.B...C");
    ok &= cdb.MoveAbsolute("A.B.C");
    return ok;
}

bool ConfigurationDatabaseTest::TestCreateRelative_ValidPath() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateRelative("A");
    ok &= cdb.MoveToRoot();
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.CreateRelative("A.B");
    ok &= cdb.MoveAbsolute("A.A.B");
    ok &= !cdb.MoveAbsolute("A.B");
    ok &= cdb.MoveToRoot();
    ok &= cdb.CreateRelative("D.E.F");
    ok &= cdb.CreateRelative("D.E.F");
    ok &= cdb.MoveAbsolute("D.E.F.D.E.F");

    return ok;
}

bool ConfigurationDatabaseTest::TestCreateRelative_InvalidPath() {
    ConfigurationDatabase cdb;
    bool ok = !cdb.MoveRelative("A.B");

    ok &= cdb.CreateRelative("A.B");
    ok &= cdb.CreateRelative("C");
    ok &= cdb.MoveToAncestor(1);
    ok &= !cdb.CreateRelative("C");
    ok &= cdb.CreateRelative("B");
    ok &= cdb.MoveAbsolute("A.B.B");
    ok &= cdb.MoveToRoot();
    ok &= !cdb.CreateRelative("A.B");

    return ok;
}

bool ConfigurationDatabaseTest::TestMoveToRoot() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B");
    ok &= cdb.MoveToRoot();
    ok &= cdb.MoveAbsolute("A.B");
    ok &= cdb.MoveToRoot();
    ok &= !cdb.CreateAbsolute("A.B");
    return ok;
}

bool ConfigurationDatabaseTest::TestMoveToAncestor_ValidNumberOfGenerations() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.CreateAbsolute("A.B.C.F.G");
    ok &= cdb.MoveToAncestor(2);
    ok &= !cdb.CreateRelative("D");
    ok &= !cdb.CreateRelative("D.E");
    ok &= !cdb.CreateRelative("F");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= cdb.MoveToAncestor(2);
    ok &= !cdb.CreateRelative("B");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= cdb.MoveToAncestor(3);
    ok &= !cdb.CreateRelative("A");
    return ok;
}

bool ConfigurationDatabaseTest::TestMoveToAncestor_InvalidNumberOfGenerations() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.CreateAbsolute("A.B.C.F.G");
    ok &= cdb.MoveToAncestor(2);
    ok &= !cdb.CreateRelative("D");
    ok &= !cdb.CreateRelative("D.E");
    ok &= !cdb.CreateRelative("F");
    //Invalid move
    ok &= !cdb.MoveToAncestor(4);
    ok &= cdb.MoveToAncestor(2);
    ok &= !cdb.CreateRelative("B");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= cdb.MoveToAncestor(3);
    ok &= !cdb.CreateRelative("A");
    return ok;
}

bool ConfigurationDatabaseTest::TestMoveAbsolute_ValidPaths() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.CreateAbsolute("A.C.D.E");
    ok &= cdb.CreateAbsolute("E.D.C.B.A");
    ok &= cdb.CreateAbsolute("E.D.C.A");
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.MoveAbsolute("A.B");
    ok &= cdb.MoveAbsolute("E.D");
    ok &= cdb.MoveAbsolute("E.D.C");
    ok &= cdb.MoveAbsolute("E.D.C.B");
    ok &= cdb.MoveAbsolute("E.D.C.B.A");
    ok &= cdb.MoveAbsolute("E.D.C");
    ok &= cdb.MoveAbsolute("A.B.C.D.E");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    return ok;
}

bool ConfigurationDatabaseTest::TestMoveAbsolute_InvalidPaths() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.CreateAbsolute("A.C.D.E");
    ok &= cdb.CreateAbsolute("E.D.C.B.A");
    ok &= cdb.CreateAbsolute("E.D.C.A");
    ok &= cdb.MoveAbsolute("E.D.C.B.A");
    ok &= !cdb.MoveAbsolute("F.D.C");
    ok &= cdb.MoveAbsolute("E.D.C");
    ok &= !cdb.MoveAbsolute("F.D.C");
    ok &= cdb.MoveAbsolute("A.B.C.D.E");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= !cdb.MoveAbsolute("A.C.D.E.E");
    ok &= !cdb.MoveAbsolute("A.B.C.D.E.F");
    ok &= !cdb.MoveAbsolute("");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= !cdb.MoveAbsolute(".");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= !cdb.MoveAbsolute("..");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    ok &= !cdb.MoveAbsolute("...");
    ok &= cdb.MoveAbsolute("A.C.D.E");
    return ok;
}

bool ConfigurationDatabaseTest::TestMoveRelative_ValidPaths() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.CreateAbsolute("A.C.D.E");
    ok &= cdb.CreateAbsolute("A.B.C.D.F");

    ok &= cdb.MoveToRoot();
    ok &= cdb.MoveRelative("A");
    ok &= cdb.MoveRelative("B");
    ok &= cdb.MoveRelative("C.D.E");
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.MoveRelative("C.D.E");

    ok &= cdb.MoveAbsolute("A");
    ok &= !cdb.MoveRelative("A");
    ok &= cdb.MoveRelative("B");
    ok &= cdb.MoveRelative("C");
    ok &= !cdb.MoveRelative("");
    ok &= cdb.MoveRelative("D.E");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= !cdb.MoveAbsolute(".");
    ok &= cdb.MoveRelative("D.E");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= !cdb.MoveAbsolute("..");
    ok &= cdb.MoveRelative(".D.E");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= !cdb.MoveAbsolute("...");
    ok &= cdb.MoveRelative("D.E.");
    ok &= cdb.MoveAbsolute("A.B.C");
    return ok;
}

bool ConfigurationDatabaseTest::TestMoveRelative_InvalidPaths() {
    ConfigurationDatabase cdb;
    bool ok = cdb.MoveToRoot();
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.CreateAbsolute("A.C.D.E");
    ok &= cdb.CreateAbsolute("A.B.C.D.F");

    ok &= cdb.MoveAbsolute("A");
    //We are in A so we cannot move to A
    ok &= !cdb.MoveRelative("A");
    ok &= cdb.MoveRelative("B");
    ok &= cdb.MoveRelative("C");
    //Do not move to an invalid node
    ok &= !cdb.MoveRelative("");
    //Returned false and the node should not have moved
    ok &= cdb.MoveRelative("D.E");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= !cdb.MoveAbsolute(".");
    ok &= cdb.MoveRelative("D.E");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= !cdb.MoveAbsolute("..");
    ok &= cdb.MoveRelative(".D.E");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= !cdb.MoveAbsolute("...");
    ok &= cdb.MoveRelative("D.E.");
    ok &= cdb.MoveAbsolute("A.B.C");
    return ok;
}

bool ConfigurationDatabaseTest::TestDelete() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A.B.C.D.E");
    ok = cdb.CreateAbsolute("A.B.C.F.G");

    ok &= cdb.MoveAbsolute("A.B.C");
    //Delete node D
    ok &= cdb.Delete("D");
    //D no longer exist
    ok &= !cdb.MoveAbsolute("A.B.C.D");
    //Buf F.G should still exist
    ok &= cdb.MoveAbsolute("A.B.C.F.G");
    //Guarantee that we can recreate (only possible if it was indeed deleted)
    ok &= cdb.CreateAbsolute("A.B.C.D.E");
    ok &= cdb.MoveAbsolute("A.B.C.D.E");

    //Make sure we can delete under the root node
    ok &= cdb.MoveToRoot();
    ok &= cdb.Delete("A");
    ok = cdb.CreateAbsolute("A.B.C.D.E");
    return ok;
}

bool ConfigurationDatabaseTest::TestRead_Invalid() {
    ConfigurationDatabase cdb;
    uint32 readValue = 0;
    bool ok = !cdb.Read("", readValue);
    ok = !cdb.Read(NULL, readValue);
    ok = cdb.CreateAbsolute("A.B.C");
    uint32 writeValue = 5;
    ok &= cdb.Write("value", writeValue);
    ok &= !cdb.Read("value1", readValue);
    ok &= (writeValue != readValue);

    return ok;
}

bool ConfigurationDatabaseTest::TestRead_StructuredData() {
    ConfigurationDatabase source;
    source.CreateAbsolute("A.B.C");
    int32 x = 1;
    source.Write("x", x);
    source.CreateAbsolute("A.B.D");
    int32 y = 2;
    source.Write("y", 2);
    source.MoveToRoot();
    ConfigurationDatabase destination;
    destination.CreateAbsolute("root");
    source.Read("A", destination);
    bool ok = destination.MoveAbsolute("root.B.C");
    ok &= destination.Read("x", y);
    ok &= destination.MoveAbsolute("root.B.D");
    ok &= destination.Read("y", x);
    return (y == 1) && (x == 2);
}

bool ConfigurationDatabaseTest::TestRead_Object() {
    ConfigurationDatabase source;
    source.CreateAbsolute("A");
    source.Write("Class", "TestIntrospectionObjectFrom");
    uint32 member1 = 1;
    source.Write("member1_from", member1);
    float32 member2_x = 1;
    source.Write("member2_from", &member2_x);
    float64 member3[32];
    for (uint32 i = 0u; i < 32; i++) {
        member3[i] = i;
    }
    source.Write("member3_from", member3);
    const char8* member4[2][2];
    member4[0][0] = (char8*) "1";
    member4[0][1] = (char8*) "2";
    member4[1][0] = (char8*) "3";
    member4[1][1] = (char8*) "4";

    source.Write("member4_from", member4);
    uint32 member5Ref = 5;
    uint32 *member5 = &member5Ref;
    source.CreateRelative("member5_from");
    source.Write("Class", "TestIntrospectionNestedStructureFrom");
    source.Write("nestedMember1_from", member5);
    uint32 member6 = 12345;
    source.Write("nestedMember2_from", member6);

    TestIntrospectionObjectTo testDestination;
    // set the buffer to the pointer!
    char8 outBuff[64];
    testDestination.member5_to.nestedMember2_to = outBuff;

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    source.MoveToRoot();
    if (!source.Read("A", destination)) {
        return false;
    }
    if (StringHelper::Compare(testDestination.member1_to, "1") != 0) {
        return false;
    }
    if (testDestination.member2_to != (uintp) &member2_x) {
        return false;
    }
    for (uint32 i = 0; i < 32; i++) {
        if (testDestination.member3_to[i] != i) {
            return false;
        }
    }
    bool ok = (testDestination.member4_to[0][0] == 1);
    ok &= (testDestination.member4_to[0][1] == 2);
    ok &= (testDestination.member4_to[1][0] == 3);
    ok &= (testDestination.member4_to[1][1] == 4);

    if (!ok) {
        return false;
    }
    if (*testDestination.member5_to.nestedMember1_to != 5) {
        return false;
    }

    return StringHelper::Compare(testDestination.member5_to.nestedMember2_to, "12345") == 0;
}

bool ConfigurationDatabaseTest::TestAddToCurrentNode() {
    ConfigurationDatabase cdb;
    ReferenceT<ConfigurationDatabaseNode> obj(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    return cdb.AddToCurrentNode(obj);
}

bool ConfigurationDatabaseTest::TestAddToCurrentNode_InvalidReference() {
    ConfigurationDatabase cdb;
    ReferenceT<ReferenceContainer> obj;
    return !cdb.AddToCurrentNode(obj);
}

bool ConfigurationDatabaseTest::TestWrite_Overwrite() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A.B.C");
    uint32 writeValue = 5;
    ok &= cdb.Write("value", writeValue);
    uint32 readValue;
    ok &= cdb.Read("value", readValue);
    ok &= (writeValue == readValue);
    writeValue = 7;
    ok &= cdb.Write("value", writeValue);
    readValue = 3;
    ok &= cdb.Read("value", readValue);
    ok &= (writeValue == readValue);
    return ok;
}

bool ConfigurationDatabaseTest::TestWrite_Invalid() {
    ConfigurationDatabase cdb;
    uint32 writeValue = 5;
    bool ok = !cdb.Write("", writeValue);

    return ok;
}

bool ConfigurationDatabaseTest::TestWrite_StructuredData() {
    ConfigurationDatabase source;
    source.CreateAbsolute("A.B.C");
    int32 x = 1;
    source.Write("x", x);
    source.CreateAbsolute("A.B.D");
    int32 y = 2;
    source.Write("y", 2);
    source.MoveAbsolute("A");
    ConfigurationDatabase destination;
    destination.Write("A", source);
    bool ok = destination.MoveAbsolute("A.B.C");
    ok &= destination.Read("x", y);
    ok &= destination.MoveAbsolute("A.B.D");
    ok &= destination.Read("y", x);
    return (y == 1) && (x == 2);
}

bool ConfigurationDatabaseTest::TestWrite_Object() {
    TestIntrospectionObjectFrom sourceTest;
    sourceTest.member1_from = 1;
    float32 member2_x = 2;
    sourceTest.member2_from = &member2_x;
    for (uint32 i = 0; i < 32; i++) {
        sourceTest.member3_from[i] = i;
    }
    sourceTest.member4_from[0][0] = (char8*) "1";
    sourceTest.member4_from[0][1] = (char8*) "2";
    sourceTest.member4_from[1][0] = (char8*) "3";
    sourceTest.member4_from[1][1] = (char8*) "4";

    uint32 member5Ref = 5;
    sourceTest.member5_from.nestedMember1_from = &member5Ref;
    sourceTest.member5_from.nestedMember2_from = 12345;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &sourceTest);

    ConfigurationDatabase destination;
    if (!destination.Write("myStruct", source)) {
        return false;
    }

    destination.MoveAbsolute("myStruct");

    char8 member1[32];
    destination.Read("member1_from", member1);
    if (StringHelper::Compare(member1, "1") != 0) {
        return false;
    }

    uint64 member2;
    destination.Read("member2_from", member2);
    if (member2 != (uintp) sourceTest.member2_from) {
        return false;
    }

    float32 member3[32];
    destination.Read("member3_from", member3);
    for (uint32 i = 0; i < 32; i++) {
        if (member3[i] != i) {
            return false;
        }
    }

    uint32 member4[2][2];
    destination.Read("member4_from", member4);

    bool ok = (member4[0][0] == 1);
    ok &= (member4[0][1] == 2);
    ok &= (member4[1][0] == 3);
    ok &= (member4[1][1] == 4);

    destination.MoveRelative("member5_from");
    uint32 *member5 = NULL;
    destination.Read("nestedMember1_from", member5);

    char8 member6[32];
    destination.Read("nestedMember2_from", member6);

    return StringHelper::Compare(member6, "12345") == 0;
}

bool ConfigurationDatabaseTest::TestGetNumberOfChildren() {
    ConfigurationDatabase cdb;
    bool ok = cdb.GetNumberOfChildren() == 0;
    ok &= cdb.CreateAbsolute("A.B.C");
    ok &= cdb.CreateAbsolute("A.D");
    ok &= cdb.CreateAbsolute("A.B.E");
    ok &= cdb.CreateAbsolute("A.B.F");
    cdb.MoveToRoot();
    ok &= cdb.GetNumberOfChildren() == 1;
    cdb.MoveAbsolute("A");
    ok &= cdb.GetNumberOfChildren() == 2;
    cdb.MoveAbsolute("A.B");
    ok &= cdb.GetNumberOfChildren() == 3;
    return ok;
}

bool ConfigurationDatabaseTest::TestGetChildName() {
    const char8 *childNames[] = { "A", "B", "C", "D", 0 };
    const char8 *nephewNames[] = { "E", "F", "G", "H", 0 };
    ConfigurationDatabase cdb;
    bool ok = true;
    uint32 i = 0u;
    while (childNames[i] != NULL) {
        ok &= cdb.CreateAbsolute(childNames[i]);
        i++;
    }
    ok &= cdb.MoveAbsolute(childNames[0]);
    i = 0u;
    while (nephewNames[i] != NULL) {
        ok &= cdb.CreateRelative(nephewNames[i]);
        ok &= cdb.MoveToAncestor(1);
        i++;
    }
    ok &= cdb.MoveToRoot();
    i = 0u;
    while (childNames[i] != NULL) {
        ok &= StringHelper::Compare(childNames[i], cdb.GetChildName(i)) == 0;
        i++;
    }
    ok &= cdb.MoveAbsolute(childNames[0]);
    i = 0u;
    while (nephewNames[i] != NULL) {
        ok &= StringHelper::Compare(nephewNames[i], cdb.GetChildName(i)) == 0;
        i++;
    }
    ok &= cdb.GetChildName(i) == NULL;
    return ok;
}

bool ConfigurationDatabaseTest::TestGetName() {
    ConfigurationDatabase cdb;
    if (cdb.GetName() != NULL) {
        return false;
    }

    cdb.CreateAbsolute("A.B.C");
    cdb.MoveAbsolute("A");
    bool ret = (StringHelper::Compare(cdb.GetName(), "A") == 0);
    cdb.MoveAbsolute("A.B");
    ret &= (StringHelper::Compare(cdb.GetName(), "B") == 0);
    cdb.MoveAbsolute("A.B.C");
    ret &= (StringHelper::Compare(cdb.GetName(), "C") == 0);
    return ret;
}

bool ConfigurationDatabaseTest::TestGetType_Invalid() {
    ConfigurationDatabase cdb;
    AnyType t = cdb.GetType("");
    bool ok = (t.GetTypeDescriptor() == VoidType);
    t = cdb.GetType("Node");
    ok = (t.GetTypeDescriptor() == VoidType);
    t = cdb.GetType(NULL);
    ok = (t.GetTypeDescriptor() == VoidType);

    return ok;
}

bool ConfigurationDatabaseTest::TestCopy() {
    ConfigurationDatabase sourceCDB;
    sourceCDB.CreateAbsolute("A.B.C");
    bool ok = sourceCDB.Write("value", 1);
    sourceCDB.CreateAbsolute("A.B.D");
    ok &= sourceCDB.Write("value", 2);
    sourceCDB.CreateAbsolute("A.B.T");
    ok &= sourceCDB.Write("value", true);
    sourceCDB.CreateAbsolute("A.B.F");
    ok &= sourceCDB.Write("value", false);
    sourceCDB.CreateAbsolute("A.B.E");
    ConfigurationDatabase destinationCDB;
    destinationCDB.CreateAbsolute("B");
    ok &= sourceCDB.MoveAbsolute("A.B");
    ok &= sourceCDB.Copy(destinationCDB);
    ok &= destinationCDB.MoveRelative("C");
    ok &= !destinationCDB.MoveAbsolute("A");
    ok &= destinationCDB.MoveAbsolute("B.C");
    uint32 readValue;
    ok &= destinationCDB.Read("value", readValue);
    ok &= (readValue == 1);
    ok &= destinationCDB.MoveAbsolute("B.D");
    ok &= destinationCDB.Read("value", readValue);
    ok &= (readValue == 2);
    bool readValueB;
    ok &= destinationCDB.MoveAbsolute("B.T");
    ok &= destinationCDB.Read("value", readValueB);
    ok &= (readValueB);
    ok &= destinationCDB.MoveAbsolute("B.F");
    ok &= destinationCDB.Read("value", readValueB);
    ok &= (!readValueB);

    return ok;
}

bool ConfigurationDatabaseTest::TestLock() {
    ConfigurationDatabase cdb;
    return cdb.Lock(TTInfiniteWait);
}

bool ConfigurationDatabaseTest::TestUnlock() {
    ConfigurationDatabase cdb;
    cdb.Lock(TTInfiniteWait);
    cdb.Unlock();
    return cdb.Lock(TTInfiniteWait);
}

bool ConfigurationDatabaseTest::TestInitialise() {
    const char8 * const config = ""
            "+Config1 = {"
            "    Class = ConfigurationDatabase"
            "    param1 = \"Test\""
            "    param2 = 5"
            "}"
            "+Config2 = {"
            "    Class = ConfigurationDatabase"
            "    x = \"Test2\""
            "    y = {1 2 3 4}"
            "    z = {{1 2} {2 3} {3 4} {4 5}}"
            "    w1 = \"true\""
            "    w2 = \"yes\""
            "    w3 = \"1\""
            "    w4 = 1"
            "    w5 = \"false\""
            "}";

    ConfigurationDatabase cdb;
    StreamString configStr = config;
    configStr.Seek(0);
    StandardParser parser(configStr, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<ConfigurationDatabase> db1 = ObjectRegistryDatabase::Instance()->Find("Config1");
    if (ret) {
        ret = db1.IsValid();
    }
    if (ret) {
        StreamString param1Value;
        db1->Read("param1", param1Value);
        ret = (param1Value == "Test");
    }
    if (ret) {
        uint32 param2Value;
        db1->Read("param2", param2Value);
        ret = (param2Value == 5);
    }
    ReferenceT<ConfigurationDatabase> db2 = ObjectRegistryDatabase::Instance()->Find("Config2");
    if (ret) {
        ret = db2.IsValid();
    }
    if (ret) {
        StreamString xValue;
        db2->Read("x", xValue);
        ret = (xValue == "Test2");
    }
    if (ret) {
        Vector<uint32> yValue(4);
        db2->Read("y", yValue);
        ret = (yValue[0] == 1);
        ret &= (yValue[1] == 2);
        ret &= (yValue[2] == 3);
        ret &= (yValue[3] == 4);
    }
    if (ret) {
        Matrix<uint32> zValue(4, 2);
        db2->Read("z", zValue);
        ret = (zValue(0, 0) == 1);
        ret &= (zValue(0, 1) == 2);
        ret &= (zValue(1, 0) == 2);
        ret &= (zValue(1, 1) == 3);
        ret &= (zValue(2, 0) == 3);
        ret &= (zValue(2, 1) == 4);
        ret &= (zValue(3, 0) == 4);
        ret &= (zValue(3, 1) == 5);
    }
    if (ret) {
        bool w1;
        db2->Read("w1", w1);
        ret = (w1);
    }
    if (ret) {
        bool w2;
        db2->Read("w2", w2);
        ret = (w2);
    }
    if (ret) {
        bool w3;
        db2->Read("w3", w3);
        ret = (w3);
    }
    if (ret) {
        bool w4;
        db2->Read("w4", w4);
        ret = (w4);
    }
    if (ret) {
        bool w5;
        db2->Read("w5", w5);
        ret = (!w5);
    }

    return ret;
}

bool ConfigurationDatabaseTest::TestCleanUp() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    ok = cdb.MoveToRoot();
    ok &= cdb.Write("AAA", 5);
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.CreateAbsolute(".A.B");
    ok &= cdb.MoveAbsolute("A.B.");
    ok &= cdb.CreateAbsolute("A.B.C");
    ok &= cdb.CreateAbsolute("D.E.F");
    ok &= cdb.MoveAbsolute("D.E.F");
    ok &= cdb.MoveAbsolute("A.B.C");
    ok &= cdb.MoveToRoot();
    ok &= (cdb.GetNumberOfChildren() > 0u);
    uint32 val;
    ok &= cdb.Read("AAA", val);
    cdb.Purge();
    ok &= (cdb.GetNumberOfChildren() == 0u);
    ok &= !cdb.Read("AAA", val);
    return ok;
}

bool ConfigurationDatabaseTest::TestGetCurrentNode() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    ok = cdb.MoveToRoot();
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.CreateAbsolute(".A.B");
    ok &= cdb.CreateAbsolute("A.B.C");
    ok &= cdb.CreateAbsolute("A.B.D");
    ok &= cdb.CreateAbsolute("A.B.E");
    ok &= cdb.CreateAbsolute("A.B.F");
    ok &= cdb.MoveAbsolute("A.B");
    //ReferenceT<ConfigurationDatabaseNode> refC = cdb.GetCurrentNode();
    //return (refC->Size() == 4);
    return true;
}

bool ConfigurationDatabaseTest::TestCopyConstructor() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    ok &= cdb.CreateAbsolute("A.B");
    ok &= cdb.CreateAbsolute("A.B.C");
    ok &= cdb.CreateAbsolute("A.B.D");
    ok &= cdb.MoveAbsolute("A");
    ConfigurationDatabase cdb2 = cdb;
    ok &= cdb2.MoveAbsolute("A.B.D");
    ok &= cdb.MoveRelative("B");
    return ok;
}

bool ConfigurationDatabaseTest::TestCopyOperatorEqual() {
    return TestCopyConstructor();
}

bool ConfigurationDatabaseTest::TestMoveToChild() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    ok &= cdb.CreateAbsolute("A.B");
    ok &= cdb.CreateAbsolute("A.C");
    ok &= cdb.CreateAbsolute("A.D");
    ok &= cdb.CreateAbsolute("A.C.E");
    ok &= cdb.CreateAbsolute("A.C.F");
    ok &= cdb.MoveAbsolute("A");
    ok &= cdb.MoveToChild(1);
    ok &= cdb.MoveRelative("E");
    return ok;
}

bool ConfigurationDatabaseTest::TestSetCurrentNodeAsRootNode() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    ok &= cdb.CreateAbsolute("A.B");
    ok &= cdb.CreateAbsolute("A.B.C");
    ok &= cdb.CreateAbsolute("A.B.D");
    ok &= cdb.MoveAbsolute("A.B");
    cdb.SetCurrentNodeAsRootNode();
    ok &= cdb.MoveToRoot();
    ok &= cdb.MoveRelative("C");
    return ok;
}

bool ConfigurationDatabaseTest::TestPurge() {
    ConfigurationDatabase cdb;
    cdb.Purge();
    bool ok = true;
    ok &= cdb.Write("Node1", 1);
    ok &= cdb.Write("Node2", 2);
    ok &= (cdb.GetNumberOfChildren() == 2);
    ok &= cdb.CreateAbsolute("B");
    ok &= cdb.MoveToRoot();
    ok &= (cdb.GetNumberOfChildren() == 3);
    cdb.Purge();
    ok &= (cdb.GetNumberOfChildren() == 0);
    ok &= cdb.Write("A", 1);
    ok &= (cdb.GetNumberOfChildren() == 1);
    ok &= cdb.CreateAbsolute("B");
    ok &= (cdb.GetNumberOfChildren() == 0);
    ok &= cdb.MoveToRoot();
    ok &= (cdb.GetNumberOfChildren() == 2);
    ok &= cdb.MoveAbsolute("B");
    cdb.Purge();
    ok &= (cdb.GetNumberOfChildren() == 0);
    return ok;
}

bool ConfigurationDatabaseTest::TestCopyOperatorEqualBug() {
    ConfigurationDatabase cdb;
    bool ok = cdb.Write("Node1", 2);
    ok &= cdb.Write("Node2", 3);
    ok &= cdb.CreateAbsolute("B.C.D");
    ok &= (cdb.GetNumberOfChildren() == 0);
    ok &= (cdb.MoveToRoot());
    ok &= (cdb.GetNumberOfChildren() == 3);
    ConfigurationDatabase cdb2;
    cdb2 = cdb;
    //This second copy was incorrectly forcing a Purge. Let's make sure this is no longer the case
    cdb2 = cdb;
    uint32 node1Val = 0u;
    ok &= cdb2.Read("Node1", node1Val);
    ok &= (node1Val == 2);
    ok &= cdb.MoveAbsolute("B.C.D");

    return ok;
}
