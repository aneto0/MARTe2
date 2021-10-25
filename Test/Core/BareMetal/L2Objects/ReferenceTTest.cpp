/**
 * @file ReferenceTTTest.cpp
 * @brief Source file for class ReferenceTTTest
 * @date 10/08/2015
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
 * the class ReferenceTTTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceTTest.h"

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
ReferenceTTest::ReferenceTTest() {
    eventSem.Create();
    nRefs = 0;
}

bool ReferenceTTest::TestDefaultConstructor() {

    ReferenceT<Object> ref;
    return !ref.IsValid();

}

bool ReferenceTTest::TestCopyConstructorReference() {

    Reference toCopy("IntegerObject");

    (dynamic_cast<IntegerObject*>(toCopy.operator->()))->SetVariable(1);

    ReferenceT<IntegerObject> ret(toCopy);

    return ret->GetVariable() == 1;

}

bool ReferenceTTest::TestCopyConstructorReferenceParentToChild() {

    Reference integer("IntegerObject");
    ReferenceT<SpecialIntegerObject> specialInteger(integer);

    return !specialInteger.IsValid();
}

bool ReferenceTTest::TestCopyConstructorReferenceChildToParent() {

    Reference specialInteger("SpecialIntegerObject");
    ReferenceT<IntegerObject> integer(specialInteger);

    return integer->NumberOfReferences() == 2;
}

bool ReferenceTTest::TestCopyConstructorReferenceT() {

    ReferenceT<IntegerObject> toCopy("IntegerObject");

    (dynamic_cast<IntegerObject*>(toCopy.operator->()))->SetVariable(1);

    ReferenceT<IntegerObject> ret(toCopy);

    return ret->GetVariable() == 1;

}

bool ReferenceTTest::TestCopyConstructorReferenceTParentToChild() {

    ReferenceT<IntegerObject> integer("IntegerObject");
    ReferenceT<SpecialIntegerObject> specialInteger(integer);

    return !specialInteger.IsValid();
}

bool ReferenceTTest::TestCopyConstructorReferenceTChildToParent() {

    ReferenceT<SpecialIntegerObject> specialInteger("SpecialIntegerObject");
    ReferenceT<IntegerObject> integer(specialInteger);

    return integer->NumberOfReferences() == 2;
}

bool ReferenceTTest::TestCopyConstructorNullPtr() {
    //invalid reference
    ReferenceT<Object> toCopy;
    ReferenceT<Object> ret(toCopy);

    return !toCopy.IsValid();
}

bool ReferenceTTest::TestCreateConstructor() {

    HeapI* mem = NULL;
    ReferenceT<IntegerObject> ref(mem);

    if (!ref.IsValid()) {
        return false;
    }
    ref->SetVariable(2);

    return ref->GetVariable() == 2;
}

bool ReferenceTTest::TestBuildObjectConstructor() {

    ReferenceT<IntegerObject> buildObj("IntegerObject");

    if (buildObj->NumberOfReferences() != 1) {
        return false;
    }

    //check if the object really exists
    buildObj->SetVariable(1);

    return buildObj->GetVariable() == 1;
}

bool ReferenceTTest::TestBuildFakeObjectConstructor() {

    //an object with this name is not registered!
    ReferenceT<Object> buildObj("FakeObject");

    //the reference should be invalid!
    return !buildObj.IsValid();
}

bool ReferenceTTest::TestCopyFromObjPtrConstructor() {

    ReferenceT<IntegerObject> myIntObj = ReferenceT<IntegerObject>("IntegerObject");

    myIntObj->SetVariable(2);

    //checks if it has zero references
    if (myIntObj->NumberOfReferences() != 1) {
        return false;
    }

    ReferenceT<IntegerObject> ret(myIntObj);

    //checks if it has zero references
    if (myIntObj->NumberOfReferences() != 2) {
        return false;
    }

    return ret->GetVariable() == 2;
}

bool ReferenceTTest::TestCopyFromObjPtrConstructorNullPtr() {

    ReferenceT<Object> ret((Object*) NULL);

    return !ret.IsValid();
}

bool ReferenceTTest::TestDestructor() {

    //an object with this name is not registered!
    ReferenceT<IntegerObject> buildObj("IntegerObject");

    if ((buildObj.NumberOfReferences() != 1) && (buildObj.IsValid())) {
        return false;
    }

    buildObj.~ReferenceT();
    return (buildObj.NumberOfReferences() == 0) && (!buildObj.IsValid());
}

bool ReferenceTTest::TestInitialiseNoCreation() {
    ReferenceT<IntegerObject> test = Reference("IntegerObject");
    ConfigurationDatabase cdb;
    uint32 value = 1;
    cdb.Write("var", value);
    if (!test.Initialise(cdb, true)) {
        return false;
    }
    return test->GetVariable() == value;
}

bool ReferenceTTest::TestInitialiseCreation() {
    ReferenceT<IntegerObject> test;
    ConfigurationDatabase cdb;
    uint32 value = 1;
    cdb.Write("Class", "IntegerObject");
    cdb.Write("var", value);
    if (!test.Initialise(cdb, false)) {
        return false;
    }
    return test->GetVariable() == value;
}

bool ReferenceTTest::TestInitialiseNoObject() {
    ReferenceT<IntegerObject> test;
    ConfigurationDatabase cdb;
    int32 value = 1;
    cdb.Write("var", value);
    return !test.Initialise(cdb, true);
}

bool ReferenceTTest::TestInitialiseNoClassName() {
    ReferenceT<IntegerObject> test;
    ConfigurationDatabase cdb;
    int32 value = 1;
    cdb.Write("var", value);
    return !test.Initialise(cdb, false);
}


bool ReferenceTTest::TestInitialiseIncompatibleCast() {
    ReferenceT<IntegerObject> test;
    ConfigurationDatabase cdb;
    int32 value = 1;
    cdb.Write("Class", "FloatObject");
    cdb.Write("var", value);
    return !test.Initialise(cdb, false);
}



bool ReferenceTTest::TestRemoveReference() {

    ReferenceT<IntegerObject> intObjRef("IntegerObject");

    //creates an array of references to the object
    ReferenceT<IntegerObject> refs[32];

    for (uint32 i = 0; i < 32; i++) {
        refs[i] = intObjRef;
    }

    if ((intObjRef.operator->())->NumberOfReferences() != 33) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        refs[i].RemoveReference();
        if ((intObjRef.operator->())->NumberOfReferences() != (32 - i)) {
            return false;
        }
        if (refs[i].IsValid()) {
            return false;
        }
    }

    intObjRef.RemoveReference();

    return !intObjRef.IsValid();

}
bool ReferenceTTest::TestCopyOperatorReference() {

    Reference intObjRef = Reference("IntegerObject");

    (dynamic_cast<IntegerObject*>(intObjRef.operator->()))->SetVariable(2);

    ReferenceT<IntegerObject> copyObj = intObjRef;

    if (intObjRef.operator->() != copyObj.operator->()) {
        return false;
    }

    return ((dynamic_cast<IntegerObject*>(intObjRef.operator->()))->NumberOfReferences() == 2) && (copyObj->GetVariable() == 2);
}

bool ReferenceTTest::TestCopyOperatorReferenceNull() {
    Reference nullRef;
    Reference copyObj = nullRef;
    if (copyObj.IsValid()) {
        return false;
    }

    return copyObj.NumberOfReferences() == 0;
}

bool ReferenceTTest::TestCopyOperatorReferenceT() {

    ReferenceT<IntegerObject> intObjRef("IntegerObject");
    intObjRef->SetVariable(2);

    ReferenceT<IntegerObject> copyObj = intObjRef;

    if (intObjRef.operator->() != copyObj.operator->()) {
        return false;
    }

    return ((intObjRef.operator->())->NumberOfReferences() == 2) && (copyObj->GetVariable() == 2);
}

bool ReferenceTTest::TestCopyOperatorReferenceTNull() {
    ReferenceT<Object> nullRef;
    ReferenceT<Object> copyObj = nullRef;
    if (copyObj.IsValid()) {
        return false;
    }

    return copyObj.NumberOfReferences() == 0;
}

bool ReferenceTTest::TestCopyOperatorObject() {

    ReferenceT<IntegerObject> source("IntegerObject");

    source->SetVariable(2);

    ReferenceT<IntegerObject> copyObj = source.operator->();
    if (source.operator->() != copyObj.operator->()) {
        return false;
    }

    return (source.operator->()->NumberOfReferences() == 2) && (copyObj->GetVariable() == 2);

}

bool ReferenceTTest::TestCopyOperatorObjectNull() {
    ReferenceT<Object> copyObj = (Object*) NULL;
    if (copyObj.IsValid()) {
        return false;
    }

    return copyObj.NumberOfReferences() == 0;
}

bool ReferenceTTest::TestIsValid() {

    //an object with this name is not registered!
    ReferenceT<IntegerObject> fakeObj("FakeObject");

    //the reference should be invalid!
    if (fakeObj.IsValid()) {
        return false;
    }

    ReferenceT<IntegerObject> copyNull((IntegerObject*) NULL);
    //the reference should be invalid!
    if (copyNull.IsValid()) {
        return false;
    }

    ReferenceT<IntegerObject> buildObj("IntegerObject");
    //the reference should be valid!
    if (!buildObj.IsValid()) {
        return false;
    }

    buildObj.RemoveReference();

    return !buildObj.IsValid();

}

bool ReferenceTTest::TestNumberOfReferences() {

    ReferenceT<IntegerObject> buildObj("IntegerObject");

    ReferenceT<IntegerObject> builtFromRef(buildObj);
    ReferenceT<IntegerObject> builtFromObj(buildObj.operator->());

    ReferenceT<IntegerObject> refs[4];
    refs[0] = buildObj;
    refs[1] = builtFromRef;
    refs[2] = (Object*) (buildObj.operator->());
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

bool ReferenceTTest::TestEqualOperator() {


    ReferenceT<IntegerObject> buildObj("IntegerObject");

    ReferenceT<IntegerObject> copy(buildObj);

    if (!(buildObj == copy)) {
        return false;
    }

    //another instance of the same class
    ReferenceT<IntegerObject> test("IntegerObject");

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

bool ReferenceTTest::TestDifferentOperator() {

    ReferenceT<IntegerObject> buildObj("IntegerObject");

    ReferenceT<IntegerObject> copy(buildObj);

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

void CreateRefsOnStack(ReferenceTTest &rt) {

    ReferenceT<Object> newRef[32];
    for (uint32 i = 0; i < 32; i++) {
        newRef[i] = rt.storedRef;
    }

    rt.eventSem.Wait();
}

bool ReferenceTTest::TestInFunctionOnStack() {

    storedRef = ReferenceT<Object>("Object");

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

void CreateRefsOnHeap(ReferenceTTest &rt) {

    rt.arrayRefs = (ReferenceT<Object> **) HeapManager::Malloc(sizeof(ReferenceT<Object>*) * rt.nRefs);

    for (uint32 i = 0; i < rt.nRefs; i++) {
        rt.arrayRefs[i] = new ReferenceT<Object>;
        *(rt.arrayRefs[i]) = rt.storedRef;
    }
    rt.eventSem.Wait();
}

bool ReferenceTTest::TestInFunctionOnHeap(uint32 nRefs) {

    storedRef = ReferenceT<Object>("Object");

    this->nRefs = nRefs;

    //See how many threads are already running, as in some implementations
    //(e.g. FreeRTOS) there may be at least one task already there before.
    uint32 numOfThreadsBefore = Threads::NumberOfThreads();
    Threads::BeginThread((ThreadFunctionType) CreateRefsOnHeap, this);

    uint32 totalNRefs = (nRefs + 1);

    uint32 j = 0;
    while (storedRef.NumberOfReferences() != totalNRefs) {
        if (j++ > 1000) {
            return false;
        }

        Sleep::MSec(10);
    }
    eventSem.Post();

    while (Threads::NumberOfThreads() != numOfThreadsBefore) {
        Sleep::MSec(10);
    }
    Sleep::MSec(10);

    if (storedRef.NumberOfReferences() != totalNRefs) {
        return false;
    }

    // free the references on the heap
    for (uint32 i = 0; i < nRefs; i++) {
        delete arrayRefs[i];
    }

    // free the pointers array
    HeapManager::Free((void*&) arrayRefs);
    return true;

}

bool ReferenceTTest::TestRightInherithance() {

    ReferenceT<IntegerObject> integer = ReferenceT<IntegerObject>("IntegerObject");

    ReferenceT<SpecialIntegerObject> specialInteger = ReferenceT<SpecialIntegerObject>("SpecialIntegerObject");

    integer = specialInteger;

    if (!integer.IsValid()) {
        return false;
    }

    if ((integer->NumberOfReferences() != 2)) {
        return false;
    }

    integer = ReferenceT<IntegerObject>("IntegerObject");

    //not possible the assignment from top-down
    specialInteger = integer;

    return !specialInteger.IsValid();
}

bool ReferenceTTest::TestWrongInherithance() {

    ReferenceT<IntegerObject> integer = ReferenceT<IntegerObject>("IntegerObject");

    ReferenceT<FloatObject> floatn = ReferenceT<FloatObject>("FloatObject");

    floatn = integer;

    if (floatn.IsValid()) {
        return false;
    }

    floatn = ReferenceT<FloatObject>("FloatObject");

    integer = floatn;

    return !integer.IsValid();
}

