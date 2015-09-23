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
#include <typeinfo>

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

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

bool ClassRegistryDatabaseTest::TestAdd() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    uint32 sizeDB = db->GetSize();

    ClassProperties testClassProperties("TestAdd", "TestAdd", "V");

    //The add function is called directly by the constructor. It cannot be deleted before the execution of the program.
    ClassRegistryItem *myItem = new ClassRegistryItem(testClassProperties, NULL);

    uint32 newSizeDB = db->GetSize();

    if (sizeDB != (newSizeDB - 1)) {
        return false;
    }

    const ClassRegistryItem *ret = db->Find("TestAdd");

    bool retVal = (StringHelper::Compare(ret->GetClassProperties()->GetName(), "TestAdd") == 0);
    retVal &= (StringHelper::Compare(ret->GetClassProperties()->GetVersion(), "V") == 0);

    return retVal;
}


bool ClassRegistryDatabaseTest::TestFindDLL(const char8* dllName,
                                            const char8* className,
                                            bool validName) {

    char8 fullName[64];
    StringHelper::Copy(fullName, dllName);
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    StringHelper::Concatenate(fullName, "::");
    StringHelper::Concatenate(fullName, className);

    return !(validName ^ (db->Find(fullName) != NULL));
}

bool ClassRegistryDatabaseTest::TestFind(const char8 *name,
                                         bool create) {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (create) {
        ClassProperties testClassProperties(name, "", "V");

        //The add function is called directly by the constructor. It cannot be deleted before the execution of the program.
        ClassRegistryItem *myItem = new ClassRegistryItem(testClassProperties, NULL);
        bool found = (db->Find(name) != NULL);
        //These are deleted by the the ClassRegistryDatabase destructor
        return found;
    }

    return (db->Find(name) == NULL);

}

bool ClassRegistryDatabaseTest::TestFindTypeIdName() {

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    bool found = (db->FindTypeIdName(typeid(Object).name()) != NULL);
    //These are deleted by the the ClassRegistryDatabase destructor
    return found;
}

bool ClassRegistryDatabaseTest::TestGetSize() {

    const char* names[] = { "1", "2", "3", "4", NULL };

    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    uint32 prevSize = db->GetSize();

    ClassRegistryItem *root;

    uint32 i = 0;
    //add the elements to the database.
    while (names[i] != NULL) {
        ClassProperties *testClassProperties = new ClassProperties(names[i], names[i], "V");
        ClassRegistryItem *element = new ClassRegistryItem(*testClassProperties, NULL);
        delete testClassProperties;
        if (i == 0) {
            root = element;
        }
        i++;
    }

    return ((db->GetSize() - prevSize) == 4);
}

bool ClassRegistryDatabaseTest::TestPeek() {
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();
    ClassProperties testClassProperties("TestElementAt", "TestElementAt", "V");

    //the add function is called directly by the constructor. myItem cannot be deleted before the end of the program.
    ClassRegistryItem *myItem = new ClassRegistryItem(testClassProperties, NULL);
    const ClassRegistryItem *peekedItem = db->Peek(db->GetSize() - 1);
    bool ok = (peekedItem == myItem);

    uint32 peededItemUniqueIdentifier = peekedItem->GetClassProperties()->GetUniqueIdentifier();
    ok &= (peededItemUniqueIdentifier == (db->GetSize() - 1));

    return ok;
}

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
