/**
 * @file BufferedStreamIOBufferTest.h
 * @brief Header file for class BufferedStreamIOBufferTest
 * @date 14/10/2015
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

 * @details This header file contains the declaration of the class BufferedStreamIOBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAMIOBUFFERTEST_H_
#define BUFFEREDSTREAMIOBUFFERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamIOBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Test the BufferedStreamIOBuffer public functions.
 */
class BufferedStreamIOBufferTest {

public:

    /**
     * @brief Tests if the default constructor initializes the internal buffer to null
     * and the stream pointer to null.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the stream pointer is initializated correctly.
     */
    bool TestFullConstructor();

    /**
     * @brief Tests the GetTimeout function.
     */
    bool TestGetTimeout();

    /**
     * @brief Tests the SetTimeout function.
     */
    bool TestSetTimeout();

    /**
     * @brief Tests if the refill fills the buffer reading data from the stream below.
     */
    bool TestRefill();

    /**
     * @brief Tests if the stream pointer is null the function returns false.
     */
    bool TestRefill_NULL_Stream();

    /**
     * @brief Tests if the buffer pointer is null the function returns false.
     */
    bool TestRefill_NULL_Buffer();

    /**
     * @brief Tests if the flushes empties the buffer writing data from the stream below.
     */
    bool TestFlush();

    /**
     * @brief Tests if the stream pointer is null the function returns false.
     */
    bool TestFlush_NULL_Stream();

    /**
     * @brief Tests if the buffer pointer is null the function returns false.
     */
    bool TestFlush_NULL_Buffer();

    /**
     * @brief Tests if after a resync the position is correct.
     */
    bool TestResync();


    /**
     * @brief Tests if the stream pointer is null the function returns false.
     */
    bool TestResync_NULL_Stream();

    /**
     * @brief Tests if the buffer pointer is null the function returns false.
     */
    bool TestResync_NULL_Buffer();

    /**
     * @brief Tests if the function sets correctly the buffer size.
     */
    bool TestSetBufferSize(uint32 size);

    /**
     * @brief Tests if the stream pointer is returned.
     */
    bool TestGetStream();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BUFFEREDSTREAMIOBUFFERTEST_H_ */

