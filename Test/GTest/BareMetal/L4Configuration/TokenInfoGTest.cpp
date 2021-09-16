/**
 * @file TokenInfoGTest.cpp
 * @brief Source file for class TokenInfoGTest
 * @date 02/12/2015
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
 * the class TokenInfoGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "TokenInfoTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestDefaultConstructor) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestSet) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestSet("Hello",1));
}

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestSet_0) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestSet("Hello",0));
}

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestSet_NULL) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestSet(NULL,1));
}



TEST(BareMetal_L4Configuration_TokenInfoGTest,TestSet_0_NULL) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestSet(NULL,0));
}

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestGetTokenId) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestGetTokenId(1));
}

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestGetTokenId_0) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestGetTokenId(0));
}


TEST(BareMetal_L4Configuration_TokenInfoGTest,TestGetDescription) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestGetDescription("Hello"));
}

TEST(BareMetal_L4Configuration_TokenInfoGTest,TestGetDescription_NULL) {
    TokenInfoTest tokenInfo;
    ASSERT_TRUE(tokenInfo.TestGetDescription(NULL));
}
