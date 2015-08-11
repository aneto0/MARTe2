/**
 * @file ReferenceTTTest.cpp
 * @brief Source file for class ReferenceTTTest
 * @date 10/ago/2015
 * @author pc
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
#include "ObjectTestHelper.h"
#include "ClassRegistryDatabase.h"
#include "Threads.h"
#include "Sleep.h"
#include "Memory.h"
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

    Heap mem;
    Reference toCopy("IntegerObject", mem);

    (dynamic_cast<IntegerObject*>(toCopy.operator->()))->SetVariable(1);

    ReferenceT<IntegerObject> ret(toCopy);

    return ret->GetVariable() == 1;

}

bool ReferenceTTest::TestCopyConstructorReferenceParentToChild() {

    Heap mem;
    Reference integer("IntegerObject", mem);
    ReferenceT<SpecialIntegerObject> specialInteger(integer);

    return !specialInteger.IsValid();
}

bool ReferenceTTest::TestCopyConstructorReferenceChildToParent() {

    Heap mem;
    Reference specialInteger("SpecialIntegerObject", mem);
    ReferenceT<IntegerObject> integer(specialInteger);

    return integer->NumberOfReferences() == 2;
}

bool ReferenceTTest::TestCopyConstructorReferenceT() {

    Heap mem;
    ReferenceT<IntegerObject> toCopy("IntegerObject", mem);

    (dynamic_cast<IntegerObject*>(toCopy.operator->()))->SetVariable(1);

    ReferenceT<IntegerObject> ret(toCopy);

    return ret->GetVariable() == 1;

}

bool ReferenceTTest::TestCopyConstructorReferenceTParentToChild() {

    Heap mem;
    ReferenceT<IntegerObject> integer("IntegerObject", mem);
    ReferenceT<SpecialIntegerObject> specialInteger(integer);

    return !specialInteger.IsValid();
}

bool ReferenceTTest::TestCopyConstructorReferenceTChildToParent() {

    Heap mem;
    ReferenceT<SpecialIntegerObject> specialInteger("SpecialIntegerObject", mem);
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

    Heap mem;
    ReferenceT<IntegerObject> ref(mem);

    if (!ref.IsValid()) {
        return false;
    }
    ref->SetVariable(2);

    return ref->GetVariable() == 2;
}

bool ReferenceTTest::TestBuildObjectConstructor() {

    Heap mem;
    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

    if (buildObj->NumberOfReferences() != 1) {
        return false;
    }

    //check if the object really exists
    buildObj->SetVariable(1);

    return buildObj->GetVariable() == 1;
}

bool ReferenceTTest::TestBuildFakeObjectConstructor() {

    Heap mem;

    //an object with this name is not registered!
    ReferenceT<Object> buildObj("FakeObject", mem);

    //the reference should be invalid!
    return !buildObj.IsValid();
}

bool ReferenceTTest::TestCopyFromObjPtrConstructor() {

    Heap mem;
    ReferenceT<IntegerObject> myIntObj = ReferenceT<IntegerObject>("IntegerObject", mem);

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
    Heap mem;

    //an object with this name is not registered!
    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

    if ((buildObj.NumberOfReferences() != 1) && (buildObj.IsValid())) {
        return false;
    }

    buildObj.~ReferenceT();
    return (buildObj.NumberOfReferences() == 0) && (!buildObj.IsValid());
}

//TODO
bool ReferenceTTest::TestInitialise() {
    return true;
}

bool ReferenceTTest::TestRemoveReference() {

    Heap mem;

    ReferenceT<IntegerObject> intObjRef("IntegerObject", mem);

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

    Heap mem;

    Reference intObjRef = Reference("IntegerObject", mem);

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

    Heap mem;

    ReferenceT<IntegerObject> intObjRef("IntegerObject", mem);
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

    Heap mem;

    ReferenceT<IntegerObject> source("IntegerObject", mem);

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

    Heap mem;

    //an object with this name is not registered!
    ReferenceT<IntegerObject> fakeObj("FakeObject", mem);

    //the reference should be invalid!
    if (fakeObj.IsValid()) {
        return false;
    }

    ReferenceT<IntegerObject> copyNull((IntegerObject*) NULL);
    //the reference should be invalid!
    if (copyNull.IsValid()) {
        return false;
    }

    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);
    //the reference should be valid!
    if (!buildObj.IsValid()) {
        return false;
    }

    buildObj.RemoveReference();

    return !buildObj.IsValid();

}

bool ReferenceTTest::TestNumberOfReferences() {

    Heap mem;

    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

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

    Heap mem;

    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

    ReferenceT<IntegerObject> copy(buildObj);

    if (!(buildObj == copy)) {
        return false;
    }

    //another instance of the same class
    ReferenceT<IntegerObject> test("IntegerObject", mem);

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

    Heap mem;

    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

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

bool ReferenceTTest::TestCloneReference() {

    Heap mem;

    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

    Reference fakeObj("FakeObject", mem);

    return !(buildObj.Clone(fakeObj));
}

bool ReferenceTTest::TestCloneReferenceT() {

    Heap mem;

    ReferenceT<IntegerObject> buildObj("IntegerObject", mem);

    ReferenceT<IntegerObject> fakeObj("FakeObject", mem);

    return !(buildObj.Clone(fakeObj)) && !(fakeObj.Clone(buildObj));
}

void CreateRefsOnStack(ReferenceTTest &rt) {

    ReferenceT<Object> newRef[32];
    for (uint32 i = 0; i < 32; i++) {
        newRef[i] = rt.storedRef;
    }

    rt.eventSem.Wait();
}

bool ReferenceTTest::TestInFunctionOnStack() {

    Heap mem;
    storedRef = ReferenceT<Object>("Object", mem);

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

    while (Threads::NumberOfThreads() != 0) {
        Sleep::MSec(10);
    }
    Sleep::MSec(10);

    return storedRef.NumberOfReferences() == 1;

}

void CreateRefsOnHeap(ReferenceTTest &rt) {

    rt.arrayRefs = (ReferenceT<Object> **) Memory::Malloc(sizeof(ReferenceT<Object>*) * rt.nRefs);

    for (uint32 i = 0; i < rt.nRefs; i++) {
        rt.arrayRefs[i] = new ReferenceT<Object>;
        *(rt.arrayRefs[i]) = rt.storedRef;
    }

    rt.eventSem.Wait();
}

bool ReferenceTTest::TestInFunctionOnHeap(uint32 nRefs) {

    Heap mem;
    storedRef = ReferenceT<Object>("Object", mem);

    Threads::BeginThread((ThreadFunctionType) CreateRefsOnHeap, this);

    this->nRefs = nRefs;

    uint32 totalNRefs = (nRefs + 1);

    uint32 j = 0;
    while (storedRef.NumberOfReferences() != totalNRefs) {
        if (j++ > 100) {
            return false;
        }

        Sleep::MSec(10);
    }

    eventSem.Post();

    while (Threads::NumberOfThreads() != 0) {
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
    Memory::Free((void*&) arrayRefs);
    return true;

}

bool ReferenceTTest::TestRightInherithance() {

    Heap mem;

    ReferenceT<IntegerObject> integer = ReferenceT<IntegerObject>("IntegerObject", mem);

    ReferenceT<SpecialIntegerObject> specialInteger = ReferenceT<SpecialIntegerObject>("SpecialIntegerObject", mem);

    integer = specialInteger;

    if (!integer.IsValid()) {
        return false;
    }

    if ((integer->NumberOfReferences() != 2)) {
        return false;
    }

    integer = ReferenceT<IntegerObject>("IntegerObject", mem);

    //not possible the assignment from top-down
    specialInteger = integer;

    return !specialInteger.IsValid();
}

bool ReferenceTTest::TestWrongInherithance() {

    Heap mem;

    ReferenceT<IntegerObject> integer = ReferenceT<IntegerObject>("IntegerObject", mem);

    ReferenceT<FloatObject> floatn = ReferenceT<FloatObject>("FloatObject", mem);

    floatn = integer;

    if (floatn.IsValid()) {
        return false;
    }

    floatn = ReferenceT<FloatObject>("FloatObject", mem);

    integer = floatn;

    return !integer.IsValid();
}

