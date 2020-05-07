/**
 * @file PseudoCodeFunctionsGTest.cpp
 * @brief Source file for class PseudoCodeFunctionsGTest
 * @date 06/05/2020
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
 * the class PseudoCodeFunctionsGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PseudoCodeFunctionsTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestDefaultConstructor) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;
    ASSERT_TRUE(pseudoCodeFunctionsTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestSinFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    ASSERT_TRUE((pseudoCodeFunctionsTest.TestFunctionRecordTypes<2, 1, 1>("SIN", expectedInputTypes, expectedOutputTypes)));
}
