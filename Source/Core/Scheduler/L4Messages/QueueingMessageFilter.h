/**
 * @file QueueingMessageFilter.h
 * @brief Header file for class QueueingMessageFilter
 * @date August 22, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class MessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEINGMESSAGEFILTER_H_
#define QUEUEINGMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageFilter.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO
 * @details TODO
 */
class DLL_API QueueingMessageFilter: public MessageFilter {
public:

    /**
     * @brief Constructor.
     */
    QueueingMessageFilter();

    /**
     * @brief Destructor.
     */
    virtual ~QueueingMessageFilter();

    /**
     * TODO
     * Simply consumes all messages and puts them in a Q
    */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * TODO
    */
    ErrorManagement::ErrorType GetMessage(ReferenceT<Message> &message,const TimeoutType &timeout=TTInfiniteWait);

private:

    /**
     * TODO
     * The message consuming filters
     * used by SortMessage
     *
     * */
    ReferenceContainer messageQ;

    /**
     * TODO
     * To wake consuming threads
     *
     * */
    EventSem newMessagesAlarm;


};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}


#endif /* QUEUEINGMESSAGEFILTER_H_ */

