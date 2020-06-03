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
#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectTestHelper.h"
#include "Reference.h"
#include "StandardParser.h"
#include "StringHelper.h"
#include "StreamString.h"

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

bool ObjectTest::TestSetGetName(const char8 *name) {

    //creates an object
    Object myObj;

    myObj.SetName(name);

    return name == NULL ? myObj.GetName() == NULL : (StringHelper::Compare(name, myObj.GetName()) == 0);

}

bool ObjectTest::TestDuplicateName() {

    char name[] = { 'H', 'e', 'l', 'l', 'o', '\0' };

    Object myObj;
    myObj.SetName(name);

    name[0] = 'B';

    //if the name string is duplicated in memory and it isn't only a pointer copy
    //the object should have the original name.
    bool ok = (StringHelper::Compare(name, myObj.GetName()) != 0);

    //The name should now be updated.
    myObj.SetName(name);
    ok = (StringHelper::Compare(name, myObj.GetName()) == 0);
    return ok;
}

bool ObjectTest::TestGetUniqueName(const char8* name,
                                   uint32 buffSize) {

    const uint32 ptrSize = sizeof(void*) * 2;
    const uint32 stringSize = StringHelper::Length(name) + 2;

    const uint32 size = 128;

    Object myObj;
    uintp ptr = (uintp) &myObj;
    char buffer[size];
    for (uint32 i = 0; i < size; i++) {
        buffer[i] = 0;
    }

    myObj.SetName(name);

    myObj.GetUniqueName(buffer, buffSize);

    if (buffer[0] != 'x') {
        return false;
    }

    //checks if the name is equal to the object pointer
    uint32 n = 1;
    bool zeros = true;
    for (int32 i = (ptrSize - 1); i >= 0; i--) {
        uint32 character = (ptr >> (4 * i)) & 0xf;

        //skip the initial zeros
        if ((!character) && (zeros)) {
            continue;
        }
        else {
            zeros = false;
        }

        //if we compared all the buffSize exit true
        if (n >= buffSize) {
            return true;
        }
        //create the test char
        char8 test = (character < 10) ? (character + '0') : ((character - 10) + 'A');
        if (buffer[n] != test) {
            return false;
        }
        n++;
    }

    //the remained size id the minimum between buffSize-n and the nameSize (+2 because of "::"
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

}

bool ObjectTest::TestGetUniqueName2() {

    Object object1;
    Object object2;

    const char8 *sameName = "Hello";

    object1.SetName(sameName);
    object2.SetName(sameName);

    if (StringHelper::Compare(object1.GetName(), object2.GetName()) != 0) {
        return false;
    }

    char uniqueName1[32];
    object1.GetUniqueName(uniqueName1, 32);

    char uniqueName2[32];
    object2.GetUniqueName(uniqueName2, 32);

    return StringHelper::Compare(uniqueName1, uniqueName2) != 0;
}

bool ObjectTest::TestGetProperties() {
    Object obj;
    const ClassProperties* properties = obj.GetClassProperties();
    const char8 *name = properties->GetName();
    const char8 *version = properties->GetVersion();
    return (StringHelper::Compare("Object", name) == 0) && (StringHelper::Compare("1.0", version) == 0);
}

