/**
 * @file ConfigurationDatabase.cpp
 * @brief Source file for class ConfigurationDatabase
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
#include "ConfigurationDatabaseTest.h"
#include "AnyObject.h"

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
    uint32 readValue;
    bool ok = !cdb.Read("", readValue);
    ok = !cdb.Read(NULL, readValue);
    ok = cdb.CreateAbsolute("A.B.C");
    uint32 writeValue = 5;
    ok &= cdb.Write("value", writeValue);
    ok &= !cdb.Read("value1", readValue);
    ok &= (writeValue != readValue);

    return ok;
}

bool ConfigurationDatabaseTest::TestAddToCurrentNode() {
    ConfigurationDatabase cdb;
    ReferenceT<ReferenceContainer> obj(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
    ConfigurationDatabase destinationCDB;
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
    return ok;
}

bool ConfigurationDatabaseTest::TestLock() {
    ConfigurationDatabase cdb;
    return cdb.Lock(TTInfiniteWait);
}

bool ConfigurationDatabaseTest::TestUnlock() {
    ConfigurationDatabase cdb;
    bool ok = cdb.Lock(TTInfiniteWait);
    cdb.Unlock();
    return cdb.Lock(TTInfiniteWait);
}
