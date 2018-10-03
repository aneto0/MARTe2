/**
 * @file Message.cpp
 * @brief Source file for class Message
 * @date 15/04/2016
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

 * @details This source file contains the definition of all the methods for
 * the class Message (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Message.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Message::Message() :
        ReferenceContainer(),
        destination(),
        function(),
        maxWait(),
        flags() {
    sender = NULL_PTR(const Object *);
}

/*lint -e{1540} sender is destroyed by caller of SetSender.*/
Message::~Message() {
}

void Message::SetAsReply(const bool flag) {
    flags.isReply = flag;
}

void Message::SetExpectsReply(const bool flag) {
    flags.expectsReply = flag;
}

void Message::SetExpectsIndirectReply(const bool flag) {
    flags.expectsIndirectReply = flag;
}

bool Message::ExpectsReply() const {
    return (flags.expectsReply);
}

bool Message::ExpectsIndirectReply() const {
    return flags.expectsIndirectReply;
}

bool Message::IsReply() const {
    return flags.isReply;
}

CCString Message::GetDestination() {
	return destination;
//    return destination.Buffer();
}

bool Message::Initialise(StructuredDataI &data) {
    bool ret = (ReferenceContainer::Initialise(data));
    if (ret) {
        ret = data.Read("Destination", destination);
        REPORT_ERROR(ret, "Destination not set");

        if (ret) {
            ret = data.Read("Function", function);
            REPORT_ERROR(ret, "Function not set");
        }
        if (ret) {
            maxWait = MilliSeconds::Infinite;
            uint32 msecWait;
            if (data.Read("ReplyTimeout", msecWait)) {
                maxWait = MilliSeconds(msecWait,Units::ms);
            }

            DynamicCString messageFlags;
//            StreamString messageFlags;
            if (data.Read("Mode", messageFlags)) {
                flags = MessageFlags(messageFlags.GetList());
            }
        }
    }

    return ret;
}

Message::MessageFlags::MessageFlags() {
    expectsReply = false;
    expectsIndirectReply = false;
    isReply = false;
}

Message::MessageFlags::MessageFlags(CCString asString) {
    expectsReply 		 = (asString == CCString("ExpectsReply"));
    expectsIndirectReply = (asString == CCString("ExpectsIndirectReply"));
//    expectsReply = (StringHelper::Compare(asString.GetList(), "ExpectsReply") == 0);
//    expectsIndirectReply = (StringHelper::Compare(asString.GetList(), "ExpectsIndirectReply") == 0);
    if (bool(expectsIndirectReply)) {
        expectsReply = true;
    }
    isReply = false;
}

const Object * const Message::GetSender() const {
    return sender;
}

void Message::SetSender(const Object * const senderIn) {
    sender = senderIn;
}

CCString Message::GetFunction() {
//    return function.Buffer();
	return function;
}

void Message::SetReplyTimeout(const MilliSeconds &maxWaitIn) {
    maxWait = maxWaitIn;
}

MilliSeconds Message::GetReplyTimeout() const {
    return maxWait;
}

CLASS_REGISTER(Message, "1.0")

}
