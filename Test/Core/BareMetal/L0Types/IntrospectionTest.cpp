/**
 * @file IntrospectionTest.cpp
 * @brief Source file for class IntrospectionTest
 * @date 18/01/2016
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
 * the class IntrospectionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IntrospectionTest.h"
#include "StringHelper.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct TestIntrospectionStructure {
    uint32 member1;
    float32 *member2;
    float64 member3[32];
    const char8 * const member4;
};

static IntrospectionEntry member1Field = { "member1", TypeDescriptor(false, UnsignedInteger,
introspectionMemberSize(TestIntrospectionStructure, member1) * 8u), "", introspectionMemberIndex(TestIntrospectionStructure, member1) };

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member2, false, Float, "*");

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member3, false, Float, "32");

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member4, true, CCString, "");

static const IntrospectionEntry* fields[] = { &member1Field, &TestIntrospectionStructure_member2_introspectionEntry,
        &TestIntrospectionStructure_member3_introspectionEntry, &TestIntrospectionStructure_member4_introspectionEntry };

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IntrospectionTest::TestConstructor() {
    Introspection testIntrospection(fields);

    ////// check member 1

    const IntrospectionEntry member1Copy = testIntrospection[0];
    if (StringHelper::Compare(member1Copy.memberName, "member1") != 0) {
        return false;
    }

    if (member1Copy.type.isConstant) {
        return false;
    }

    if (member1Copy.type.type != UnsignedInteger) {
        return false;
    }

    if (member1Copy.type.numberOfBits != 32) {
        return false;
    }

    if (StringHelper::Compare(member1Copy.modifiers, "") != 0) {
            return false;
        }

    if (member1Copy.byteOffset != 0) {
        return false;
    }

    //////// check member2

    const IntrospectionEntry member2Copy = testIntrospection[1];
    if (StringHelper::Compare(member2Copy.memberName, "member2") != 0) {
        printf("\n1\n");
        return false;
    }

    if (member2Copy.type.isConstant) {
        printf("\n2\n");
        return false;
    }

    if (member2Copy.type.type != Float) {
        printf("\n3\n");
        return false;
    }

    if (member2Copy.type.numberOfBits != sizeof(float32*)*8) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member2Copy.modifiers, "*") != 0) {
        printf("\n5\n");
            return false;
        }


    // the members are aligned !
    if (member2Copy.byteOffset != 8) {
        printf("\n6 %d\n", member2Copy.byteOffset );
        return false;
    }


    return true;
}

