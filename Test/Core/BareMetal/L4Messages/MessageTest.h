/**
 * @file MessageTest.h
 * @brief Header file for class MessageTest
 * @date 14/06/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class MessageTest
 * with all of its public, protected and private members. It may also include
 * definitions for methods which need to be visible to the compiler.
 */

#ifndef MESSAGETEST_H_
#define MESSAGETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Message.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the Message public methods.
 */
class MessageTest {
public:

    /**
     * @brief Default constructor
     */
    MessageTest();

    /**
     * @brief Destructor
     */
    virtual ~MessageTest();

    /**
     * @brief Tests the default constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the SetAsReply method
     */
    bool TestSetAsReply();

    /**
     * @brief Tests the IsReplyMessage method
     */
    bool TestIsReply();

    /**
     * @brief Tests the TestSetExpectsReply method
     */
    bool TestSetExpectsReply();

    /**
     * @brief Tests the SetExpectsIndirectReply method
     */
    bool TestSetExpectsIndirectReply();

    /**
     * @brief Tests the ExpectsReply method
     */
    bool TestExpectsReply();

    /**
     * @brief Tests the ExpectsIndirectReply method
     */
    bool TestExpectsIndirectReply();

    /**
     * @brief Tests the GetDestination method
     */
    bool TestGetDestination();

    /**
     * @brief Tests the GetSender method
     */
    bool TestGetSender();

    /**
     * @brief Tests the SetSender method
     */
    bool TestSetSender();

    /**
     * @brief Tests the GetFunction method
     */
    bool TestGetFunction();

    /**
     * @brief Tests the SetReplyTimeout method
     */
    bool TestSetReplyTimeout();

private:

    /**
     * @brief Data structure for testing initialisation of messages
     */
    struct MessageInitTableTest {
        const MARTe::char8* destination;
        const MARTe::char8* function;
        const MARTe::TimeoutType replyTimeout;
        const MARTe::char8* mode;
        const MARTe::char8* configuration;
        bool expected;
    };

    /**
     * @brief Tests the Initialise method (parameterised version)
     */
    bool TestInitialise(const MessageInitTableTest * table);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGETEST_H_ */

