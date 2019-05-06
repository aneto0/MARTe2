/**
 * @file InternetHost.cpp
 * @brief Source file for class InternetHost
 * @date 28/10/2015
 * @author Giuseppe Ferro
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
 * the class InternetHost (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetHost.h"
#include "FastPollingMutexSem.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

StreamString InternetHost::GetHostName() const {

    StreamString hostname = "hostname";
    return hostname;
}

const char8 *InternetHost::GetLocalHostName() {
    return "localhost";
}

const char8 *InternetHost::GetLocalAddress() {
    return "127.0.0.1";
}

uint32 InternetHost::GetLocalAddressAsNumber() {

    return ((127 << 24) + 1);
}

InternetHost::InternetHost(const uint16 port,
                           const char8 * const addr) :
        address(addr, port) {
}

uint16 InternetHost::GetPort() const {
    return address.myPort;
}

StreamString InternetHost::GetAddress() const {
    StreamString dotName;
    dotName.Printf("%u.%u.%u.%u", address.ipAddr[0], address.ipAddr[1], address.ipAddr[2], address.ipAddr[3]);
    return dotName;
}

/**  returns the host number associated to this InternetHost*/
uint32 InternetHost::GetAddressAsNumber() const {
    return ((address.ipAddr[0] << 24) + (address.ipAddr[1] << 16) + (address.ipAddr[2] << 8) + address.ipAddr[3]);
}

/** sets the port value  */
void InternetHost::SetPort(const uint16 port) {
    address.myPort = port;
}

bool InternetHost::SetAddress(const char8 * const addr) {
    address.SetAddress(addr);
    return true;
}

bool InternetHost::SetAddressByHostName(const char8 * hostName) {
    return false;
}

void InternetHost::SetAddressByNumber(const uint32 number) {
    address.ipAddr[3] = (number >> 24) & 0xFF;
    address.ipAddr[2] = (number >> 16) & 0xFF;
    address.ipAddr[1] = (number >> 8) & 0xFF;
    address.ipAddr[0] = (number) & 0xFF;
}

/**  The address of the local host */
bool InternetHost::SetLocalAddress() {
    return SetAddressByHostName(static_cast<const char8*>(NULL));
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: sockets will change this attribute then the full access to this
 * member is allowed.
 */
InternetHostCore *InternetHost::GetInternetHost() {
    return &address;
}

uint32 InternetHost::Size() const {
    return static_cast<uint32>(sizeof(address));
}

}

