/**
 * @file TypeConversionTest.cpp
 * @brief Source file for class TypeConversionTest
 * @date 12/11/2015
 * @author Giuseppe Ferro
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

#include "ConfigurationDatabase.h"
#include "IntrospectionTestHelper.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "TypeConversionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

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

bool TypeConversionTest::TestBooleanToCArrayScalar_Trunc() {
    bool ret = true;
    char8 output[3];

    bool input = true;

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "tr") != 0) {
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
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TestIntrospectionObjectTo testDestination;

    // set the buffer to the pointer!
    char8 outBuff[64];
    testDestination.member5_to.nestedMember2_to=outBuff;


    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    if (!TypeConvert(destination, source)) {
        return false;
    }

    if (StringHelper::Compare(testDestination.member1_to, "1") != 0) {
        return false;
    }
    if (testDestination.member2_to != (uintp) testSource.member2_from) {
        return false;
    }
    for (uint32 i = 0u; i < 32; i++) {
        if (testDestination.member3_to[i] != i) {
            return false;
        }
    }

    bool ok = (testDestination.member4_to[0][0] == 4);
    ok &= (testDestination.member4_to[0][1] == 5);
    ok &= (testDestination.member4_to[1][0] == 6);
    ok &= (testDestination.member4_to[1][1] == 7);

    if(!ok){
        return false;
    }

    if(*testDestination.member5_to.nestedMember1_to != 5){
        return false;
    }

    return StringHelper::Compare(testDestination.member5_to.nestedMember2_to , "12345")==0;
}




bool TypeConversionTest::TestObjectToObject_Reverse() {

    TestIntrospectionObjectTo testSource;
    testSource.member1_to[0] = '1';
    testSource.member1_to[1] = '\0';
    float32 sourceMember2 = 2.0;
    testSource.member2_to = (uint64)&sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_to[i] = i;
    }
    testSource.member4_to[0][0] = 4;
    testSource.member4_to[0][1] = 5;
    testSource.member4_to[1][0] = 6;
    testSource.member4_to[1][1] = 7;

    uint32 member5=5;
    testSource.member5_to.nestedMember1_to = &member5;
    testSource.member5_to.nestedMember2_to = (char8*)"12345";

    TestIntrospectionObjectFrom testDestination;
    //assign a buffer to destination
    char8 buff11[16];
    char8 buff12[16];
    char8 buff21[16];
    char8 buff22[16];
    testDestination.member4_from[0][0]=buff11;
    testDestination.member4_from[0][1]=buff12;
    testDestination.member4_from[1][0]=buff21;
    testDestination.member4_from[1][1]=buff22;


    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    if (!TypeConvert(destination, source)) {
        return false;
    }

    if (testDestination.member1_from!=1) {
        return false;
    }
    if ((uintp)testDestination.member2_from != testSource.member2_to) {
        return false;
    }
    for (uint32 i = 0u; i < 32; i++) {
        if (!IsEqual(testDestination.member3_from[i], (float32)i)) {
            return false;
        }
    }

    bool ok = (StringHelper::Compare(testDestination.member4_from[0][0], "4")==0);
    ok &= (StringHelper::Compare(testDestination.member4_from[0][1], "5")==0);
    ok &= (StringHelper::Compare(testDestination.member4_from[1][0], "6")==0);
    ok &= (StringHelper::Compare(testDestination.member4_from[1][1], "7")==0);

    if(!ok){
        return false;
    }

    if(*testDestination.member5_from.nestedMember1_from != 5){
        return false;
    }

    return testDestination.member5_from.nestedMember2_from == 12345;
}

bool TypeConversionTest::TestObjectToObject_ErrorNoSourceIntrospection() {

    TestNoIntrospectionObject testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TestIntrospectionObjectTo testDestination;

    // set the buffer to the pointer!
    char8 outBuff[64];
    testDestination.member5_to.nestedMember2_to=outBuff;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestNoIntrospectionObject")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    return (!TypeConvert(destination, source));

}

bool TypeConversionTest::TestObjectToObject_ErrorNoDestIntrospection() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TestNoIntrospectionObject testDestination;


    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestNoIntrospectionObject")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    return (!TypeConvert(destination, source));

}

bool TypeConversionTest::TestObjectToObject_NoCompatibility() {
    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][0] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TestIntrospectionObjectNoCompatibility testDestination;
    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(
            false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectNoCompatibility")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    return (!TypeConvert(destination, source));
}

bool TypeConversionTest::TestStructuredDataToObject_SourceIntrospection() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TestIntrospectionObjectTo testDestination;
    // set the buffer to the pointer!
    char8 outBuff[64];
    testDestination.member5_to.nestedMember2_to=outBuff;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("testSource");
    cdb.Write("Class", "TestIntrospectionObjectFrom");
    cdb.Write("member1_from", testSource.member1_from);
    // invert the order!
    cdb.Write("member3_from", testSource.member3_from);
    cdb.Write("member2_from", testSource.member2_from);

    cdb.Write("member4_from", testSource.member4_from);
    cdb.CreateRelative("member5_from");
    cdb.Write("nestedMember1_from", testSource.member5_from.nestedMember1_from);
    cdb.Write("nestedMember2_from", testSource.member5_from.nestedMember2_from);
    cdb.Write("Class", "TestIntrospectionNestedStructureFrom");
    cdb.MoveAbsolute("testSource");

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    if (!TypeConvert(destination, cdb)) {
        return false;
    }

    if (StringHelper::Compare(testDestination.member1_to, "1") != 0) {
        return false;
    }
    if (testDestination.member2_to != (uintp) testSource.member2_from) {
        return false;
    }
    for (uint32 i = 0u; i < 32; i++) {
        if (testDestination.member3_to[i] != i) {
            return false;
        }
    }

    bool ok = (testDestination.member4_to[0][0] == 4);
    ok &= (testDestination.member4_to[0][1] == 5);
    ok &= (testDestination.member4_to[1][0] == 6);
    ok &= (testDestination.member4_to[1][1] == 7);

    if(!ok){
        return false;
    }

    if(*testDestination.member5_to.nestedMember1_to != 5){
        return false;
    }

    return StringHelper::Compare(testDestination.member5_to.nestedMember2_to , "12345")==0;
}

bool TypeConversionTest::TestStructuredDataToObject_SourceIntrospectionArray() {
    const char8 * const config = ""
            "+Types = {"
            "  Class = ReferenceContainer"
            "  +TypeConversionTestEx0 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = char8"
            "       NumberOfElements = {64}"
            "     }"
            "  }"
            "  +TypeConversionTestEx1 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = uint32"
            "       NumberOfElements = {1}"
            "     }"
            "     Field2 = {"
            "       Type = TypeConversionTestEx0"
            "       NumberOfElements = {2,3}"
            "     }"
            "   }"
            "   +TypeConversionTestEx2 = {"
            "     Class = IntrospectionStructure"
            "     Field1A = {"
            "       Type = TypeConversionTestEx1"
            "       NumberOfElements = {3, 1, 2}"
            "     }"
            "     Field2A = {"
            "       Type = float32"
            "       NumberOfElements = 3"
            "     }"
            "     Field3A = {"
            "       Type = float64"
            "       NumberOfElements = 1"
            "     }"
            "     Field4A = {"
            "       Type = uint8"
            "       NumberOfElements = 1"
            "     }"
            "     Field5A = {"
            "       Type = uint16"
            "       NumberOfElements = 1"
            "     }"
            "   }"
            "}";

    struct __attribute__((__packed__)) TypeConversionTestEx0 {
        char8 field1[64];
    };
    struct __attribute__((__packed__)) TypeConversionTestEx1 {
        uint32 field1;
        TypeConversionTestEx0 field2[2][3];
    };
    struct __attribute__((__packed__)) TypeConversionTestEx2 {
        TypeConversionTestEx1 field1A[3][1][2];
        float32 field2A[3];
        float64 field3A;
        uint8 field4A;
        uint16 field5A;
    };
    ConfigurationDatabase cdb;
    StreamString configStr = config;
    configStr.Seek(0);
    StandardParser parser(configStr, cdb);
    bool ok = parser.Parse();

    if (ok) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ConfigurationDatabase source;
    for (uint32 i=0; i<3; i++) {
        for (uint32 j=0; j<1; j++) {
            for (uint32 k=0; k<2; k++) {
                StreamString field1Name;
                field1Name.Printf("Field1A[%d][%d][%d]", i, j, k);
                source.CreateAbsolute(field1Name.Buffer());
                source.Write("Field1", (i + 1) * 10 + j + k);
                for (uint32 a=0; a<2; a++) {
                    for (uint32 b=0; b<3; b++) {
                        StreamString field2CharName;
                        field2CharName.Printf("Field2[%d][%d]", a, b);
                        source.CreateRelative(field2CharName.Buffer());
                        StreamString value;
                        uint32 valInt = a*10+b;
                        value.Printf("AAA%dBBB", valInt);
                        source.Write("Field1", value.Buffer());
                        source.MoveToAncestor(1u);
                    }
                }
            }
        }
    }
    source.MoveToRoot();
    float32 farr[3];
    farr[0] = -5;
    farr[1] = -3;
    farr[2] = -7;
    source.Write("Field2A", farr);
    source.Write("Field3A", 103);
    source.Write("Field4A", 104);
    source.Write("Field5A", 105);
    /*StreamString temp;
    temp.Printf("%!", source);
    printf("%s\n\n", temp.Buffer());*/
    
    TypeConversionTestEx2 destStruct;
    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TypeConversionTestEx2")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &destStruct);
    source.MoveToRoot();

    ok = TypeConvert(destination, source);
    for (uint32 i=0; i<3; i++) {
        for (uint32 j=0; j<1; j++) {
            for (uint32 k=0; k<2; k++) {
                ok &= destStruct.field1A[i][j][k].field1 == ((i + 1) * 10 + j + k);
                for (uint32 a=0; a<2; a++) {
                    for (uint32 b=0; b<3; b++) {
                        StreamString stored = destStruct.field1A[i][j][k].field2[a][b].field1;
                        StreamString value;
                        uint32 valInt = a*10+b;
                        value.Printf("AAA%dBBB", valInt);
                        ok &= (stored == value);
                    }
                }
            }
        }
    }
    if (ok) {
        ok = destStruct.field2A[0] == -5;
        ok = destStruct.field2A[1] == -3;
        ok = destStruct.field2A[2] == -7;
    }
    return ok;
}


