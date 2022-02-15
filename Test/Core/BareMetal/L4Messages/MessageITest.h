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

/**
 * Tests the MessageI public methods.
 */
class MessageITest {
public:

    /**
     * @brief Default constructor
     */
    MessageITest();

    /**
     * @brief Destructor
     */
    virtual ~MessageITest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the SendMessage method
     */
    bool TestSendMessage();

    /**
     * @brief Tests the SendMessage method with a NULL source
     */
    bool TestSendMessage_NULL_Source();

    /**
     * @brief Tests the SendMessage method with an invalid messages
     */
    bool TestSendMessage_False_InvalidMessage();

    /**
     * @brief Tests the SendMessage method with not expected Indirect reply
     */
    bool TestSendMessage_False_NotExpectedIndirectReply();

    /**
     * @brief Tests the SendMessage method with no destination for reply
     */
    bool TestSendMessage_False_NoDestinationForReply();

    /**
     * @brief Tests the SendMessage method with no destination for expected reply
     */
    bool TestSendMessage_False_NoDestinationForExpectedReply();

    /**
     * @brief Tests the SendMessage method with invalid destination
     */
    bool TestSendMessage_False_InvalidDestination();

    /**
     * @brief Tests the SendMessage method with invalid destination (that does not exist)
     */
    bool TestSendMessage_False_InvalidDestination_Not_Exist();

    /**
     * @brief Tests the SendMessage method with invalid function
     */
    bool TestSendMessage_False_InvalidFunction();

    /**
     * @brief Tests the SendMessageAndWaitReply method
     */
    bool TestSendMessageAndWaitReply();

    /**
     * @brief Tests the SendMessageAndWaitReply method with invalid message
     */
    bool TestSendMessageAndWaitReply_False_InvalidMessage();

    /**
     * @brief Tests the SendMessageAndWaitReply method with reply of reply
     */
    bool TestSendMessageAndWaitReply_False_ReplyOfReply();

    /**
     * @brief Tests the SendMessage using a custom catcher
     */
    bool TestSendMessage_ExpectsIndirectReply();

    /**
     * @brief Tests the TestSendMessageAndWaitIndirectReply
     */
    bool TestSendMessageAndWaitIndirectReply();

    /**
     * @brief Tests the SendMessageAndWaitIndirectReply method with invalid message
     */
    bool TestSendMessageAndWaitIndirectReply_InvalidMessage();

    /**
     * @brief Tests the SendMessageAndWaitIndirectReply method with invalid destination (check that the filter was removed)
     */
    bool TestSendMessageAndWaitIndirectReply_InvalidDestination();

    /**
     * @brief Tests the WaitForReply function
     */
    bool TestWaitForReply();

    /**
     * @brief Tests the WaitForReply function with an invalid message
     */
    bool TestWaitForReply_InvalidMessage();

    /**
     * @brief Tests the WaitForReply function with a message that does not expect a reply
     */
    bool TestWaitForReply_NotExpectsReply();

    /**
     * @brief Tests the WaitForReply function and timeout
     */
    bool TestWaitForReply_Timeout();

    /**
     * @brief Tests the InstallMessageFilter method
     */
    bool TestInstallMessageFilter();

    /**
     * @brief Tests the RemoveMessageFilter method
     */
    bool TestRemoveMessageFilter();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEITEST_H_ */

