/**
 * @file Message.h
 * @brief Header file for class Message
 * @date Apr 5, 2016
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

 * @details This header file contains the declaration of the class Message
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "CString.h"
#include "StreamString.h"
#include "TimeoutType.h"
#include "BitBoolean.h"
#include "BitRange.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of a message that can be sent to or received from MARTe::Object instances.
 * @details The Message is a ReferenceContainer, so it can contain a generic pay-load.
 */
class DLL_API Message: public ReferenceContainer {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
Message    ();
    /**
     * @brief Destructor
     */
    virtual ~Message();

    /**
     * @see ReferenceContainer::Initialise(*)
     * @detail The following parameters have to be specified in the StructuredDataI which initialises the
     * Message:
     *   Destination = "Address in the ObjectRegistryDatabase of the Object which must receive this Message"
     *   Function = "The name of the destination method which has to be called" (this method has to be registered in the ClassRegistryDatabase)
     *   MaxWait (optional) = "The timeout in milliseconds" (default is TTInfiniteWait, namely infinite timeout)
     *   Mode (optional) = "The message type". Can be one of the following;
     *     "ExpectsReply": after sending the message, the sender waits for a reply.
     *     "ExpectsImmediateReply": after sending the message the sender waits for an immediate reply.
     *   By default, the sender will not wait for a message reply.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Sets or Unsets this Message as a Reply.
     * @param[in] flag if true the message is set as a reply, otherwise it is not considered as a reply.
     */
    void MarkAsReply(const bool flag=true);

    /**
     * @brief Specifies if this message requires an immediate reply.
     * marked by send when requiring reply
     * @param[in] flag if true means that this messages requires a reply after being received, if false id does needs a reply.
     */
    void MarkImmediateReplyExpected(const bool flag=true);

    /**
     * @brief Specifies if this message requires a reply.
     * marked by send when requiring reply
     * @param[in] flag if true means that this messages requires a reply after being received, if false id does needs a reply.
     */
    void MarkLateReplyExpected(const bool flag=true);

    /**
     * @brief Checks if this message requires a reply (immediate or not).
     * @return true if this message requires a reply, false otherwise.
     */
    bool ReplyExpected() const;

    /**
     * @brief Checks if this message requires an immediate reply.
     * @return true if this message requires an immediate reply, false otherwise.
     */
    bool ImmediateReplyExpected()const;

    /**
     * @brief Checks if this message requires a late reply.
     * @return true if this message requires a late reply, false otherwise.
     */
    bool LateReplyExpected()const;

    /**
     * @brief Checks if this message is a reply.
     * @return true if this message is a reply, false otherwise.
     */
    bool IsReplyMessage()const;

    /**
     * @brief Retrieved the address of the destination Object in the ObjectRegistryDatabase.
     * @return the address of the destination Object in the ObjectRegistryDatabase.
     */
    CCString GetDestination();

    /**
     * @brief Retrieved the address of the sender Object in the ObjectRegistryDatabase.
     * @details After that the destination received a message expecting a reply, it will call this function to get the
     * destination of the reply (namely the address of the sender Object).
     * @return the address of the sender Object in the ObjectRegistryDatabase.
     */
    inline CCString GetSender();

    /**
     * @brief Sets the sender Object address in the ObjectRegistryDatabase.
     * @param[in] senderName is the address of the sender Object in the ObjectRegistryDatabase.
     */
    inline void SetSender(CCString senderName);

    /**
     * @brief Retrieves the name of the registered class method of the destination Object which has to be called.
     * @return the name of the registered class method of the destination Object which has to be called.
     */
    inline CCString GetFunction();

    /**
     * @brief Sets the time to wait for a reply.
     * @param[in] maxWaitIn is the timeout time in milliseconds.
     */
    inline void SetReplyTimeout(const TimeoutType &maxWaitIn);

    inline TimeoutType GetReplyTimeout();

private:

    /**
     * The originator of the message
     * empty means anonymous
     * */
    StreamString sender;

    /**
     * The destination of the message
     * addressed from ObjectRegistryDatabase forward
     * */
    StreamString destination;

    /**
     * The function to be called
     * */
    StreamString function;

    /**
     * In case of synchronous communication, how long to wait
     * */
    TimeoutType maxWait;

    /**
     * @brief Defines the Message types.
     */
    struct MessageFlags {

        /**
         * @brief Default Constructor.
         * @post
         *   Message::IsReply() == false &&
         *   Message::ExpectsReply() == false &&
         *   Message::ExpectsImmediateReply() == false &&
         *   Message::ExpectsLateReply() == false;
         */
        MessageFlags() {
            expectsReply = false;
            expectsImmediateReply = false;
            isReply = false;
        }

        /**
         * @brief Constructor from string.
         * @param[in] asString specifies the Message type.
         * @post
         *   if(asString == "ExpectsReply") then
         *     Message::ExpectsReply() == true &&
         *     Message::ExpectsLateReply() == true &&
         *     Message::ExpectsImmediateReply() == false;
         *
         *   if(asString == "ExpectsImmediateReply") then
         *     Message::ExpectsReply() == true &&
         *     Message::ExpectsLateReply() == false &&
         *     Message::ExpectsImmediateReply() == true;
         */
        MessageFlags(CCString asString) {
            expectsReply = (StringHelper::Compare(asString.GetList(),"ExpectsReply")==0);
            expectsImmediateReply = (StringHelper::Compare(asString.GetList(),"ExpectsImmediateReply")==0);
            if (expectsImmediateReply==true) {
                expectsReply = true;
            }
            isReply = false;
        }

        /*lint -e{9018} Use of union allows to use this memory to describe both objects and basic types.*/
        union {

            /**
             * True if reply is required
             * Set by the sender
             */
            BitBoolean<uint8, 0u> expectsReply;

            /**
             * True if reply is required
             * and I am going to wait for it in the call itself
             * set by the sender
             */
            BitBoolean<uint8, 1u> expectsImmediateReply;

            /**
             * True for a reply message, false for a normal message
             * in a reply message sender and destination are implicitly flipped
             * set by the recipient::HandleMessage() or by the caller of the recipient::SortMessage()
             * if this message is sent than it will call the sender::HandleReply() function
             */
            BitBoolean<uint8, 2u> isReply;

            /**
             * Unmapped area
             */
            BitRange<uint8,4u ,3u> unMapped;

            /**
             * To set the Message mode using an 8-bit integer.
             */
            uint8 format_as_uint8;
        };

    }flags;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CCString Message::GetSender() {
    return sender.Buffer();
}

void Message::SetSender(CCString senderName) {
    sender = senderName.GetList();
}

CCString Message::GetFunction() {
    return function.Buffer();
}

void Message::SetReplyTimeout(const TimeoutType &maxWaitIn) {
    maxWait = maxWaitIn;
}

TimeoutType Message::GetReplyTimeout() {
    return maxWait;
}

}

#endif /* MESSAGE_H_ */

