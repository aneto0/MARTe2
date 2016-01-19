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
#include "Object.h"
#include "ClassRegistryDatabase.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct TestIntrospectionNestedStructure {
    uint32 nestedMember1;
};

struct TestIntrospectionStructure {
    uint32 member1;
    float32 *member2;
    float64 member3[32];
    const char8 * const member4;
    TestIntrospectionNestedStructure member5;
};

static Introspection &Init() {

    DECLARE_BASIC_CLASS_MEMBER(TestIntrospectionNestedStructure, nestedMember1, false, UnsignedInteger, 32, "");

    static const IntrospectionEntry* nestedFields[] = { &TestIntrospectionNestedStructure_nestedMember1_introspectionEntry, 0 };

    static Introspection nestedIntrospection(nestedFields);
    INTROSPECTION_CLASS_REGISTER(TestIntrospectionNestedStructure, "1.0", nestedIntrospection)

    static IntrospectionEntry member1Field = { "member1", TypeDescriptor(false, UnsignedInteger, 32), "", introspectionMemberSize(TestIntrospectionStructure,
                                                                                                                                  member1),
            introspectionMemberIndex(TestIntrospectionStructure, member1) };

    DECLARE_BASIC_CLASS_MEMBER(TestIntrospectionStructure, member2, false, Float, 32, "*");

    DECLARE_BASIC_CLASS_MEMBER(TestIntrospectionStructure, member3, false, Float, 64, "32");

    DECLARE_BASIC_CLASS_MEMBER(TestIntrospectionStructure, member4, true, CCString, sizeof(const char8*) * 8, "C");

    DECLARE_STRUCTURED_CLASS_MEMBER(TestIntrospectionStructure, member5, false, TestIntrospectionNestedStructure, "");

    static const IntrospectionEntry* fields[] = { &member1Field, &TestIntrospectionStructure_member2_introspectionEntry,
            &TestIntrospectionStructure_member3_introspectionEntry, &TestIntrospectionStructure_member4_introspectionEntry, &TestIntrospectionStructure_member5_introspectionEntry, 0 };

    static Introspection testIntrospection(fields);
    INTROSPECTION_CLASS_REGISTER(TestIntrospectionStructure, "1.0", testIntrospection)

    return testIntrospection;
}

class Initializer {
public:
    Initializer() {
        Init();
    }
};

static Initializer initializer;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IntrospectionTest::TestConstructor() {

    ////// check member 1

    const IntrospectionEntry member1Copy = Init()[0];
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

    if (member1Copy.size != 4) {
        return false;
    }

    //////// check member2

    const IntrospectionEntry member2Copy = Init()[1];
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

    if (member2Copy.type.numberOfBits != 32) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member2Copy.modifiers, "*") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member2Copy.size != sizeof(float32*)) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member2Copy.byteOffset != sizeof(float32*)) {
        printf("\n6 %d\n", member2Copy.byteOffset);
        return false;
    }
    return true;
}

bool IntrospectionTest::TestPositionOperator() {

    //////// check member3
    const IntrospectionEntry member3Copy = Init()[2];
    if (StringHelper::Compare(member3Copy.memberName, "member3") != 0) {
        printf("\n1\n");
        return false;
    }

    if (member3Copy.type.isConstant) {
        printf("\n2\n");
        return false;
    }

    if (member3Copy.type.type != Float) {
        printf("\n3\n");
        return false;
    }

    if (member3Copy.type.numberOfBits != 64) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member3Copy.modifiers, "32") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member3Copy.size != sizeof(float64) * 32) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member3Copy.byteOffset != 2 * sizeof(float32*)) {
        printf("\n6 %d\n", member3Copy.byteOffset);
        return false;
    }
    return true;
}

bool IntrospectionTest::TestMacroToAddBasicInClassRegistryDatabase() {
    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("TestIntrospectionStructure");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry member4Copy = (*introInfo->GetIntrospection())[3];
    if (StringHelper::Compare(member4Copy.memberName, "member4") != 0) {
        printf("\n1\n");
        return false;
    }

    if (!member4Copy.type.isConstant) {
        printf("\n2\n");
        return false;
    }

    if (member4Copy.type.type != CCString) {
        printf("\n3\n");
        return false;
    }

    if (member4Copy.type.numberOfBits != sizeof(const char8*) * 8) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member4Copy.modifiers, "C") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member4Copy.size != sizeof(const char8*)) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member4Copy.byteOffset != 2 * sizeof(float32*) + 32 * sizeof(float64)) {
        printf("\n6 %d\n", member4Copy.byteOffset);
        return false;
    }
    return true;

}

bool IntrospectionTest::TestMacroToAddStructuredInClassRegistryDatabase() {

    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("TestIntrospectionStructure");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry member4Copy = (*introInfo->GetIntrospection())[4];
    if (StringHelper::Compare(member4Copy.memberName, "member5") != 0) {
        printf("\n1\n");
        return false;
    }

    if (member4Copy.type.isConstant) {
        printf("\n2\n");
        return false;
    }

    if (!member4Copy.type.isStructuredData) {
        printf("\n3\n");
        return false;
    }

    if (StringHelper::Compare(member4Copy.modifiers, "") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member4Copy.size != sizeof(TestIntrospectionNestedStructure)) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member4Copy.byteOffset != 2 * sizeof(float32*) + 32 * sizeof(float64) + sizeof(const char8*)) {
        printf("\n6 %d\n", member4Copy.byteOffset);
        return false;
    }
    return true;

}
