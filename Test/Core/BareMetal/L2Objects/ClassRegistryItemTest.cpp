/**
 * @file ClassRegistryItemTest.cpp
 * @brief Source file for class ClassRegistryItemTest
 * @date 11/08/2015
 * @author Giuseppe Ferrò
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
 * the class ClassRegistryItemTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItemTest.h"
#include "MemoryCheck.h"
#include "StringHelper.h"
#include "ClassRegistryDatabase.h"
#include "IntrospectionEntry.h"
#include "Introspection.h"
#include "Object.h"
#include <typeinfo>
#include "ReferenceContainer.h"
#include "ClassWithCallableMethods.h"
#include "ErrorType.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MyObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()
};
CLASS_REGISTER(MyObject, "1.0")


class MyObject2: public Object {
public:
    CLASS_REGISTER_DECLARATION()
};
CLASS_REGISTER(MyObject2, "1.0")

class DummyObjectBuilder: public ObjectBuilderT<MyObject2> {
    Object *Build(HeapI* const heap) const {
        Object *p = (Object*) HeapManager::Malloc(sizeof(MyObject2));
        char *pp = (char*) p;
        (*pp) = 9;
        return p;
    }
};

class TestIntrospectionCRI: public Object {
public:
    CLASS_REGISTER_DECLARATION()
};
CLASS_REGISTER(TestIntrospectionCRI, "1.1")

static DummyObjectBuilder dummyObjectBuilder;

//static ClassProperties testClassPropertiesNormal("TestNormalCRI", typeid(Object).name(), "World");

//myItem cannot be destroyed until the end of the execution of the program.
static ClassRegistryItem myItem = *(ClassRegistryItemT<MyObject2>::Instance());

//static ClassProperties testClassPropertiesIntro("TestIntrospectionCRI", "TestIntrospectionCRI", "1.1");

static IntrospectionEntry member1Field("member1", "uint32", "", "", 4, 0);

static const IntrospectionEntry* fields[] = { &member1Field, 0 };
static Introspection introspectionTest(fields, 4);

static ClassRegistryItem myItemIntro = *(ClassRegistryItemT<TestIntrospectionCRI>::Instance());

static ClassRegistryItem myItemFull = *(ClassRegistryItemT<MyObject>::Instance());

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ClassRegistryItemTest::TestConstructor() {

    // checks the attributes.
    if (myItem.GetNumberOfInstances() != 0) {
        return false;
    }

    if (myItem.GetLoadableLibrary() != NULL) {
        return false;
    }

    if (myItem.GetIntrospection() != NULL) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetName(), "MyObject2") != 0) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetTypeIdName(), typeid(MyObject2).name()) != 0) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetVersion(), "1.0") != 0) {
        return false;
    }

    if (myItem.GetObjectBuilder() != &dummyObjectBuilder) {
        return false;
    }

    //checks if the class is in the database
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (db->Find("TestNormalCRI") != NULL) {
        return false;
    }

    HeapI * h = NULL;

    //check if the correct function is saved
    Object *instance = myItem.GetObjectBuilder()->Build(h);

    if (instance == NULL) {
        return false;
    }

    bool retVal = true;
    if ((*((char*) instance)) != 9) {
        retVal = false;
    }

    HeapManager::Free((void*&) instance);


    myItemIntro.SetIntrospection(&introspectionTest);

    myItemFull.SetIntrospection(&introspectionTest);
//    myItemFull.SetObjectBuilder(&dummyObjectBuilder);


    return retVal;

}

bool ClassRegistryItemTest::TestIntrospectionConstructor() {
    // checks the attributes.
    if (myItemIntro.GetNumberOfInstances() != 0) {
        return false;
    }

    if (myItemIntro.GetLoadableLibrary() != NULL) {
        return false;
    }

    if (myItemIntro.GetIntrospection() != &introspectionTest) {
        return false;
    }

    if (StringHelper::Compare((myItemIntro.GetClassProperties())->GetName(), "TestIntrospectionCRI") != 0) {
        return false;
    }

    if (StringHelper::Compare((myItemIntro.GetClassProperties())->GetTypeIdName(), typeid(TestIntrospectionCRI).name()) != 0) {
        return false;
    }

    if (StringHelper::Compare((myItemIntro.GetClassProperties())->GetVersion(), "1.1") != 0) {
        return false;
    }

    if (myItemIntro.GetObjectBuilder() == NULL) {
        return false;
    }
    //checks if the class is in the database
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (db->Find("TestIntrospectionCRI") == NULL) {
        return false;
    }
    return true;
}

bool ClassRegistryItemTest::TestFullConstructor() {
    // checks the attributes.
    if (myItemFull.GetNumberOfInstances() != 0) {
        return false;
    }

    if (myItemFull.GetLoadableLibrary() != NULL) {
        return false;
    }

    if (myItemFull.GetIntrospection() != &introspectionTest) {
        return false;
    }

    if (StringHelper::Compare((myItemFull.GetClassProperties())->GetName(), "MyObject") != 0) {
        return false;
    }

    if (StringHelper::Compare((myItemFull.GetClassProperties())->GetTypeIdName(), typeid(MyObject).name()) != 0) {
        return false;
    }

    if (StringHelper::Compare((myItemFull.GetClassProperties())->GetVersion(), "1.0") != 0) {
        return false;
    }

    //checks if the class is in the database
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (db->Find("TestIntrospectionCRI") == NULL) {
        return false;
    }
    return true;
}

//bool ClassRegistryItemTest::TestDestructor() {
//
//    //Checks if the class is in the database. The item cannot be destroyed until the end of the execution of the program.
//    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();
//
//    if (db->Find("TestNormalCRI") != NULL) {
//        return false;
//    }
//
//    //Create a LoadableLibray
//    const LoadableLibrary *dummy = new LoadableLibrary();
//    myItem.SetLoadableLibrary(dummy);
//
//    myItem.~ClassRegistryItem();
//    dummy = myItem.GetLoadableLibrary();
//
//    //checks that dummy was destructed
//    return (dummy == NULL);
//}

bool ClassRegistryItemTest::TestIncrementNumberOfInstances() {

    if (myItem.GetNumberOfInstances() != 0) {
        return false;
    }

    uint32 currentInstances = myItem.GetNumberOfInstances();

    myItem.IncrementNumberOfInstances();

    return myItem.GetNumberOfInstances() == (currentInstances + 1);

}

bool ClassRegistryItemTest::TestDecrementNumberOfInstances() {

    uint32 currentInstances = myItem.GetNumberOfInstances();

    myItem.IncrementNumberOfInstances();

    myItem.DecrementNumberOfInstances();

    return myItem.GetNumberOfInstances() == currentInstances;

}

bool ClassRegistryItemTest::TestGetNumberOfInstances(uint32 nInstances) {

    uint32 currentInstances = myItem.GetNumberOfInstances();

    for (uint32 i = 0; i < nInstances; i++) {
        myItem.IncrementNumberOfInstances();
    }

    if (myItem.GetNumberOfInstances() != (currentInstances + nInstances)) {
        return false;
    }

    for (uint32 i = 0; i < nInstances; i++) {
        myItem.DecrementNumberOfInstances();
    }

    return myItem.GetNumberOfInstances() == currentInstances;
}

bool ClassRegistryItemTest::TestGetClassPropertiesCopy() {

    ClassProperties propertiesCopy("TestNormalCRI", "TestNormalCRI", "World");

    myItem.GetClassPropertiesCopy(propertiesCopy);

    bool ok = (StringHelper::Compare(propertiesCopy.GetName(), "MyObject2") == 0);
    ok &= (StringHelper::Compare(propertiesCopy.GetTypeIdName(), typeid(MyObject2).name()) == 0);
    ok &= (StringHelper::Compare(propertiesCopy.GetVersion(), "1.0") == 0);
    return ok;
}

bool ClassRegistryItemTest::TestGetClassProperties() {

    const ClassProperties *propertiesCopy = myItem.GetClassProperties();

    bool ok = (StringHelper::Compare(propertiesCopy->GetName(), "MyObject2") == 0);
    ok &= (StringHelper::Compare(propertiesCopy->GetTypeIdName(), typeid(MyObject2).name()) == 0);
    ok &= (StringHelper::Compare(propertiesCopy->GetVersion(), "1.0") == 0);
    return ok;
}

bool ClassRegistryItemTest::TestSetGetLoadableLibrary(const char8 *llname) {

    // the library is automatically deleted
    LoadableLibrary *myLib = new LoadableLibrary;

    myLib->Open(llname);

    myItem.SetLoadableLibrary(myLib);

    if (myItem.GetLoadableLibrary() != myLib) {
        return false;
    }
    return true;
}

bool ClassRegistryItemTest::TestGetObjectBuildFunction() {

    if (myItem.GetObjectBuilder() != &dummyObjectBuilder) {
        return false;
    }

    HeapI * h = NULL;
    //call the function to see if it behaves as expected
    Object* instance = myItem.GetObjectBuilder()->Build(h);
    bool retVal = true;
    if ((*((char*) instance)) != 9) {
        retVal = false;
    }

    HeapManager::Free((void*&) instance);
    return retVal;

}

bool ClassRegistryItemTest::TestGetIntrospection() {

    return myItemIntro.GetIntrospection() == &introspectionTest;
}

bool ClassRegistryItemTest::TestCallRegisteredMethod() {
    bool result = true;
    ClassRegistryItem* target = ClassRegistryItemT<ClassWithCallableMethods>::Instance();
    ReferenceContainer params;
    ClassWithCallableMethods context;
    ErrorManagement::ErrorType status;
    status = target->CallRegisteredMethod(&context, "NonRegisteredMethod", params);
    result &= status.unsupportedFeature;
    status = target->CallRegisteredMethod(&context, "MethodK", params);
    result &= status.functionError;
    status = target->CallRegisteredMethod(&context, "MethodX", params);
    result &= status;
    status = target->CallRegisteredMethod(&context, "MethodY", params);
    result &= status;
    status = target->CallRegisteredMethod(&context, "MethodZ", params);
    result &= status;
    return result;
}

bool ClassRegistryItemTest::TestSetUniqueIdentifier(uint32 uid) {
    bool ret = true;

    ClassUID prev = myItem.GetClassProperties()->GetUniqueIdentifier();
    myItem.SetUniqueIdentifier(prev+1);
    if (myItem.GetClassProperties()->GetUniqueIdentifier() != (prev + 1)) {
        ret = false;
    }
    myItem.SetUniqueIdentifier(prev);
    return ret;
}

