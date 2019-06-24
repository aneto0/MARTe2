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

#include "ClassWithCallableMethods.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectTestHelper.h"
#include "ObjectTestIntro.h"
#include "Reference.h"
#include "SimpleStructuredData.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


bool ObjectTest::TestExportData() {
    bool result = true;
    {
        /*
         * This test verifies that method ObjectTest::ExportData signals an
         * error because the testing object is an instance of a non registered
         * class.
         */
        bool test = false;
        NonRegisteredIntegerObject obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test1");
        obj.member = 10;
        test = !obj.ExportData(cdb);
        result = result && test;
    }
    {
        /*
         * This test verifies that method ObjectTest::ExportData signals an
         * error because the testing object is an instance of a registered but
         * non introspectable class.
         */
        bool test = false;
        NonIntrospectableIntegerObject obj;
        ConfigurationDatabase cdb;
        obj.SetName("Test2");
        obj.member = 20;
        test = !obj.ExportData(cdb);
        result = result && test;
    }
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

