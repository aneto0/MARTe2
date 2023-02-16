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

#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItemT.h"
#include "HeapManager.h"
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

/**
 * @brief Helper class to support the testing of references.
 */
class DummyObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    DummyObject() {
        member = 1;
    }

    virtual ~DummyObject() {
    }

    int32 member;

};

CLASS_REGISTER(DummyObject, "1.0")

DECLARE_CLASS_MEMBER(DummyObject, member, int32, "", "");
static const IntrospectionEntry* objFields[] = { &DummyObject_member_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(DummyObject, objFields);
//CLASS_INTROSPECTION_REGISTER(DummyObject, "1.0", DummyObject_introspection)

DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructure, nestedMember1, uint32, "", "");

static const IntrospectionEntry* nestedFields[] = { &TestIntrospectionNestedStructure_nestedMember1_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionNestedStructure, nestedFields);
//INTROSPECTION_REGISTER(TestIntrospectionNestedStructure, "1.0", TestIntrospectionNestedStructure_introspection)

static IntrospectionEntry member1Field("member1", "uint32", "", "", INTROSPECTION_MEMBER_SIZE(TestIntrospectionStructure, member1),
                                       INTROSPECTION_MEMBER_INDEX(TestIntrospectionStructure, member1));

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member2, float32, "*", "");

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member3, float64, "[32]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member4, string, "C", "");

DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member5, TestIntrospectionNestedStructure, "", "");

static const IntrospectionEntry* fields[] = { &member1Field, &TestIntrospectionStructure_member2_introspectionEntry,
        &TestIntrospectionStructure_member3_introspectionEntry, &TestIntrospectionStructure_member4_introspectionEntry,
        &TestIntrospectionStructure_member5_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionStructure, fields);
