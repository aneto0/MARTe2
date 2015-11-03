/**
 * @file BasicUDPSocketTest.h
 * @brief Header file for class BasicUDPSocketTest
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

 * @details This header file contains the declaration of the class BasicUDPSocketTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICUDPSOCKETTEST_H_
#define BASICUDPSOCKETTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocket.h"
#include "EventSem.h"
#include "Threads.h"
#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

struct ConnectListenUDPTestTable{
    uint32 port;
    uint32 nClients;
    bool expected;
};


struct ReadWriteUDPTestTable{
    const char8* string;
    const char8* result;
    uint32 size;
    uint32 expectedSize;
    uint32 port;
    TimeoutType timeout;
    uint32 nClients;
    bool expected;
    bool isBlocking;
    bool isServer;
    bool isTimeout;
};


class BasicUDPSocketTest {
public:

    BasicUDPSocketTest();

    bool TestDefaultConstructor();

    bool TestSize();

    bool TestSeek();

    bool TestRelativeSeek();

    bool TestPosition();

    bool TestSetSize();

    bool TestCanWrite();

    bool TestCanRead();

    bool TestCanSeek();

    bool TestIsValid();

    bool TestOpen();

    bool TestClose();

    bool TestListen(const ConnectListenUDPTestTable* table);

    bool TestConnect(const ConnectListenUDPTestTable* table);

    bool TestRead(const ReadWriteUDPTestTable* table);

    bool TestPeek(const ReadWriteUDPTestTable* table);

    bool TestWrite(const ReadWriteUDPTestTable* table);

public:

    InternetHost server;
    bool retVal;
    bool noError;
    EventSem eventSem;
    int32 exitCondition;
    bool isBlocking;
    uint32 alives;
    FastPollingMutexSem sem;
    const char8* string;
    const char8* result;
    uint32 size;
    uint32 expectedSize;
    bool isServer;
    TimeoutType timeout;
    bool isTimeout;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUDPSOCKETTEST_H_ */