bool ObjectTest::TestExportData() {
    bool result = true;
    {
        /*
         * This test verifies that method ObjectTest::ExportData returns the
         * following tree (from a testing object which is an instance of a
         * registered and introspectable class):
         * root
         *  |+"Test3"
         *   |-"Class": "IntrospectableIntegerObject"
         *   |-"member": 30
         */
        bool test_status = true;
        bool test_values = true;
        IntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test3");
        obj.member = 30;
        test_status = (test_status && (obj.ExportData(cdb)));
        if (test_status) {
            StructuredDataI& sd = cdb;
            StreamString className;
            int32 member;
            test_status = (test_status && (sd.MoveRelative("Test3")));
            test_status = (test_status && (sd.Read("Class", className)));
            test_values = (test_values && (className == "IntrospectableIntegerObject"));
            test_status = (test_status && (sd.Read("member", member)));
            test_values = (test_values && (member == 30));
            test_status = (test_status && (sd.MoveToAncestor(1u)));
        }
        result = result && test_status && test_values;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportData returns the
         * following tree (from a testing object which is an instance of a
         * registered and introspectable class):
         * root
         *  |+"Test4"
         *   |-"Class": "IntrospectableObjectWith2Members"
         *   |-"member1": 10
         *   |-"member2": 20
         */
        bool test_status = true;
        bool test_values = true;
        IntrospectableObjectWith2Members obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test4");
        obj.member1 = 10;
        obj.member2 = 20;
        test_status = (test_status && (obj.ExportData(cdb)));
        if (test_status) {
            StructuredDataI& sd = cdb;
            StreamString className;
            int32 member1;
            uint64 member2;
            test_status = (test_status && (sd.MoveRelative("Test4")));
            test_status = (test_status && (sd.Read("Class", className)));
            test_values = (test_values && (className == "IntrospectableObjectWith2Members"));
            test_status = (test_status && (sd.Read("member1", member1)));
            test_values = (test_values && (member1 == 10));
            test_status = (test_status && (sd.Read("member2", member2)));
            test_values = (test_values && (member2 == 20));
            test_status = (test_status && (sd.MoveToAncestor(1u)));
        }
        result = result && test_status && test_values;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportData returns the
         * following tree (from a testing object which is an instance of a
         * registered and introspectable class):
         * root
         *  |+"Test5"
         *   |-"Class": "IntrospectableObjectWith3Members"
         *   |-"member1": 30
         *   |-"member2": 60
         *   |+"member3"
         *    |-"Class": "IntrospectableIntegerObject"
         *    |-"member": 90
         */
        bool test_status = true;
        bool test_values = true;
        IntrospectableObjectWith3Members obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test5");
        obj.member1 = 10;
        obj.member2 = 20;
        obj.member3.member = 30;
        test_status = (test_status && (obj.ExportData(cdb)));
        if (test_status) {
            StructuredDataI& sd = cdb;
            StreamString className;
            int32 member1;
            uint64 member2;
            IntrospectableIntegerObject member3;
            test_status = (test_status && (sd.MoveRelative("Test5")));
            test_status = (test_status && (sd.Read("Class", className)));
            test_values = (test_values && (className == "IntrospectableObjectWith3Members"));
            test_status = (test_status && (sd.Read("member1", member1)));
            test_values = (test_values && (member1 == 10));
            test_status = (test_status && (sd.Read("member2", member2)));
            test_values = (test_values && (member2 == 20));
            test_status = (test_status && (sd.Read("member3", member3)));
            test_values = (test_values && (member3.member == 30));
            {
                StreamString className;
                int32 member;
                test_status = (test_status && (sd.MoveRelative("member3")));
                test_status = (test_status && (sd.Read("Class", className)));
                test_values = (test_values && (className == "IntrospectableIntegerObject"));
                test_status = (test_status && (sd.Read("member", member)));
                test_values = (test_values && (member == 30));
                test_status = (test_status && (sd.MoveToAncestor(1u)));
            }
            test_status = (test_status && (sd.MoveToAncestor(1u)));
        }
        result = result && test_status && test_values;
    }
    return result;
}

