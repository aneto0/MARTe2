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
#include <typeinfo>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
Object* dummyBuildFcn(HeapI * const h) {
    Object *p = (Object*) HeapManager::Malloc(sizeof(Object));
    char *pp = (char*) p;
    (*pp) = 9;
    return p;
}

bool ClassRegistryItemTest::TestConstructor() {

    ClassProperties testClassProperties("Hello", typeid(Object).name(), "World");

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

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

    if (StringHelper::Compare((myItem.GetClassProperties())->GetName(), "Hello") != 0) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetTypeIdName(), typeid(Object).name()) != 0) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetVersion(), "World") != 0) {
        return false;
    }

    //checks if the class is in the database
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (db->Find("Hello") == NULL) {
        return false;
    }

    HeapI * h = NULL;

    //check if the correct function is saved
    Object *instance = myItem.GetObjectBuildFunction()(h);

    if (instance == NULL) {
        return false;
    }

    bool retVal = true;
    if ((*((char*) instance)) != 9) {
        retVal = false;
    }

    HeapManager::Free((void*&) instance);

    return retVal;

}

bool ClassRegistryItemTest::TestIntrospectionCostructor() {
    ClassProperties testClassProperties("TestIntrospectionCRI", "TestIntrospectionCRI", "1.1");

    IntrospectionEntry member1Field("member1", "uint32",  "", "", 4, 0);

    const IntrospectionEntry* fields[] = { &member1Field, 0 };
    Introspection introspectionTest(fields);

    ClassRegistryItem myItem(testClassProperties, introspectionTest);
    // checks the attributes.
    if (myItem.GetNumberOfInstances() != 0) {
        return false;
    }

    if (myItem.GetLoadableLibrary() != NULL) {
        return false;
    }

    if (myItem.GetIntrospection() != &introspectionTest) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetName(), "TestIntrospectionCRI") != 0) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetTypeIdName(), "TestIntrospectionCRI") != 0) {
        return false;
    }

    if (StringHelper::Compare((myItem.GetClassProperties())->GetVersion(), "1.1") != 0) {
        return false;
    }

    if (myItem.GetObjectBuildFunction() != NULL) {
        return false;
    }
    //checks if the class is in the database
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (db->Find("TestIntrospectionCRI") == NULL) {
        return false;
    }
    return true;
}

bool ClassRegistryItemTest::TestDestructor() {
    ClassProperties testClassProperties("Hello", "", "World");

    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    //Checks if the class is in the database. The item cannot be destroyed until the end of the execution of the program.
    ClassRegistryDatabase *db = ClassRegistryDatabase::Instance();

    if (db->Find("Hello") == NULL) {
        return false;
    }

    //Create a LoadableLibray
    const LoadableLibrary *dummy = new LoadableLibrary();
    myItem.SetLoadableLibrary(dummy);

    myItem.~ClassRegistryItem();
    dummy = myItem.GetLoadableLibrary();

    //checks that dummy was destructed
    return (dummy == NULL);
}

bool ClassRegistryItemTest::TestIncrementNumberOfInstances() {

    ClassProperties testClassProperties("Hello", "", "World");

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    if (myItem.GetNumberOfInstances() != 0) {
        return false;
    }

    myItem.IncrementNumberOfInstances();

    return myItem.GetNumberOfInstances() == 1;

}

bool ClassRegistryItemTest::TestDecrementNumberOfInstances() {

    ClassProperties testClassProperties("Hello", "", "World");

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    if (myItem.GetNumberOfInstances() != 0) {
        return false;
    }

    myItem.IncrementNumberOfInstances();

    if (myItem.GetNumberOfInstances() != 1) {
        return false;
    }

    myItem.DecrementNumberOfInstances();

    return myItem.GetNumberOfInstances() == 0;

}

