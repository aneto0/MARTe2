/**
 * @file ReplyMessageCatcherMessageFilterTest.h
 * @brief Header file for class ReplyMessageCatcherMessageFilterTest
 * @date 09/09/2016
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

 * @details This header file contains the declaration of the class ReplyMessageCatcherMessageFilterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REPLYMESSAGECATCHERMESSAGEFILTERTEST_H_
#define REPLYMESSAGECATCHERMESSAGEFILTERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReplyMessageCatcherMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the ReplyMessageCatcherMessageFilterTest public methods.
 */
class ReplyMessageCatcherMessageFilterTest {
public:

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the SetMessageToCatch method.
     */
    bool TestSetMessageToCatch();

    /**
     * @brief Tests the Wait method.
     */
    bool TestWait();

    /**
     * @brief Tests the Wait method with a timeout.
     */
    bool TestWait_Timeout();

    /**
     * Auxiliary variables needed for the TestWait
     */
    MARTe::uint32 waitState;
    MARTe::ReplyMessageCatcherMessageFilter waitFilter;
    MARTe::ReferenceT<MARTe::Message> waitMessage;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REPLYMESSAGECATCHERMESSAGEFILTERTEST_H_ */