bool ObjectTest::TestExportMetadata() {
    const int32 LEVELS[] = { -1, 0, 1, 2, MAX_INT32 };
    bool result = true;
    {
        /*
         * This test verifies that method ObjectTest::ExportMetata signals an
         * error because the testing object is an instance of a non registered
         * class.
         */
        bool test = false;
        NonRegisteredIntegerObject obj;
        ConfigurationDatabase cdb;
        test = !obj.ExportMetadata(cdb);
        result = result && test;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportMetdata signals an
         * error because the testing object is an instance of a registered but
         * non introspectable class.
         */
        bool test = false;
        NonIntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        test = !obj.ExportMetadata(cdb);
        result = result && test;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportMetadata returns
         * the following tree (from a testing object which is an instance of
         * a registered and introspectable class):
         * root
         *  |+"IntrospectableIntegerObject"
         *   |+"member"
         *    |-"type": "int32"
         *    |-"modifiers": ""
         *    |-"attributes": ""
         *    |-"size": sizeof(int32)
         *    |-"pointer": &this+offsetof(IntrospectableIntegerObject, member)
         */
        bool test_status = true;
        bool test_values = true;
        for (int i = 0; i < 5; i++) {
            IntrospectableIntegerObject obj;
            ConfigurationDatabase cdb;
            test_status = (test_status && (obj.ExportMetadata(cdb, LEVELS[i])));
            if (test_status) {
                StructuredDataI& sd = cdb;
                test_status = (test_status && (sd.MoveRelative("IntrospectableIntegerObject")));
                {
                    StreamString type;
                    StreamString modifiers;
                    StreamString attributes;
                    uint32 size;
                    uintp pointer;
                    test_status = (test_status && (sd.MoveRelative("member")));
                    test_status = (test_status && (sd.Read("type", type)));
                    test_values = (test_values && (type == "int32"));
                    test_status = (test_status && (sd.Read("modifiers", modifiers)));
                    test_values = (test_values && (modifiers == ""));
                    test_status = (test_status && (sd.Read("attributes", attributes)));
                    test_values = (test_values && (attributes == ""));
                    test_status = (test_status && (sd.Read("size", size)));
                    test_values = (test_values && (size == sizeof(int32)));
                    test_status = (test_status && (sd.Read("pointer", pointer)));
                    test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + offsetof(IntrospectableIntegerObject, member))));
                    test_status = (test_status && (sd.MoveToAncestor(1u)));
                }
                test_status = (test_status && (sd.MoveToAncestor(1u)));
            }
        }
        result = result && test_status && test_values;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportMetadata returns
         * the following tree (from a testing object which is an instance of
         * a registered and introspectable class):
         * root
         *  |+"IntrospectableObjectWith2Members"
         *   |+"member1"
         *    |-"type": "int32"
         *    |-"modifiers": ""
         *    |-"attributes": ""
         *    |-"size": sizeof(int32)
         *    |-"pointer": &this+offsetof(IntrospectableObjectWith2Members, member1)
         *   |+"member2"
         *    |-"type": "uint64"
         *    |-"modifiers": ""
         *    |-"attributes": ""
         *    |-"size": sizeof(uint64)
         *    |-"pointer": &this+offsetof(IntrospectableObjectWith2Members, member2)
         */
        bool test_status = true;
        bool test_values = true;
        for (int i = 0; i < 5; i++) {
            IntrospectableObjectWith2Members obj;
            ConfigurationDatabase cdb;
            test_status = (test_status && (obj.ExportMetadata(cdb, LEVELS[i])));
            if (test_status) {
                StructuredDataI& sd = cdb;
                test_status = (test_status && (sd.MoveRelative("IntrospectableObjectWith2Members")));
                {
                    StreamString type;
                    StreamString modifiers;
                    StreamString attributes;
                    uint32 size;
                    uintp pointer;
                    test_status = (test_status && (sd.MoveRelative("member1")));
                    test_status = (test_status && (sd.Read("type", type)));
                    test_values = (test_values && (type == "int32"));
                    test_status = (test_status && (sd.Read("modifiers", modifiers)));
                    test_values = (test_values && (modifiers == ""));
                    test_status = (test_status && (sd.Read("attributes", attributes)));
                    test_values = (test_values && (attributes == ""));
                    test_status = (test_status && (sd.Read("size", size)));
                    test_values = (test_values && (size == sizeof(int32)));
                    test_status = (test_status && (sd.Read("pointer", pointer)));
                    test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + offsetof(IntrospectableObjectWith2Members, member1))));
                    test_status = (test_status && (sd.MoveToAncestor(1u)));
                }
                {
                    StreamString type;
                    StreamString modifiers;
                    StreamString attributes;
                    uint32 size;
                    uintp pointer;
                    test_status = (test_status && (sd.MoveRelative("member2")));
                    test_status = (test_status && (sd.Read("type", type)));
                    test_values = (test_values && (type == "uint64"));
                    test_status = (test_status && (sd.Read("modifiers", modifiers)));
                    test_values = (test_values && (modifiers == ""));
                    test_status = (test_status && (sd.Read("attributes", attributes)));
                    test_values = (test_values && (attributes == ""));
                    test_status = (test_status && (sd.Read("size", size)));
                    test_values = (test_values && (size == sizeof(uint64)));
                    test_status = (test_status && (sd.Read("pointer", pointer)));
                    test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + offsetof(IntrospectableObjectWith2Members, member2))));
                    test_status = (test_status && (sd.MoveToAncestor(1u)));
                }
                test_status = (test_status && (sd.MoveToAncestor(1u)));
            }
        }
        result = result && test_status && test_values;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportMetadata returns
         * the following tree (from a testing object which is an instance of
         * a registered and introspectable class):
         * root
         *  |+"IntrospectableObjectWith3Members"
         *   |+"member1"
         *    |-"type": "int32"
         *    |-"modifiers": ""
         *    |-"attributes": ""
         *    |-"size": sizeof(int32)
         *    |-"pointer": &this+offsetof(IntrospectableObjectWith3Members, member1)
         *   |+"member2"
         *    |-"type": "uint64"
         *    |-"modifiers": ""
         *    |-"attributes": ""
         *    |-"size": sizeof(uint64)
         *    |-"pointer": &this+offsetof(IntrospectableObjectWith3Members, member2)
         *   |+"member3"
         *    |-"type": "IntrospectableIntegerObject"
         *    |-"modifiers": ""
         *    |-"attributes": ""
         *    |-"size": sizeof(uint64)
         *    |-"pointer": &this+offsetof(IntrospectableObjectWith3Members, member3)
         *    |+"IntrospectableIntegerObject"
         *     |+"member"
         *      |-"type": "int32"
         *      |-"modifiers": ""
         *      |-"attributes": ""
         *      |-"size": sizeof(int32)
         *      |-"pointer": &this.member+offsetof(IntrospectableIntegerObject, member)
         */
        bool test_status = true;
        bool test_values = true;
        for (int i = 0; i < 5; i++) {
            IntrospectableObjectWith3Members obj;
            ConfigurationDatabase cdb;
            test_status = (test_status && (obj.ExportMetadata(cdb, LEVELS[i])));
            if (test_status) {
                StructuredDataI& sd = cdb;
                test_status = (test_status && (sd.MoveRelative("IntrospectableObjectWith3Members")));
                {
                    StreamString type;
                    StreamString modifiers;
                    StreamString attributes;
                    uint32 size;
                    uintp pointer;
                    test_status = (test_status && (sd.MoveRelative("member1")));
                    test_status = (test_status && (sd.Read("type", type)));
                    test_values = (test_values && (type == "int32"));
                    test_status = (test_status && (sd.Read("modifiers", modifiers)));
                    test_values = (test_values && (modifiers == ""));
                    test_status = (test_status && (sd.Read("attributes", attributes)));
                    test_values = (test_values && (attributes == ""));
                    test_status = (test_status && (sd.Read("size", size)));
                    test_values = (test_values && (size == sizeof(int32)));
                    test_status = (test_status && (sd.Read("pointer", pointer)));
                    test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + offsetof(IntrospectableObjectWith3Members, member1))));
                    test_status = (test_status && (sd.MoveToAncestor(1u)));
                }
                {
                    StreamString type;
                    StreamString modifiers;
                    StreamString attributes;
                    uint32 size;
                    uintp pointer;
                    test_status = (test_status && (sd.MoveRelative("member2")));
                    test_status = (test_status && (sd.Read("type", type)));
                    test_values = (test_values && (type == "uint64"));
                    test_status = (test_status && (sd.Read("modifiers", modifiers)));
                    test_values = (test_values && (modifiers == ""));
                    test_status = (test_status && (sd.Read("attributes", attributes)));
                    test_values = (test_values && (attributes == ""));
                    test_status = (test_status && (sd.Read("size", size)));
                    test_values = (test_values && (size == sizeof(uint64)));
                    test_status = (test_status && (sd.Read("pointer", pointer)));
                    test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + offsetof(IntrospectableObjectWith3Members, member2))));
                    test_status = (test_status && (sd.MoveToAncestor(1u)));
                }
                {
                    StreamString type;
                    StreamString modifiers;
                    StreamString attributes;
                    uint32 size;
                    uintp pointer;
                    test_status = (test_status && (sd.MoveRelative("member3")));
                    test_status = (test_status && (sd.Read("type", type)));
                    test_values = (test_values && (type == "IntrospectableIntegerObject"));
                    test_status = (test_status && (sd.Read("modifiers", modifiers)));
                    test_values = (test_values && (modifiers == ""));
                    test_status = (test_status && (sd.Read("attributes", attributes)));
                    test_values = (test_values && (attributes == ""));
                    test_status = (test_status && (sd.Read("size", size)));
                    test_values = (test_values && (size == sizeof(IntrospectableIntegerObject)));
                    test_status = (test_status && (sd.Read("pointer", pointer)));
                    test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + offsetof(IntrospectableObjectWith3Members, member3))));
                    if ((LEVELS[i] == -1) || (LEVELS[i] > 0)) {
                        StreamString type;
                        StreamString modifiers;
                        StreamString attributes;
                        uint32 size;
                        uintp pointer;
                        test_status = (test_status && (sd.MoveRelative("IntrospectableIntegerObject.member")));
                        test_status = (test_status && (sd.Read("type", type)));
                        test_values = (test_values && (type == "int32"));
                        test_status = (test_status && (sd.Read("modifiers", modifiers)));
                        test_values = (test_values && (modifiers == ""));
                        test_status = (test_status && (sd.Read("attributes", attributes)));
                        test_values = (test_values && (attributes == ""));
                        test_status = (test_status && (sd.Read("size", size)));
                        test_values = (test_values && (size == sizeof(int32)));
                        test_status = (test_status && (sd.Read("pointer", pointer)));
                        test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj.member3) + offsetof(IntrospectableIntegerObject, member))));
                        test_status = (test_status && (sd.MoveToAncestor(1u)));
                    }
                    test_status = (test_status && (sd.MoveToAncestor(1u)));
                }
                test_status = (test_status && (sd.MoveToAncestor(1u)));
            }
        }
        result = result && test_status && test_values;
    }
    return result;
}

