/**
 * @file ReferenceTest.cpp
 * @brief Source file for class ReferenceTest
 * @date 07/08/2015
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
 * the class ReferenceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceTest.h"
#include "ReferenceT.h"
#include "MemoryCheck.h"
#include "ObjectTestHelper.h"
#include "ClassRegistryDatabase.h"
#include "Threads.h"
#include "Sleep.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceTest::ReferenceTest() {
    eventSem.Create();
    nRefs = 0;
}

bool ReferenceTest::TestDefaultConstructor() {

    Reference ref;
    return !ref.IsValid();

}

bool ReferenceTest::TestCopyConstructor() {

    Reference toCopy("IntegerObject");

    (dynamic_cast<IntegerObject*>(toCopy.operator->()))->SetVariable(1);

    Reference ret(toCopy);

    return (dynamic_cast<IntegerObject*>(ret.operator->()))->GetVariable() == 1;

}

bool ReferenceTest::TestCopyConstructorNullPtr() {
    //invalid reference
    Reference toCopy;
    Reference ret(toCopy);

    return !toCopy.IsValid();
}

bool ReferenceTest::TestBuildObjectConstructor() {

    Reference buildObj("IntegerObject");

    if (buildObj->NumberOfReferences() != 1) {
        return false;
    }

    //check if the object really exists
    (dynamic_cast<IntegerObject*>(buildObj.operator->()))->SetVariable(1);

    return (dynamic_cast<IntegerObject*>(buildObj.operator->()))->GetVariable() == 1;
}

bool ReferenceTest::TestBuildFakeObjectConstructor() {

    //an object with this name is not registered!
    Reference buildObj("FakeObject");

    //the reference should be invalid!
    return !buildObj.IsValid();
}

bool ReferenceTest::TestCopyFromObjPtrConstructor() {

    Reference myIntObj("IntegerObject");

    (dynamic_cast<IntegerObject*>(myIntObj.operator->()))->SetVariable(2);

    if ((dynamic_cast<IntegerObject*>(myIntObj.operator->()))->NumberOfReferences() != 1) {
        return false;
    }

    Reference ref1 = (dynamic_cast<IntegerObject*>(myIntObj.operator->()));

    if (myIntObj->NumberOfReferences() != 2) {
        return false;
    }

    Reference ref2;
    ref2 = (dynamic_cast<IntegerObject*>(myIntObj.operator->()));

    //checks if it has zero references
    if (myIntObj->NumberOfReferences() != 3) {
        return false;
    }

    Reference ref3("IntegerObject");

    ref3 = (dynamic_cast<IntegerObject*>(myIntObj.operator->()));

    //checks if it has zero references
    if (myIntObj->NumberOfReferences() != 4) {
        return false;
    }

    return ((dynamic_cast<IntegerObject*>(ref1.operator->()))->GetVariable() == 2) && ((dynamic_cast<IntegerObject*>(ref2.operator->()))->GetVariable() == 2)
            && ((dynamic_cast<IntegerObject*>(ref3.operator->()))->GetVariable() == 2);
}

bool ReferenceTest::TestCopyFromObjPtrConstructorNullPtr() {

    Reference ret((Object*) NULL);

    return !ret.IsValid();
}

bool ReferenceTest::TestDestructor() {

    //an object with this name is not registered!
    Reference buildObj("IntegerObject");

    if ((buildObj.NumberOfReferences() != 1) && (buildObj.IsValid())) {
        return false;
    }

    buildObj.~Reference();
    return (buildObj.NumberOfReferences() == 0) && (!buildObj.IsValid());
}

bool ReferenceTest::TestInitialiseNoCreation() {
    Reference intObjRef = Reference("IntegerObject");
    ConfigurationDatabase cdb;
    uint32 value = 1;
    cdb.Write("var", value);
    if (!intObjRef.Initialise(cdb, true)) {
        return false;
    }
    ReferenceT<IntegerObject> test = intObjRef;
    return test->GetVariable() == value;
}

bool ReferenceTest::TestInitialiseCreation() {
    Reference intObjRef;
    ConfigurationDatabase cdb;
    uint32 value = 1;
    cdb.Write("Class", "IntegerObject");
    cdb.Write("var", value);
    if (!intObjRef.Initialise(cdb, false)) {
        return false;
    }
    ReferenceT<IntegerObject> test = intObjRef;
    return test->GetVariable() == value;
}

bool ReferenceTest::TestInitialiseNoObject() {
    Reference intObjRef;
    ConfigurationDatabase cdb;
    int32 value = 1;
    cdb.Write("var", value);
    return !intObjRef.Initialise(cdb, true);
}

bool ReferenceTest::TestInitialiseNoClassName(){
    Reference intObjRef;
    ConfigurationDatabase cdb;
    int32 value = 1;
    cdb.Write("var", value);
    return !intObjRef.Initialise(cdb, false);
}


bool ReferenceTest::TestRemoveReference() {

    Reference intObjRef = Reference("IntegerObject");

    //creates an array of references to the object
    Reference refs[32];

    for (uint32 i = 0; i < 32; i++) {
        refs[i] = intObjRef;
    }

    if ((dynamic_cast<IntegerObject*>(intObjRef.operator->()))->NumberOfReferences() != 33) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        refs[i].RemoveReference();
        if ((dynamic_cast<IntegerObject*>(intObjRef.operator->()))->NumberOfReferences() != (32 - i)) {
            return false;
        }
        if (refs[i].IsValid()) {
            return false;
        }
    }

    intObjRef.RemoveReference();

    return !intObjRef.IsValid();

}

bool ReferenceTest::TestCopyOperatorReference() {

    Reference intObjRef = Reference("IntegerObject");
    (dynamic_cast<IntegerObject*>(intObjRef.operator->()))->SetVariable(2);

    Reference copyObj = intObjRef;

    if (dynamic_cast<IntegerObject*>(intObjRef.operator->()) != dynamic_cast<IntegerObject*>(copyObj.operator->())) {
        return false;
    }

    return ((dynamic_cast<IntegerObject*>(intObjRef.operator->()))->NumberOfReferences() == 2)
            && ((dynamic_cast<IntegerObject*>(copyObj.operator->()))->GetVariable() == 2);
}

bool ReferenceTest::TestCopyOperatorReferenceNull() {
    Reference nullRef;
    Reference copyObj = nullRef;
    if (copyObj.IsValid()) {
        return false;
    }

    return copyObj.NumberOfReferences() == 0;
}

bool ReferenceTest::TestCopyOperatorObject() {
    ClassRegistryDatabase::Instance();

    Reference intObjRef = Reference("IntegerObject");
    (dynamic_cast<IntegerObject*>(intObjRef.operator->()))->SetVariable(2);

    Reference copyObj = (dynamic_cast<IntegerObject*>(intObjRef.operator->()));
    if ((dynamic_cast<IntegerObject*>(intObjRef.operator->())) != dynamic_cast<IntegerObject*>(copyObj.operator->())) {
        return false;
    }

    return ((dynamic_cast<IntegerObject*>(intObjRef.operator->()))->NumberOfReferences() == 2)
            && ((dynamic_cast<IntegerObject*>(copyObj.operator->()))->GetVariable() == 2);

}

bool ReferenceTest::TestCopyOperatorObjectNull() {
    Reference copyObj = (Object*) NULL;
    if (copyObj.IsValid()) {
        return false;
    }

    return copyObj.NumberOfReferences() == 0;
}

bool ReferenceTest::TestIsValid() {

    //an object with this name is not registered!
    Reference fakeObj("FakeObject");

    //the reference should be invalid!
    if (fakeObj.IsValid()) {
        return false;
    }

    Reference copyNull((Object*) NULL);
    //the reference should be invalid!
    if (copyNull.IsValid()) {
        return false;
    }

    Reference buildObj("IntegerObject");
    //the reference should be valid!
    if (!buildObj.IsValid()) {
        return false;
    }

    buildObj.RemoveReference();

    return !buildObj.IsValid();

}

bool ReferenceTest::TestNumberOfReferences() {

    Reference buildObj("IntegerObject");

    Reference builtFromRef(buildObj);
    Reference builtFromObj(dynamic_cast<IntegerObject*>(buildObj.operator->()));

    Reference refs[4];
    refs[0] = buildObj;
    refs[1] = builtFromRef;
    refs[2] = (dynamic_cast<IntegerObject*>(buildObj.operator->()));
    refs[3] = builtFromObj;

    builtFromObj.RemoveReference();
    builtFromRef.RemoveReference();

    for (uint32 i = 0; i < 4; i++) {
        if (refs[i].NumberOfReferences() != 5) {
            return false;
        }
    }

    for (uint32 i = 4; i > 0; i--) {
        refs[i - 1].RemoveReference();
        if (refs[i - 1].NumberOfReferences() != 0) {
            return false;
        }

        //checks if the others see the correct number of refs
        for (uint32 j = 0; j < (i - 1); j++) {
            if (refs[j].NumberOfReferences() != i) {
                return false;
            }
        }
    }

    return buildObj.NumberOfReferences() == 1;

}

bool ReferenceTest::TestEqualOperator() {

    Reference buildObj("IntegerObject");

    Reference copy(buildObj);

    if (!(buildObj == copy)) {
        return false;
    }

    //another instance of the same class
    Reference test("IntegerObject");

    if (buildObj == test) {
        return false;
    }

    copy = (Object*) NULL;

    if (buildObj == copy) {
        return false;
    }

    copy = buildObj;
    if (!(buildObj == copy)) {
        return false;
    }

    copy = (Object*) NULL;
    buildObj = (Object*) NULL;

    return (buildObj == copy);

}

bool ReferenceTest::TestDifferentOperator() {

    Reference buildObj("IntegerObject");

    Reference copy(buildObj);

    if (buildObj != copy) {
        return false;
    }

    copy = (Object*) NULL;

    if (!(buildObj != copy)) {
        return false;
    }

    copy = buildObj;
    if (buildObj != copy) {
        return false;
    }

    copy = (Object*) NULL;
    buildObj = (Object*) NULL;

    return !(buildObj != copy);

}

void CreateRefsOnStack(ReferenceTest &rt) {

    Reference newRef[32];
    for (uint32 i = 0; i < 32; i++) {
        newRef[i] = rt.storedRef;
    }

    rt.eventSem.Wait();
}

bool ReferenceTest::TestInFunctionOnStack() {

    storedRef = Reference("Object");
    
    //See how many threads are already running, as in some implementations
    //(e.g. FreeRTOS) there may be at least one task already there before.
    uint32 numOfThreadsBefore = Threads::NumberOfThreads();

    Threads::BeginThread((ThreadFunctionType) CreateRefsOnStack, this);

    uint32 totalNRefs = 33;

    uint32 j = 0;
    while (storedRef.NumberOfReferences() != totalNRefs) {
        if (j++ > 100) {
            return false;
        }

        Sleep::MSec(10);
    }

    eventSem.Post();

    while (Threads::NumberOfThreads() != numOfThreadsBefore) {
        Sleep::MSec(10);
    }
    Sleep::MSec(10);

    return storedRef.NumberOfReferences() == 1;

}

void CreateRefsOnHeap(ReferenceTest &rt) {

    rt.arrayRefs = (Reference **) HeapManager::Malloc(sizeof(Reference*) * rt.nRefs);

    for (uint32 i = 0; i < rt.nRefs; i++) {
        rt.arrayRefs[i] = new Reference;
        *(rt.arrayRefs[i]) = rt.storedRef;
    }

    rt.eventSem.Wait();
}

bool ReferenceTest::TestInFunctionOnHeap(uint32 nRefs) {

    ClassRegistryDatabase::Instance();

    storedRef = Reference("Object");
    
    //See how many threads are already running, as in some implementations
    //(e.g. FreeRTOS) there may be at least one task already there before.
    uint32 numOfThreadsBefore = Threads::NumberOfThreads();

    this->nRefs = nRefs;

    Threads::BeginThread((ThreadFunctionType) CreateRefsOnHeap, this);

    uint32 totalNRefs = (nRefs + 1);

    bool ret = true;
    uint32 j = 0;
    while (storedRef.NumberOfReferences() != totalNRefs) {
        if (j++ > 100) {
            ret = false;
            break;
        }

        Sleep::MSec(10);
    }

    eventSem.Post();

    while (Threads::NumberOfThreads() != numOfThreadsBefore) {
        Sleep::MSec(10);
    }
    Sleep::MSec(10);

    if (storedRef.NumberOfReferences() != totalNRefs) {
        ret = false;
    }

    // free the references on the heap
    for (uint32 i = 0; i < nRefs; i++) {
        delete arrayRefs[i];
    }

    // free the pointers array
    HeapManager::Free((void*&) arrayRefs);
    return ret;

}

bool ReferenceTest::TestRightInherithance() {

    Reference integer = Reference("IntegerObject");

    Reference specialInteger = Reference("SpecialIntegerObject");

    specialInteger = integer;
    if ((specialInteger->NumberOfReferences() != 2)) {
        return false;
    }

    specialInteger = Reference("SpecialIntegerObject");

    integer = specialInteger;

    return (integer->NumberOfReferences() == 2);

}

bool ReferenceTest::TestWrongInherithance() {

    Reference integer = Reference("IntegerObject");

    Reference floatn = Reference("FloatObject");

    floatn = integer;
    if (floatn->NumberOfReferences() != 2) {
        return false;
    }
    floatn = Reference("FloatObject");

    integer = floatn;

    return (integer->NumberOfReferences() == 2);

}

void ActsOnRefs1(ReferenceTest &rt) {

    //add a certain number of references to the first object
    for (uint32 i = 0; i < (rt.nRefs); i++) {
        rt.arrayRefs[i] = new Reference;
        *(rt.arrayRefs[i]) = (rt.storedRef);
    }

    //add a certain number of references to the second object
    for (uint32 i = (rt.nRefs); i < (2 * rt.nRefs); i++) {
        rt.arrayRefs[i] = new Reference;
        *(rt.arrayRefs[i]) = (rt.storedRef2);
    }

    rt.eventSem.Wait();

    for (uint32 i = (2 * rt.nRefs); i < (3 * rt.nRefs); i++) {
        rt.arrayRefs[i]->RemoveReference();
    }

    for (uint32 i = (3 * rt.nRefs); i < (4 * rt.nRefs); i++) {
        rt.arrayRefs[i]->RemoveReference();
    }

}

void ActsOnRefs2(ReferenceTest &rt) {

    //add a certain number of references to the first object
    for (uint32 i = (2 * rt.nRefs); i < (3 * rt.nRefs); i++) {
        rt.arrayRefs[i] = new Reference;
        *(rt.arrayRefs[i]) = (rt.storedRef);
    }

    //add a certain number of references to the second object
    for (uint32 i = (3 * rt.nRefs); i < (4 * rt.nRefs); i++) {
        rt.arrayRefs[i] = new Reference;
        *(rt.arrayRefs[i]) = (rt.storedRef2);
    }

    rt.eventSem.Wait();

    for (uint32 i = 0; i < rt.nRefs; i++) {
        rt.arrayRefs[i]->RemoveReference();
    }

    for (uint32 i = rt.nRefs; i < (2 * rt.nRefs); i++) {
        rt.arrayRefs[i]->RemoveReference();
    }
}

bool ReferenceTest::HugeTest(uint32 nRefs) {

    this->nRefs = nRefs;
    arrayRefs = (Reference**) HeapManager::Malloc(4 * sizeof(Reference*) * nRefs);

    storedRef = Reference("Object");
    storedRef2 = Reference("Object");

    Threads::BeginThread((ThreadFunctionType) ActsOnRefs1, this);
    Threads::BeginThread((ThreadFunctionType) ActsOnRefs2, this);

    uint32 k = 0;
    bool ret = true;
    while (((storedRef.operator->())->NumberOfReferences() < (2 * nRefs + 1)) && ((storedRef2.operator->())->NumberOfReferences() < (2 * nRefs + 1))) {
        if (k++ > 100) {
            ret = false;
            break;
        }
        Sleep::MSec(100);
    }

    eventSem.Post();

    k = 0;
    while ((storedRef.NumberOfReferences() != 1) && (storedRef2.NumberOfReferences() != 1)) {
        if (k++ > 100) {
            ret = false;
            break;
        }
        Sleep::MSec(100);
    }

    for (uint32 i = 0; i < (4 * nRefs); i++) {
        delete arrayRefs[i];
    }

    HeapManager::Free((void*&) arrayRefs);
    return ret;
}

