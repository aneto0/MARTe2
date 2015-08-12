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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include "Reference.h"
#include "TestObjectHelper1.h"
#include "StringHelper.h"

bool ClassRegistryDatabaseTest::TestInstance() {

    ClassRegistryDatabase db = ClassRegistryDatabase::Instance();

    // see ObjectTestHelper: IntegerObject should be registered.

    ClassRegistryItem *integerItem = db.Find("IntegerObject");

    if (integerItem == NULL) {
        return false;
    }

    //none has still instantiated it
    if (integerItem->GetNumberOfInstances() != 0) {
        return false;
    }

    //creates an instance
    Heap h;
    ReferenceT<IntegerObject> integerRef = ReferenceT<IntegerObject>("IntegerObject", h);

    //none has still instantiated it
    if (integerItem->GetNumberOfInstances() != 1) {
        return false;
    }

    //gets again the database and checks that is the same instance
    ClassRegistryDatabase db1 = ClassRegistryDatabase::Instance();

    ClassRegistryItem *integerItemPtr = db1.Find("IntegerObject");

    //none has still instantiated it
    return (integerItem->GetNumberOfInstances() == 1) && (integerItemPtr != NULL);
}

bool ClassRegistryDatabaseTest::TestDeleteTrue() {
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    ClassProperties testClassProperties("TestDeleteTrue", "");

    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, NULL);

    uint32 size = db->Size();

    //checks if the class is in the database

    if (db->Find("TestDeleteTrue") == NULL) {
        return false;
    }

    if (!db->Delete(&myItem)) {
        return false;
    }

    if ((size - 1) != db->Size()) {
        return false;
    }

    return db->Find("TestDeleteTrue") == NULL;
}

bool ClassRegistryDatabaseTest::TestDeleteFalse() {

    ClassRegistryItem *fakePtr;
    //checks if the class is in the database
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    if (db->Delete(fakePtr)) {
        return false;
    }

    return !db->Delete(NULL);
}

bool ClassRegistryDatabaseTest::TestAdd() {

    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    uint32 sizeDB = db->Size();

    ClassProperties testClassProperties("TestAdd", "V");

    //the add function is called directly by the constructor
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, NULL);

    uint32 newSizeDB = db->Size();

    if (sizeDB != (newSizeDB - 1)) {
        return false;
    }

    ClassRegistryItem *ret = db->Find("TestAdd");

    bool retVal = (StringHelper::Compare(ret->GetClassProperties()->GetName(), "TestAdd") == 0)
            && (StringHelper::Compare(ret->GetClassProperties()->GetVersion(), "V") == 0);

    return db->Delete(ret) && retVal;
}

bool ClassRegistryDatabaseTest::TestAddTheSameName() {

    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    uint32 sizeDB = db->Size();

    ClassProperties testClassProperties1("Hello", "World");
    ClassProperties testClassProperties2("Hello", "Mondo");

    //the add function is called directly by the constructor
    ClassRegistryItem myItem1 = ClassRegistryItem(testClassProperties1, NULL);
    ClassRegistryItem myItem2 = ClassRegistryItem(testClassProperties2, NULL);

    uint32 newSizeDB = db->Size();

    if (sizeDB != (newSizeDB - 1)) {
        return false;
    }

    ClassRegistryItem *ret = db->Find("Hello");
///////////////////////////////////////////////////// BUG
    bool retVal = (StringHelper::Compare(ret->GetClassProperties()->GetName(), "Hello") == 0)
            && (StringHelper::Compare(ret->GetClassProperties()->GetVersion(), "Mondo") == 0);

    return retVal && db->Delete(ret);

}

bool ClassRegistryDatabaseTest::TestFindDLL(const char8* dllname) {

    char8 fullName[32];
    StringHelper::Copy(fullName, dllname);
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    StringHelper::Concatenate(fullName, "::NoClassName");

    return !(db->Find(fullName));
}

bool ClassRegistryDatabaseTest::TestFind(const char8 *name) {

    ClassProperties testClassProperties(name, "V");

    //the add function is called directly by the constructor
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, NULL);
    ClassRegistryDatabase* db = &(ClassRegistryDatabase::Instance());

    return (db->Find(name)) && (db->Delete(&myItem));

}

bool ClassRegistryDatabaseTest::TestList() {

    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    if (db->List() != db->ElementAt(0)) {
        return false;
    }
    ClassProperties testClassProperties("TestList", "V");

    //the add function is called directly by the constructor
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, NULL);

    uint32 size = db->Size();

    ClassRegistryItem *cursor = db->List();

    for (uint32 i = 0; i < (size - 1); i++) {

        cursor = (ClassRegistryItem*) cursor->Next();
    }

    if (cursor == NULL) {
        return true;
    }
    bool retVal = (StringHelper::Compare(cursor->GetClassProperties()->GetName(), "TestList") == 0)
            && (StringHelper::Compare(cursor->GetClassProperties()->GetVersion(), "V") == 0);

    return retVal && (db->Delete(&myItem));
}

bool ClassRegistryDatabaseTest::TestSize(uint32 nElements) {

    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    uint32 prevSize = db->Size();

    ClassRegistryItem *root;
    ClassProperties testClassProperties("TestList", "V");

    char s[2];
    //add the elements to the database.
    for (uint32 i = 0; i < nElements; i++) {
        s[0] = (char) i + '0';
        s[1] = '\0';
        ClassProperties *testClassProperties = new ClassProperties(s, "V");
        ClassRegistryItem *element = new ClassRegistryItem(*testClassProperties, NULL);
        delete testClassProperties;
        if (i == 0) {
            root = element;
        }
    }

    for (uint32 i = 0; i < nElements; i++) {
        ClassRegistryItem *newRoot = (ClassRegistryItem *) root->Next();
        db->Delete(root);
        root = newRoot;
    }

    return db->Size() == prevSize;

}

bool ClassRegistryDatabaseTest::TestElementAt() {
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());
    ClassProperties testClassProperties("TestElementAt", "V");

    //the add function is called directly by the constructor
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, NULL);

    return (db->List() == db->ElementAt(0)) && (db->ElementAt(db->Size() - 1) == &myItem) && (db->ElementAt(db->Size()) == NULL);

}

bool ClassRegistryDatabaseTest::TestInstances() {
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");
    ClassRegistryItem *floatObjProp = db->Find("FloatObject");
    ClassRegistryItem *doubleObjProp = db->Find("DoubleObject");

    if ((integerObjProp == NULL) || (specialIntegerObjProp == NULL) || (floatObjProp == NULL) || (doubleObjProp == NULL)) {
        return false;
    }

    if ((integerObjProp->GetNumberOfInstances() != 0) || (specialIntegerObjProp->GetNumberOfInstances() != 0) || (floatObjProp->GetNumberOfInstances() != 0)
            || (doubleObjProp->GetNumberOfInstances() != 0)) {
        return false;
    }

    Heap h;
    ReferenceT<CollectInts> refCollectInts = ReferenceT<CollectInts>("CollectInts", h);

    if ((integerObjProp->GetNumberOfInstances() != 1) || (specialIntegerObjProp->GetNumberOfInstances() != 1)) {
        return false;
    }

    if (db->Find("CollectInts")->GetNumberOfInstances() != 1) {
        return false;
    }

    ReferenceT<CollectInts> refCollectFloats = ReferenceT<CollectInts>("CollectFloats", h);

    if ((floatObjProp->GetNumberOfInstances() != 1) || (doubleObjProp->GetNumberOfInstances() != 1)) {
        return false;
    }

    if (db->Find("CollectFloats")->GetNumberOfInstances() != 1) {
        return false;
    }
    return true;


}

