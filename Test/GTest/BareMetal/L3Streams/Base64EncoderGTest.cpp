/**
 * @file Base64EncoderGTest.cpp
 * @brief Source file for class Base64EncoderGTest
 * @date 26/09/2018
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
 * the class Base64EncoderGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "Base64EncoderTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L3Streams_Base64EncoderGTest,TestEncode) {
    Base64EncoderTest test;
    ASSERT_TRUE(test.TestEncode("abc123!?$*&()'-=@~", "YWJjMTIzIT8kKiYoKSctPUB+"));
}

TEST(BareMetal_L3Streams_Base64EncoderGTest,TestEncode_Size1) {
    Base64EncoderTest test;
    ASSERT_TRUE(test.TestEncode("a", "YQ=="));
}

TEST(BareMetal_L3Streams_Base64EncoderGTest,TestDecode) {
    Base64EncoderTest test;
    ASSERT_TRUE(test.TestDecode("Y2lhbw==", "ciao"));
}

TEST(BareMetal_L3Streams_Base64EncoderGTest,TestDecode_Size1) {
    Base64EncoderTest test;
    ASSERT_TRUE(test.TestDecode("bWVyY===", "mer`"));
}

