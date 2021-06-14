/**
 * @file BinaryTreeGTest.cpp
 * @brief Source file for class BinaryTreeGTest
 * @date 19/08/2019
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
 * the class BinaryTreeGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BinaryTreeTest.h"
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

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    BinaryTree<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    BinaryTree<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    BinaryTree<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    BinaryTree<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    BinaryTree<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_uint8) {
    BinaryTreeTest<uint8, Fnv1aHashFunction> test;
    BinaryTree<uint8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_uint16) {
    BinaryTreeTest<uint16, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_uint16) {
    BinaryTreeTest<uint16, DjbHashFunction> test;
    BinaryTree<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_uint16) {
    BinaryTreeTest<uint16, DjbHashFunction> test;
    BinaryTree<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_uint16) {
    BinaryTreeTest<uint16, DjbHashFunction> test;
    BinaryTree<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_uint16) {
    BinaryTreeTest<uint16, DjbHashFunction> test;
    BinaryTree<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_uint16) {
    BinaryTreeTest<uint16, DjbHashFunction> test;
    BinaryTree<uint16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_uint16) {
    BinaryTreeTest<uint16, Fnv1aHashFunction> test;
    BinaryTree<uint16, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_uint32) {
    BinaryTreeTest<uint32, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_uint32) {
    BinaryTreeTest<uint32, DjbHashFunction> test;
    BinaryTree<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_uint32) {
    BinaryTreeTest<uint32, DjbHashFunction> test;
    BinaryTree<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_uint32) {
    BinaryTreeTest<uint32, DjbHashFunction> test;
    BinaryTree<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_uint32) {
    BinaryTreeTest<uint32, DjbHashFunction> test;
    BinaryTree<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_uint32) {
    BinaryTreeTest<uint32, DjbHashFunction> test;
    BinaryTree<uint32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_uint32) {
    BinaryTreeTest<uint32, Fnv1aHashFunction> test;
    BinaryTree<uint32, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    BinaryTree<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    BinaryTree<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    BinaryTree<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    BinaryTree<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestGetValue(t, "elem1", 1);
    x1 &= test.TestGetValue(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    BinaryTree<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSquareOperator(t, "elem1", 1);
    x1 &= test.TestSquareOperator(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_uint64) {
    BinaryTreeTest<uint64, Fnv1aHashFunction> test;
    BinaryTree<uint64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    BinaryTree<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    BinaryTree<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    BinaryTree<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", 1);
    x1 &= test.TestInsert(t, "elem2", 2);
    x1 &= test.TestInsert(t, "elem3", 3);
    x1 &= test.TestInsert(t, "elem4", 4);
    x1 &= test.TestSearch(t, "elem1", 1);
    x1 &= test.TestSearch(t, "elem4", 4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    BinaryTree<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    BinaryTree<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_int8) {
    BinaryTreeTest<int8, Fnv1aHashFunction> test;
    BinaryTree<int8, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_int16) {
    BinaryTreeTest<int16, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_int16) {
    BinaryTreeTest<int16, DjbHashFunction> test;
    BinaryTree<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_int16) {
    BinaryTreeTest<int16, DjbHashFunction> test;
    BinaryTree<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_int16) {
    BinaryTreeTest<int16, DjbHashFunction> test;
    BinaryTree<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSearch(t, "elem1", -1);
    x1 &= test.TestSearch(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_int16) {
    BinaryTreeTest<int16, DjbHashFunction> test;
    BinaryTree<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_int16) {
    BinaryTreeTest<int16, DjbHashFunction> test;
    BinaryTree<int16, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_int16) {
    BinaryTreeTest<int16, Fnv1aHashFunction> test;
    BinaryTree<int16, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_int32) {
    BinaryTreeTest<int32, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_int32) {
    BinaryTreeTest<int32, DjbHashFunction> test;
    BinaryTree<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_int32) {
    BinaryTreeTest<int32, DjbHashFunction> test;
    BinaryTree<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_int32) {
    BinaryTreeTest<int32, DjbHashFunction> test;
    BinaryTree<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSearch(t, "elem1", -1);
    x1 &= test.TestSearch(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_int32) {
    BinaryTreeTest<int32, DjbHashFunction> test;
    BinaryTree<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_int32) {
    BinaryTreeTest<int32, DjbHashFunction> test;
    BinaryTree<int32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_int32) {
    BinaryTreeTest<int32, Fnv1aHashFunction> test;
    BinaryTree<int32, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    BinaryTree<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    BinaryTree<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    BinaryTree<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSearch(t, "elem1", -1);
    x1 &= test.TestSearch(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    BinaryTree<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestGetValue(t, "elem1", -1);
    x1 &= test.TestGetValue(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    BinaryTree<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    x1 &= test.TestSquareOperator(t, "elem1", -1);
    x1 &= test.TestSquareOperator(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_int64) {
    BinaryTreeTest<int64, Fnv1aHashFunction> test;
    BinaryTree<int64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1);
    x1 &= test.TestInsert(t, "elem2", -2);
    x1 &= test.TestInsert(t, "elem3", -3);
    x1 &= test.TestInsert(t, "elem4", -4);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_float32) {
    BinaryTreeTest<float32, DjbHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_float32) {
    BinaryTreeTest<float32, DjbHashFunction> test;
    BinaryTree<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_float32) {
    BinaryTreeTest<float32, DjbHashFunction> test;
    BinaryTree<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_float32) {
    BinaryTreeTest<float32, DjbHashFunction> test;
    BinaryTree<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSearch(t, "elem1", -1.23);
    x1 &= test.TestSearch(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_float32) {
    BinaryTreeTest<float32, DjbHashFunction> test;
    BinaryTree<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestGetValue(t, "elem1", -1.23);
    x1 &= test.TestGetValue(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_float32) {
    BinaryTreeTest<float32, DjbHashFunction> test;
    BinaryTree<float32, DjbHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSquareOperator(t, "elem1", -1.23);
    x1 &= test.TestSquareOperator(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_float32) {
    BinaryTreeTest<float32, Fnv1aHashFunction> test;
    BinaryTree<float32, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestConstructor_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestInsert_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestRemove_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestRemove(t, "elem1");
    x1 &= test.TestRemove(t, "elem4");
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSearch_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSearch(t, "elem1", -1.23);
    x1 &= test.TestSearch(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestGetValue_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestGetValue(t, "elem1", -1.23);
    x1 &= test.TestGetValue(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperator_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    x1 &= test.TestSquareOperator(t, "elem1", -1.23);
    x1 &= test.TestSquareOperator(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSet_float64) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    bool x1 = test.TestInsert(t, "elem1", -1.23);
    x1 &= test.TestInsert(t, "elem2", -2.23);
    x1 &= test.TestInsert(t, "elem3", -3.23);
    x1 &= test.TestInsert(t, "elem4", -4.23);
    ASSERT_TRUE(x1);
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestReset) {
    BinaryTreeTest<float64, Fnv1aHashFunction> test;
    BinaryTree<float64, Fnv1aHashFunction> t;
    ASSERT_TRUE(test.TestReset(t));
}

TEST(BareMetal_L1Portability_BinaryTreeGTest,TestSquareOperatorNonExistendId) {
    BinaryTreeTest<Reference, Fnv1aHashFunction> test;
    BinaryTree<Reference, Fnv1aHashFunction> t;
    ASSERT_TRUE(test.TestSquareOperatorNonExistendId(t));
}
