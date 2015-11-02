/**
 * @file BasicUDPSocketTest.cpp
 * @brief Source file for class BasicUDPSocketTest
 * @date 02/11/2015
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
 * the class BasicUDPSocketTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool BasicUDPSocketTest::TestDefaultConstructor() {
    BasicUDPSocket socket;

    if (socket.IsValid()) {
        return false;
    }

    if (socket.GetDestination().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (socket.GetDestination().GetPort() != 0) {
        return false;
    }

    if (socket.GetSource().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (socket.GetSource().GetPort() != 0) {
        return false;
    }

    return socket.IsBlocking();
}



bool BasicUDPSocketTest::TestSeek() {
    BasicUDPSocket socket;
    return !socket.Seek(0);
}

bool BasicUDPSocketTest::TestSize() {
    BasicUDPSocket socket;
    return socket.Size() == 0xffffffffffffffff;
}

bool BasicUDPSocketTest::TestRelativeSeek() {
    BasicUDPSocket socket;
    return !socket.RelativeSeek(0);
}

bool BasicUDPSocketTest::TestPosition() {
    BasicUDPSocket socket;
    return socket.Position() == 0xffffffffffffffff;
}

bool BasicUDPSocketTest::TestSetSize() {
    BasicUDPSocket socket;
    return !socket.SetSize(1);
}

bool BasicUDPSocketTest::TestCanWrite() {
    BasicUDPSocket socket;
    return socket.CanWrite();
}

bool BasicUDPSocketTest::TestCanRead() {

    BasicUDPSocket socket;
    return socket.CanRead();
}

bool BasicUDPSocketTest::TestCanSeek() {
    BasicUDPSocket socket;
    return !socket.CanSeek();
}

bool BasicUDPSocketTest::TestIsValid() {

    BasicUDPSocket socket;

    if (socket.IsValid()) {
        return false;
    }

    socket.Open();

    if (!socket.IsValid()) {
        return false;
    }

    socket.Close();

    return !socket.IsValid();
}

bool BasicUDPSocketTest::TestOpen() {
    return TestIsValid();
}

bool BasicUDPSocketTest::TestClose() {
    return TestIsValid();
}



