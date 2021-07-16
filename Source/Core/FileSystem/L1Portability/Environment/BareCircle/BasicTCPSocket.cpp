/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
 * @date 23/10/2015
 * @author Giuseppe Ferrò
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
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <circle/net/in.h>
#include <circle/net/socket.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"
#include "BasicSocket.h"
#include "BasicTCPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicTCPSocket::BasicTCPSocket() :
    BasicSocket() {

}

BasicTCPSocket::~BasicTCPSocket() {

}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size) {
    bool retVal = true;
    int32 readResult;

    if(IsValid()) {
        readResult = static_cast<int32>((connectionSocket.socket)->Receive(output, size, (IsBlocking()?0:MSG_DONTWAIT)));
        if(readResult < 0) {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::Read() Read failure");
        }
        else {
            size = readResult;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::Read() Read operation failed due to invalid socket");
    }
    
    if(retVal) {
        size = static_cast<uint32>(readResult);
    }
    else {
        size = 0u;
    }
    
    return retVal;
}


bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size,
                          const TimeoutType &timeout) {
    return Read(output, size);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size) {
    bool retVal = true;
    int32 writeResult;

    if(IsValid()) {
        writeResult = static_cast<int32>((connectionSocket.socket)->Send(input, size, 0));

        if(writeResult < 0) {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::Write() Write failure");
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocet::Write() Write operation failed due to invalid socket");
    }

    if(retVal) {
        size = static_cast<uint32>(writeResult);
    }

    return retVal && (size > 0);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size,
                           const TimeoutType &timeout) {
    return Write(input, size);
}

bool BasicTCPSocket::Open() {
    bool retVal = connectionSocket.Create(SocketKindTCP);

    if(retVal) {
        retVal = IsValid();
    }

    return retVal;
}

bool BasicTCPSocket::Listen(const uint16 port,
                            const int32 maxConnections) const {

    bool retVal = true;
    int32 cSocketErrNo = 0;

    if(IsValid()) {
        cSocketErrNo = (connectionSocket.socket)->Bind(port);

        //Bind returns 0 on success while < 0 on error
        if(cSocketErrNo < 0) {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::Listen() bind failure");
        }
        
        if(retVal) {
            cSocketErrNo = (connectionSocket.socket)->Listen(maxConnections);
            if(cSocketErrNo < 0) {
                retVal = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::Listen() listen failure");
            }
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::Listen() invalid socket handle");
    }


    return retVal;
}

bool BasicTCPSocket::Connect(const char8 * const address,
                             const uint16 port,
                             const TimeoutType &timeout) {
    
    bool retVal = true;

    destination.SetPort(port);
    retVal = destination.SetAddress(address);

    if(!retVal) {
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "BasicTCPSocket::Connect() SetAddress failed");
    }

    if(retVal) {
        CIPAddress destinationCIPAddr(destination.GetInternetHost()->ipAddressQuadU8);
        retVal = (connectionSocket.socket->Connect(destinationCIPAddr, port) == 0);
    }

    if(!retVal) {
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "BasicTCPSocket::Connect() Connect failed");
    }

    return retVal;
}

bool BasicTCPSocket::IsConnected() const {
    //Indirect evaluation of connection status
    //Foreign IP address is 0 if socket is disconnected
    return (connectionSocket.socket->GetForeignIP() != 0);

}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const TimeoutType &timeout,
                                               BasicTCPSocket *client) {
    BasicTCPSocket *retVal = NULL_PTR(BasicTCPSocket*);

    if(IsValid()) {
        CIPAddress remoteIPAddress;
        uint16 remoteIPPort;

        CSocket *waitSocket = (connectionSocket.socket)->Accept(&remoteIPAddress, &remoteIPPort);
        if(waitSocket != NULL_PTR(CSocket*)) {
            client = new BasicTCPSocket();
            ::CString ipAddrString;
            remoteIPAddress.Format(&ipAddrString);
            InternetHost clientDestination(remoteIPPort, (const char*)ipAddrString);
            (client->connectionSocket).SetSocket(waitSocket, SocketKindTCP);
            client->SetDestination(clientDestination);           
            retVal = client;
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::WaitConnection() Error waiting, accept failed");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicTCPSocket::WaitConnection() Error waiting, socket is not valid");
    }
 
    return retVal;
}

bool BasicTCPSocket::Peek(char8* const buffer,
                          uint32 &size) const {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::Seek(const uint64 pos) {
    /* Socket cannot seek */
    return false;
}

uint64 BasicTCPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicTCPSocket::Position() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the size of a socket is undefined. */
bool BasicTCPSocket::SetSize(const uint64 size) {
    return false;
}

bool BasicTCPSocket::CanWrite() const {
    return true;
}

bool BasicTCPSocket::CanRead() const {
    return true;
}

bool BasicTCPSocket::CanSeek() const {
    return false;
}

}

