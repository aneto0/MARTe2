/**
 * @file HttpDefinitionGTest.cpp
 * @brief Source file for class HttpDefinitionGTest
 * @date 17/09/2018
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
 * the class HttpDefinitionGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpDefinitionTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4HttpService_HttpDefinitionGTest,TestHttpEncode) {
    HttpDefinitionTest target;
    ASSERT_TRUE(target.TestHttpEncode());
}

TEST(BareMetal_L4HttpService_HttpDefinitionGTest,TestHexDecode) {
    HttpDefinitionTest target;
    ASSERT_TRUE(target.TestHexDecode());
}

TEST(BareMetal_L4HttpService_HttpDefinitionGTest,TestHttpDecode) {
    HttpDefinitionTest target;
    ASSERT_TRUE(target.TestHttpDecode());
}

TEST(BareMetal_L4HttpService_HttpDefinitionGTest,TestIsReplyCode) {
    HttpDefinitionTest target;
    ASSERT_TRUE(target.TestIsReplyCode());
}

TEST(BareMetal_L4HttpService_HttpDefinitionGTest,TestGenerateReplyCode) {
    HttpDefinitionTest target;
    ASSERT_TRUE(target.TestGenerateReplyCode());
}

TEST(BareMetal_L4HttpService_HttpDefinitionGTest,TestGetErrorCodeString) {
    HttpDefinitionTest target;
    ASSERT_TRUE(target.TestGetErrorCodeString());
}
