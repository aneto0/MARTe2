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

#include "ClassMethodInterfaceMapper.h"
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItemTest.h"
#include "ClassWithCallableMethods.h"
#include "ErrorType.h"
#include "LoadableLibrary.h"
//#include "MemoryCheck.h"
#include "Object.h"
#include "Reference.h"
#include "ReferenceContainer.h"
#include "CLASSMEMBERREGISTER.h"
//#include "StringHelper.h"
//#include <typeinfo>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MyObject: public Object {
	uint32 member1;
public:
    CLASS_REGISTER_DECLARATION()
	CLASS_PRIVATE_MEMBER_DECLARE(member1)

};
//CLASS_MEMBER_REGISTER(MyObject,member1)
CLASS_REGISTER(MyObject, "1.0")
CLASS_PRIVATE_MEMBER_REGISTER(MyObject, member1)




class MyObject2: public Object {
public:
    CLASS_REGISTER_DECLARATION()
};
CLASS_REGISTER(MyObject2, "1.0")

// will override the default object builder
class DummyObjectBuilder: public ObjectBuilderT<MyObject2> {
    Object *Build(HeapManager::HeapId  heapId) const {
        Object *p = (Object*) HeapManager::Malloc(sizeof(MyObject2));
        char *pp = (char*) p;
        (*pp) = 9;
        return p;
    }
};
// the constructor links this to the ClassRegistry. BUT also CLASS_REGISTRY installs an ObjectBuilder the same way. which order?
static DummyObjectBuilder dummyObjectBuilder;

class TestIntrospectionCRI: public Object {
	uint32 member1;
public:
    CLASS_REGISTER_DECLARATION()
	CLASS_PRIVATE_MEMBER_DECLARE(member1)
};
CLASS_REGISTER(TestIntrospectionCRI, "1.1")
CLASS_PRIVATE_MEMBER_REGISTER(TestIntrospectionCRI,member1)
//CLASS_MEMBER_REGISTER(TestIntrospectionCRI,member1)

#if 0

static ClassRegistryItem myItem = *(ClassRegistryItemT<MyObject2>::Instance());

static IntrospectionEntry member1Field("member1", "uint32", "", "", 4, 0);

static const IntrospectionEntry* fields[] = { &member1Field, 0 };
static Introspection introspectionTest(fields, 4);

static ClassRegistryItem myItemIntro = *(ClassRegistryItemT<TestIntrospectionCRI>::Instance());

static ClassRegistryItem myItemFull = *(ClassRegistryItemT<MyObject>::Instance());

#endif

class ClassWithCallableMethods3: public ClassWithCallableMethods {
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ClassRegistryItemTest::TestConstructor() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    // checks the attributes.
    if (cri->GetNumberOfInstances() != 0) {
        return false;
    }

    if (cri->GetLoadableLibrary() != NULL) {
        return false;
    }

    if (cri->NumberOfMembers() > 0) {
        return false;
    }

    if (!(cri->GetClassName() == "MyObject2"))  {
        return false;
    }

    if (!(cri->GetTypeidName() == typeid(MyObject2).name()))  {
        return false;
    }

    if (!(cri->GetClassVersion() == "1.0"))  {
        return false;
    }

    if (cri->GetObjectBuilder() != &dummyObjectBuilder) {
        return false;
    }

    if (ClassRegistryDatabase::Find("TestNormalCRI") != NULL){
    //    if (db->Find("TestNormalCRI") != NULL) {
        return false;
    }

    //check if the correct function is saved
//    Object *instance = cri->GetObjectBuilder()->Build(h);
    Object *instance = cri->GetObjectBuilder()->Build(HeapManager::standardHeapId);

    if (instance == NULL) {
        return false;
    }

    bool retVal = true;
    if ((*((char*) instance)) != 9) {
        retVal = false;
    }

    HeapManager::Free((void*&) instance);
/* these have no effect here so move them in a more appropriate place
    myItemIntro.SetIntrospection(&introspectionTest);

    cri->SetIntrospection(&introspectionTest);
//    cri->SetObjectBuilder(&dummyObjectBuilder);
*/
    return retVal;

}

bool ClassRegistryItemTest::TestIntrospectionConstructor() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<TestIntrospectionCRI>();

    // checks the attributes.
    if (cri->GetNumberOfInstances() != 0) {
        return false;
    }

    if (cri->GetLoadableLibrary() != NULL) {
        return false;
    }

    if (cri->NumberOfMembers() ==0 ) {
        return false;
    }

    if (!(cri->GetClassName() == "TestIntrospectionCRI")) {
        return false;
    }

    if (!(cri->GetTypeidName() ==  typeid(TestIntrospectionCRI).name())) {
        return false;
    }

    if (!(cri->GetClassVersion() == "1.1")) {
        return false;
    }

    if (cri->GetObjectBuilder() == NULL) {
        return false;
    }

    if (ClassRegistryDatabase::Find("TestIntrospectionCRI") == NULL) {
        return false;
    }

    return true;
}

