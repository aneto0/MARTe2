/**
 * @file QueuedReplyMessageCatcherFilter.cpp
 * @brief Source file for class QueuedReplyMessageCatcherFilter
 * @date 7/10/2016
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
 * the class QueuedReplyMessageCatcherFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

QueuedReplyMessageCatcherFilter::QueuedReplyMessageCatcherFilter() :
        Object(),
        MessageFilter(false) {
}

void ReplyMessageCatcherMessageFilter::SetMessageToCatch(const ReferenceT<Message> &message) {
    messageToCatch = message;
}

ReplyMessageCatcherMessageFilter::~ReplyMessageCatcherMessageFilter() {
    caught = true;
}

ErrorManagement::ErrorType ReplyMessageCatcherMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest) {

    ErrorManagement::ErrorType ret(true);

    if (messageToTest == messageToCatch) {

        //check reply flag
        if (!messageToTest->IsReply()) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "The message caught is not a reply %s", messageToTest.operator ->()->GetName())
            ret.warning = true;
        }
        HandleReplyMessage(messageToTest);

    }
    else {
        ret.unsupportedFeature = true;
    }

    return ret;

}
}

	
