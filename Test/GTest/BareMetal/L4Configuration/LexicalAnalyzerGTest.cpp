/**
 * @file LexicalAnalyzerGTest.cpp
 * @brief Source file for class LexicalAnalyzerGTest
 * @date 26/11/2015
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
 * the class LexicalAnalyzerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "LexicalAnalyzerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_LexicalAnalyzerGTest, TestDefaultConstructor) {
    LexicalAnalyzerTest laTest;
    ASSERT_TRUE(laTest.TestConstructor());
}

TEST(BareMetal_L4Configuration_LexicalAnalyzerGTest, TestGetToken) {
    LexicalAnalyzerTest laTest;
    ASSERT_TRUE(laTest.TestGetToken());
}

TEST(BareMetal_L4Configuration_LexicalAnalyzerGTest, TestPeekToken) {
    LexicalAnalyzerTest laTest;
    ASSERT_TRUE(laTest.TestPeekToken());
}

TEST(BareMetal_L4Configuration_LexicalAnalyzerGTest, TestEscape) {
    LexicalAnalyzerTest laTest;
    ASSERT_TRUE(laTest.TestEscape());
}

TEST(BareMetal_L4Configuration_LexicalAnalyzerGTest, TestComments) {
    LexicalAnalyzerTest laTest;
    ASSERT_TRUE(laTest.TestComments());
}
