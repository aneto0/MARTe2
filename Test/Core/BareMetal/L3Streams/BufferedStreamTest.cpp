/**
 * @file BufferedStreamTest.cpp
 * @brief Source file for class BufferedStreamTest
 * @date 15/10/2015
 * @author Andre' Neto
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
 * the class BufferedStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamTest.h"
#include "StreamTestHelper.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BufferedStreamTest::TestDefaultConstructor() {
    DummySingleBufferedStream myStream;
    return (myStream.GetTimeout() == TTInfiniteWait);
}

bool BufferedStreamTest::TestAnyType() {
    DummySingleBufferedStream myStream;
    AnyType anyStream = myStream;
    bool ok = anyStream.GetDataPointer() == &myStream;
    ok = !anyStream.GetTypeDescriptor().isConstant;
    ok = (anyStream.GetTypeDescriptor().numberOfBits == 0u);
    return ok;
}


bool BufferedStreamTest::TestGetTimeout() const {
    TimeoutType tt = 1;
    DummySingleBufferedStream myStream;
    myStream.SetTimeout(tt);
    return (myStream.GetTimeout() == tt);
}

bool BufferedStreamTest::TestSetTimeout() {
    TimeoutType tt = 1;
    DummySingleBufferedStream myStream;
    myStream.SetTimeout(tt);
    return (myStream.GetTimeout() == tt);
}
