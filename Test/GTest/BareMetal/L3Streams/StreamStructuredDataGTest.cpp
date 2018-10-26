/**
 * @file StreamStructuredDataGTest.cpp
 * @brief Source file for class StreamStructuredDataGTest
 * @date 07/09/2018
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
 * the class StreamStructuredDataGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "StreamStructuredDataTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(StreamStructuredDataGTest, TestRead) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;
    ASSERT_TRUE(myStreamStructuredDataTest.TestRead());
}

TEST(StreamStructuredDataGTest, TestGetType) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;
    ASSERT_TRUE(myStreamStructuredDataTest.TestGetType());
}

TEST(StreamStructuredDataGTest, TestWriteStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;
    uint32 x = 1;
    int32 y = -1;
    const char8 *z = "hello";
    float32 floatArr[] = { -1.5, 3.25, 55.4 };
    StreamString stringMat[][2] = { { "Hello", "Ciao" }, { "World", "Mondo" } };
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z, floatArr, stringMat, voidAnyType };
    const char8 *names[] = { "x", "y", "z", "floatArr", "stringMat", NULL};
    StreamStructuredDataTestWriteStruct table;
    table.toWrite = at;
    table.varnames = names;
    table.desResult = "\n\rx =+1\n\r"
            "y =-1\n\r"
            "z =\"hello\"\n\r"
            "floatArr ={ -1.500000 3.250000 55.400004 } \n\r"
            "stringMat ={ { \"Hello\" \"Ciao\" } { \"World\" \"Mondo\" } } ";
    ASSERT_TRUE(myStreamStructuredDataTest.TestWrite(&table));
}

TEST(StreamStructuredDataGTest, TestWriteJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;
    uint32 x = 1;
    int32 y = -1;
    const char8 *z = "hello";
    float32 floatArr[] = { -1.5, 3.25, 55.4 };
    StreamString stringMat[][2] = { { "Hello", "Ciao" }, { "World", "Mondo" } };
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z, floatArr, stringMat, voidAnyType };
    const char8 *names[] = { "x", "y", "z", "floatArr", "stringMat", NULL};
    StreamStructuredDataTestWriteStruct table;
    table.toWrite = at;
    table.varnames = names;
    table.desResult = "\n\r\"x\": +1,\n\r"
            "\"y\": -1,\n\r"
            "\"z\": \"hello\",\n\r"
            "\"floatArr\": [ -1.500000, 3.250000, 55.400004 ] ,\n\r"
            "\"stringMat\": [ [ \"Hello\", \"Ciao\" ] ,[ \"World\", \"Mondo\" ] ] ";
    ASSERT_TRUE(myStreamStructuredDataTest.TestWrite(&table));
}

TEST(StreamStructuredDataGTest, TestWriteXML) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;
    uint32 x = 1;
    int32 y = -1;
    const char8 *z = "hello";
    float32 floatArr[] = { -1.5, 3.25, 55.4 };
    StreamString stringMat[][2] = { { "Hello", "Ciao" }, { "World", "Mondo" } };
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z, floatArr, stringMat, voidAnyType };
    const char8 *names[] = { "x", "y", "z", "floatArr", "stringMat", NULL};
    StreamStructuredDataTestWriteStruct table;
    table.toWrite = at;
    table.varnames = names;
    table.desResult = "\n\r<x>+1</x>\n\r"
            "<y>-1</y>\n\r"
            "<z>\"hello\"</z>\n\r"
            "<floatArr>[ -1.500000, 3.250000, 55.400004 ] </floatArr>\n\r"
            "<stringMat>[ [ \"Hello\", \"Ciao\" ] ,[ \"World\", \"Mondo\" ] ] </stringMat>";
    ASSERT_TRUE(myStreamStructuredDataTest.TestWrite(&table));
}

TEST(StreamStructuredDataGTest, TestCopy) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;
    ASSERT_TRUE(myStreamStructuredDataTest.TestCopy());
}

TEST(StreamStructuredDataGTest, TestAddToCurrentNodeStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestAddToCurrentNode(&arg));
}


TEST(StreamStructuredDataGTest, TestAddToCurrentNodeStandard_DeleteBlocks) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(StreamStructuredDataGTest, TestAddToCurrentNodeJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(StreamStructuredDataGTest, TestAddToCurrentNodeJson2) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A", "A.B", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(StreamStructuredDataGTest, TestAddToCurrentNodeXML) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>";

    ASSERT_TRUE(myStreamStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToRootStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToRoot(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToRootJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToRoot(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToRootXML) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>\n\r"
            "</B>\n\r"
            "</A>";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToRoot(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToAncestorStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.B", "A.C.D", "A.C.D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.nMoveUps = 2u;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "B = {\n\r"
            "}\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToAncestor(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToAncestorJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.B", "A.C.D", "A.C.D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.nMoveUps = 2u;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"B\": {\n\r"
            "},\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToAncestor(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToAncestorXML) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.B", "A.C.D", "A.C.D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.nMoveUps = 2u;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<B>\n\r"
            "</B>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToAncestor(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveAbsoluteStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveAbsolute(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveAbsoluteJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveAbsolute(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveAbsoluteXML) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>";

    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveAbsolute(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveRelativeStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C.D", "D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {";
    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveRelative(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveRelativeJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C.D", "D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {";
    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveRelative(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveRelativeXML) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C.D", "D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>";
    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveRelative(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToChildStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {";

    uint32 index[] = { 0, 1, 0 };
    arg.index = index;
    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToChild(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToChildJson) {
    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {";

    uint32 index[] = { 0, 1, 0 };
    arg.index = index;
    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToChild(&arg));
}

TEST(StreamStructuredDataGTest, TestMoveToChild) {
    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>";

    uint32 index[] = { 0, 1, 0 };
    arg.index = index;
    ASSERT_TRUE(myStreamStructuredDataTest.TestMoveToChild(&arg));
}

TEST(StreamStructuredDataGTest, TestCreateAbsoluteStandard) {

    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;


    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestCreateAbsolute(&arg));
}

TEST(StreamStructuredDataGTest, TestCreateAbsoluteJson) {

    StreamStructuredDataTest<JsonPrinter> myStreamStructuredDataTest;


    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestCreateAbsolute(&arg));
}


TEST(StreamStructuredDataGTest, TestCreateAbsoluteXML) {

    StreamStructuredDataTest<XMLPrinter> myStreamStructuredDataTest;

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>";

    ASSERT_TRUE(myStreamStructuredDataTest.TestCreateAbsolute(&arg));
}


TEST(StreamStructuredDataGTest, TestCreateRelativeStandard) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    StreamStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A", "C.D", "E", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "E = {";

    ASSERT_TRUE(myStreamStructuredDataTest.TestCreateRelative(&arg));
}

TEST(StreamStructuredDataGTest, TestDelete) {
    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;
    ASSERT_TRUE(myStreamStructuredDataTest.TestDelete());
}

TEST(StreamStructuredDataGTest, TestGetNameStandard) {

    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    StreamStructuredDataTestAddToCurrentNodeStruct arg;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    arg.toAdd = a;

    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    uint32 index[]={0, 1, 0};
    arg.index = index;

    ASSERT_TRUE(myStreamStructuredDataTest.TestGetName(&arg));
}

TEST(StreamStructuredDataGTest, TestGetChildName) {

    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    StreamStructuredDataTestAddToCurrentNodeStruct arg;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    arg.toAdd = a;

    const char8 *movements[] = { "B", "C", NULL };
    bool expected[] = { true, true };
    arg.expected = expected;
    arg.movements = movements;
    uint32 index[]={0, 1};
    arg.index = index;

    ASSERT_TRUE(myStreamStructuredDataTest.TestGetChildName(&arg));

}

TEST(StreamStructuredDataGTest, TestGetNumberOfChildren) {

    StreamStructuredDataTest<StandardPrinter> myStreamStructuredDataTest;

    StreamStructuredDataTestAddToCurrentNodeStruct arg;

    ReferenceT<StreamStructuredDataNodeDes> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<StreamStructuredDataNodeDes> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<StreamStructuredDataNodeDes> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<StreamStructuredDataNodeDes> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    arg.toAdd = a;
    arg.nMoveUps = 2u;

    ASSERT_TRUE(myStreamStructuredDataTest.TestGetNumberOfChildren(&arg));
}
