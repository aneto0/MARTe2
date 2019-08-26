/**
 * @file ObjectTest.cpp
 * @brief Source file for class ObjectTest
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
 * the class ObjectTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <cstddef>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectTest.h"

#include "ClassWithCallableMethods.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectTestHelper.h"
#include "Reference.h"
#include "SimpleStructuredData.h"
#include "SimpleStream.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ObjectTest::TestConstructor() {
    Object myObject;
    return myObject.NumberOfReferences() == 0;
}

bool ObjectTest::TestDestructor() {
    Object myObject;
    //nothing happen.
    myObject.~Object();
    return myObject.NumberOfReferences() == 0;
}

bool ObjectTest::TestGetIntrospectionCopy() {
    //TODO
    return true;
}

bool ObjectTest::TestInitialise() {
    /*Object myObject;
     StructuredDataI data;
     //returns false
     return !myObject.Initialise(data);*/
    //TODO
    return true;
}

bool ObjectTest::TestNumberOfReferences() {
    Reference objRef("Object");

    if (objRef->NumberOfReferences() != 1) {
        return false;
    }

    Reference secondObjRef = objRef;

    if (objRef->NumberOfReferences() != 2) {
        return false;
    }

    secondObjRef.RemoveReference();

    return (objRef->NumberOfReferences() == 1);

}

bool ObjectTest::TestSetGetName(CCString name) {

    //creates an object
    Object myObj;

    myObj.SetName(name);

    return (name.GetSize()==0) ? (myObj.GetName().GetSize()==0) : (name == myObj.GetName()) ;

}

bool ObjectTest::TestDuplicateName() {

    char name[] = { 'H', 'e', 'l', 'l', 'o', '\0' };

    Object myObj;
    myObj.SetName(name);

    name[0] = 'B';

    //if the name string is duplicated in memory and it isn't only a pointer copy
    //the object should have the original name.
    bool ok = !(myObj.GetName() == name);

    //The name should now be updated.
    myObj.SetName(name);
    ok = ok && (myObj.GetName() == name);
    return ok;
}

bool ObjectTest::TestGetUniqueName(CCString name) {

    const uint32 ptrSize = sizeof(void*) * 2;

    Object myObj;
    uintp ptr = (uintp) &myObj;

    myObj.SetName(name);

    DynamicCString uniqueName;
    CStringTool cs = uniqueName();
    myObj.GetUniqueName(cs);

//    myObj.GetUniqueName(buffer, buffSize);

    if (uniqueName[0] != 'x') {
        return false;
    }

    //checks if the name is equal to the object pointer
    uint32 n = 1;
    bool zeros = true;
    for (int32 i = (ptrSize - 1); i >= 0; i--) {
        uint32 character = static_cast<uint32>((ptr >> (4 * i)) & 0xf);

        //skip the initial zeros
        if ((!character) && (zeros)) {
            continue;
        }
        else {
            zeros = false;
        }


        //create the test char
        char8 test = static_cast<char8>((character < 10) ? (character + '0') : ((character - 10) + 'A'));
        if (uniqueName[n] != test) {
            return false;
        }
        n++;
    }

    bool ok = uniqueName[n++] == ':';
    ok = ok && uniqueName[n++] == ':';

    CCString lastPart(uniqueName.GetList()+n);
    ok = ok && (lastPart == name);
    return ok;

/*
    //the remained size is the minimum between buffSize-n and the nameSize (+2 because of "::"
    uint32 remainedSize = (stringSize + 2);
    if (remainedSize > (buffSize - n)) {
        remainedSize = (buffSize - n);
    }

    char onlyName[size];
    for (uint32 i = 0; i < size; i++) {
        onlyName[i] = 0;
    }
    StringHelper::Copy(onlyName, "::");
    StringHelper::Concatenate(onlyName, name);

    return StringHelper::CompareN(buffer + n, onlyName, remainedSize) == 0;
*/
}

bool ObjectTest::TestGetUniqueName2() {

    Object object1;
    Object object2;

    CCString sameName = "Hello";

    object1.SetName(sameName);
    object2.SetName(sameName);

    bool haveSameName =	(object1.GetName() == object2.GetName());

    if (!haveSameName) {
        return false;
    }

    DynamicCString uniqueName1;
    CStringTool cs1 = uniqueName1();
    object1.GetUniqueName(cs1);

    DynamicCString uniqueName2;
    CStringTool cs2 = uniqueName2();
    object2.GetUniqueName(cs2);

    return !(uniqueName1 == uniqueName2);
}

bool ObjectTest::TestGetProperties() {
    Object obj;
    CCString name = obj.GetClassRegistryItem()->GetClassName();
    CCString version = obj.GetClassRegistryItem()->GetClassVersion();

//    const ClassProperties* properties = obj.GetClassProperties();
//    CCString name = properties->GetName();
//    CCString version = properties->GetVersion();
    return (name == "Object") && (version == "1.0");
}

bool ObjectTest::TestCallRegisteredMethod() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(HeapManager::standardHeapId);
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithVoidParameters");
    bool result = err.ErrorsCleared();
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_InvalidMethod() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(HeapManager::standardHeapId);
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithVoidParametersE");
    bool result = !err.ErrorsCleared();
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_StructuredDataI() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(HeapManager::standardHeapId);
    SimpleStructuredData config;
    config.Write("value", 30);
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithOutputStructuredDataI", config);
    bool result = err.ErrorsCleared();
    result &= (target->GetLastMethodExecuted()== "MethodWithOutputStructuredDataI(StructuredDataI)");
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_ReferenceContainer() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(HeapManager::standardHeapId);
    ReferenceT<SimpleStructuredData> config(HeapManager::standardHeapId);
    config->Write("value", 30);

    ReferenceContainer parameters;
    parameters.Insert(config);
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithOutputStructuredDataI", parameters);
    bool result = err.ErrorsCleared();
    result &= (target->GetLastMethodExecuted()== "MethodWithOutputStructuredDataI(StructuredDataI)");
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_StreamI() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(HeapManager::standardHeapId);
    SimpleStream parameters;
    parameters.Init("MethodWithConstInputStreamI");
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithOutputStreamI", parameters);
    bool result = err.ErrorsCleared();
    result &= (target->GetLastMethodExecuted()=="MethodWithOutputStreamI(StreamI&)");
    return result;
}

bool ObjectTest::TestIsReferenceContainer() {
    using namespace MARTe;
    Object o;
    return !o.IsReferenceContainer();
}

