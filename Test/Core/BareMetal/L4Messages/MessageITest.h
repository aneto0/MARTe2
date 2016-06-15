/**
 * @file MessageITest.h
 * @brief Header file for class MessageITest
 * @date 11/05/2016
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

 * @details This header file contains the declaration of the class MessageITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEITEST_H_
#define MESSAGEITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the TokenInfo functions
 */
class MessageITest {
public:

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    bool TestSendMessage();

    bool TestSendMessage_NULL_Source();

    bool TestSendMessage_False_InvalidMessage();


    // trying to send a reply message but the destination will not expect it!!
    bool TestSendMessage_False_NotExpectedLateReply();

    bool TestSendMessage_False_NoDestinationForReply();

    bool TestSendMessage_False_NoDestinationForExpectedReply();

    bool TestSendMessage_False_InvalidDestination();

    bool TestSendMessage_False_InvalidFunction();

    // bool TestSendMessage_False_NotReplyButImmediateReplayExpected();


    bool TestSendMessageAndWaitReply();

    bool TestSendMessageAndWaitReply_False_InvalidMessage();

    bool TestSendMessageAndWaitReply_False_ReplyOfReply();

    bool TestSendMessageAndExpectReplyLater();

    bool TestSendMessageAndExpectReplyLater_False_InvalidMessage();

    bool TestSendMessageAndExpectReplyLater_False_ReplyOfReply();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEITEST_H_ */

