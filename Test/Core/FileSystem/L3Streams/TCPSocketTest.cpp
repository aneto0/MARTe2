/**
 * @file TCPSocketTest.cpp
 * @brief Source file for class TCPSocketTest
 * @date 16/11/2016
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
 * the class TCPSocketTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "TCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Verifies if the template can solve the stream interface.
static bool CheckStream(MARTe::StreamI &stream) {
    return true;
}

static bool CheckTCPSocket(MARTe::BasicTCPSocket &file) {
    return true;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool TCPSocketTest::TestConstructor() {
    MARTe::TCPSocket f;
    return true;
}

bool TCPSocketTest::TestConstructor_Stream() {
    MARTe::TCPSocket f;
    return CheckStream(f);
}

bool TCPSocketTest::TestConstructor_BasicTCPSocket() {
    MARTe::TCPSocket f;
    return CheckTCPSocket(f);
}
