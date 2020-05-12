/**
 * @file PseudoCodeGTest.cpp
 * @brief Source file for class PseudoCodeGTest
 * @date 04/05/2020
 * @author Didac Magrinya
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
 * the class PseudoCodeGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "PseudoCodeTest.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//TEST(BareMetal_L4Configuration_PseudoCodeGTest,TestContextConstructor) {
    //PseudoCodeTest pseudoCodeTest;
    //ASSERT_TRUE(pseudoCodeTest.TestContextConstructor());
//}

TEST(BareMetal_L4Configuration_PseudoCodeGTest,TestIntegration) {
    PseudoCodeTest pseudoCodeTest;
    ASSERT_TRUE(pseudoCodeTest.TestIntegration());
}

TEST(BareMetal_L4Configuration_PseudoCodeGTest,TestError) {
    PseudoCodeTest pseudoCodeTest;
    ASSERT_TRUE(pseudoCodeTest.TestError());
}

TEST(BareMetal_L4Configuration_PseudoCodeGTest,TestExpression_1) {
    
    PseudoCodeTest pseudoCodeTest;
    
    float values[2] = { 10.0, 4.2 };
    
    CCString rpnCode=
            "READ A\n"
            "READ B\n"
            "SUB\n"
            "WRITE F\n"
    ;
    
    ASSERT_TRUE(pseudoCodeTest.TestExpression(rpnCode, values));
}

TEST(BareMetal_L4Configuration_PseudoCodeGTest,TestExpression_2) {
    
    PseudoCodeTest pseudoCodeTest;
    
    float values[2] = { 10.0, 7.2 };
    
    CCString rpnCode=
            "READ A\n"
            "READ B\n"
            "ADD\n"
            "WRITE F\n"
    ;
    
    ASSERT_TRUE(pseudoCodeTest.TestExpression(rpnCode, values));
}
