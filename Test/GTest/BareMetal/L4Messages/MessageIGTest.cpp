/**
 * @file MessageIGTest.cpp
 * @brief Source file for class MessageIGTest
 * @date 12/05/2016
 * @author Ivan Herrero
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
 * the class MessageIGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "MessageITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Messages_MessageIGTest,TestDefaultConstructor) {
    MessageITest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_NULL_Source) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_NULL_Source());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidMessage());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_NotExpectedIndirectReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NotExpectedIndirectReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_NoDestinationForReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NoDestinationForReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_NoDestinationForExpectedReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NoDestinationForExpectedReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_InvalidDestination) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidDestination());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_InvalidDestination_Not_Exist) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidDestination_Not_Exist());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_False_InvalidFunction) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidFunction());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessageAndWaitReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitReply());
}


TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessageAndWaitReply_False_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitReply_False_InvalidMessage());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessageAndWaitReply_False_ReplyOfReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitReply_False_ReplyOfReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessage_ExpectsIndirectReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_ExpectsIndirectReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessageAndWaitIndirectReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitIndirectReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessageAndWaitIndirectReply_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitIndirectReply_InvalidMessage());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestSendMessageAndWaitIndirectReply_InvalidDestination) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitIndirectReply_InvalidDestination());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestWaitForReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestWaitForReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestWaitForReply_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestWaitForReply_InvalidMessage());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestWaitForReply_NotExpectsReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestWaitForReply_NotExpectsReply());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestWaitForReply_Timeout) {
    MessageITest target;
    ASSERT_TRUE(target.TestWaitForReply_Timeout());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestInstallMessageFilter) {
    MessageITest target;
    ASSERT_TRUE(target.TestInstallMessageFilter());
}

TEST(BareMetal_L4Messages_MessageIGTest,TestRemoveMessageFilter) {
    MessageITest target;
    ASSERT_TRUE(target.TestRemoveMessageFilter());
}
