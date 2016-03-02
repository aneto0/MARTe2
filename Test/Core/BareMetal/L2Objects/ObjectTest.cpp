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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectTest.h"
#include "Reference.h"
#include "StringHelper.h"
#include "ObjectTestHelper.h"
#include "StreamString.h"
#include "ConfigurationDatabase.h"

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
    ClassProperties properties;
    obj.GetClassPropertiesCopy(properties);
    const char8 *name = properties.GetName();
    const char8 *version = properties.GetVersion();
    return (StringHelper::Compare("Object", name) == 0) && (StringHelper::Compare("1.0", version) == 0);
}

bool ObjectTest::TestExportData() {
    bool result = true;
    {
        bool test = false;
        NonRegisteredIntegerObject obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test1");
        obj.SetVariable(10);
        test = !obj.ExportData(cdb);
        result = result && test;
    }
    {
        bool test = false;
        NonIntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test2");
        obj.SetVariable(20);
        test = !obj.ExportData(cdb);
        result = result && test;
    }
    {
        bool test_status = false;
        bool test_values = true;
        IntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test3");
        obj.SetVariable(30);
        test_status = obj.ExportData(cdb);
        if (test_status) {
            StructuredDataI& sd = cdb;
            StreamString className;
            int32 dummyVariable;
            test_status = sd.MoveRelative(obj.GetName());
            test_status = sd.Read("Class",className);
            test_values = (test_values && (className == "IntrospectableIntegerObject"));
            test_status = sd.Read("dummyVariable", dummyVariable);
            test_values = (test_values && (dummyVariable == 30));
        }
        result = result && test_status && test_values;
    }
    return result;
}

bool ObjectTest::TestExportMetadata() {
    /*
     * Variants of case
     * An object introspectable vs one not introspectable.
     * An object from a registered class vs a not registered one.
     * A recursion level for -1, 0, 1, 2, and N.
     *
     *
     */
    bool result = true;
    {
        bool test = false;
        NonRegisteredIntegerObject obj;
        ConfigurationDatabase cdb;
        test = !obj.ExportMetadata(cdb);
        result = result && test;
    }
    {
        bool test = false;
        NonIntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        test = !obj.ExportMetadata(cdb);
        result = result && test;
    }
    {
        bool test_status = false;
        bool test_values = true;
        IntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        test_status = obj.ExportMetadata(cdb);
        if (test_status) {
            StructuredDataI& sd = cdb;
            StreamString type;
            StreamString modifiers;
            StreamString attributes;
            uint32 size;
            uintp pointer;
            test_status = sd.MoveRelative("IntrospectableIntegerObject");
            test_status = sd.MoveRelative("dummyVariable");
            test_status = sd.Read("type",type);
            test_values = (test_values && (type == "int32"));
            test_status = sd.Read("modifiers",modifiers);
            test_values = (test_values && (modifiers == ""));
            test_status = sd.Read("attributes",attributes);
            test_values = (test_values && (attributes == ""));
            test_status = sd.Read("size",size);
            test_values = (test_values && (size == 4));
            test_status = sd.Read("pointer",pointer);
            test_values = (test_values && (pointer == (reinterpret_cast<uintp>(&obj) + 24)));
        }
        result = result && test_status && test_values;
    }
    return result;
}
