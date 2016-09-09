/**
 * @file ReplyMessageCatcherMessageFilter.h
 * @brief Header file for class ReplyMessageCatcherMessageFilter
 * @date 19/08/2016
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class ReplyMessageCatcherMessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REPLYMESSAGECATCHERMESSAGEFILTER_H_
#define REPLYMESSAGECATCHERMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Allows catching (i.e. trapping) a specific message reply.
 * @details The filter is removed after catching the reply.
 */
class ReplyMessageCatcherMessageFilter: public MessageFilter {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. Initialises the MessageFilter(false).
     * @post
     *   IsPermanentFilter() == false
     */
    ReplyMessageCatcherMessageFilter();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ReplyMessageCatcherMessageFilter();

    /**
     * @brief Sets the reply message to be caught.
     * @param[in] message the reply message to be caught.
     */
    void SetMessageToCatch(const ReferenceT<Message> &message);


    /**
     * @brief Waits for the message to be caught.
     * @details This implementation polls with a sleep time of \a pollingTimeUsec.
     * @param[in] maxWait Maximum time to wait for the message to be caught.
     * @param[in] pollingTimeUsec Time to wait on the polling sleep.
     * @return ErrorManagement::NoError if the message was caught or ErrorManagement::Timeout if the time specified in \a maxWait has expired.
     */
    virtual ErrorManagement::ErrorType Wait(const TimeoutType &maxWait = TTInfiniteWait, const uint32 pollingTimeUsec = 1000);

protected:

    /**
     * @brief Verifies if the \a messageToTest is the message to be caught (see SetMessageToCatch).
     * @param[in] messageToTest The message to test.
     * @return ErrorManagement::NoError if the messageToTest was the one to be caught, otherwise it returns ErrorManagement::UnsupportedFeature.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * @brief Called when the message to catch has been found.
     * @details The default implementation sets ReplyCaught() == true.
     * @param[in] replyMessage the caught message.
     */
    virtual void HandleReplyMessage(ReferenceT<Message> &replyMessage);

    /*
     * @brief Returns true if the message was caught.
     * @return true if the message was caught.
     */
    bool ReplyCaught();

private:

    /**
     * True if the message was caught.
     */
    bool caught;

    /**
     * The message to catch.
     */
    ReferenceT<Message> messageToCatch;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REPLYMESSAGECATCHERMESSAGEFILTER_H_ */

