/**
 * @file BasicUDPSocket.cpp
 * @brief Source file for class BasicUDPSocket
 * @date 28/10/2015
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
 * the class BasicUDPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <circle/net/in.h>
#include <circle/net/netsubsystem.h>
#include <circle/sched/scheduler.h>
#include <circle/string.h>
#include <inttypes.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicUDPSocket.h"
#include "ErrorManagement.h"
#include "StreamString.h"
#include "SocketCore.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicUDPSocket::BasicUDPSocket() :
        BasicSocket() {

}

BasicUDPSocket::~BasicUDPSocket() {

}

bool BasicUDPSocket::Peek(char8* const output,
                          uint32 &size) {
    /* Cannot peek*/
    return false;
}

bool BasicUDPSocket::Read(char8* const output,
                          uint32 &size) {

    bool retVal = true;
    int32 readResult;
    CScheduler* scheduler = CScheduler::Get();

    retVal = scheduler != NULL_PTR(CScheduler*);

    if(!retVal) {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket::Read() Failure in retrieving scheduler");
    }

    if(retVal && IsValid()) {
        //Use following call to explicitly set ip/port, otherwise the ones in Connect() will be taken
        //CIPAddress remoteIPAddress(source.GetInternetHost()->ipAddressQuadU8);
        //uint16 remoteIPPort = source.GetInternetHost()->ipPort;
        //readResult = (connectionSocket.socket)->ReceiveFrom(output, size, ((IsBlocking())?0:(MSG_DONTWAIT)), &remoteIPAddress, &remoteIPPort);

        readResult = (connectionSocket.socket)->Receive(output, size, ((IsBlocking())?0:(MSG_DONTWAIT)));
        /* We need to yield asap in order for the stack to execute. The stack will, in turn, yield again and let us go on */
        
        scheduler->Yield();
        
        if(readResult < 0) {
            retVal = true;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket::Read() Read failed in ReceiveFrom");
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket::Read() Read failed due to invalid socket");
    }

    if(retVal) {
        size = static_cast<uint32>(readResult);
    }

    return retVal && (readResult > 0);

}

bool BasicUDPSocket::Write(const char8* const input,
                           uint32 &size) {

   bool retVal = true;
   int32 writeResult;

   CScheduler* scheduler = CScheduler::Get();
   retVal = (scheduler != NULL_PTR(CScheduler*));

   if(!retVal) {
       REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket::Write() Failure in retrieving scheduler");
   }

   if(retVal && IsValid()) {
       //Use following call to explicitly set ip/port, otherwise the ones in Connect() will be taken
       //CIPAddress remoteIPAddress(destination.GetInternetHost()->ipAddressQuadU8);
       //uint16 remoteIPPort = destination.GetInternetHost()->ipPort;
       //writeResult = (connectionSocket.socket)->SendTo(input, size, ((IsBlocking())?0:MSG_DONTWAIT), remoteIPAddress, remoteIPPort);
       
       writeResult = (connectionSocket.socket)->Send(input, size, ((IsBlocking())?0:MSG_DONTWAIT));

       /* We need to yield asap in order for the stack to execute. The stack will, in turn, yield again and let us go on */
       scheduler->Yield();

       if(writeResult < 0) {
           retVal = false;
           size = 0u;
           REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket::Write() Write failure in SendTo");
       }
   }
   else {
       retVal = false;
       REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocket::Write() Write failure due to invalid socket");
   }

   if(retVal) {
       size = static_cast<uint32>(writeResult);
   }

   return retVal;
}

bool BasicUDPSocket::Open() {
    bool retVal = connectionSocket.Create(SocketKindUDP);

    if(retVal) {
        retVal = IsValid();
    }

    return retVal;
}

bool BasicUDPSocket::Listen(const uint16 port) {
    /* Listen only for UDP */
    return false;
}

bool BasicUDPSocket::Connect(const char8 * const address,
                             const uint16 port) {
    
    //Connect in this context means set foreign ip/port combination
    bool retVal = true;

    destination.SetPort(port);
    retVal = destination.SetAddress(address);
    InternetHostCore* dstInternetHost = destination.GetInternetHost();
    retVal = (dstInternetHost != NULL_PTR(InternetHostCore*));

    if(!retVal) {
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "BasicUDPSocket::Connect() SetAddress failed");
    }

    if(retVal) {
        CIPAddress destinationCIPAddr(destination.GetInternetHost()->ipAddressQuadU8);
        retVal = (connectionSocket.socket->Connect(destinationCIPAddr, port) >= 0);
    }

    return retVal;
}

bool BasicUDPSocket::CanWrite() const {
    return true;
}

bool BasicUDPSocket::CanRead() const {
    return true;
}

bool BasicUDPSocket::CanSeek() const {
    return false;
}

bool BasicUDPSocket::Read(char8 * const output,
                          uint32 & size,
                          const TimeoutType &timeout) {
    //TODO: Timeout logic
    return Read(output, size);
}

bool BasicUDPSocket::Write(const char8 * const input,
                           uint32 & size,
                           const TimeoutType &timeout) {
    //TODO: Timeout logic
    return Write(input, size);
}

uint64 BasicUDPSocket::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicUDPSocket::Seek(const uint64 pos) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicUDPSocket::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicUDPSocket::Position() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the size of a socket is undefined. */
bool BasicUDPSocket::SetSize(const uint64 size) {
    return false;
}

}