bool TypeConversionTest::TestStructuredDataToObject_NoSourceIntrospection() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TestIntrospectionObjectTo testDestination;
    // set the buffer to the pointer!
    char8 outBuff[64];
    testDestination.member5_to.nestedMember2_to=outBuff;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("testSource");
    cdb.Write("member1", testSource.member1_from);
    cdb.Write("member2", testSource.member2_from);
    cdb.Write("member3", testSource.member3_from);
    cdb.Write("member4", testSource.member4_from);
    cdb.CreateRelative("member5");
    cdb.Write("Class","FakeClassName");
    cdb.Write("nestedMember1", testSource.member5_from.nestedMember1_from);
    cdb.Write("nestedMember2", testSource.member5_from.nestedMember2_from);
    cdb.MoveAbsolute("testSource");

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectTo")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);

    if (!TypeConvert(destination, cdb)) {
        return false;
    }

    if (StringHelper::Compare(testDestination.member1_to, "1") != 0) {
        return false;
    }
    if (testDestination.member2_to != (uintp) testSource.member2_from) {
        return false;
    }
    for (uint32 i = 0u; i < 32; i++) {
        if (testDestination.member3_to[i] != i) {
            return false;
        }
    }

    bool ok = (testDestination.member4_to[0][0] == 4);
    ok &= (testDestination.member4_to[0][1] == 5);
    ok &= (testDestination.member4_to[1][0] == 6);
    ok &= (testDestination.member4_to[1][1] == 7);

    if(!ok){
        return false;
    }

    if(*testDestination.member5_to.nestedMember1_to != 5){
        return false;
    }

    return StringHelper::Compare(testDestination.member5_to.nestedMember2_to , "12345")==0;
}

