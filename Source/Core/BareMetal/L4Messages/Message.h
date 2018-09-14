/**
 * @file Message.h
 * @brief Header file for class Message
 * @date 05/04/2016
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
     * @brief Default constructor.
     */
    Message();

    /**
     * @brief Destructor
     */
    virtual ~Message();

    /**
     * @brief Explores the StructuredDataI in input and builds Objects storing
     * their References.
     * @details The following parameters have to be specified in the StructuredDataI which initialises the
     * Message
     * @param[in] data StructuredDataI with the structure:
     *   Destination = "Address in the ObjectRegistryDatabase of the Object which must receive this Message"
     *   Function = "The name of the destination method which has to be called" (this method has to be registered in the ClassRegistryDatabase)
     *   MaxWait (optional) = "The timeout in milliseconds" (default is TTInfiniteWait, namely infinite timeout)
     *   Mode (optional) = "The message type". Can be one of the following;
     *     "ExpectsReply": after sending the message, the sender expects a reply in the Message.
     *     "ExpectsIndirectReply": after sending the message the sender expects a reply sent to him.
     *   By default, the sender will not wait for a message reply.
     *   !!!!! IMPORTANT !!!! a reply is always contained in the same Message object used to send
     *   a    direct reply is performed by transforming the Message object into a reply
     *   an indirect reply is performed by transforming the Message object into a reply and by sending it back to the sender (i.e. by calling SendMessage as it was a message)
     * @see ReferenceContainer::Initialise(*)
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Sets or unsets this Message as a Reply.
     * @param[in] flag if true the message is set as a reply, otherwise it is not considered as a reply.
     */
    void SetAsReply(const bool flag=true);

    /**
     * @brief Specifies if this message requires a reply
     * marked by send when requiring reply
     * @param[in] flag if true means that this messages requires a reply after being received, if false it does not needs a reply.
     */
    void SetExpectsReply(const bool flag=true);

    /**
     * @brief Specifies if this message requires an indirect reply.
     * marked by send when requiring reply
     * @param[in] flag (only if expectsReply is true) if true means that this messages requires an indirect reply , if false it needs a direct reply.
     */
    void SetExpectsIndirectReply(const bool flag=true);

    /**
     * @brief Checks if this message requires a reply (immediate or not).
     * @return true if this message requires a reply, false otherwise.
     */
    bool ExpectsReply() const;

    /**
     * @brief Checks if this message requires an indirect reply.
     * @return true if this message requires an indirect reply, false otherwise.
     */
    bool ExpectsIndirectReply() const;

    /**
     * @brief Checks if this message is a reply.
     * @return true if this message is a reply, false otherwise.
     */
    bool IsReply() const;

    /**
     * @brief Retrieved the address of the destination Object in the ObjectRegistryDatabase.
     * @return the address of the destination Object in the ObjectRegistryDatabase.
     */
    CCString GetDestination();

    /**
     * @brief Gets the address of the sender Object.
     * @details After that the destination received a message expecting a reply, it will call this function to get the
     * destination of the reply (namely the address of the sender Object).
     * @return the address of the sender Object (which shall exist in the ObjectRegistryDatabase).
     */
    const Object * const GetSender() const;

    /**
     * @brief Sets the sender Object address.
     * @param[in] senderIn is the address of the sender Object (which shall exist in the ObjectRegistryDatabase).
     */
    void SetSender(const Object * const senderIn);

    /**
     * @brief Retrieves the name of the registered class method of the destination Object which has to be called.
     * @return the name of the registered class method of the destination Object which has to be called.
     */
    CCString GetFunction();

    /**
     * @brief Sets the time to wait for a reply.
     * @param[in] maxWaitIn is the timeout time in milliseconds.
     */
    void SetReplyTimeout(const TimeoutType &maxWaitIn);

    /**
     * @brief Gets the timeout for the reply
     */
    TimeoutType GetReplyTimeout() const;

private:

    struct MessageFlags {

        /**
         * @brief Default Constructor.
         * @post
         *   Message::IsReply() == false &&
         *   Message::ExpectsReply() == false &&
         *   Message::ExpectsIndirectReply() == false
         */
        MessageFlags();

        /**
         * @brief Constructor from string.
         * @param[in] asString specifies the Message type.
         * @post
         *   if(asString == "ExpectsReply") then
         *     Message::ExpectsReply() == true &&
         *     Message::ExpectsIndirectReply() == true;
         *
         *   if(asString == "ExpectsIndirectReply") then
         *     Message::ExpectsReply() == true &&
         *     Message::ExpectsIndirectReply() == false;
         */
        MessageFlags(CCString asString);

        /*lint ++flb*/
        union {

            /**
             * True if reply is required
             * Set by the sender
             */
            BitBoolean<uint8, 0u> expectsReply;

            /**
             * True if reply is required
             * and it has to be sent to me by the receiver
             * False if reply is only generated within the original message
             * set by the sender
             */
            BitBoolean<uint8, 1u> expectsIndirectReply;

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
            BitRange<uint8, 5u ,3u> unMapped;

            /**
             * To set the Message mode using an 8-bit integer.
             */
            uint8 format_as_uint8;
        };
        /*lint --flb*/

    };

    /**
     * The originator of the message
     * empty means anonymous
     * */
    const Object * sender;

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
    MessageFlags flags;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* MESSAGE_H_ */