bool ClassRegistryItemTest::TestFullConstructor() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    // checks the attributes.
    if (cri->GetNumberOfInstances() != 0) {
        return false;
    }

    if (cri->GetLoadableLibrary() != NULL) {
        return false;
    }

    if (cri->NumberOfMembers() != 1 ) {
        return false;
    }

    if (!(cri->GetClassName()=="MyObject") ) {
        return false;
    }

    if (!(cri->GetTypeidName()== typeid(MyObject).name())) {
        return false;
    }

    if (!(cri->GetClassVersion() == "1.0") ) {
        return false;
    }


    if (ClassRegistryDatabase::Find("MyObject") == NULL) {
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
//    cri->SetLoadableLibrary(dummy);
//
//    cri->~ClassRegistryItem();
//    dummy = cri->GetLoadableLibrary();
//
//    //checks that dummy was destructed
//    return (dummy == NULL);
//}

bool ClassRegistryItemTest::TestAddMethod() {
    bool result = true;

    //Sets the target of this test to the global ClassRegistryItem of ClassWithCallableMethods3
    ClassRegistryItem* const target = ClassRegistryItem::Instance<ClassWithCallableMethods3>();

    ClassMethodInterfaceMapper *mapper1 = new ClassMethodInterfaceMapper(&ClassWithCallableMethods3::MethodWithThreeParameters_C_C_C);
    mapper1->SetMethodName("MethodWithThreeParameters_C_C_C_TestAddMethod");
    target->AddMethod(mapper1);
    ClassMethodInterfaceMapper *mapper2 = new ClassMethodInterfaceMapper(&ClassWithCallableMethods3::MethodWithThreeParameters_C_R_C);
    mapper2->SetMethodName("MethodWithThreeParameters_C_R_C_TestAddMethod");
    target->AddMethod(mapper2);

    result &= (target->FindMethod("MethodWithThreeParameters_C_C_C_TestAddMethod") != NULL);
    result &= (target->FindMethod("MethodWithThreeParameters_C_R_C_TestAddMethod") != NULL);
    result &= (target->FindMethod("MethodWithThreeParameters_C_W_C_TestAddMethod") == NULL);

    return result;
}

bool ClassRegistryItemTest::TestFindMethod() {
    return TestAddMethod();
}

bool ClassRegistryItemTest::TestIncrementNumberOfInstances() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    if (cri->GetNumberOfInstances() != 0) {
        return false;
    }

    uint32 currentInstances = cri->GetNumberOfInstances();

    cri->IncrementNumberOfInstances();

    ret.fatalError = cri->GetNumberOfInstances() != (currentInstances + 1);

    cri->DecrementNumberOfInstances();

    return ret;


}

bool ClassRegistryItemTest::TestDecrementNumberOfInstances() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    uint32 currentInstances = cri->GetNumberOfInstances();

    cri->IncrementNumberOfInstances();

    cri->DecrementNumberOfInstances();

    return cri->GetNumberOfInstances() == currentInstances;

}

bool ClassRegistryItemTest::TestGetNumberOfInstances(uint32 nInstances) {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    uint32 currentInstances = cri->GetNumberOfInstances();

    for (uint32 i = 0; i < nInstances; i++) {
        cri->IncrementNumberOfInstances();
    }

    if (cri->GetNumberOfInstances() != (currentInstances + nInstances)) {
        return false;
    }

    for (uint32 i = 0; i < nInstances; i++) {
        cri->DecrementNumberOfInstances();
    }

    return cri->GetNumberOfInstances() == currentInstances;
}


bool ClassRegistryItemTest::TestSetGetLoadableLibrary(CCString llname) {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    // the library is automatically deleted
    LoadableLibrary *myLib = new LoadableLibrary;

    myLib->Open(llname);

    cri->SetLoadableLibrary(myLib);

    if (cri->GetLoadableLibrary() != myLib) {
        return false;
    }
    return true;
}

bool ClassRegistryItemTest::TestGetObjectBuildFunction() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<MyObject2>();

    if (cri->GetObjectBuilder() != &dummyObjectBuilder) {
        return false;
    }

    //call the function to see if it behaves as expected
    Object* instance = cri->GetObjectBuilder()->Build(HeapManager::standardHeapId);
    bool retVal = true;
    if ((*((char*) instance)) != 9) {
        retVal = false;
    }

    HeapManager::Free((void*&) instance);
    return retVal;

}


