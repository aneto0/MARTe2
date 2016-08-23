/**
 * @file StringAliasAllGTest.cpp
 * @brief Source file for class StringAliasAllGTest
 * @date 12/ago/2016
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
 * the class StringAliasAllGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StringAliasAllTest.h"
#include "gtest/gtest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(StringAliasAllGTest,TestSourceToDestination_int32) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<int32> table[]={
            {"A=1 B=2 C=3","A", 1, true},
            {"A=1 B=-2 C=3","B", -2, true},
            {"A=1 B=2 C=3","C", 3, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestination(table));
}


TEST(StringAliasAllGTest,TestSourceToDestination_uint32) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<uint32> table[]={
            {"A=1 B=2 C=3","A", 1, true},
            {"A=1 B=2 C=3","B", 2, true},
            {"A=1 B=2 C=3","C", 3, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestination(table));
}


TEST(StringAliasAllGTest,TestSourceToDestination_float32) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<float32> table[]={
            {"A=1.5 B=2.5 C=-3.5","A", 1.5, true},
            {"A=1.5 B=2.5 C=-3.5","B", 2.5, true},
            {"A=1.5 B=2.5 C=-3.5","C", -3.5, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestination(table));
}


TEST(StringAliasAllGTest,TestSourceToDestination_string) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<StreamString> table[]={
            {"A=aaa B=bbb C=ccc","A", "aaa", true},
            {"A=aaa B=bbb C=ccc","B", "bbb", true},
            {"A=aaa B=bbb C=ccc","C", "ccc", true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestination(table));
}


TEST(StringAliasAllGTest,TestSourceToDestination_int32_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<int32, 3> table[]={
            {"A={-1 2 3} B={4 -5 6} C={7 8 -9}","A", {-1, 2,3}, true},
            {"A={-1 2 3} B={4 -5 6} C={7 8 -9}","B", {4, -5 ,6}, true},
            {"A={-1 2 3} B={4 -5 6} C={7 8 -9}","C", {7, 8 ,-9}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationVector(table));
}

TEST(StringAliasAllGTest,TestSourceToDestination_uint32_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<uint32, 3> table[]={
            {"A={1 2 3} B={4 5 6} C={7 8 9}","A", {1, 2,3}, true},
            {"A={1 2 3} B={4 5 6} C={7 8 9}","B", {4, 5 ,6}, true},
            {"A={1 2 3} B={4 5 6} C={7 8 9}","C", {7, 8 ,9}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationVector(table));
}

	

TEST(StringAliasAllGTest,TestSourceToDestination_float32_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<float32, 3> table[]={
            {"A={1.5 2 -3.5} B={4.25 5 6} C={7 8 9.125}","A", {1.5, 2.,-3.5}, true},
            {"A={1.5 2 -3.5} B={4.25 5 6} C={7 8 9.125}","B", {4.25, 5. ,6.}, true},
            {"A={1.5 2 -3.5} B={4.25 5 6} C={7 8 9.125}","C", {7., 8. ,9.125}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationVector(table));
}


TEST(StringAliasAllGTest,TestSourceToDestination_string_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<StreamString, 3> table[]={
            {"A={aaa bbb ccc} B={ddd eee fff} C={ggg hhh iii}","A", {"aaa", "bbb","ccc"}, true},
            {"A={aaa bbb ccc} B={ddd eee fff} C={ggg hhh iii}","B", {"ddd","eee" ,"fff"}, true},
            {"A={aaa bbb ccc} B={ddd eee fff} C={ggg hhh iii}","C", {"ggg", "hhh" ,"iii"}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationVector(table));
}




TEST(StringAliasAllGTest,TestSourceToDestination_int32_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<int32, 3, 2> table[]={
            {"A={{-1 2} {3 -4} {5 -6}} B={{7 -8} {-9 10} {11 -12}} C={{13 -14} {-15 16} {17 -18}}","A", {{-1, 2}, {3, -4}, {5, -6}}, true},
            {"A={{-1 2} {3 -4} {5 -6}} B={{7 -8} {-9 10} {11 -12}} C={{13 -14} {-15 16} {17 -18}}","B", {{7, -8}, {-9, 10}, {11 ,-12}}, true},
            {"A={{-1 2} {3 -4} {5 -6}} B={{7 -8} {-9 10} {11 -12}} C={{13 -14} {-15 16} {17 -18}}","C", {{13, -14} ,{-15, 16}, {17 ,-18}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationMatrix(table));
}

TEST(StringAliasAllGTest,TestSourceToDestination_uint32_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<uint32, 3, 2> table[]={
            {"A={{1 2} {3 4} {5 6}} B={{7 8} {9 10} {11 12}} C={{13 14} {15 16} {17 18}}","A", {{1, 2}, {3, 4}, {5, 6}}, true},
            {"A={{1 2} {3 4} {5 6}} B={{7 8} {9 10} {11 12}} C={{13 14} {15 16} {17 18}}","B", {{7, 8}, {9, 10}, {11 ,12}}, true},
            {"A={{1 2} {3 4} {5 6}} B={{7 8} {9 10} {11 12}} C={{13 14} {15 16} {17 18}}","C", {{13, 14} ,{15, 16}, {17 ,18}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationMatrix(table));
}



TEST(StringAliasAllGTest,TestSourceToDestination_float32_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<float32, 3, 2> table[]={
            {"A={{1.5 2} {-3.5 4.5} {5.125 -1.5}} B={{4.25 7.5} {5 6.25} {24.125 8.75}} C={{7 8.625} {9.125 -3.25} {-4.625 -12.75}}","A", {{1.5, 2.}, {-3.5, 4.5}, {5.125, -1.5}}, true},
            {"A={{1.5 2} {-3.5 4.5} {5.125 -1.5}} B={{4.25 7.5} {5 6.25} {24.125 8.75}} C={{7 8.625} {9.125 -3.25} {-4.625 -12.75}}","B", {{4.25, 7.5}, {5., 6.25}, {24.125, 8.75}}, true},
            {"A={{1.5 2} {-3.5 4.5} {5.125 -1.5}} B={{4.25 7.5} {5 6.25} {24.125 8.75}} C={{7 8.625} {9.125 -3.25} {-4.625 -12.75}}","C", {{7., 8.625}, {9.125, -3.25}, {-4.625, -12.75}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationMatrix(table));
}


TEST(StringAliasAllGTest,TestSourceToDestination_string_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<StreamString, 3, 2> table[]={
            {"A={{aaa bbb} {ccc ddd} {eee fff}} B={{ddd eee} {fff ggg} {hhh iii}} C={{ggg hhh} {iii lll} {mmm nnn}}","A", {{"aaa", "bbb"}, {"ccc", "ddd"}, {"eee", "fff"}}, true},
            {"A={{aaa bbb} {ccc ddd} {eee fff}} B={{ddd eee} {fff ggg} {hhh iii}} C={{ggg hhh} {iii lll} {mmm nnn}}","B", {{"ddd", "eee"}, {"fff", "ggg"}, {"hhh", "iii"}}, true},
            {"A={{aaa bbb} {ccc ddd} {eee fff}} B={{ddd eee} {fff ggg} {hhh iii}} C={{ggg hhh} {iii lll} {mmm nnn}}","C", {{"ggg", "hhh"}, {"iii", "lll"}, {"mmm", "nnn"}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestSourceToDestinationMatrix(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_int32) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<int32> table[]={
            {"A=1 B=2 C=3","A", 1, true},
            {"A=1 B=-2 C=3","B", -2, true},
            {"A=1 B=2 C=3","C", 3, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSource(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_uint32) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<uint32> table[]={
            {"A=1 B=2 C=3","A", 1, true},
            {"A=1 B=2 C=3","B", 2, true},
            {"A=1 B=2 C=3","C", 3, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSource(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_float32) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<float32> table[]={
            {"A=1.5 B=2.5 C=-3.5","A", 1.5, true},
            {"A=1.5 B=2.5 C=-3.5","B", 2.5, true},
            {"A=1.5 B=2.5 C=-3.5","C", -3.5, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSource(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_string) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTable<StreamString> table[]={
            {"A=aaa B=bbb C=ccc","A", "aaa", true},
            {"A=aaa B=bbb C=ccc","B", "bbb", true},
            {"A=aaa B=bbb C=ccc","C", "ccc", true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSource(table));
}





TEST(StringAliasAllGTest,TestDestinationToSource_int32_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<int32, 3> table[]={
            {"A={-1 2 3} B={4 -5 6} C={7 8 -9}","A", {-1, 2,3}, true},
            {"A={-1 2 3} B={4 -5 6} C={7 8 -9}","B", {4, -5 ,6}, true},
            {"A={-1 2 3} B={4 -5 6} C={7 8 -9}","C", {7, 8 ,-9}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceVector(table));
}

TEST(StringAliasAllGTest,TestDestinationToSource_uint32_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<uint32, 3> table[]={
            {"A={1 2 3} B={4 5 6} C={7 8 9}","A", {1, 2,3}, true},
            {"A={1 2 3} B={4 5 6} C={7 8 9}","B", {4, 5 ,6}, true},
            {"A={1 2 3} B={4 5 6} C={7 8 9}","C", {7, 8 ,9}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceVector(table));
}



TEST(StringAliasAllGTest,TestDestinationToSource_float32_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<float32, 3> table[]={
            {"A={1.5 2 -3.5} B={4.25 5 6} C={7 8 9.125}","A", {1.5, 2.,-3.5}, true},
            {"A={1.5 2 -3.5} B={4.25 5 6} C={7 8 9.125}","B", {4.25, 5. ,6.}, true},
            {"A={1.5 2 -3.5} B={4.25 5 6} C={7 8 9.125}","C", {7., 8. ,9.125}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceVector(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_string_Vector) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableVector<StreamString, 3> table[]={
            {"A={aaa bbb ccc} B={ddd eee fff} C={ggg hhh iii}","A", {"aaa", "bbb","ccc"}, true},
            {"A={aaa bbb ccc} B={ddd eee fff} C={ggg hhh iii}","B", {"ddd","eee" ,"fff"}, true},
            {"A={aaa bbb ccc} B={ddd eee fff} C={ggg hhh iii}","C", {"ggg", "hhh" ,"iii"}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceVector(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_int32_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<int32, 3, 2> table[]={
            {"A={{-1 2} {3 -4} {5 -6}} B={{7 -8} {-9 10} {11 -12}} C={{13 -14} {-15 16} {17 -18}}","A", {{-1, 2}, {3, -4}, {5, -6}}, true},
            {"A={{-1 2} {3 -4} {5 -6}} B={{7 -8} {-9 10} {11 -12}} C={{13 -14} {-15 16} {17 -18}}","B", {{7, -8}, {-9, 10}, {11 ,-12}}, true},
            {"A={{-1 2} {3 -4} {5 -6}} B={{7 -8} {-9 10} {11 -12}} C={{13 -14} {-15 16} {17 -18}}","C", {{13, -14} ,{-15, 16}, {17 ,-18}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceMatrix(table));
}

TEST(StringAliasAllGTest,TestDestinationToSource_uint32_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<uint32, 3, 2> table[]={
            {"A={{1 2} {3 4} {5 6}} B={{7 8} {9 10} {11 12}} C={{13 14} {15 16} {17 18}}","A", {{1, 2}, {3, 4}, {5, 6}}, true},
            {"A={{1 2} {3 4} {5 6}} B={{7 8} {9 10} {11 12}} C={{13 14} {15 16} {17 18}}","B", {{7, 8}, {9, 10}, {11 ,12}}, true},
            {"A={{1 2} {3 4} {5 6}} B={{7 8} {9 10} {11 12}} C={{13 14} {15 16} {17 18}}","C", {{13, 14} ,{15, 16}, {17 ,18}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceMatrix(table));
}



TEST(StringAliasAllGTest,TestDestinationToSource_float32_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<float32, 3, 2> table[]={
            {"A={{1.5 2} {-3.5 4.5} {5.125 -1.5}} B={{4.25 7.5} {5 6.25} {24.125 8.75}} C={{7 8.625} {9.125 -3.25} {-4.625 -12.75}}","A", {{1.5, 2.}, {-3.5, 4.5}, {5.125, -1.5}}, true},
            {"A={{1.5 2} {-3.5 4.5} {5.125 -1.5}} B={{4.25 7.5} {5 6.25} {24.125 8.75}} C={{7 8.625} {9.125 -3.25} {-4.625 -12.75}}","B", {{4.25, 7.5}, {5., 6.25}, {24.125, 8.75}}, true},
            {"A={{1.5 2} {-3.5 4.5} {5.125 -1.5}} B={{4.25 7.5} {5 6.25} {24.125 8.75}} C={{7 8.625} {9.125 -3.25} {-4.625 -12.75}}","C", {{7., 8.625}, {9.125, -3.25}, {-4.625, -12.75}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceMatrix(table));
}


TEST(StringAliasAllGTest,TestDestinationToSource_string_Matrix) {
    StringAliasAllTest validateTest;

    const StringAliasAllTestTableMatrix<StreamString, 3, 2> table[]={
            {"A={{aaa bbb} {ccc ddd} {eee fff}} B={{ddd eee} {fff ggg} {hhh iii}} C={{ggg hhh} {iii lll} {mmm nnn}}","A", {{"aaa", "bbb"}, {"ccc", "ddd"}, {"eee", "fff"}}, true},
            {"A={{aaa bbb} {ccc ddd} {eee fff}} B={{ddd eee} {fff ggg} {hhh iii}} C={{ggg hhh} {iii lll} {mmm nnn}}","B", {{"ddd", "eee"}, {"fff", "ggg"}, {"hhh", "iii"}}, true},
            {"A={{aaa bbb} {ccc ddd} {eee fff}} B={{ddd eee} {fff ggg} {hhh iii}} C={{ggg hhh} {iii lll} {mmm nnn}}","C", {{"ggg", "hhh"}, {"iii", "lll"}, {"mmm", "nnn"}}, true},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestDestinationToSourceMatrix(table));
}









