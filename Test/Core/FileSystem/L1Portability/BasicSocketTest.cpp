/**
 * @file BasicSocketTest.cpp
 * @brief Source file for class BasicSocketTest
 * @date 29/10/2015
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
 * the class BasicSocketTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool BasicSocketTest::TestDefaultConstructor() {
	BasicTCPSocket socket;

	DynamicCString address;
    socket.GetDestination().GetAddress(address);
    if (!address.IsSameAs("0.0.0.0") ) {
        return false;
    }

    if (socket.GetDestination().GetPort() != 0) {
        return false;
    }

    socket.GetSource().GetAddress(address);
    if (!address.IsSameAs("0.0.0.0") ) {
        return false;
    }

    if (socket.GetSource().GetPort() != 0) {
        return false;
    }

    return !socket.IsValid();
}

bool BasicSocketTest::TestSetBlocking(bool isBlocked) {

    BasicTCPSocket basicSocket;
    basicSocket.Open();
    bool ok = basicSocket.SetBlocking(isBlocked);
    basicSocket.Close();
    return ok;
}

bool BasicSocketTest::TestSetBlocking_false(bool isBlocked) {
    BasicTCPSocket basicSocket;

    return !basicSocket.SetBlocking(isBlocked);
}

bool BasicSocketTest::TestClose() {

    BasicTCPSocket basicSocket;
    basicSocket.Open();
    return basicSocket.Close();
}

bool BasicSocketTest::TestClose_false() {

    BasicTCPSocket basicSocket;
    return !basicSocket.Close();
}

bool BasicSocketTest::TestGetSource() {

    BasicTCPSocket basicSocket;

    DynamicCString address;
    basicSocket.GetSource().GetAddress(address);
    if (!address.IsSameAs("0.0.0.0") ) {
        return false;
    }

    if (basicSocket.GetSource().GetPort() != 0) {
        return false;
    }

    InternetHost sourceHost;

    sourceHost.SetAddress("127.0.0.1");
    sourceHost.SetPort(8080);

    basicSocket.SetSource(sourceHost);

    InternetHost copy = basicSocket.GetSource();

    copy.GetAddress(address);
    if (!address.IsSameAs("127.0.0.1")) {
        return false;
    }

    return copy.GetPort() == 8080;
}

bool BasicSocketTest::TestGetDestination() {

    BasicTCPSocket basicSocket;
    DynamicCString address;
    basicSocket.GetDestination().GetAddress(address);
    if (!address.IsSameAs("0.0.0.0") ) {
        return false;
    }

    if (basicSocket.GetDestination().GetPort() != 0) {
        return false;
    }

    InternetHost destinationHost;

    destinationHost.SetAddress("127.0.0.1");
    destinationHost.SetPort(8080);

    basicSocket.SetDestination(destinationHost);

    InternetHost copy = basicSocket.GetDestination();

    copy.GetAddress(address);
    if (!address.IsSameAs("127.0.0.1")) {
        return false;
    }

    return copy.GetPort() == 8080;
}

bool BasicSocketTest::TestSetSource() {
    return TestGetSource();
}

bool BasicSocketTest::TestSetDestination() {
    return TestGetDestination();
}

bool BasicSocketTest::TestIsBlocking(bool isBlocked) {
    BasicTCPSocket socket;

    if (!socket.IsBlocking()) {
        return false;
    }

    if (!socket.Open()) {
        return false;
    }

    if (!socket.SetBlocking(isBlocked)) {
        return false;
    }

    bool ok = socket.IsBlocking() == isBlocked;
    socket.Close();
    return ok;
}

