/**
 * @file QueuedReplyMessageCatcherFilterTest.h
 * @brief Header file for class QueuedReplyMessageCatcherFilterTest
 * @date 10/10/2016
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class QueuedReplyMessageCatcherFilterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEDREPLYMESSAGECATCHERFILTERTEST_H_
#define QUEUEDREPLYMESSAGECATCHERFILTERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "QueuedReplyMessageCatcherFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the QueuedReplyMessageCatcherFilterTest public methods.
 */
class QueuedReplyMessageCatcherFilterTest {
public:

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the SetMessageToCatch method.
     */
    bool TestSetMessagesToCatch();

    /**
     * @brief Tests the SetEventSemaphore method.
     */
    bool TestSetEventSemaphore();

    /**
     * @brief Tests the ConsumeMessage method.
     */
    bool TestConsumeMessage();

    /**
     * @brief Tests the ConsumeMessage method without consuming all the messages.
     */
    bool TestConsumeMessage_MissingMessage();

    /**
     * @brief Tests the ConsumeMessage method adding a message that does not require reply.
     */
    bool TestConsumeMessage_MessageNotReply();

    /**
     * @brief Tests the ConsumeMessage method without setting the EventSem.
     */
    bool TestConsumeMessage_MessageNoEventSem();

    /**
     * @brief Tests the ConsumeMessage method by adding an invalid message to the container.
     */
    bool TestConsumeMessage_InvalidMessage();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REPLYMESSAGECATCHERMESSAGEFILTERTEST_H_ */

