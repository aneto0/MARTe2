/**
 * @file SlkActionGTest.cpp
 * @brief Source file for class SlkActionGTest
 * @date 07/dic/2015
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
 * the class SlkActionGTest (public, protected, and private). Be aware that some 
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
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "SlkActionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SlkSlkActionGTest,TestConstructor) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestConstructor());
}


TEST(SlkSlkActionGTest,TestParseScalar) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestParseScalarStandardGrammar());
}


TEST(SlkSlkActionGTest,TestParseVector) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestParseVectorStandardGrammar());
}


TEST(SlkSlkActionGTest,TestParseMatrix) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestParseMatrixStandardGrammar());
}

TEST(SlkSlkActionGTest,TestParseScalarXML) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestParseScalarXMLGrammar());
}


TEST(SlkSlkActionGTest,TestParseVectorXML) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestParseVectorXMLGrammar());
}


TEST(SlkSlkActionGTest,TestParseMatrixXML) {
    SlkActionTest slkActionTest;
    ASSERT_TRUE(slkActionTest.TestParseMatrixXMLGrammar());
}
