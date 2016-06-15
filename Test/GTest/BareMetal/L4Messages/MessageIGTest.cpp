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

#include "gtest/gtest.h"
#include "MessageITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MessageIGTest,TestDefaultConstructor) {
    MessageITest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(MessageIGTest,TestSendMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage());
}

TEST(MessageIGTest,TestSendMessage_NULL_Source) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_NULL_Source());
}

TEST(MessageIGTest,TestSendMessage_False_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidMessage());
}

TEST(MessageIGTest,TestSendMessage_False_NotExpectedLateReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NotExpectedLateReply());
}

TEST(MessageIGTest,TestSendMessage_False_NoDestinationForReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NoDestinationForReply());
}

TEST(MessageIGTest,TestSendMessage_False_NoDestinationForExpectedReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NoDestinationForExpectedReply());
}

TEST(MessageIGTest,TestSendMessage_False_InvalidDestination) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidDestination());
}

TEST(MessageIGTest,TestSendMessage_False_InvalidFunction) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_InvalidFunction());
}

/*TEST(MessageIGTest,TestSendMessage_False_NotReplyButImmediateReplayExpected) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessage_False_NotReplyButImmediateReplayExpected());
}*/

TEST(MessageIGTest,TestSendMessageAndWaitReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitReply());
}


TEST(MessageIGTest,TestSendMessageAndWaitReply_False_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitReply_False_InvalidMessage());
}


TEST(MessageIGTest,TestSendMessageAndWaitReply_False_ReplyOfReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndWaitReply_False_ReplyOfReply());
}

TEST(MessageIGTest,TestSendMessageAndExpectReplyLater) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndExpectReplyLater());
}

TEST(MessageIGTest,TestSendMessageAndExpectReplyLater_False_InvalidMessage) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndExpectReplyLater_False_InvalidMessage());
}

TEST(MessageIGTest,TestSendMessageAndExpectReplyLater_False_ReplyOfReply) {
    MessageITest target;
    ASSERT_TRUE(target.TestSendMessageAndExpectReplyLater_False_ReplyOfReply());
}