bool TypeConversionTest::TestStructuredDataToObject_ErrorNoDestIntrospection() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("testSource");
    cdb.Write("member1", testSource.member1_from);
    cdb.Write("member2", testSource.member2_from);
    cdb.Write("member3", testSource.member3_from);
    cdb.Write("member4", testSource.member4_from);
    cdb.CreateRelative("member5");
    cdb.Write("nestedMember1", testSource.member5_from.nestedMember1_from);
    cdb.Write("nestedMember2", testSource.member5_from.nestedMember2_from);
    cdb.MoveAbsolute("testSource");

    TestNoIntrospectionObject testDestination;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(false, ClassRegistryDatabase::Instance()->Find("TestNoIntrospectionObject")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);
    return (!TypeConvert(destination, cdb));
}

bool TypeConversionTest::TestStructuredDataToObject_NoCompatibility() {
    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("testSource");
    cdb.Write("member1", testSource.member1_from);
    cdb.Write("member2", testSource.member2_from);
    cdb.Write("member3", testSource.member3_from);
    cdb.Write("member4", testSource.member4_from);
    cdb.CreateRelative("member5");
    cdb.Write("nestedMember1", testSource.member5_from.nestedMember1_from);
    cdb.Write("nestedMember2", testSource.member5_from.nestedMember2_from);
    cdb.MoveAbsolute("testSource");

    TestIntrospectionObjectNoCompatibility testDestination;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    TypeDescriptor destinationDes(
            false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectNoCompatibility")->GetClassProperties()->GetUniqueIdentifier());
    AnyType destination(destinationDes, 0u, &testDestination);
    return (!TypeConvert(destination, cdb));
}

bool TypeConversionTest::TestObjectToStructuredData() {

    TestIntrospectionObjectFrom testSource;
    testSource.member1_from = 1u;
    float32 sourceMember2 = 2.0;
    testSource.member2_from = &sourceMember2;
    for (uint32 i = 0u; i < 32; i++) {
        testSource.member3_from[i] = i;
    }
    testSource.member4_from[0][0] = (char8*)"4";
    testSource.member4_from[0][1] = (char8*)"5";
    testSource.member4_from[1][0] = (char8*)"6";
    testSource.member4_from[1][1] = (char8*)"7";

    uint32 member5=5;
    testSource.member5_from.nestedMember1_from = &member5;
    testSource.member5_from.nestedMember2_from = 12345;

    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TestIntrospectionObjectFrom")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &testSource);

    ConfigurationDatabase cdb;
    if (!TypeConvert(cdb, source)) {
        printf("\nError in type convert\n");
    }

    char8 destMember1[32];
    char8 className[32];
    char8 nestedClassName[64];
    uintp destMember2;
    float32 destMember3[32];
    uint32 destMember4[2][2];
    uintp destMember5;
    char8 destMember6[32];

    cdb.MoveAbsolute("testSource");
    cdb.Read("Class", className);
    cdb.Read("member1_from", destMember1);
    cdb.Read("member2_from", destMember2);
    cdb.Read("member3_from", destMember3);
    cdb.Read("member4_from", destMember4);
    cdb.MoveRelative("member5_from");
    cdb.Read("Class", nestedClassName);
    cdb.Read("nestedMember1_from", destMember5);
    cdb.Read("nestedMember2_from", destMember6);


    if (StringHelper::Compare(className, "TestIntrospectionObjectFrom") != 0) {
        return false;
    }

    if (StringHelper::Compare(nestedClassName, "TestIntrospectionNestedStructureFrom") != 0) {
        return false;
    }

    if (StringHelper::Compare(destMember1, "1") != 0) {
        return false;
    }
    if (destMember2 != (uintp) testSource.member2_from) {
        return false;
    }
    for (uint32 i = 0; i < 32; i++) {
        if (destMember3[i] != i) {
            return false;
        }
    }
    bool ok = (destMember4[0][0] == 4);
    ok &= (destMember4[0][1] == 5);
    ok &= (destMember4[1][0] == 6);
    ok &= (destMember4[1][1] == 7);

    if(!ok){
        return false;
    }

    if(*(uint32*)destMember5!=5){
        return false;
    }
    return StringHelper::Compare(destMember6, "12345")==0;
}

bool TypeConversionTest::TestObjectArrayToStructuredData() {
    const char8 * const config = ""
            "+Types = {"
            "  Class = ReferenceContainer"
            "  +TypeConversionTestEx0 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = char8"
            "       NumberOfElements = {64}"
            "     }"
            "  }"
            "  +TypeConversionTestEx1 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = uint32"
            "       NumberOfElements = {1}"
            "     }"
            "     Field2 = {"
            "       Type = TypeConversionTestEx0"
            "       NumberOfElements = {2,3}"
            "     }"
            "   }"
            "   +TypeConversionTestEx2 = {"
            "     Class = IntrospectionStructure"
            "     Field1A = {"
            "       Type = TypeConversionTestEx1"
            "       NumberOfElements = {3, 1, 2}"
            "     }"
            "     Field2A = {"
            "       Type = float32"
            "       NumberOfElements = 3"
            "     }"
            "     Field3A = {"
            "       Type = float64"
            "       NumberOfElements = 1"
            "     }"
            "     Field4A = {"
            "       Type = uint8"
            "       NumberOfElements = 1"
            "     }"
            "     Field5A = {"
            "       Type = uint16"
            "       NumberOfElements = 1"
            "     }"
            "   }"
            "}";

    struct __attribute__((__packed__)) TypeConversionTestEx0 {
        char8 field1[64];
    };
    struct __attribute__((__packed__)) TypeConversionTestEx1 {
        uint32 field1;
        TypeConversionTestEx0 field2[2][3];
    };
    struct __attribute__((__packed__)) TypeConversionTestEx2 {
        TypeConversionTestEx1 field1A[3][1][2];
        float32 field2A[3];
        float64 field3A;
        uint8 field4A;
        uint16 field5A;
    };
    ConfigurationDatabase cdb;
    StreamString configStr = config;
    configStr.Seek(0);
    StandardParser parser(configStr, cdb);
    bool ok = parser.Parse();

    if (ok) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }

    TypeConversionTestEx2 sourceStruct;
    TypeDescriptor sourceDes(false, ClassRegistryDatabase::Instance()->Find("TypeConversionTestEx2")->GetClassProperties()->GetUniqueIdentifier());
    AnyType source(sourceDes, 0u, &sourceStruct);

    for (uint32 i=0; i<3; i++) {
        for (uint32 j=0; j<1; j++) {
            for (uint32 k=0; k<2; k++) {
                ok &= sourceStruct.field1A[i][j][k].field1 = ((i + 1) * 10 + j + k);
                for (uint32 a=0; a<2; a++) {
                    for (uint32 b=0; b<3; b++) {
                        StreamString value;
                        uint32 valInt = a*10+b;
                        value.Printf("AAA%dBBB", valInt);
                        StringHelper::Copy(sourceStruct.field1A[i][j][k].field2[a][b].field1, value.Buffer());
                    }
                }
            }
        }
    }
    sourceStruct.field2A[0] = -5;
    sourceStruct.field2A[1] = -3;
    sourceStruct.field2A[2] = -7;
    sourceStruct.field3A = 103;
    sourceStruct.field4A = 104;
    sourceStruct.field5A = 105;

    ConfigurationDatabase destination;
    ok = TypeConvert(destination, source);

    for (uint32 i=0; i<3; i++) {
        for (uint32 j=0; j<1; j++) {
            for (uint32 k=0; k<2; k++) {
                StreamString field1Name;
                field1Name.Printf("Field1A[%d][%d][%d]", i, j, k);
                ok &= destination.MoveAbsolute(field1Name.Buffer());
                uint32 field1Value = 0;
                ok &= destination.Read("Field1", field1Value);
                ok &= (field1Value == ((i + 1) * 10 + j + k));
                for (uint32 a=0; a<2; a++) {
                    for (uint32 b=0; b<3; b++) {
                        StreamString field2CharName;
                        field2CharName.Printf("Field2[%d][%d]", a, b);
                        destination.MoveRelative(field2CharName.Buffer());
                        StreamString valueExp;
                        uint32 valInt = a*10+b;
                        valueExp.Printf("AAA%dBBB", valInt);
                        StreamString value;
                        destination.Read("Field1", value);
                        ok &= (value == valueExp);
                        destination.MoveToAncestor(1u);
                    }
                }
            }
        }
    }
    destination.MoveToRoot();
    float32 farr[3];
    float64 val64 = 0;
    uint8 val8 = 0;
    uint16 val16 = 0;
    ok &= destination.Read("Field2A", farr);
    ok &= destination.Read("Field3A", val64);
    ok &= destination.Read("Field4A", val8);
    ok &= destination.Read("Field5A", val16);
    /*StreamString temp;
    destination.MoveToRoot();
    temp.Printf("%!", destination);
    printf("%s\n\n", temp.Buffer());*/
    ok &= (farr[0] == -5);
    ok &= (farr[1] == -3);
    ok &= (farr[2] == -7);
    ok &= (val64 == 103);
    ok &= (val8 == 104);
    ok &= (val16 == 105);
    return ok;
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

    if (StringHelper::Compare(destMember1, "Hello World") != 0) {
        return false;
    }
    if (destMember2 != sourceMember2) {
        return false;
    }
    for (uint32 i = 0; i < 32; i++) {
        if (destMember3[i] != i) {
            return false;
        }
    }
    if (destMember4 != 4) {
        return false;
    }

    return destMember5 == 5;
}
