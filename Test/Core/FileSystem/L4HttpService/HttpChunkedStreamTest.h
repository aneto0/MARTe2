/**
 * @file HttpChunkedStreamTest.h
 * @brief Header file for class HttpChunkedStreamTest
 * @date 01/10/2018
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

 * @details This header file contains the declaration of the class HttpChunkedStreamTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPCHUNKEDSTREAMTEST_H_
#define HTTPCHUNKEDSTREAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpChunkedStream.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the HttpChunkedStream methods
 */
class HttpChunkedStreamTest {
public:

    /**
     * @brief Constructor.
     */
    HttpChunkedStreamTest();

    /**
     * @brief Destructor.
     */
    virtual ~HttpChunkedStreamTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Flush method
     */
    bool TestFlush();

    /**
     * @brief Tests the FinalChunk method
     */
    bool TestFinalChunk();

    /**
     * @brief Tests the SetChunkMode method
     */
    bool TestSetChunkMode();

    /**
     * @brief Tests the IsChunkMode method
     */
    bool TestIsChunkMode();

    /**
     * Internal semaphore was for the tests.
     */
    EventSem eventSem;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPCHUNKEDSTREAMTEST_H_ */

