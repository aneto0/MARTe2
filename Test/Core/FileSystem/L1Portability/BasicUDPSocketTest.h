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

/**
 * @brief A structure used to test the connect and listen functions.
 */
struct ConnectListenUDPTestTable {
    uint32 port;
    uint32 nClients;
    bool expected;
    bool isValid;
};

/**
 * @brief A structure used to test the read and write functions.
 */
struct ReadWriteUDPTestTable {
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
    bool isValid;
};

/**
 * @brief Tests all the BasicUDPSocket public functions.
 */
class BasicUDPSocketTest {
private:
    /**
    * @brief Keeps the number of threads which were already running before the test itself
    *        Used for portable implementations, where some threads belong to the OS itself (e.g. FreeRTOS)
    */
    uint32 numOfThreadsBefore;
public:

    /**
     * @brief Tests class constructor.
     */
    BasicUDPSocketTest();

    /**
     * @brief Checks if the socket is initialized as invalid.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the function returns the max uint64.
     */
    bool TestSize();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestSeek();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestRelativeSeek();

    /**
     * @brief Tests if the function returns the max uint64.
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
     * @brief Tests if after Open the function returns false and after Close it returns true.
     */
    bool TestIsValid();

    /**
     * @brief Tests if after Open the socket is valid.
     */
    bool TestOpen();

    /**
     * @brief Tests if after Close the socket is invalid.
     */
    bool TestClose();

    /**
     * @brief Tests if the function initializes correctly the server for connection.
     */
    bool TestListen(const ConnectListenUDPTestTable* table);

    /**
     * @brief Tests if the function initializes correctly the client for connection.
     */
    bool TestConnect(const ConnectListenUDPTestTable* table);

    /**
     * @brief Tests if the read function behaves correctly, also in non-block mode and within timeout.
     */
    bool TestRead(const ReadWriteUDPTestTable* table);

    /**
     * @brief Tests if the peek function behaves correctly, also in non-block mode and within timeout.
     */
    bool TestPeek(const ReadWriteUDPTestTable* table);

    /**
     * @brief Tests if the write function behaves correctly, also in non-block mode and within timeout.
     */
    bool TestWrite(const ReadWriteUDPTestTable* table);

    /**
     * @brief Tests if the Join function behaves correctly
     */
    bool TestJoin(const ConnectListenUDPTestTable* table);

public:

    InternetHost server;
    bool retVal;
    bool NoError;
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
    bool isValidServer;
    bool isValidClient;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUDPSOCKETTEST_H_ */