//INTROSPECTION_REGISTER(TestIntrospectionStructure, "1.0", TestIntrospectionStructure_introspection)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IntrospectionTest::TestConstructor() {

    ////// check member 1

    const IntrospectionEntry member1Copy = TestIntrospectionStructure_introspection[0];
    if (StringHelper::Compare(member1Copy.GetMemberName(), "member1") != 0) {
        return false;
    }

    if (member1Copy.IsConstant()) {
        return false;
    }

    if (member1Copy.GetMemberTypeDescriptor().type != UnsignedInteger) {
        return false;
    }

    if (member1Copy.GetMemberTypeDescriptor().numberOfBits != 32) {
        return false;
    }

    if (StringHelper::Compare(member1Copy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (member1Copy.GetMemberByteOffset() != 0) {
        return false;
    }

    if (member1Copy.GetMemberSize() != 4) {
        return false;
    }

    //////// check member2

    const IntrospectionEntry member2Copy = TestIntrospectionStructure_introspection[1];
    if (StringHelper::Compare(member2Copy.GetMemberName(), "member2") != 0) {
        return false;
    }

    if (member2Copy.IsConstant()) {
        return false;
    }

    if (member2Copy.GetMemberTypeDescriptor().type != Float) {
        return false;
    }

    if (member2Copy.GetMemberTypeDescriptor().numberOfBits != 32) {
        return false;
    }

    if (StringHelper::Compare(member2Copy.GetMemberModifiers(), "*") != 0) {
        return false;
    }

    if (member2Copy.GetMemberSize() != sizeof(float32*)) {
        return false;
    }

    // the members are aligned !
    if (member2Copy.GetMemberByteOffset() != sizeof(float32*)) {
        return false;
    }
    return true;
}

bool IntrospectionTest::TestPositionOperator() {

    //////// check member3
    const IntrospectionEntry member3Copy = TestIntrospectionStructure_introspection[2];
    if (StringHelper::Compare(member3Copy.GetMemberName(), "member3") != 0) {
        return false;
    }

    if (member3Copy.IsConstant()) {
        return false;
    }

    if (member3Copy.GetMemberTypeDescriptor().type != Float) {
        return false;
    }

    if (member3Copy.GetMemberTypeDescriptor().numberOfBits != 64) {
        return false;
    }

    if (StringHelper::Compare(member3Copy.GetMemberModifiers(), "[32]") != 0) {
        return false;
    }

    if (member3Copy.GetMemberSize() != sizeof(float64) * 32) {
        return false;
    }

    // the members are aligned !
    if (member3Copy.GetMemberByteOffset() != 2 * sizeof(float32*)) {
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
    if (StringHelper::Compare(member4Copy.GetMemberName(), "member4") != 0) {
        return false;
    }

    if (!member4Copy.IsConstant()) {
        return false;
    }

    if (member4Copy.GetMemberTypeDescriptor().type != BT_CCString) {
        return false;
    }

    if (member4Copy.GetMemberTypeDescriptor().numberOfBits != sizeof(const char8*) * 8) {
        return false;
    }

    if (StringHelper::Compare(member4Copy.GetMemberModifiers(), "C") != 0) {
        return false;
    }

    if (member4Copy.GetMemberSize() != sizeof(const char8*)) {
        return false;
    }

    // the members are aligned !
    if (member4Copy.GetMemberByteOffset() != 2 * sizeof(float32*) + 32 * sizeof(float64)) {
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
    if (StringHelper::Compare(member4Copy.GetMemberName(), "member5") != 0) {
        return false;
    }

    if (member4Copy.IsConstant()) {
        return false;
    }

    if (!member4Copy.GetMemberTypeDescriptor().isStructuredData) {
        return false;
    }

    if (StringHelper::Compare(member4Copy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (member4Copy.GetMemberSize() != sizeof(TestIntrospectionNestedStructure)) {
        return false;
    }

    // the members are aligned !
    if (member4Copy.GetMemberByteOffset() != 2 * sizeof(float32*) + 32 * sizeof(float64) + sizeof(const char8*)) {
        return false;
    }
    return true;

}

bool IntrospectionTest::TestGetNumberOfMembers() {

    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *item = instance->Find("TestIntrospectionStructure");
    if (item == NULL) {
        return false;
    }

    const Introspection *classIntro = item->GetIntrospection();
    return classIntro->GetNumberOfMembers() == 5u;
}

bool IntrospectionTest::TestGetClassSize() {
    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *item = instance->Find("TestIntrospectionStructure");
    if (item == NULL) {
        return false;
    }

    const Introspection *classIntro = item->GetIntrospection();
    return classIntro->GetClassSize() == sizeof(TestIntrospectionStructure);
}

bool IntrospectionTest::TestSetIntrospectionEntryMemory() {
    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *item = instance->Find("TestIntrospectionStructure");
    if (item == NULL) {
        return false;
    }

    Introspection *classIntro = const_cast<Introspection *>(item->GetIntrospection());
    uint32 t = 0u;
    classIntro->SetIntrospectionEntryMemory(&t);

    return classIntro->GetIntrospectionEntryMemory() == &t;
}

bool IntrospectionTest::TestGetIntrospectionEntryMemory() {
    return TestSetIntrospectionEntryMemory();
}

bool IntrospectionTest::TestMacroToAddObjectClassRegistryDatabase() {

    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("DummyObject");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry memberCopy = (*introInfo->GetIntrospection())[0];
    if (StringHelper::Compare(memberCopy.GetMemberName(), "member") != 0) {
        return false;
    }

    if (memberCopy.IsConstant()) {
        return false;
    }

    if (memberCopy.GetMemberTypeDescriptor().isStructuredData) {
        return false;
    }

    if (StringHelper::Compare(memberCopy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (memberCopy.GetMemberSize() != sizeof(int32)) {
        return false;
    }

    DummyObject obj;
    char8* objBegin = (char8*) &obj;

    char8* memberPtrChar = objBegin + memberCopy.GetMemberByteOffset();

    int32* memberPtr = (int32*) memberPtrChar;

    *memberPtr = 2;

    return obj.member == 2;

}
