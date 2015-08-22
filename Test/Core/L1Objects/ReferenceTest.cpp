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
ReferenceTest::ReferenceTest() {
    eventSem.Create();
    nRefs = 0;
}

bool ReferenceTest::TestDefaultConstructor() {

    Reference ref;
    return !ref.IsValid();

}

bool ReferenceTest::TestCopyConstructor() {

    Heap mem;
    Reference toCopy("IntegerObject", mem);

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

    Heap mem;
    Reference buildObj("IntegerObject", mem);

    if (buildObj->NumberOfReferences() != 1) {
        return false;
    }

    //check if the object really exists
    (dynamic_cast<IntegerObject*>(buildObj.operator->()))->SetVariable(1);

    return (dynamic_cast<IntegerObject*>(buildObj.operator->()))->GetVariable() == 1;
}

bool ReferenceTest::TestBuildFakeObjectConstructor() {

    Heap mem;

    //an object with this name is not registered!
    Reference buildObj("FakeObject", mem);

    //the reference should be invalid!
    return !buildObj.IsValid();
}

bool ReferenceTest::TestCopyFromObjPtrConstructor() {

    Heap mem;
    Reference myIntObj("IntegerObject", mem);

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

    Reference ref3("IntegerObject", mem);

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
    Heap mem;

    //an object with this name is not registered!
    Reference buildObj("IntegerObject", mem);

    if ((buildObj.NumberOfReferences() != 1) && (buildObj.IsValid())) {
        return false;
    }

    buildObj.~Reference();
    return (buildObj.NumberOfReferences() == 0) && (!buildObj.IsValid());
}

//TODO
bool ReferenceTest::TestInitialise() {
    return true;
}

bool ReferenceTest::TestRemoveReference() {

    Heap mem;

    Reference intObjRef = Reference("IntegerObject", mem);

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

    Heap mem;

    Reference intObjRef = Reference("IntegerObject", mem);
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
    ClassRegistryDatabase db = ClassRegistryDatabase::Instance();

    Heap mem;

    Reference intObjRef = Reference("IntegerObject", mem);
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

    Heap mem;

    //an object with this name is not registered!
    Reference fakeObj("FakeObject", mem);

    //the reference should be invalid!
    if (fakeObj.IsValid()) {
        return false;
    }

    Reference copyNull((Object*) NULL);
    //the reference should be invalid!
    if (copyNull.IsValid()) {
        return false;
    }

    Reference buildObj("IntegerObject", mem);
    //the reference should be valid!
    if (!buildObj.IsValid()) {
        return false;
    }

    buildObj.RemoveReference();

    return !buildObj.IsValid();

}

bool ReferenceTest::TestNumberOfReferences() {

    Heap mem;

    Reference buildObj("IntegerObject", mem);

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

    Heap mem;

    Reference buildObj("IntegerObject", mem);

    Reference copy(buildObj);

    if (!(buildObj == copy)) {
        return false;
    }

    //another instance of the same class
    Reference test("IntegerObject", mem);

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

    Heap mem;

    Reference buildObj("IntegerObject", mem);

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

    Heap mem;
    storedRef = Reference("Object", mem);

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

void CreateRefsOnHeap(ReferenceTest &rt) {

    rt.arrayRefs = (Reference **) Memory::Malloc(sizeof(Reference*) * rt.nRefs);

    for (uint32 i = 0; i < rt.nRefs; i++) {
        rt.arrayRefs[i] = new Reference;
        *(rt.arrayRefs[i]) = rt.storedRef;
    }

    rt.eventSem.Wait();
}

bool ReferenceTest::TestInFunctionOnHeap(uint32 nRefs) {

    ClassRegistryDatabase db = ClassRegistryDatabase::Instance();

    Heap mem;
    storedRef = Reference("Object", mem);

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

bool ReferenceTest::TestRightInherithance() {

    Heap mem;

    Reference integer = Reference("IntegerObject", mem);

    Reference specialInteger = Reference("SpecialIntegerObject", mem);

    specialInteger = integer;
    if ((specialInteger->NumberOfReferences() != 2)) {
        return false;
    }

    specialInteger = Reference("SpecialIntegerObject", mem);

    integer = specialInteger;

    return (integer->NumberOfReferences() == 2);

}

bool ReferenceTest::TestWrongInherithance() {

    Heap mem;

    Reference integer = Reference("IntegerObject", mem);

    Reference floatn = Reference("FloatObject", mem);


    floatn = integer;
    if (floatn->NumberOfReferences() != 2) {
        return false;
    }
    floatn = Reference("FloatObject", mem);

    integer = floatn;

    return (integer->NumberOfReferences() == 2);

}

