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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

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
    ReferenceT<IntegerObject> integerRef = ReferenceT<IntegerObject>("IntegerObject");

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

    ClassRegistryItem *fakePtr = NULL;
    //checks if the class is in the database
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    return (!db->Delete(fakePtr));

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
    bool retVal = (StringHelper::Compare(ret->GetClassProperties()->GetName(), "Hello") == 0)
            && (StringHelper::Compare(ret->GetClassProperties()->GetVersion(), "Mondo") == 0);

    return retVal && db->Delete(ret);

}

bool ClassRegistryDatabaseTest::TestFindDLL(const char8* dllName,
                                            const char8* className,
                                            bool validName) {

    char8 fullName[64];
    StringHelper::Copy(fullName, dllName);
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    StringHelper::Concatenate(fullName, "::");
    StringHelper::Concatenate(fullName, className);

    return !(validName ^ (db->Find(fullName) != NULL));
}

bool ClassRegistryDatabaseTest::TestFind(const char8 *name,
                                         bool create) {

    ClassRegistryDatabase* db = &(ClassRegistryDatabase::Instance());

    if (create) {
        ClassProperties testClassProperties(name, "V");

        //the add function is called directly by the constructor
        ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, NULL);
        return (db->Find(name) != NULL) && (db->Delete(&myItem));
    }

    return (db->Find(name) == NULL);

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

bool ClassRegistryDatabaseTest::TestSize() {

    const char* names[] = { "1", "2", "3", "4", NULL };

    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    uint32 prevSize = db->Size();

    ClassRegistryItem *root;

    uint32 i = 0;
//add the elements to the database.
    while (names[i] != NULL) {
        ClassProperties *testClassProperties = new ClassProperties(names[i], "V");
        ClassRegistryItem *element = new ClassRegistryItem(*testClassProperties, NULL);
        delete testClassProperties;
        if (i == 0) {
            root = element;
        }
        i++;
    }

    if ((db->Size() - prevSize) != 4) {
        return false;
    }

    i = 0;
    while (names[i] != NULL) {
        ClassRegistryItem *newRoot = (ClassRegistryItem *) root->Next();
        db->Delete(root);
        root = newRoot;
        i++;
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

bool ClassRegistryDatabaseTest::TestCreateInstances() {
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");

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

    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");

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
    ClassRegistryDatabase *db = &(ClassRegistryDatabase::Instance());

    ClassRegistryItem *integerObjProp = db->Find("IntegerObject");
    ClassRegistryItem *specialIntegerObjProp = db->Find("SpecialIntegerObject");

    ReferenceT<SpecialIntegerObject> father2child = ReferenceT<SpecialIntegerObject>("IntegerObject");

    //the dynamic cast fails, the object is created but destroyed immediately.
    if (integerObjProp->GetNumberOfInstances() != 0) {
        return false;
    }

    //the reference should be invalid!
    return (!father2child.IsValid()) && (specialIntegerObjProp->GetNumberOfInstances() == 0);

}
