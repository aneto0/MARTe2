/**
 * @file ClassRegistryDatabaseTest.cpp
 * @brief Source file for class ClassRegistryDatabaseTest
 * @date 06/08/2015
 * @author Andre' Neto
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
 * the class ClassRegistryDatabaseTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryDatabaseTest.h"
#include "ReferenceT.h"
#include "ObjectTestHelper.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//static ClassProperties testClassProperties("TestAdd", "TestAdd", "V");
//static ClassRegistryItemT<Object> myItem(testClassProperties);
ClassProperties testClassPropertiesLongName(
        "abcdefghijklmnopqrstuvxyzaaabacadafagahaiajakalamanaoapaqarasatauavaxayazbabbbcbdbfbgbhbibjbkblbmbnbobpbqbrbsbtbubvbwbxbybzcacbcccdcfcgchcicjckclcmcncocp::asdf",
        "", "V");

//The add function is called directly by the constructor. It cannot be deleted before the execution of the program.
//ClassRegistryItemT<Object> myItemLongName(testClassPropertiesLongName);

class DummyClassRegistryDatabase: public ClassRegistryDatabase {
public:


    DummyClassRegistryDatabase() {

    }
    virtual ~DummyClassRegistryDatabase() {
        CleanUp();
    }

};


class DummyClassRegistryItem: public ClassRegistryItem {
public:


    DummyClassRegistryItem(ClassProperties &cp):ClassRegistryItem(cp) {

    }
    virtual ~DummyClassRegistryItem() {
    }

};


class ObjToCleanUp: public Object {
    ObjToCleanUp() {

    }
    virtual ~ObjToCleanUp() {

    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ClassRegistryDatabaseTest::TestInstance() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    //ObjectTestHelper: IntegerObject should be registered.
    const ClassRegistryItem *integerItem = db->Find("IntegerObject");

    if (integerItem == NULL) {
        return false;
    }

    //No one has still instantiated it, so the number of instance should be zero
    if (integerItem->GetNumberOfInstances() != 0) {
        return false;
    }

    //creates an instance
    ReferenceT<IntegerObject> integerRef = ReferenceT<IntegerObject>("IntegerObject");

    //none has still instantiated it
    if (integerItem->GetNumberOfInstances() != 1) {
        return false;
    }

    //gets again the database and checks that it is the same instance
    ClassRegistryDatabase *db1 = ClassRegistryDatabase::Instance();

    const ClassRegistryItem *integerItemPtr = db1->Find("IntegerObject");

    //Only one instance should exist and it should not be null
    bool ok = (integerItem->GetNumberOfInstances() == 1);
    ok &= (integerItemPtr != NULL);
    return ok;
}


bool ClassRegistryDatabaseTest::TestFindDLL(const MARTe::char8* dllName,
                                            const MARTe::char8* className,
                                            bool validName) {

    char8 fullName[64];
    StringHelper::Copy(fullName, dllName);
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    StringHelper::Concatenate(fullName, "::");
    StringHelper::Concatenate(fullName, className);

    return !(validName ^ (db->Find(fullName) != NULL));
}

bool ClassRegistryDatabaseTest::TestFind() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    bool found = (db->Find("IntegerObject") != NULL);
//    //These are deleted by the the ClassRegistryDatabase destructor
    return found;
}

bool ClassRegistryDatabaseTest::TestFindLongName() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();
    const char *name =
            "abcdefghijklmnopqrstuvxyzaaabacadafagahaiajakalamanaoapaqarasatauavaxayazbabbbcbdbfbgbhbibjbkblbmbnbobpbqbrbsbtbubvbwbxbybzcacbcccdcfcgchcicjckclcmcncocp::asdf";

    return (db->Find(name) != NULL);

}

bool ClassRegistryDatabaseTest::TestFindTypeIdName() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    bool found = (db->FindTypeIdName(typeid(Object).name()) != NULL);
    //These are deleted by the the ClassRegistryDatabase destructor
    return found;
}

bool ClassRegistryDatabaseTest::TestGetSize() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    return db->GetSize() > 0u;
}

//bool ClassRegistryDatabaseTest::TestPeek() {
//    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();
//
//    const ClassRegistryItem *testItem = db->Find("TestAdd");
//
//    const ClassRegistryItem *peekedItem = db->Peek(testItem->GetClassProperties()->GetUniqueIdentifier());
//    return (peekedItem == testItem);
//
//}

bool ClassRegistryDatabaseTest::TestCreateInstances() {
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    const ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    const ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");

    if ((integerObjProp == NULL) || (specialIntegerObjProp == NULL)) {
        return false;
    }

    if ((integerObjProp->GetNumberOfInstances() != 0) || (specialIntegerObjProp->GetNumberOfInstances() != 0)) {
        return false;
    }

    const uint32 instances = 32;
    ReferenceT<IntegerObject> refInts[instances];

    for (uint32 i = 0; i < instances; i++) {
        refInts[i] = ReferenceT<IntegerObject>("IntegerObject");
    }

    if (integerObjProp->GetNumberOfInstances() != instances) {
        return false;
    }

    for (uint32 i = 0; i < instances; i++) {
        refInts[i].RemoveReference();

        if (integerObjProp->GetNumberOfInstances() != (instances - 1 - i)) {
            return false;
        }
    }

//the internal objects are not counted as instances.
    ReferenceT<CollectInts> refCollectInts = ReferenceT<CollectInts>("CollectInts");

    if ((integerObjProp->GetNumberOfInstances() != 0) || (specialIntegerObjProp->GetNumberOfInstances() != 0)) {
        return false;
    }

    return (db->Find("CollectInts")->GetNumberOfInstances() == 1);
}

bool ClassRegistryDatabaseTest::TestPolimorphismChild2Father() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    const ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    const ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");

    ReferenceT<IntegerObject> child2father = ReferenceT<IntegerObject>("SpecialIntegerObject");

    //only the named class #instances must be incremented
    if ((specialIntegerObjProp->GetNumberOfInstances() != 1) || (integerObjProp->GetNumberOfInstances() != 0)) {
        return false;
    }

    if (child2father.NumberOfReferences() != 1) {
        return false;
    }

    child2father.RemoveReference();
    return (specialIntegerObjProp->GetNumberOfInstances() == 0) && (integerObjProp->GetNumberOfInstances() == 0);

}

bool ClassRegistryDatabaseTest::TestPolimorphismFather2Child() {
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    const ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    const ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");

    ReferenceT<SpecialIntegerObject> father2child = ReferenceT<SpecialIntegerObject>("IntegerObject");

    //the dynamic cast fails, the object is created but destroyed immediately.
    if (integerObjProp->GetNumberOfInstances() != 0) {
        return false;
    }

    //the reference should be invalid!
    return (!father2child.IsValid()) && (specialIntegerObjProp->GetNumberOfInstances() == 0);

}

bool ClassRegistryDatabaseTest::TestGetClassName() {
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();
    return (StringHelper::Compare(db->GetClassName(), "ClassRegistryDatabase") == 0);
}

bool ClassRegistryDatabaseTest::TestCleanUp() {
    DummyClassRegistryDatabase testDB;
    ClassProperties cp;
    ClassRegistryItem* p = new DummyClassRegistryItem(cp);
    testDB.Add(p);
    if (testDB.GetSize() != 1) {
        return false;
    }

    testDB.CleanUp();
    return testDB.GetSize() == 0;
}
