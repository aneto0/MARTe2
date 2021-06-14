/**
 * @file QueueingMessageFilterGTest.cpp
 * @brief Source file for class QueueingMessageFilterGTest
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
#include "QueueingMessageFilterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L4Messages_QueueingMessageFilterGTest,TestConstructor) {
    QueueingMessageFilterTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L4Messages_QueueingMessageFilterGTest,TestConsumeMessage) {
    QueueingMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage());
}

TEST(Scheduler_L4Messages_QueueingMessageFilterGTest,TestGetMessage) {
    QueueingMessageFilterTest target;
    ASSERT_TRUE(target.TestGetMessage());
}

TEST(Scheduler_L4Messages_QueueingMessageFilterGTest,TestGetMessage_Wait) {
    QueueingMessageFilterTest target;
    ASSERT_TRUE(target.TestGetMessage_Wait());
}

TEST(Scheduler_L4Messages_QueueingMessageFilterGTest,TestGetMessage_Wait_Timeout) {
    QueueingMessageFilterTest target;
    ASSERT_TRUE(target.TestGetMessage_Wait_Timeout());
}

TEST(Scheduler_L4Messages_QueueingMessageFilterGTest,TestIsPermanentFilter) {
    QueueingMessageFilterTest target;
    ASSERT_TRUE(target.TestIsPermanentFilter());
}
