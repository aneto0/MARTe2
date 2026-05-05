/**
 * @file UnorderedMapGTest.cpp
 * @brief Source file for class UnorderedMapGTest
 * @date 10/09/2025
 * @author Andre Neto
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
 * the class UnorderedMapGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "UnorderedMapTest.h"
#include "DjbHashFunction.h"
#include "Fnv1aHashFunction.h"
#include "Reference.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    UnorderedMap<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    UnorderedMap<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    UnorderedMap<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    UnorderedMap<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    UnorderedMap<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_uint8) {
    UnorderedMapTest<uint8, Fnv1aHashFunction> test;
    UnorderedMap<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_uint16) {
    UnorderedMapTest<uint16, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_uint16) {
    UnorderedMapTest<uint16, DjbHashFunction> test;
    UnorderedMap<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_uint16) {
    UnorderedMapTest<uint16, DjbHashFunction> test;
    UnorderedMap<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_uint16) {
    UnorderedMapTest<uint16, DjbHashFunction> test;
    UnorderedMap<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_uint16) {
    UnorderedMapTest<uint16, DjbHashFunction> test;
    UnorderedMap<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_uint16) {
    UnorderedMapTest<uint16, DjbHashFunction> test;
    UnorderedMap<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_uint16) {
    UnorderedMapTest<uint16, Fnv1aHashFunction> test;
    UnorderedMap<uint16, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_uint32) {
    UnorderedMapTest<uint32, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_uint32) {
    UnorderedMapTest<uint32, DjbHashFunction> test;
    UnorderedMap<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_uint32) {
    UnorderedMapTest<uint32, DjbHashFunction> test;
    UnorderedMap<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_uint32) {
    UnorderedMapTest<uint32, DjbHashFunction> test;
    UnorderedMap<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestInsert(t, "elem5", 5);
    x1 &= test.TestInsert(t, "elem6", 6);
    x1 &= test.TestInsert(t, "elem7", 7);
    x1 &= test.TestInsert(t, "elem8", 8);
    x1 &= test.TestInsert(t, "elem9", 9);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem9", 9);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_uint32) {
    UnorderedMapTest<uint32, DjbHashFunction> test;
    UnorderedMap<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_uint32) {
    UnorderedMapTest<uint32, DjbHashFunction> test;
    UnorderedMap<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_uint32) {
    UnorderedMapTest<uint32, Fnv1aHashFunction> test;
    UnorderedMap<uint32, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    UnorderedMap<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    UnorderedMap<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    UnorderedMap<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    UnorderedMap<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    UnorderedMap<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_uint64) {
    UnorderedMapTest<uint64, Fnv1aHashFunction> test;
    UnorderedMap<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    UnorderedMap<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    UnorderedMap<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    UnorderedMap<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    UnorderedMap<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    UnorderedMap<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_int8) {
    UnorderedMapTest<int8, Fnv1aHashFunction> test;
    UnorderedMap<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_int16) {
    UnorderedMapTest<int16, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_int16) {
    UnorderedMapTest<int16, DjbHashFunction> test;
    UnorderedMap<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_int16) {
    UnorderedMapTest<int16, DjbHashFunction> test;
    UnorderedMap<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_int16) {
    UnorderedMapTest<int16, DjbHashFunction> test;
    UnorderedMap<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSearch(t, "elem1", -1);
    x1 &= test.TestSearch(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_int16) {
    UnorderedMapTest<int16, DjbHashFunction> test;
    UnorderedMap<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_int16) {
    UnorderedMapTest<int16, DjbHashFunction> test;
    UnorderedMap<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_int16) {
    UnorderedMapTest<int16, Fnv1aHashFunction> test;
    UnorderedMap<int16, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_int32) {
    UnorderedMapTest<int32, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_int32) {
    UnorderedMapTest<int32, DjbHashFunction> test;
    UnorderedMap<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_int32) {
    UnorderedMapTest<int32, DjbHashFunction> test;
    UnorderedMap<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_int32) {
    UnorderedMapTest<int32, DjbHashFunction> test;
    UnorderedMap<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSearch(t, "elem1", -1);
    x1 &= test.TestSearch(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_int32) {
    UnorderedMapTest<int32, DjbHashFunction> test;
    UnorderedMap<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_int32) {
    UnorderedMapTest<int32, DjbHashFunction> test;
    UnorderedMap<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_int32) {
    UnorderedMapTest<int32, Fnv1aHashFunction> test;
    UnorderedMap<int32, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    UnorderedMap<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    UnorderedMap<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    UnorderedMap<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSearch(t, "elem1", -1);
    x1 &= test.TestSearch(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    UnorderedMap<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    UnorderedMap<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_int64) {
    UnorderedMapTest<int64, Fnv1aHashFunction> test;
    UnorderedMap<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_float32) {
    UnorderedMapTest<float32, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_float32) {
    UnorderedMapTest<float32, DjbHashFunction> test;
    UnorderedMap<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_float32) {
    UnorderedMapTest<float32, DjbHashFunction> test;
    UnorderedMap<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_float32) {
    UnorderedMapTest<float32, DjbHashFunction> test;
    UnorderedMap<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSearch(t, "elem1", -1.23);
    x1 &= test.TestSearch(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_float32) {
    UnorderedMapTest<float32, DjbHashFunction> test;
    UnorderedMap<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestGetValue(t, "elem1", -1.23);
    x1 &= test.TestGetValue(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_float32) {
    UnorderedMapTest<float32, DjbHashFunction> test;
    UnorderedMap<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSquareOperator(t, "elem1", -1.23);
    x1 &= test.TestSquareOperator(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_float32) {
    UnorderedMapTest<float32, Fnv1aHashFunction> test;
    UnorderedMap<float32, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestConstructor_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestInsert_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestRemove_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSearch_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSearch(t, "elem1", -1.23);
    x1 &= test.TestSearch(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestGetValue_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestGetValue(t, "elem1", -1.23);
    x1 &= test.TestGetValue(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperator_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSquareOperator(t, "elem1", -1.23);
    x1 &= test.TestSquareOperator(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSet_float64) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestReset) {
    UnorderedMapTest<float64, Fnv1aHashFunction> test;
    UnorderedMap<float64, Fnv1aHashFunction> t;
    ASSERT_TRUE(test.TestReset(t));
}

TEST(BareMetal_L1Portability_UnorderedMapGTest,TestSquareOperatorNonExistendId) {
    UnorderedMapTest<Reference, Fnv1aHashFunction> test;
    UnorderedMap<Reference, Fnv1aHashFunction> t;
    ASSERT_TRUE(test.TestSquareOperatorNonExistendId(t));
}

