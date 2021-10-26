/**
 * @file BasicTCPSocketTest.h
 * @brief Header file for class BasicTCPSocketTest
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

 * @details This header file contains the declaration of the class BasicTCPSocketTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTCPSOCKETTEST_H_
#define BASICTCPSOCKETTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicTCPSocket.h"
#include "FastPollingMutexSem.h"
#include "Threads.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief A structure used to test BasicTCPSocketTest::Listen() and BasicTCPSocketTest::Connect().
 */
struct ConnectListenTestTable {
    uint16 port;
    uint32 maxConnectionsIn;
    uint32 nClientsIn;
    TimeoutType timeoutIn;
    bool expected;
    bool isBlockingIn;
    bool isServer;
    bool isValid;
};

/**
 * @brief A structure used to test BasicTCPSocketTest::Read() and BasicTCPSocketTest::Write().
 */
struct ReadWriteTestTable {
    uint32 sizeIn;
    uint32 expectedSizeIn;
    const char8 *stringIn;
    const char8* resultIn;
    uint32 nClientsIn;
    TimeoutType timeoutIn;
    bool expected;
    bool isBlockingIn;
    bool isServer;
    bool isTimeoutIn;
    bool isValid;
};

/**
 * @brief A structure used to test BasicTCPSocketTest::WaitConnection().
 */
struct WaitConnectionTestTable {
    uint32 nClientsIn;
    bool createOnHeapIn;
    TimeoutType timeoutIn;
    bool isBlockingIn;
    bool expected;
    bool isValid;
};


/**
 * @brief Tests all the BasicTCPSocket public methods.
 */
class BasicTCPSocketTest {
private:
    /**
    * @brief Keeps the number of threads which were already running before the test itself
    *        Used for portable implementations, where some threads belong to the OS itself (e.g. FreeRTOS)
    */
    uint32 numOfThreadsBefore;
public:

    /**
     * @brief Default constructor for test class.
     */
    BasicTCPSocketTest();

    /**
     * @brief Tests if the default constructor intializes the socket as invalid.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestSeek();

    /**
     * @brief Tests if the function returns max uint64
     */
    bool TestSize();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestRelativeSeek();

    /**
     * @brief Tests if the function returns max uint64
     */
    bool TestPosition();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestSetSize();

    /**
     * @brief Tests if the function returns true.
     */
    bool TestCanWrite();

    /**
     * @brief Tests if the function returns true.
     */
    bool TestCanRead();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestCanSeek();

    /**
     * @brief Tests if connecting less clients than the listen queue size, the connect succeeds.
     */
    bool TestListen(const ConnectListenTestTable *table);

    /**
     * @brief Tests if connecting less clients than the listen queue size, the connect succeeds.
     */
    bool TestConnect(const ConnectListenTestTable *table);

    /**
     * @brief Tests if the function behaves as expected, also within timeout and in blocking mode.
     */
    bool TestRead(const ReadWriteTestTable *table);

    /**
     * @brief Tests if the function behaves as expected, also within timeout and in blocking mode.
     */
    bool TestPeek(const ReadWriteTestTable *table);

    /**
     * @brief Tests if the function behaves as expected, also within timeout and in blocking mode.
     */
    bool TestWrite(const ReadWriteTestTable *table);

    /**
     * @brief Tests if the function behaves as expected, also within timeout and in blocking mode.
     */
    bool TestWaitConnection(const WaitConnectionTestTable *table);

    /**
     * @brief Tests if the socket is valid after a Open()
     */
    bool TestOpen();

    /**
     * @brief Tests if the socket is invalid after a Close()
     */
    bool TestClose();

    /**
     * @brief Tests if the function returns true if the socket was opened.
     */
    bool TestIsValid();

    /**
     * @brief Tests if the function returns true if the socket is connected.
     */
    bool TestIsConnected(bool connect,
                         bool expected);

public:
    InternetHost server;
    uint32 maxConnections;
    bool retVal;
    FastPollingMutexSem sem;
    uint32 nClients;
    uint32 alives;
    int32 exitCondition;
    ThreadFunctionType serverJob;
    TimeoutType timeout;
    EventSem eventSem;
    const char8 *serviceName;
    bool isBlocking;
    const char8* string;
    const char8* result;
    uint32 size;
    uint32 expectedSize;
    bool isServer;
    bool isTimeout;
    bool NoError;
    bool createSocketOnHeap;
    bool isValidClient;
    bool isValidServer;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTCPSOCKETTEST_H_ */