bool ObjectTest::TestCallRegisteredMethod() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithVoidParameters");
    bool result = err.ErrorsCleared();
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_InvalidMethod() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithVoidParametersE");
    bool result = !err.ErrorsCleared();
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_StructuredDataI() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase config;
    config.Write("value", 30);
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithOutputStructuredDataI", config);
    bool result = err.ErrorsCleared();
    result &= (StringHelper::Compare(target->GetLastMethodExecuted().Buffer(), "MethodWithOutputStructuredDataI(StructuredDataI)") == 0u);
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_ReferenceContainer() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ConfigurationDatabase> config(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    config->Write("value", 30);

    ReferenceContainer parameters;
    parameters.Insert(config);
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithOutputStructuredDataI", parameters);
    bool result = err.ErrorsCleared();
    result &= (StringHelper::Compare(target->GetLastMethodExecuted().Buffer(), "MethodWithOutputStructuredDataI(StructuredDataI)") == 0u);
    return result;
}

bool ObjectTest::TestCallRegisteredMethod_StreamI() {
    using namespace MARTe;
    ReferenceT<ClassWithCallableMethods> target(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString parameters = "MethodWithConstInputStreamI";
    ErrorManagement::ErrorType err = target->CallRegisteredMethod("MethodWithOutputStreamI", parameters);
    bool result = err.ErrorsCleared();
    result &= (StringHelper::Compare(target->GetLastMethodExecuted().Buffer(), "MethodWithOutputStreamI(StreamI&)") == 0u);
    return result;
}

bool ObjectTest::TestIsReferenceContainer() {
    using namespace MARTe;
    Object o;
    return !o.IsReferenceContainer();
}
