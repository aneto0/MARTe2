/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
 * @date 20/04/2019
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
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifdef LWIP_ENABLED
#include "lwip/sockets.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "ErrorManagement.h"
#include "Select.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define sock_errno()  errno

namespace MARTe {

BasicTCPSocket::BasicTCPSocket() :
        BasicSocket() {
}

BasicTCPSocket::~BasicTCPSocket() {

}

bool BasicTCPSocket::Open() {
    bool ret = false;
#ifdef LWIP_ENABLED

#endif
    return ret;
}

bool BasicTCPSocket::Listen(const uint16 port,
                            const int32 maxConnections) const {

    bool ret = false;
#ifdef LWIP_ENABLED

#endif
    return ret;
}

bool BasicTCPSocket::Connect(const char8 * const address,
                             const uint16 port,
                             const TimeoutType &timeout) {
    destination.SetPort(port);
    bool ret = IsValid();
    bool wasBlocking = IsBlocking();
#ifdef LWIP_ENABLED

#endif

    return ret;
}

bool BasicTCPSocket::IsConnected() const {

    int32 ret = -1;
#ifdef LWIP_ENABLED
  
#endif

    return (ret == 0);

}

BasicTCPSocket *BasicTCPSocket::WaitConnection(const TimeoutType &timeout,
                                               BasicTCPSocket *client) {
    BasicTCPSocket *ret = static_cast<BasicTCPSocket *>(NULL);
#ifdef LWIP_ENABLED

#endif

    return ret;
}

bool BasicTCPSocket::Peek(char8* const buffer,
                          uint32 &size) const {
    int32 ret = -1;
#ifdef LWIP_ENABLED

#endif

    return (ret > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size) {
    uint32 sizetoRead = size;
    size = 0u;
    int32 readBytes = 0;
#ifdef LWIP_ENABLED
    
#endif

    return (readBytes > 0);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size) {
    int32 writtenBytes = 0;
    uint32 sizeToWrite = size;
    size = 0u;
#ifdef LWIP_ENABLED
   
#endif
    return (writtenBytes > 0);
}

bool BasicTCPSocket::Read(char8* const output,
                          uint32 &size,
                          const TimeoutType &timeout) {

    uint32 sizeToRead = size;
    size = 0u;
#ifdef LWIP_ENABLED
    
#endif
    return (size > 0u);
}

bool BasicTCPSocket::Write(const char8* const input,
                           uint32 &size,
                           const TimeoutType &timeout) {

    uint32 sizeToWrite = size;
    size = 0u;
#ifdef LWIP_ENABLED
    
#endif
    return (size > 0u);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: sockets cannot seek. */
bool BasicTCPSocket::Seek(const uint64 pos) {
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
    return IsValid();
}

bool BasicTCPSocket::CanRead() const {
    return IsValid();
}

bool BasicTCPSocket::CanSeek() const {
    return false;
}

}

