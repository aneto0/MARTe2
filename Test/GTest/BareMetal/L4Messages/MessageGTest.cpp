/**
 * @file MessageGTest.cpp
 * @brief Source file for class MessageGTest
 * @date 14/06/2016
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
#include "MessageTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Messages_MessageGTest,TestConstructor) {
    MessageTest target;
    ASSERT_TRUE(target.TestConstructor());
}


TEST(BareMetal_L4Messages_MessageGTest,TestInitialise) {
    MessageTest target;
    ASSERT_TRUE(target.TestInitialise());
}


TEST(BareMetal_L4Messages_MessageGTest,TestSetAsReply) {
    MessageTest target;
    ASSERT_TRUE(target.TestSetAsReply());
}

TEST(BareMetal_L4Messages_MessageGTest,TestIsReply) {
    MessageTest target;
    ASSERT_TRUE(target.TestIsReply());
}

TEST(BareMetal_L4Messages_MessageGTest,TestSetExpectsReply) {
    MessageTest target;
    ASSERT_TRUE(target.TestSetExpectsReply());
}

TEST(BareMetal_L4Messages_MessageGTest,TestSetExpectsIndirectReply) {
    MessageTest target;
    ASSERT_TRUE(target.TestSetExpectsIndirectReply());
}

TEST(BareMetal_L4Messages_MessageGTest,TestExpectsReply) {
    MessageTest target;
    ASSERT_TRUE(target.TestExpectsReply());
}

TEST(BareMetal_L4Messages_MessageGTest,TestExpectsIndirectReply) {
    MessageTest target;
    ASSERT_TRUE(target.TestExpectsIndirectReply());
}

TEST(BareMetal_L4Messages_MessageGTest,TestGetDestination) {
    MessageTest target;
    ASSERT_TRUE(target.TestGetDestination());
}

TEST(BareMetal_L4Messages_MessageGTest,TestGetSender) {
    MessageTest target;
    ASSERT_TRUE(target.TestGetSender());
}

TEST(BareMetal_L4Messages_MessageGTest,TestSetSender) {
    MessageTest target;
    ASSERT_TRUE(target.TestSetSender());
}

TEST(BareMetal_L4Messages_MessageGTest,TestGetFunction) {
    MessageTest target;
    ASSERT_TRUE(target.TestGetFunction());
}

TEST(BareMetal_L4Messages_MessageGTest,TestSetReplyTimeout) {
    MessageTest target;
    ASSERT_TRUE(target.TestSetReplyTimeout());
}
