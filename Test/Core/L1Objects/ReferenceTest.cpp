/**
 * @file ReferenceTest.cpp
 * @brief Source file for class ReferenceTest
 * @date 07/ago/2015
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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

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

    ClassRegistryDatabase db = ClassRegistryDatabase::Instance();

    Heap mem;
    IntegerObject *myIntObj = dynamic_cast<IntegerObject*>(db.CreateByName("IntegerObject", mem));

    myIntObj->SetVariable(2);

    //checks if it has zero references
    if (myIntObj->NumberOfReferences() != 0) {
        return false;
    }

    Reference ret(myIntObj);

    //checks if it has zero references
    if (myIntObj->NumberOfReferences() != 1) {
        return false;
    }

    return (dynamic_cast<IntegerObject*>(ret.operator->()))->GetVariable() == 2;
}

bool ReferenceTest::TestCopyFromObjPtrConstructorNullPtr() {

    Reference ret((Object*) NULL);

    return !ret.IsValid();
}

//TODO
bool ReferenceTest::TestInitialise() {
    return true;
}

bool ReferenceTest::TestRemoveReference() {

    ClassRegistryDatabase db = ClassRegistryDatabase::Instance();

    Heap mem;
    IntegerObject *myIntObj = dynamic_cast<IntegerObject*>(db.CreateByName("IntegerObject", mem));

    Reference intObjRef(myIntObj);

    //creates an array of references to the object
    Reference refs[32];

    for (uint32 i = 0; i < 32; i++) {
        refs[i] = intObjRef;
    }

    if (myIntObj->NumberOfReferences() != 33) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        refs[i].RemoveReference();
        if (myIntObj->NumberOfReferences() != (32 - i)) {
            return false;
        }
        if (refs[i].IsValid()) {
            return false;
        }
    }

    intObjRef.RemoveReference();

    //now the object should be deleted
    if (!myIntObj->Deleted()) {
        return false;
    }

    return !intObjRef.IsValid();

}
