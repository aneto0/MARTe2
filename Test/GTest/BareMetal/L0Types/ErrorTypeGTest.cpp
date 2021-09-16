/**
 * @file BasicTypeGTest.cpp
 * @brief Source file for class BasicTypeGTest
 * @date 08/09/2015
 * @author Andre' Neto
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
 * the class BasicTypeGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "ErrorTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_ErrorTypeGTest,TestConstructor_Default) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestConstructor_Default());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestConstructor_False) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestConstructor_False());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestConstructor_BitSet) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestConstructor_BitSet());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestErrorsCleared) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestErrorsCleared());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestOperatorBool) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestOperatorBool());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestOperatorErrorIntegerFormat) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestOperatorErrorIntegerFormat());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestOperatorEquals) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestOperatorEquals());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestOperatorNotEqual) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestOperatorNotEqual());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestOperatorAssignment) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestOperatorAssignment());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestSetError) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestSetError());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestClearError) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestClearError());
}

TEST(BareMetal_L0Types_ErrorTypeGTest,TestContains) {
    ErrorTypeTest target;
    ASSERT_TRUE(target.TestContains());
}
