/**
 * @file BasicTCPSocketTest.cpp
 * @brief Source file for class BasicTCPSocketTest
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
 * the class BasicTCPSocketTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicTCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool BasicTCPSocketTest::TestDefaultConstructor() {

    BasicTCPSocket basicTCPSocket;

    if (basicTCPSocket.GetDestination().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (basicTCPSocket.GetDestination().GetPort() != 0) {
        return false;
    }

    if (basicTCPSocket.GetSource().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (basicTCPSocket.GetSource().GetPort() != 0) {
        return false;
    }

    return !basicTCPSocket.IsValid();

}

bool BasicTCPSocketTest::TestSeek() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.Seek(0);
}

bool BasicTCPSocketTest::TestSize() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.Size() == 0xffffffffffffffff;
}

bool BasicTCPSocketTest::TestRelativeSeek() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.RelativeSeek(0);
}

bool BasicTCPSocketTest::TestPosition() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.Position() == 0xffffffffffffffff;
}

bool BasicTCPSocketTest::TestSetSize() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.SetSize(1);
}

bool BasicTCPSocketTest::TestCanWrite() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.CanWrite();
}

bool BasicTCPSocketTest::TestCanRead() {

    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.CanRead();
}

bool BasicTCPSocketTest::TestCanSeek() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.CanSeek();
}