bool ClassRegistryItemTest::TestGetNumberOfInstances(uint32 nInstances) {

    ClassProperties testClassProperties("Hello", "", "World");

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    for (uint32 i = 0; i < nInstances; i++) {
        myItem.IncrementNumberOfInstances();
    }

    if (myItem.GetNumberOfInstances() != nInstances) {
        return false;
    }

    for (uint32 i = 0; i < nInstances; i++) {
        myItem.DecrementNumberOfInstances();
    }

    return myItem.GetNumberOfInstances() == 0;
}

bool ClassRegistryItemTest::TestGetClassPropertiesCopy(const char8* name,
                                                       const char8* typeidName,
                                                       const char8* version) {

    ClassProperties testClassProperties(name, typeidName, version);

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    ClassProperties propertiesCopy("Hello", "Hello", "World");

    myItem.GetClassPropertiesCopy(propertiesCopy);

    bool ok = (name == NULL ? propertiesCopy.GetName() == NULL : StringHelper::Compare(propertiesCopy.GetName(), name) == 0);
    ok &= (typeidName == NULL ? propertiesCopy.GetTypeIdName() == NULL : StringHelper::Compare(propertiesCopy.GetTypeIdName(), typeidName) == 0);
    ok &= (version == NULL ? propertiesCopy.GetVersion() == NULL : StringHelper::Compare(propertiesCopy.GetVersion(), version) == 0);
    return ok;
}

bool ClassRegistryItemTest::TestGetClassProperties(const char8* name,
                                                   const char8* typeidName,
                                                   const char8* version) {

    ClassProperties testClassProperties(name, typeidName, version);

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    const ClassProperties *propertiesCopy = myItem.GetClassProperties();

    bool ok = (name == NULL ? propertiesCopy->GetName() == NULL : StringHelper::Compare(propertiesCopy->GetName(), name) == 0);
    ok &= (typeidName == NULL ? propertiesCopy->GetTypeIdName() == NULL : StringHelper::Compare(propertiesCopy->GetTypeIdName(), typeidName) == 0);
    ok &= (version == NULL ? propertiesCopy->GetVersion() == NULL : StringHelper::Compare(propertiesCopy->GetVersion(), version) == 0);
    return ok;
}

bool ClassRegistryItemTest::TestSetGetLoadableLibrary(const char8 *llname) {

    ClassProperties testClassProperties("Hello", "", "World");

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

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

    ClassProperties testClassProperties("Hello", "", "World");

    //myItem cannot be destroyed until the end of the execution of the program.
    ClassRegistryItem myItem = ClassRegistryItem(testClassProperties, dummyBuildFcn);

    if (myItem.GetObjectBuildFunction() != dummyBuildFcn) {
        return false;
    }

    HeapI * h = NULL;
    //call the function to see if it behaves as expected
    Object* instance = myItem.GetObjectBuildFunction()(h);
    bool retVal = true;
    if ((*((char*) instance)) != 9) {
        retVal = false;
    }

    HeapManager::Free((void*&) instance);
    return retVal;

}

bool ClassRegistryItemTest::TestGetIntrospection() {
    ClassProperties testClassProperties("TestIntrospectionCRI", "TestIntrospectionCRI", "1.1");

    IntrospectionEntry member1Field("member1", "uint32", "",  "", 32, 0);

    const IntrospectionEntry* fields[] = { &member1Field, 0 };
    Introspection introspectionTest(fields);

    ClassRegistryItem myItem(testClassProperties, introspectionTest);
    return myItem.GetIntrospection() == &introspectionTest;
}

bool ClassRegistryItemTest::TestSetUniqueIdentifier(uint32 uid) {
    ClassProperties testClassProperties("CRITTestSetUniqueIdentifier", "", "0");
    ClassRegistryItem *myItem = new ClassRegistryItem(testClassProperties, dummyBuildFcn);
    if (myItem == NULL) {
        delete myItem;
        return false;
    }
    delete myItem;
    //myItem.SetUniqueIdentifier(uid);
    //return (myItem.GetClassProperties().GetUniqueIdentifier() == uid);
    return true;
}

