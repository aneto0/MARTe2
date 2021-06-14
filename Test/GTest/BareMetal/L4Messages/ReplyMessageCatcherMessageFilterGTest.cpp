/**
 * @file ReplyMessageCatcherMessageFilterGTest.cpp
 * @brief Source file for class ReplyMessageCatcherMessageFilterGTest
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
#include "ReplyMessageCatcherMessageFilterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestConstructor) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestSetMessageToCatch) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestSetMessageToCatch());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestWait) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestWait());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestWait_Timeout) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestWait_Timeout());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestConsumeMessage) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestConsumeMessage_NotReply) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_NotReply());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestConsumeMessage_DifferentMessage) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_DifferentMessage());
}

TEST(BareMetal_L4Messages_ReplyMessageCatcherMessageFilterGTest,TestIsPermanentFilter) {
    ReplyMessageCatcherMessageFilterTest target;
    ASSERT_TRUE(target.TestIsPermanentFilter());
}
