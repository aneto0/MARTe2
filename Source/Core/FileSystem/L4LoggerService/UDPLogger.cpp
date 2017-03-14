/**
 * @file UDPSocket.cpp
 * @brief Source file for class UDPSocket
 * @date 14/03/2017
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
 * the class UDPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "UDPLogger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
UDPLogger::UDPLogger() : Object(), LoggerConsumerI() {
}

UDPLogger::~UDPLogger() {
    if(!udpSocket.Close()) {
        REPORT_ERROR(ErrorManagement::Warning, "Failed to close the UDP socket");
    }
}

void UDPLogger::ConsumeLogMessage(LoggerPage *logPage) {
    StreamString logMsg;
    PrintToStream(logPage, logMsg);
    uint32 msgSize = logMsg.Size();
    (void) udpSocket.Write(logMsg.Buffer(), msgSize);
}

bool UDPLogger::Initialise(StructuredDataI &data) {
    bool ok = udpSocket.Open();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Failed to open the UDP socket");
    }
    StreamString address;
    if (ok) {
        ok = data.Read("Address", address);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Address parameter is compulsory");
        }
    }
    uint16 port;
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Port parameter is compulsory");
        }
    }
    if (ok) {
        ok = udpSocket.Connect(address.Buffer(), port);
    }
    return ok;
}

CLASS_REGISTER(UDPLogger, "1.0")

}
