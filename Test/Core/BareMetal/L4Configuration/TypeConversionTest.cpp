/**
 * @file TypeConversionTest.cpp
 * @brief Source file for class TypeConversionTest
 * @date 12/nov/2015
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
 * the class TypeConversionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeConversionTest.h"
#include "Object.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

struct TestIntrospectionNestedStructureFrom {
    uint32 nestedMember1_from;
};

struct TestIntrospectionNestedStructureTo {
    uint8 nestedMember1_to;
};

struct TestIntrospectionObjectFrom {
    uint32 member1_from;
    float32 *member2_from;
    float64 member3_from[32];
    const char8 * member4_from;
    TestIntrospectionNestedStructureFrom member5_from;
};

struct TestIntrospectionObjectTo {
    char8 member1_to[32];
    uint64 member2_to;
    float32 member3_to[32];
    uint32 member4_to;
    TestIntrospectionNestedStructureTo member5_to;
};

// the from introspection
DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructureFrom, nestedMember1_from, uint32,  "","");

static const IntrospectionEntry* nestedFields[] = { &TestIntrospectionNestedStructureFrom_nestedMember1_from_introspectionEntry, 0 };

static Introspection nestedIntrospectionFrom(nestedFields);
INTROSPECTION_CLASS_REGISTER(TestIntrospectionNestedStructureFrom, "1.0", nestedIntrospectionFrom)

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member1_from, uint32, "","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member2_from, float32, "*","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member3_from, float64, "[32]","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member4_from, string, "","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member5_from, TestIntrospectionNestedStructureFrom,  "","");

static const IntrospectionEntry* fieldsFrom[] = { &TestIntrospectionObjectFrom_member1_from_introspectionEntry,
        &TestIntrospectionObjectFrom_member2_from_introspectionEntry, &TestIntrospectionObjectFrom_member3_from_introspectionEntry,
        &TestIntrospectionObjectFrom_member4_from_introspectionEntry, &TestIntrospectionObjectFrom_member5_from_introspectionEntry, 0 };

static Introspection testIntrospectionFrom(fieldsFrom);
INTROSPECTION_CLASS_REGISTER(TestIntrospectionObjectFrom, "1.0", testIntrospectionFrom)

//the to introspection
DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructureTo, nestedMember1_to, uint8,  "","");

static const IntrospectionEntry* nestedFieldsTo[] = { &TestIntrospectionNestedStructureTo_nestedMember1_to_introspectionEntry, 0 };

static Introspection nestedIntrospectionTo(nestedFieldsTo);
INTROSPECTION_CLASS_REGISTER(TestIntrospectionNestedStructureTo, "1.0", nestedIntrospectionTo)

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member1_to, char8,  "[32]","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member2_to, uint64, "","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member3_to, float32, "[32]","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member4_to, uint32, "","");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member5_to, TestIntrospectionNestedStructureTo,  "","");

static const IntrospectionEntry* fieldsTo[] = { &TestIntrospectionObjectTo_member1_to_introspectionEntry,
        &TestIntrospectionObjectTo_member2_to_introspectionEntry, &TestIntrospectionObjectTo_member3_to_introspectionEntry,
        &TestIntrospectionObjectTo_member4_to_introspectionEntry, &TestIntrospectionObjectTo_member5_to_introspectionEntry, 0 };

static Introspection testIntrospectionTo(fieldsTo);
INTROSPECTION_CLASS_REGISTER(TestIntrospectionObjectTo, "1.0", testIntrospectionTo)

bool TypeConversionTest::TestCArrayToCCStringScalar() {

    bool ret = true;
    char8 input[32];

    StringHelper::Copy(input, "HelloWorld");

    char8 *output = (char8*) HeapManager::Malloc(32);

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(input, output) != 0) {
        ret = false;
    }

    HeapManager::Free((void*&) output);

    return ret;
}
bool TypeConversionTest::TestCArrayToCCStringVector() {
    bool ret = true;
    char8 input[2][32];

    StringHelper::Copy(&input[0][0], "HelloWorld_1");
    StringHelper::Copy(&input[1][0], "HelloWorld_2");

    char8 **vector = (char8**) HeapManager::Malloc(2 * sizeof(char*));
    vector[0] = (char8*) HeapManager::Malloc(32 * sizeof(char));
    vector[1] = (char8*) HeapManager::Malloc(32 * sizeof(char));

    Vector<char8*> output(vector, 2);

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(&input[0][0], output[0]) != 0) {
        ret = false;
    }
    if (StringHelper::Compare(&input[1][0], output[1]) != 0) {
        ret = false;
    }

    HeapManager::Free((void*&) output[0]);
    HeapManager::Free((void*&) output[1]);
    HeapManager::Free((void*&) output);

    return ret;
}

bool TypeConversionTest::TestCArrayToCCStringMatrix() {

    bool ret = true;
    const uint32 nRows = 2;
    const uint32 nCols = 3;
    const uint32 buffSize = 32;
    char8 input[nRows][nCols][buffSize];

    StringHelper::Copy(&input[0][0][0], "HelloWorld_1_1");
    StringHelper::Copy(&input[0][1][0], "HelloWorld_1_2");
    StringHelper::Copy(&input[0][2][0], "HelloWorld_1_3");
    StringHelper::Copy(&input[1][0][0], "HelloWorld_2_1");
    StringHelper::Copy(&input[1][1][0], "HelloWorld_2_2");
    StringHelper::Copy(&input[1][2][0], "HelloWorld_2_3");

    char8 ***matrix = (char8***) HeapManager::Malloc(2 * sizeof(char**));

    for (uint32 i = 0; i < nRows; i++) {
        matrix[i] = (char8**) HeapManager::Malloc(nCols * sizeof(char**));
        for (uint32 j = 0; j < nCols; j++) {
            matrix[i][j] = (char8*) HeapManager::Malloc(buffSize * sizeof(char));
        }
    }

    Matrix<char8*> output(matrix, 2, 3);

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (StringHelper::Compare(&input[i][j][0], output[i][j]) != 0) {
                ret = false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            HeapManager::Free((void*&) matrix[i][j]);
        }
        HeapManager::Free((void*&) matrix[i]);
    }

    HeapManager::Free((void*&) matrix);

    return ret;

}

bool TypeConversionTest::TestIntegerToCArrayScalar_Trunc() {
    bool ret = true;
    char8 output[5];

    int32 input = 12345;

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "1234") != 0) {
        ret = false;
    }

    return ret;
}

bool TypeConversionTest::TestFloatToCArrayScalar_Trunc() {
    bool ret = true;
    char8 output[5];

    float32 input = 1.0;

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "1.00") != 0) {
        ret = false;
    }

    return ret;
}

bool TypeConversionTest::TestCCStringToCArrayScalar_Trunc() {

    bool ret = true;
    char8 output[5];

    const char8* input = "HelloWorld";

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "Hell") != 0) {
        ret = false;
    }

    return ret;
}

bool TypeConversionTest::TestCCStringToCArrayVector_Trunc() {
    bool ret = true;
    char8 output[2][6];

    const char8* input[] = { "HelloWorld", "12345" };

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(&output[0][0], "Hello") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[1][0], "12345") != 0) {
        ret = false;
    }

    return ret;

}

bool TypeConversionTest::TestCCStringToCArrayMatrix_Trunc() {
    bool ret = true;
    char8 output[2][3][6];

    const char8* input[][3] = { { "HelloWorld", "12345", "a" }, { "abc", "abcdefghilmnopq", "**********" } };

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(&output[0][0][0], "Hello") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[0][1][0], "12345") != 0) {
        ret = false;
    }
    if (StringHelper::Compare(&output[0][2][0], "a") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[1][0][0], "abc") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[1][1][0], "abcde") != 0) {
        ret = false;
    }
    if (StringHelper::Compare(&output[1][2][0], "*****") != 0) {
        ret = false;
    }

    return ret;

}

bool TypeConversionTest::TestObjectToObject() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from = "4";

    testSource.member5_from.nestedMember1_from = 5;

    TestIntrospectionObjectTo testDestination;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    if (!TypeConvert(destination, source)) {
        printf("\nError in type convert\n");
    }

    printf("\n1) %s\n", testDestination.member1_to);

    printf("\n2) %llx %llx\n", testDestination.member2_to, (uintp) testSource.member2_from);

    printf("\n3) %f\n", testDestination.member3_to[31]);

    printf("\n4) %d\n", testDestination.member4_to);

    printf("\n5) %d\n", testDestination.member5_to.nestedMember1_to);

    return true;
}

bool TypeConversionTest::TestStructuredDataToObject() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from = "4";

    testSource.member5_from.nestedMember1_from = 5;

    TestIntrospectionObjectTo testDestination;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("testSource");
    cdb.Write("Class", "TestIntrospectionObjectFrom");
    cdb.Write("member1_from", testSource.member1_from);
    cdb.Write("member2_from", reinterpret_cast<uintp>(testSource.member2_from));
    cdb.Write("member3_from", testSource.member3_from);
    cdb.Write("member4_from", testSource.member4_from);
    cdb.CreateRelative("member5_from");
    cdb.Write("nestedMember1_from", testSource.member5_from.nestedMember1_from);
    cdb.Write("Class", "TestIntrospectionNestedStructureFrom");
    cdb.MoveAbsolute("testSource");

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    if (!TypeConvert(destination, cdb)) {
        printf("\nError in type convert\n");
    }

    printf("\n1) %s\n", testDestination.member1_to);

    printf("\n2) %llx %llx\n", testDestination.member2_to, (uintp) testSource.member2_from);

    printf("\n3) %f\n", testDestination.member3_to[31]);

    printf("\n4) %d\n", testDestination.member4_to);

    printf("\n5) %d\n", testDestination.member5_to.nestedMember1_to);

    return true;
}

bool TypeConversionTest::TestObjectToStructuredData() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from = "4";

    testSource.member5_from.nestedMember1_from = 5;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    ConfigurationDatabase cdb;
    if (!TypeConvert(cdb, source)) {
        printf("\nError in type convert\n");
    }

    char8 destMember1[32];
    uintp destMember2;
    float32 destMember3[32];
    uint32 destMember4;
    uint8 destMember5;

    cdb.MoveAbsolute("testSource");
    cdb.Read("member1_from", destMember1);
    cdb.Read("member2_from", destMember2);
    cdb.Read("member3_from", destMember3);
    cdb.Read("member4_from", destMember4);
    cdb.MoveRelative("member5_from");
    cdb.Read("nestedMember1_from", destMember5);

    printf("\n1) %s\n", destMember1);

    printf("\n2) %llx %llx\n", destMember2, (uintp) testSource.member2_from);

    printf("\n3) %f\n", destMember3[31]);

    printf("\n4) %d\n", destMember4);

    printf("\n5) %d\n", destMember5);

    return true;
}

bool TypeConversionTest::TestStructuredDataToStructuredData() {

    ConfigurationDatabase sourceCDB;
    const char8* sourceMember1 = "Hello World";
    uint16 sourceMember2 = 2;
    float32 sourceMember3[32];
    for (uint32 i = 0u; i < 32; i++) {
        sourceMember3[i] = i;
    }
    uint32 sourceMember4 = 4;
    uint8 sourceMember5 = 5;

    sourceCDB.CreateAbsolute("testSource");
    sourceCDB.Write("member1_from", sourceMember1);
    sourceCDB.Write("member2_from", sourceMember2);
    sourceCDB.Write("member3_from", sourceMember3);
    sourceCDB.Write("member4_from", sourceMember4);
    sourceCDB.CreateRelative("member5_from");
    sourceCDB.Write("nestedMember1_from", sourceMember5);

    sourceCDB.MoveToRoot();

    ConfigurationDatabase destinationCDB;
    //need to be inside

    TypeConvert(destinationCDB, sourceCDB);
    //destinationCDB.MoveToRoot();

    char8 destMember1[32];
    uint16 destMember2;
    float32 destMember3[32];
    uint32 destMember4;
    uint8 destMember5;

    destinationCDB.MoveAbsolute("testSource");
    destinationCDB.Read("member1_from", destMember1);
    destinationCDB.Read("member2_from", destMember2);
    destinationCDB.Read("member3_from", destMember3);
    destinationCDB.Read("member4_from", destMember4);
    destinationCDB.MoveRelative("member5_from");
    destinationCDB.Read("nestedMember1_from", destMember5);

    printf("\n1) %s\n", destMember1);

    printf("\n2) %llx\n", destMember2);

    printf("\n3) %f\n", destMember3[31]);

    printf("\n4) %d\n", destMember4);

    printf("\n5) %d\n", destMember5);

    return true;
}
