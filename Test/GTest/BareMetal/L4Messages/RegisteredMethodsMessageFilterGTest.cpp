/**
 * @file RegisteredMethodsMessageFilterGTest.cpp
 * @brief Source file for class RegisteredMethodsMessageFilterGTest
 * @date 09/09/2016
 * @author Andre Neto
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
 * the class MessageGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "MARTe2UTest.h"
#include "RegisteredMethodsMessageFilterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Messages_RegisteredMethodsMessageFilterGTest,TestConstructor) {
    RegisteredMethodsMessageFilterTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(BareMetal_L4Messages_RegisteredMethodsMessageFilterGTest,TestSetDestinationObject) {
    RegisteredMethodsMessageFilterTest target;
    ASSERT_TRUE(target.TestSetDestinationObject());
}

TEST(BareMetal_L4Messages_RegisteredMethodsMessageFilterGTest,TestConsumeMessage) {
    RegisteredMethodsMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage());
}

TEST(BareMetal_L4Messages_RegisteredMethodsMessageFilterGTest,TestConsumeMessage_Reply) {
    RegisteredMethodsMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_Reply());
}

TEST(BareMetal_L4Messages_RegisteredMethodsMessageFilterGTest,TestConsumeMessage_IndirectReply) {
    RegisteredMethodsMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_IndirectReply());
}

TEST(BareMetal_L4Messages_RegisteredMethodsMessageFilterGTest,TestIsPermanentFilter) {
    RegisteredMethodsMessageFilterTest target;
    ASSERT_TRUE(target.TestIsPermanentFilter());
}
