/**
 * @file FastMathGTest.cpp
 * @brief Source file for class FastMathGTest
 * @date 26/06/2015
 * @author Giuseppe Ferr�
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
 * the class FastMathGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "FastMathTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FastMathGTest,TestFloatToInt32) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.TestFloatToInt32(2.0, 2));
}

TEST(FastMathGTest,TestSin) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.TestSin(0, 0));
}

TEST(FastMathGTest,TestCos) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.TestCos(0, 1));
}

TEST(FastMathGTest,All) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.All());
}

TEST(FastMathGTest,CircleTest) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.CircleTest());
}

