/**
 * @file HttpMessageInterface.cpp
 * @brief Source file for class HttpMessageInterface
 * @date 25/09/2019
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
 * the class HttpMessageInterface (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "HttpMessageInterface.h"
#include "JsonPrinter.h"
#include "Message.h"
#include "ReplyMessageCatcherMessageFilter.h"
#include "StreamStructuredData.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

HttpMessageInterface::HttpMessageInterface() :
        ReferenceContainer(), HttpDataExportI(), MessageI() {
}

HttpMessageInterface::~HttpMessageInterface() {

}

bool HttpMessageInterface::Initialise(StructuredDataI &data) {
    bool ok = ReferenceContainer::Initialise(data);
    uint32 nOfMessages = Size();
    uint32 n;
    for (n = 0u; (n < nOfMessages) && (ok); n++) {
        ReferenceT<Message> msg = Get(n);
        ok = (msg.IsValid());
        if (!ok) {
            uint32 ii = n;
            REPORT_ERROR(ErrorManagement::ParametersError, "All childs shall be Messages and item in position %d is not", ii);
        }
    }
    return ok;
}

/*lint -e{613} sdata cannot be NULL as otherwise ok would be false*/
bool HttpMessageInterface::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    bool ok = HttpDataExportI::GetAsStructuredData(data, protocol);
    StreamStructuredData<JsonPrinter> *sdata;
    if (ok) {
        sdata = dynamic_cast<StreamStructuredData<JsonPrinter> *>(&data);
        /*lint -e{665} StreamStructuredData<JsonPrinter> is only used to define the pointer type of the NULL_PTR*/
        ok = (sdata != NULL_PTR(StreamStructuredData<JsonPrinter> *));
    }
    if (ok) {
        //lint -e{644} sdata initialised otherwise ok would be false*/
        ok = sdata->GetPrinter()->PrintBegin();
    }
    if (ok) {
        StreamString msgName;
        if (protocol.GetInputCommand("msg", msgName)) {
            ok = SendMessageFromHttp(protocol);
            ok = data.Write("OK", ok ? 1u : 0u);
        }
        else {
            ok = ReferenceContainer::ExportData(data);
        }
    }
    //Print the closing {
    if (ok) {
        ok = sdata->GetPrinter()->PrintEnd();
    }
    return ok;
}

bool HttpMessageInterface::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    bool ok = HttpDataExportI::GetAsText(stream, protocol);
    if (ok) {
        ok = SendMessageFromHttp(protocol);
    }
    return ok;
}

bool HttpMessageInterface::SendMessageFromHttp(HttpProtocol &protocol) {
    bool ok = true;
    StreamString msgName;
    if (protocol.GetInputCommand("msg", msgName)) {
        ReferenceT<Message> msg = Find(msgName.Buffer());
        if (msg.IsValid()) {
            msg->SetAsReply(false);
            ErrorManagement::ErrorType err;
            if (msg->ExpectsReply()) {
                err = MessageI::SendMessageAndWaitReply(msg, this);
            }
            else {
                err = MessageI::SendMessage(msg, this);
            }
            if (err != ErrorManagement::NoError) {
                ok = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send message %s", msgName.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Message %s does not exist or is not valid", msgName.Buffer());
            ok = false;
        }
    }
    return ok;
}

CLASS_REGISTER(HttpMessageInterface, "1.0")
}

